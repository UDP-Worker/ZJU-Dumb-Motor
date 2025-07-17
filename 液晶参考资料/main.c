#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_exti.h"
#include "stm32f10x_spi.h"
#include "sys_cfg.h"
#include "system.h"
#include "adc_init.h"
#include "time_init.h"
#include "uart_init.h"
#include "gpio_init.h"
#include "clk_init.h"
#include "stm32f10x_pwr.h"
#include "spi_lcd_display.h"
#include "stmflash.h"
#include "delay.h"
#include "math.h"
#include "songs.h"

#define _NOEXTERN_STRING_DEF_
#include "global_params.h"
#undef _NOEXTERN_STRING_DEF_
	
//要写入到STM32 FLASH的字符串数组
#define FLASH_SAVE_ADDR  0x0800F800		//0x8000 0000-0x8001 0000
//设置FLASH 保存地址(必须为偶数，
//且其值要大于本代码所占用FLASH的大小+0X08000000)	


void ADC1_2_IRQHandler(void)
{
			//ADC_SoftwareStartConvCmd(ADC1, ENABLE);		//使能指定的ADC1的软件转换启动功能	
			EXTI_ClearITPendingBit(EXTI_IMR_MR11);	
}

void EXTI3_IRQHandler(void)
{
			//system_init();
			EXTI_ClearITPendingBit(EXTI_Line3);  //清除LINE3上的中断标志位  
}

void EXTI15_10_IRQHandler(void)
{
			//system_init();
			EXTI_ClearITPendingBit(EXTI_Line10);	
}

void TIM1_UP_IRQHandler(void)
{
			TIM1->SR = 0x0000;
			//TIM_ClearITPendingBit(TIM1,TIM_IT_Update);
}


void DMA1_Channel1_IRQHandler(void)
{		
				DMA_ClearITPendingBit(DMA1_IT_TC1); 	
//			DMA1_Channel1->CCR &= ~0x00001;//Channel disabled				
//			DMA1_Channel1->CNDTR = DATA_LENGTH;	 //设置传输数据长度
//			DMA1_Channel1->CCR |= 0x00001;//Channel Enabled		
}

void TIM2_IRQHandler(void)
{	
			TIM2->SR &= ~0x000F;	//清除定时器2的全部中断标志
			//TIM_ClearITPendingBit(TIM2, TIM_IT_CC4);
			//TIM_ClearITPendingBit(TIM2, TIM_IT_Update);  //清除TIMx更新中断标志 
}

// *********处理定时器3：延迟后进入STOP模式*************
void TIM3_IRQHandler(void)
{
			music_count += 1; 
			//Count++; 
			TIM_ClearITPendingBit(TIM3, TIM_IT_Update);  //清除TIMx更新中断标志 
}

// ******************异步串口1模块的接收数据处理******************
void USART1_IRQHandler(void)
{
		if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
		{	
			rx_buf[rx_serial++] =  USART_ReceiveData(USART1);	//读取接收到的数据
			if(rx_serial>RX_BUF_LENGTH)
				rx_serial = 0;
		}	
} 

// ******************串口2的数据处理******************************
void USART2_IRQHandler(void)
{
		if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET){}
} 

