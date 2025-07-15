#include "stm32f10x.h"
#include "delay.h"
#include "motor.h"
#include "keysacn.h"
#include "IRSEARCH.h"
#include "IRAvoid.h"

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
	 TIM4_PWM_Init(7199,0);  //初始化PWM
	 ZYSTM32_brake(500);
	 keysacn();		
		while(1)
		{  
		 AVoidRun();  //避障函数
		}
 }

