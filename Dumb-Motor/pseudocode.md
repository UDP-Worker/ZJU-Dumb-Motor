# **程序思想实现**

> 这个文档给出了一些程序实现，但是不是为这个项目设计的，诸如外设、频率、计时器都需要按照我们的项目重写，一定不要照抄这里的代码！

------

## **目录**

1. 工程文件与模块划分
2. 全局数据结构
3. 外设初始化函数
4. 中断服务函数（ISR）
5. 指令队列执行器
6. 循迹逻辑实现 (`follow.c`)
7. 超声避障逻辑实现 (`avoid.c`)
8. 有限状态机 (`fsm.c`)
9. 主循环 (`main.c`)
10. 配置参数与常量 (`config.h`)
11. 编译与下载

------

## 1 工程文件与模块划分

```
src/
 ├─ main.c          // 启动代码 + 无限循环
 ├─ init.c          // 外设初始化
 ├─ isr.c           // 所有中断入口
 ├─ queue.c         // 指令队列执行器
 ├─ follow.c        // 对地红外循迹
 ├─ avoid.c         // 舵机‑超声避障
 ├─ fsm.c           // 系统有限状态机
 └─ config.h        // 所有可调常量
```

每个 *.c* 文件都 #include "config.h"，禁止硬编码魔数。

------

## 2 全局数据结构 (`config.h`)

```c
// ===== 可调常量 =====
#define TURN_MS        52    // 原地 ≈10° 转向的 PWM 维持时间
#define STEP_LIMIT      3    // 单边压线最大记数
#define FWD_MS         60    // 单步直行持续时间
#define BWD_MS         60    // 单步倒退持续时间
#define BRAKE_MS       40    // 刹车持续时间
#define D_SAFE        400    // 超声安全距离 (mm)

// ===== 指令定义 =====
typedef enum {FWD, BRK, L10, R10, BWD} Act;
typedef struct {Act a; uint16_t t; } Cmd;

// ===== 队列 =====
#define QLEN 32
extern volatile Cmd q[QLEN];
extern volatile uint8_t qi, qj;

// ===== 传感器状态 =====
extern volatile uint8_t g_L, g_R;      // 0/1
extern volatile uint16_t us_dist[21];   // mm
extern volatile uint8_t us_idx;         // 0–20

// ===== FSM =====
typedef enum {FOLLOW, AVOID, RECOVER} State;
extern volatile State g_state;
```

------

## 3 外设初始化 (`init.c`)

```c
void init_hw(void){
  // 时钟树与系统滴答
  SysTick_Config(SystemCoreClock/1000);   // 1 ms 节拍

  // GPIO：两路对地红外输入
  GPIO_Config(IR_L_PIN, INPUT_PULLUP);
  GPIO_Config(IR_R_PIN, INPUT_PULLUP);

  // PWM：TIM1‑CH1/CH2 驱动左右轮 H 桥
  TIM1_PWM_Init(LEFT_PWM_CH, 20_000);  // 20 kHz
  TIM1_PWM_Init(RIGHT_PWM_CH,20_000);

  // 舵机 PWM：TIM2‑CH1
  TIM2_PWM_Init(SERVO_CH, 50);         // 50 Hz

  // 超声：TRIG‑GPIO 推挽输出，ECHO‑TIM4‑CH1 输入捕获
  GPIO_Config(TRIG_PIN, OUTPUT_PP);
  TIM4_IC_Init(ECHO_CH);

  // TIM3：1 kHz 中断读取 IR
  TIM3_Init(1000);
}
```

> **仅复制即可**，内部 HAL/LL 调用按个人库封装实现。

------

## 4 中断服务函数 (`isr.c`)

```c
// — 1 kHz 对地红外采样 —
void TIM3_IRQHandler(void){
  TIM3->SR = 0;                // 清溢出
  g_L = !GPIO_Read(IR_L_PIN);  // 黑带 → 1
  g_R = !GPIO_Read(IR_R_PIN);
}

// — 1 ms 系统节拍 —
void SysTick_Handler(void){
  queue_tick();      // 执行器计时
  fsm_tick();        // 有限状态机节拍
}

// — 超声回波捕获 —
void TIM4_IRQHandler(void){
  if(TIM4->SR & CC1IF){
     static uint16_t t_start;
     if(ECHO_PIN_HIGH) t_start = TIM4->CCR1;
     else{
        uint16_t dt = TIM4->CCR1 - t_start;
        us_dist[us_idx] = dt * 17 / 100; // mm (简化换算)
     }
     TIM4->SR &= ~CC1IF;
  }
}
```

------

## 5 指令队列执行器 (`queue.c`)

```c
volatile Cmd q[QLEN];
volatile uint8_t qi=0, qj=0;   // 读 / 写
static uint16_t cur_left=0, cur_right=0; // 当前 PWM 占空 (0‑1000)
static uint16_t rest = 0;      // 当前指令剩余 ms

void enqueue(Act a, uint16_t t){
  q[qj] = (Cmd){a,t};
  qj = (qj+1)&(QLEN-1);
}

// 每 1 ms 调用
void queue_tick(void){
  if(rest){ rest--; return; }
  if(qi==qj){ motor_fwd(); rest = FWD_MS; return; }

  Cmd c = q[qi]; qi = (qi+1)&(QLEN-1);
  switch(c.a){
    case FWD: motor_fwd();              break;
    case BWD: motor_bwd();              break;
    case BRK: motor_brake();            break;
    case L10: motor_turn_left();        break;
    case R10: motor_turn_right();       break;
  }
  rest = c.t;
}
```

