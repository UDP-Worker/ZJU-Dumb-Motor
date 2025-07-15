#include "stm32f10x.h"
#include "delay.h"
#include "motor.h"
#include "keysacn.h"
#include "IRSEARCH.h"
#include "IRAvoid.h"

/*
void ZYSTM32_run(int speed,int time);       //ǰ������
void ZYSTM32_brake(int time);               //ɲ������
void ZYSTM32_Left(int speed,int time);      //��ת����
void ZYSTM32_Spin_Left(int speed,int time); //����ת����
void ZYSTM32_Right(int speed,int time);     //��ת����
void ZYSTM32_Spin_Right(int speed,int time);//����ת����
void ZYSTM32_back(int speed,int time);      //���˺���

*/


 int main(void)
 {	
	 delay_init();
	 KEY_Init();
	 IRSearchInit();
	 IRAvoidInit();
	 TIM4_PWM_Init(7199,0);  //��ʼ��PWM
	 ZYSTM32_brake(500);
	 keysacn();		
		while(1)
		{  
		 AVoidRun();  //���Ϻ���
		}
 }