// *****************主函数*************************
int main(void)
{
			system_init();
			//uart_exti_init();
			LCD_PSB_LOW;	//0：采用串行接口方式进行液晶显示				
			LCD_RST_HIGH;	
			lcd_port_init();
//			CLEARGDRAM(0x0000);
			Disp(1,0,16,"浙江大学光电学院");//显示数据到LCD12864子程序			
			Disp(2,1,12,"电子线路设计");//显示数据到LCD12864子程序
			Disp(3,2,8,"87951197");//显示数据到LCD12864子程序
			Disp(4,1,12,"液晶显示成功");//显示数据到LCD12864子程序

//			lcd_cmd = 0x32;
//			write_com(0x01);//清除显示	
//			while(1)
//			{
//				write_com(0x02);//游标归位		
//				systick_delay_us(4600);//4.6mS
//				write_com(0x04);//位址归位		
//				systick_delay_us(4600);//4.6mS
//				//write_com(0x32);//基本指令集动作
//				write_com(lcd_cmd);//扩充指令集动作
//				systick_delay_us(4600);//4.6mS
////				write_com(0x03);//扩充指令集-允许垂直卷动地址
////				systick_delay_us(4600);//4.6mS  128*64
//				for(dis_addr=0;dis_addr<64;dis_addr++)
//				{
//					Lcd_Clear(dis_addr,0xff00);				
//				}
//				for(dis_addr=0;dis_addr<128*64;dis_addr++)
//				{
//					Lcd_Clear(dis_addr,0x00FF);				
//				}
//			}
//	
//	
//		while(1)
//		{
//					CLEARGDRAM(0x0000);
//				//WRCGRAM4(0X40);
//					systick_delay_ms(1000);
//					CLEARGDRAM(0x55AA);
//					systick_delay_ms(1000);
//					CLEARGDRAM(0xFFFF);
//					systick_delay_ms(1000);
//		}
			ctrl_current_status = (ctrl_port_check()>>11)&0x03;
			ctrl_before_status = 0;
			rx_serial = 0;
			while(1)
			{
				USART_SendData(USART1,0x55);
//				USART1->DR = 0x55;
				systick_delay_ms(20);
				sys_sw_led_check();
				ctrl_current_status = (ctrl_port_check()>>11)&0x03;
				if(ctrl_current_status != ctrl_before_status)
				{
							ctrl_before_status = ctrl_current_status;
							CLEARGDRAM(0x0000);//清除液晶显示
							Disp(1,0,16,"浙江大学光电学院");//显示数据到LCD12864子程序			
							Disp(2,1,12,"电子线路设计");//显示数据到LCD12864子程序
							Disp(3,2,8,"87951197");//显示数据到LCD12864子程序
							Disp(4,1,12,"液晶显示成功");//显示数据到LCD12864子程序
				}
				switch(ctrl_current_status)
				{
					case 0x00:		//两口线全低
							CLEARGDRAM(0x0000);//液晶初始化子程序
							Disp(1,0,16,"系统进行按键检测");//显示数据到LCD12864子程序
							Disp(3,2,8,"87951197");//显示数据到LCD12864子程序
							Disp(4,0,16,"浙江大学光电学院");//显示数据到LCD12864子程序			
							do
							{
										sys_sw_led_check();
										key=scankey();
										switch(key)
										{
											case 0x01:Disp(2,1,12,"KEY_VALUE:01");	break;//显示数据到LCD12864子程序
											case 0x02:Disp(2,1,12,"KEY_VALUE:02");	break;//显示数据到LCD12864子程序
											case 0x03:Disp(2,1,12,"KEY_VALUE:03");	break;//显示数据到LCD12864子程序
											case 0x04:Disp(2,1,12,"KEY_VALUE:04");	break;//显示数据到LCD12864子程序
											case 0x05:Disp(2,1,12,"KEY_VALUE:05");	break;//显示数据到LCD12864子程序
											case 0x06:Disp(2,1,12,"KEY_VALUE:06");	break;//显示数据到LCD12864子程序
											case 0x07:Disp(2,1,12,"KEY_VALUE:07");	break;//显示数据到LCD12864子程序
											case 0x08:Disp(2,1,12,"KEY_VALUE:08");	break;//显示数据到LCD12864子程序
											case 0x09:Disp(2,1,12,"KEY_VALUE:09");	break;//显示数据到LCD12864子程序
											case 0x0A:Disp(2,1,12,"KEY_VALUE:10");	break;//显示数据到LCD12864子程序
											case 0x0B:Disp(2,1,12,"KEY_VALUE:11");	break;//显示数据到LCD12864子程序
											case 0x0C:Disp(2,1,12,"KEY_VALUE:12");	break;//显示数据到LCD12864子程序
											case 0x0D:Disp(2,1,12,"KEY_VALUE:13");	break;//显示数据到LCD12864子程序
											case 0x0E:Disp(2,1,12,"KEY_VALUE:14");	break;//显示数据到LCD12864子程序
											case 0x0F:Disp(2,1,12,"KEY_VALUE:15");	break;//显示数据到LCD12864子程序
											case 0x10:Disp(2,1,12,"KEY_VALUE:16");	break;//显示数据到LCD12864子程序
											default:break;											
										}
										ctrl_current_status = (ctrl_port_check()>>11)&0x03;
								}while(ctrl_current_status == 0x00);
								break;
					case 0x01:
								CLEARGDRAM(0x0000);//液晶初始化子程序
								Disp(1,0,16,"系统进行音乐播放");//显示数据到LCD12864子程序
								Disp(2,1,13,"Playing Music");//显示数据到LCD12864子程序  
								Disp(3,2,8,"87951197");//显示数据到LCD12864子程序
								Disp(4,0,16,"浙江大学光电学院");//显示数据到LCD12864子程序		
								//TIM3_Int_Init(100,3599);//10Khz的计数频率:8*9/2=36M;36000000/3600=10000
								Play_Song(0);			 
								break;
					case 0x02:
								CLEARGDRAM(0x0000);
								Disp(1,0,16,"系统进行光耦检测");//显示数据到LCD12864子程序				
								ShowQQChar(0x88,"                    ",16);
								ShowQQChar(0x90,"                    ",16);
								ShowQQChar(0x98,"                    ",16);
								do
								{
									 sys_sw_led_check();
									 if(OPT_CHECK&0x0100)
									 {
										 ShowQQChar(0x91,"1:高",2);
									 }
									 else
									 {
										 ShowQQChar(0x91,"1:低",2);							 
									 }
									 if(OPT_CHECK&0x0200)
									 {
										 ShowQQChar(0x93,"2:高",2);
									 }
									 else
									 {
										 ShowQQChar(0x93,"2:低",2);
									 }						 
									 if(OPT_CHECK&0x0400)
									 {
										 ShowQQChar(0x95,"3:高",2);
									 }
									 else
									 {
										 ShowQQChar(0x95,"3:低",2);
									 }
									 
									 if(OPT_CHECK&0x0800)
									 {
										 ShowQQChar(0x89,"4:高",2);
									 }
									 else
									 {
										 ShowQQChar(0x89,"4:低",2);
									 }						 
									 if(OPT_CHECK&0x1000)
									 {
										 ShowQQChar(0x8B,"5:高",2);
									 }
									 else
									 {
										 ShowQQChar(0x8B,"5:低",2);
									 }
									 
									 if(OPT_CHECK&0x2000)
									 {
										 ShowQQChar(0x8D,"6:高",2);
									 }
									 else
									 {
										 ShowQQChar(0x8D,"6:低",2);
									 }
									 
									 if(OPT_CHECK&0x4000)
									 {
										 ShowQQChar(0x9B,"7:高",2);
									 }
									 else
									 {
										 ShowQQChar(0x9B,"7:低",2);
									 }
									 systick_delay_us(100000);
									 ctrl_current_status = (ctrl_port_check()>>11)&0x03;
								}while(ctrl_current_status == 0x02);
								CLEARGDRAM(0x0000);//液晶初始化子程序
								break;
					default:
						break;
				}
			}
}