*`motor_\*` 函数只需设置左右 PWM 的占空和方向，不含逻辑判断。*

------

## 6 循迹逻辑 (`follow.c`)

```c
static int8_t err = 0;       // -2 .. +2
static uint8_t step_cnt = 0; // 0 .. STEP_LIMIT

// 每 10 ms 在 FOLLOW 状态调用
void follow_update(void){
  if(qi!=qj) return;          // 队列忙，先不插指令

  // 1. 计算误差
  if(!g_L && !g_R) err = 0;           // 正常
  else if(g_L && !g_R)  err = +1;     // 左压线
  else if(!g_L && g_R)  err = -1;     // 右压线
  else                  err = (err>0)?+2:-2; // 完全跨线，保持符号

  // 2. 更新计数器
  if(err==0) step_cnt = 0;
  else if(step_cnt<STEP_LIMIT) step_cnt++;

  // 3. 生成指令
  switch(err){
    case 0: enqueue(FWD, FWD_MS); break;
    case +1: enqueue(R10, TURN_MS);
             enqueue(FWD, FWD_MS/2);
             break;
    case -1: enqueue(L10, TURN_MS);
             enqueue(FWD, FWD_MS/2);
             break;
    default: // ±2
             enqueue(BRK, BRAKE_MS);
             enqueue(BWD, BWD_MS);
             enqueue((err>0)?R10:L10, TURN_MS*2);
  }
}
```

------

## 7 超声避障逻辑 (`avoid.c`)

### 7.1 舵机扫描

```c
#define SERVO_MIN 1000  // μs
#define SERVO_MAX 2000  // μs
static int8_t sweep_idx = -10; // -10 .. +10 对应 21 扇区

// 每 50 ms 调用一次
void servo_sweep(void){
  sweep_idx++;
  if(sweep_idx>10) sweep_idx=-10;
  us_idx = sweep_idx + 10;          // 保存给 ISR
  uint16_t pulse = SERVO_MIN + (SERVO_MAX-SERVO_MIN)*(us_idx)/20;
  TIM2_SetPulse(SERVO_CH, pulse);
  trig_ultrasonic();                // 发送一次 TRIG 10 μs
}
```

### 7.2 航向计算与指令生成

```c
// FOLLOW → AVOID 的判决：前方扇区 dist < D_SAFE
bool front_blocked(void){
  return us_dist[10] < D_SAFE;
}

void avoid_plan(void){
  // 1. 把 dist 转成二值 free[]
  uint8_t free[21];
  for(int i=0;i<21;i++)
    free[i] = (us_dist[i] > D_SAFE) ? 1 : 0;

  // 2. 跑道越界保护
  if(g_L) for(int i=0;i<10;i++) free[i]=0;   // 屏蔽左边
  if(g_R) for(int i=11;i<21;i++) free[i]=0;  // 屏蔽右边

  // 3. 找最长连续 1 区间
  int best_len=0,best_start=0,len=0,start=0;
  for(int i=0;i<=21;i++){
    if(i<21 && free[i]){
       if(len==0) start=i; len++;
    }else{
       if(len>best_len){best_len=len;best_start=start;}
       len=0;
    }
  }
  int center = best_start + best_len/2; // 0..20
  int n = center - 10;   // -10..+10  →  -100°..+100°
  // 4. 把 n 离散成 TURN_MS 步数
  if(n>0) for(int k=0;k<n;k++) enqueue(L10, TURN_MS);
  if(n<0) for(int k=0;k<-n;k++) enqueue(R10, TURN_MS);
  enqueue(FWD, 120);
}
```

> `avoid_plan()` 仅在 FSM 进入 AVOID 时被调用一次。

------

## 8 有限状态机 (`fsm.c`)

```c
volatile State g_state = FOLLOW;
static uint16_t recover_ms = 0;

void fsm_tick(void){          // 每 1 ms 调用
  switch(g_state){
    case FOLLOW:
      if(front_blocked() && qi==qj){
        avoid_plan(); g_state = AVOID; }
      else if(!(SysTick->VAL % 10)) // 每 10 ms
        follow_update();
      break;

    case AVOID:
      if(qi==qj) { g_state = RECOVER; recover_ms=0; }
      break;

    case RECOVER:
      if(g_L||g_R) recover_ms=0; else recover_ms++;
      if(recover_ms > 200) g_state = FOLLOW;
      break;
  }
}
```

------

## 9 主循环 (`main.c`)

```c
int main(void){
  init_hw();
  while(1){
     // 舵机扫描周期 50 ms
     static uint32_t t_servo = 0;
     if(millis() - t_servo >= 50){
        t_servo = millis();
        servo_sweep();
     }
     // 其他后台任务可加在此处
  }
}
```

`millis()` 可在 SysTick 中自增全局计数器实现。