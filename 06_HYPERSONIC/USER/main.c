#include "stm32f10x.h"
#include "delay.h"
#include "motor.h"
#include "keysacn.h"
#include "IRSEARCH.h"
#include "IRAvoid.h"
#include "usart.h"
#include "UltrasonicWave.h"
#include "timer.h"


//extern int U_temp;
/*
void ZYSTM32_run(int speed,int time);       //前进函数
void ZYSTM32_brake(int time);               //刹车函数
void ZYSTM32_Left(int speed,int time);      //左转函数
void ZYSTM32_Spin_Left(int speed,int time); //左旋转函数
void ZYSTM32_Right(int speed,int time);     //右转函数
void ZYSTM32_Spin_Right(int speed,int time);//右旋转函数
void ZYSTM32_back(int speed,int time);      //后退函数
*/

 int main(void)
 {	

	 delay_init();
	 KEY_Init();
	 IRSearchInit();
	 IRAvoidInit();
   Timerx_Init(5000,7199);   //10Khz的计数频率，计数到5000为500ms 
	 UltrasonicWave_Configuration();               //对超声波模块初始化
	 uart_init(115200);
	 TIM4_PWM_Init(7199,0);  //初始化PWM
	 ZYSTM32_brake(500);
	 keysacn();		
		while(1)
		{  		
	 	  //printf("测到的距离值为：%d\n",UltrasonicWave_StartMeasure());
			if(UltrasonicWave_StartMeasure()<40)
      {
     
				ZYSTM32_back(50,500);
        ZYSTM32_Right(50,500);				
        BEEP_SET;
      }
    else     
      {
        ZYSTM32_run(50,1);
        BEEP_RESET; 
      }			
		}
 }