/**************************播放音乐*************************/
void Play_Song(uint8_t i)
{
			 uint16_t Temp1;//,temp2;
			 uint32_t Addr;
			 music_count = 0; 
			 Addr = i * 217;
			 TIM2->CCMR2 = 0x6000;	//将定时器2的CH4通道置为快速PWM模式			 
			 do
			 {
					 //sys_sw_led_check();
					 Temp1 = SONG[Addr++];//脉冲频率参数
					 if ( Temp1 == 0xFF ) //休止符        
					 {
									TIM2->CCMR2 &= 0x2000;	//强制低电平TIM_OCMode_Inactive			
									systick_delay_us(100000);
									TIM2->CCMR2 |= 0x2000;	//还原为TIM_PWM1 MODE
					 }
					 else if ( Temp1 == 0x00 ) //歌曲结束符  
					 {
							TIM_Cmd(TIM2, DISABLE);	//关闭定时器3
							TIM2->CCMR2 &= 0x2000;	//强制低电平TIM_OCMode_Inactive			 
							return;
					 }
					 else
					 {
								//TIM2_CC4_10mS(Temp1*32);	
								//b_curr_que = 0;	
								TIM2->ARR = Temp1<<1;//定时器的脉宽信号
								TIM2->CCR4 = Temp1;//<<7;//PWM占空比为50%
								TIM2->CR1 |= TIM_CR1_CEN;				 
								TIM3->CR1 |= TIM_CR1_CEN;
						 		music_timers = SONG[Addr++];	//脉冲次数
						  //systick_delay_ms(Temp1);
								while(1)
								{
									if(music_count>=music_timers)
									{
										music_count = 0;
										break;
									}							
								}						 
								ctrl_current_status = ((GPIOA->IDR & 0x1800)>>11)&0x03;//监控控制端口的状态是否还是播放模式
								if(ctrl_current_status != 0x01) break;//否，退出
					 }
			 }while(ctrl_current_status == 0x01);
			 TIM3->CR1 &= ~TIM_CR1_CEN;		//关闭定时器3				 
			 TIM2->CR1 &= ~TIM_CR1_CEN;		//关闭定时器2				 
			 TIM2->CCMR2 &= ~0x2000;	//强制低电平TIM_OCMode_Inactive			 
}


// *****************检测控制端口的状态**************
uint16_t ctrl_port_check(void)
{
			return 	GPIO_ReadInputData(GPIOA) & 0x1800;		
}

// **********检测控制端口，控制系统运行模式**********
void sys_sw_led_check(void)
{
				if(GPIO_ReadInputData(SW_S1_GPIO_Port)&0x0002) 					
					GPIO_SetBits(DIS_LED1_GPIO_Port,DIS_LED1);					
				else
					GPIO_ResetBits(DIS_LED1_GPIO_Port,DIS_LED1);				
				if(GPIOA->IDR&0x0004) 
					GPIO_SetBits(DIS_LED2_GPIO_Port,DIS_LED2);
				else
					GPIO_ResetBits(DIS_LED2_GPIO_Port,DIS_LED2);
}

// ********扫描按键，输出按键的键值******************
uint8_t scankey(void)//矩阵键盘翻转扫描
{
		uint keyvalue=0;
		GPIO_WriteBit(KEYS_GPIO_Port,0x000F,Bit_RESET);
		//GPIO_Write(KEYS_GPIO_Port,0x00F0);
		systick_delay_us(20);
		if(GPIO_ReadInputDataBit(KEYINT_GPIO_Port,KEYINT))		//检查是否有按键输入
		{
			return 0x0000;
		}
		else	
		{
				GPIO_WriteBit(KEYS_GPIO_Port,0x00FF,Bit_SET);
				GPIO_ResetBits(KEYS_GPIO_Port,KEY4_PIN);
				systick_delay_us(200);//延时200uS
			
				if(GPIO_ReadInputDataBit(KEYS_GPIO_Port,KEY3_PIN)==0x0000)
				{
						systick_delay_us(20);
						if(GPIO_ReadInputDataBit(KEYS_GPIO_Port,KEY3_PIN)==0x0000)
						{
							keyvalue=1;
						}						
						while(GPIO_ReadInputDataBit(KEYS_GPIO_Port,KEY3_PIN)==0x0000){};						
				}
				if(GPIO_ReadInputDataBit(KEYS_GPIO_Port,KEY2_PIN)==0x0000)
				{
						systick_delay_us(20);
						if(GPIO_ReadInputDataBit(KEYS_GPIO_Port,KEY2_PIN)==0x0000)
						{
							keyvalue=2;
						}						
						while(GPIO_ReadInputDataBit(KEYS_GPIO_Port,KEY2_PIN)==0x0000){};
				}
				if(GPIO_ReadInputDataBit(KEYS_GPIO_Port,KEY1_PIN)==0x0000)
				{
						systick_delay_us(20);
						if(GPIO_ReadInputDataBit(KEYS_GPIO_Port,KEY1_PIN)==0x0000)
						{
							keyvalue=3;
						}						
						while(GPIO_ReadInputDataBit(KEYS_GPIO_Port,KEY1_PIN)==0x0000){};
				}
				if(GPIO_ReadInputDataBit(KEYS_GPIO_Port,KEY0_PIN)==0x0000)
				{
						systick_delay_us(20);
						if(GPIO_ReadInputDataBit(KEYS_GPIO_Port,KEY0_PIN)==0x0000)
						{
							keyvalue=4;
						}						
						while(GPIO_ReadInputDataBit(KEYS_GPIO_Port,KEY0_PIN)==0x0000){};
				}

				GPIO_Write(KEYS_GPIO_Port,0x00FF);
				GPIO_ResetBits(KEYS_GPIO_Port,KEY5_PIN);
				systick_delay_us(200);//延时200uS
				if(GPIO_ReadInputDataBit(KEYS_GPIO_Port,KEY3_PIN)==0x0000)
				{
						systick_delay_us(20);
						if(GPIO_ReadInputDataBit(KEYS_GPIO_Port,KEY3_PIN)==0x0000)
						{
							keyvalue=5;
						}						
						while(GPIO_ReadInputDataBit(KEYS_GPIO_Port,KEY3_PIN)==0x0000){};						
				}
				if(GPIO_ReadInputDataBit(KEYS_GPIO_Port,KEY2_PIN)==0x0000)
				{
						systick_delay_us(20);
						if(GPIO_ReadInputDataBit(KEYS_GPIO_Port,KEY2_PIN)==0x0000)
						{
							keyvalue=6;
						}						
						while(GPIO_ReadInputDataBit(KEYS_GPIO_Port,KEY2_PIN)==0x0000){};
				}
				if(GPIO_ReadInputDataBit(KEYS_GPIO_Port,KEY1_PIN)==0x0000)
				{
						systick_delay_us(20);
						if(GPIO_ReadInputDataBit(KEYS_GPIO_Port,KEY1_PIN)==0x0000)
						{
							keyvalue=7;
						}						
						while(GPIO_ReadInputDataBit(KEYS_GPIO_Port,KEY1_PIN)==0x0000){};
				}
				if(GPIO_ReadInputDataBit(KEYS_GPIO_Port,KEY0_PIN)==0x0000)
				{
						systick_delay_us(20);
						if(GPIO_ReadInputDataBit(KEYS_GPIO_Port,KEY0_PIN)==0x0000)
						{
							keyvalue=8;
						}						
						while(GPIO_ReadInputDataBit(KEYS_GPIO_Port,KEY0_PIN)==0x0000){};
				}

				GPIO_Write(KEYS_GPIO_Port,0x00FF);
				GPIO_ResetBits(KEYS_GPIO_Port,KEY6_PIN);
				systick_delay_us(200);//延时200uS
				if(GPIO_ReadInputDataBit(KEYS_GPIO_Port,KEY3_PIN)==0x0000)
				{
						systick_delay_us(20);
						if(GPIO_ReadInputDataBit(KEYS_GPIO_Port,KEY3_PIN)==0x0000)
						{
							keyvalue=9;
						}						
						while(GPIO_ReadInputDataBit(KEYS_GPIO_Port,KEY3_PIN)==0x0000){};			
				}
				if(GPIO_ReadInputDataBit(KEYS_GPIO_Port,KEY2_PIN)==0x0000)
				{
						systick_delay_us(20);
						if(GPIO_ReadInputDataBit(KEYS_GPIO_Port,KEY2_PIN)==0x0000)
						{
							keyvalue=10;
						}						
						while(GPIO_ReadInputDataBit(KEYS_GPIO_Port,KEY2_PIN)==0x0000){};
				}
				if(GPIO_ReadInputDataBit(KEYS_GPIO_Port,KEY1_PIN)==0x0000)
				{
						systick_delay_us(20);
						if(GPIO_ReadInputDataBit(KEYS_GPIO_Port,KEY1_PIN)==0x0000)
						{
							keyvalue=11;
						}						
						while(GPIO_ReadInputDataBit(KEYS_GPIO_Port,KEY1_PIN)==0x0000){};
				}
				if(GPIO_ReadInputDataBit(KEYS_GPIO_Port,KEY0_PIN)==0x0000)
				{
						systick_delay_us(20);
						if(GPIO_ReadInputDataBit(KEYS_GPIO_Port,KEY0_PIN)==0x0000)
						{
							keyvalue=12;
						}						
						while(GPIO_ReadInputDataBit(KEYS_GPIO_Port,KEY0_PIN)==0x0000){};
				}
				
				
				GPIO_Write(KEYS_GPIO_Port,0x00FF);
				GPIO_ResetBits(KEYS_GPIO_Port,KEY7_PIN);
				systick_delay_us(200);//延时200uS
				if(GPIO_ReadInputDataBit(KEYS_GPIO_Port,KEY3_PIN)==0x0000)
				{
						systick_delay_us(20);
						if(GPIO_ReadInputDataBit(KEYS_GPIO_Port,KEY3_PIN)==0x0000)
						{
							keyvalue=13;
						}						
						while(GPIO_ReadInputDataBit(KEYS_GPIO_Port,KEY3_PIN)==0x0000){};					
				}
				if(GPIO_ReadInputDataBit(KEYS_GPIO_Port,KEY2_PIN)==0x0000)
				{
						systick_delay_us(20);
						if(GPIO_ReadInputDataBit(KEYS_GPIO_Port,KEY2_PIN)==0x0000)
						{
							keyvalue=14;
						}						
						while(GPIO_ReadInputDataBit(KEYS_GPIO_Port,KEY2_PIN)==0x0000){};
				}
				if(GPIO_ReadInputDataBit(KEYS_GPIO_Port,KEY1_PIN)==0x0000)
				{
						systick_delay_us(20);
						if(GPIO_ReadInputDataBit(KEYS_GPIO_Port,KEY1_PIN)==0x0000)
						{
							keyvalue=15;
						}						
						while(GPIO_ReadInputDataBit(KEYS_GPIO_Port,KEY1_PIN)==0x0000){};
				}
				if(GPIO_ReadInputDataBit(KEYS_GPIO_Port,KEY0_PIN)==0x0000)
				{
						systick_delay_us(20);
						if(GPIO_ReadInputDataBit(KEYS_GPIO_Port,KEY0_PIN)==0x0000)
						{
							keyvalue=16;
						}						
						while(GPIO_ReadInputDataBit(KEYS_GPIO_Port,KEY0_PIN)==0x0000){};
				}
		}
		return keyvalue;
}

//  *********************系统初始化函数*******************************
void system_init(void)
{
			GPIO_PinRemapConfig(GPIO_Remap_PD01, ENABLE); //PD1和PD0作为外部时钟端口
			//PD0 remapped on OSC_IN, PD1 remapped on OSC_OUT
		//Stm32_Clock_Init(RCC_PLLMul_16);//使用内部RC=8M的振荡时钟;8/2*16=64MHz
			Stm32_Clock_HSE(9);//外部时钟PLL初始化	//8*9=72M
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);	//使能AFIO时钟，UART3重映射时钟	
			GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE); //使能仿真SW模式
			NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
			systick_delay_init();//系统systick参数初始化
			SYS_io_init();//系统输入输出口初始化
			//system_spi_init();	//同步串行口初始化：由于液晶不支持同步串口，暂无法使用
			uart1_init(115200);
			//TIM3_Int_Init(10000/5,7199);
			systick_delay_init();		
			TIM2_CC4_43uS(100);	//采用43uS的PWM基准脉冲，输出音乐方波
			TIM3_Init_10ms();	//定时器3初始化，10mS的周期定时中断
}

// *****************定时器2初始化********************
// 利用定时器2的CH4(PA3)，采用快速PWM1模式播放音乐
// 注意：采用43uS的基本脉冲输出音乐电平信号
void TIM2_CC4_43uS(uint16_t par_pwm)
{
			//TIM2CLK = SystemCoreClock / 2
			/* Compute the prescaler value */
			TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
			TIM_OCInitTypeDef  TIM_OCInitStructure = { 0 };
			NVIC_InitTypeDef NVIC_InitStructure;

			RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);    /*使能定时器2的时钟 */

			/* Time base configuration */
			TIM_TimeBaseStructure.TIM_Period = par_pwm;
			TIM_TimeBaseStructure.TIM_Prescaler = 307;//614;
			TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;//基本定时器只有1(0).2(1).4(2)分频模式
			TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
			TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

			/* Output Compare Active Mode configuration: Channe4 */
			TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
			TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
			TIM_OCInitStructure.TIM_Pulse = par_pwm>>1;      //50%
			TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
			TIM_OC4Init(TIM2, &TIM_OCInitStructure);
			TIM_OC4PreloadConfig(TIM2, TIM_OCPreload_Disable);

			TIM_ARRPreloadConfig(TIM2, ENABLE);

			TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
			TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);  //使能指定的TIM2中断
			TIM_Cmd(TIM2, DISABLE);//ENABLE);//

			//中断优先级NVIC设置
			NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
			NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;  //TIM2中断
			NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //先占优先级0级
			NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;  //从优先级2级
			NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
			NVIC_Init(&NVIC_InitStructure);  //初始化NVIC寄存器
}

// ***********由于该液晶不支持SPI的上升沿或者下降沿数据锁存，暂无用********
void system_spi_init(void)//
{
			SPI_InitTypeDef SPI_InitStructure;	
			SPI_Cmd(SPI1, DISABLE);	//SPI1->CR1 &= ~0x0040;//
			SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex; 
			SPI_InitStructure.SPI_Mode = SPI_Mode_Master;                      
			SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;                  
			SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;		//SPI_CPOL_Low; 		//                      
			SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;	//SPI_CPHA_1Edge;   //                     
			SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;                          
			SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_16;
			SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB; 
			SPI_InitStructure.SPI_CRCPolynomial =0; 
			
			SPI_Init(SPI1,&SPI_InitStructure);                                                              
			SPI_Cmd(SPI1, ENABLE);	
}


//通用定时器3中断初始化
//这里时钟选择为APB1的2倍，而APB1为36M
//arr：自动重装值。
//psc：时钟预分频数
//这里使用的是定时器2!
void TIM3_Init_10ms(void)
{
			TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
			NVIC_InitTypeDef NVIC_InitStructure;

			RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); //时钟使能
	
			TIM_TimeBaseStructure.TIM_Period = 5624; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	
			TIM_TimeBaseStructure.TIM_Prescaler =256; //设置用来作为TIMx时钟频率除数的预分频值
			TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV4; //设置时钟分割:TDTS = Tck_tim
			TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
			TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //根据指定的参数初始化TIMx的时间基数单位
		 
			TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE ); //使能指定的TIM2中断,允许更新中断

			//中断优先级NVIC设置
			NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
			NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  //TIM3中断
			NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //先占优先级0级
			NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //从优先级3级
			NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
			NVIC_Init(&NVIC_InitStructure);  //初始化NVIC寄存器
			TIM_Cmd(TIM3, DISABLE);  //使能TIMx					 
}

void EXTI_Configuration(void)
{
			NVIC_InitTypeDef NVIC_InitStructure;	
			EXTI_InitTypeDef EXTI_InitStructure; 
			NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
			NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn|EXTI3_IRQn;	//设置通道
			NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;	//设置抢占优先级
			NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;	//响应优先级
			NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
			NVIC_Init(&NVIC_InitStructure);		
			
			// 清除EXTI线路挂起位
			EXTI_ClearITPendingBit(EXTI_Line3|EXTI_Line10);
			// 选择GPIO管脚用作外部中断线路
			GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource3|GPIO_PinSource10);	
			EXTI_InitStructure.EXTI_Line = EXTI_Line3|EXTI_Line10; 
			EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt; 
			EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling; 
			EXTI_InitStructure.EXTI_LineCmd = ENABLE;	
			// 初始化外设EXTI寄存器
			EXTI_Init(&EXTI_InitStructure);
	
}

void system_adc_init(uint8_t ch_n)//
{
			ADC_InitTypeDef ADC_InitStructure; 
			
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,ENABLE );	 //使能ADC1通道时钟		 

			RCC_ADCCLKConfig(RCC_PCLK2_Div6);   //设置ADC分频因子6 72M/6=12,ADC最大时间不能超过14M
			ADC_DeInit(ADC1);  //复位ADC1 

			ADC_RegularChannelConfig(ADC1, ch_n, 1, ADC_SampleTime_239Cycles5 );	//ADC1,ADC通道,采样时间为239.5周期	  			    
			ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;	//ADC工作模式:ADC1和ADC2工作在独立模式
			ADC_InitStructure.ADC_ScanConvMode = DISABLE;	//模数转换工作在单通道模式
			ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;	//模数转换工作在单次转换模式
			ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;	//转换由软件而不是外部触发启动
			ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;	//ADC数据右对齐
			ADC_InitStructure.ADC_NbrOfChannel = 1;	//顺序进行规则转换的ADC通道的数目
			ADC_Init(ADC1, &ADC_InitStructure);	//根据ADC_InitStruct中指定的参数初始化外设ADCx的寄存器   
			
			ADC_Cmd(ADC1, ENABLE);	//使能指定的ADC1			
			ADC_ResetCalibration(ADC1);	//使能复位校准			 
			while(ADC_GetResetCalibrationStatus(ADC1));	//等待复位校准结束			
			ADC_StartCalibration(ADC1);	 //开启AD校准		 
			while(ADC_GetCalibrationStatus(ADC1));	 //等待校准结束
 
//		ADC_SoftwareStartConvCmd(ADC1, ENABLE);		//使能指定的ADC1的软件转换启动功能
}


/********************************END OF FILE***********************************/


