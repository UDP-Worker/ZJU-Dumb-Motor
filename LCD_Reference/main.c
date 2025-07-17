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
	
//Ҫд�뵽STM32 FLASH���ַ�������
#define FLASH_SAVE_ADDR  0x0800F800		//0x8000 0000-0x8001 0000
//����FLASH �����ַ(����Ϊż����
//����ֵҪ���ڱ�������ռ��FLASH�Ĵ�С+0X08000000)	


void ADC1_2_IRQHandler(void)
{
			//ADC_SoftwareStartConvCmd(ADC1, ENABLE);		//ʹ��ָ����ADC1�����ת����������	
			EXTI_ClearITPendingBit(EXTI_IMR_MR11);	
}

void EXTI3_IRQHandler(void)
{
			//system_init();
			EXTI_ClearITPendingBit(EXTI_Line3);  //���LINE3�ϵ��жϱ�־λ  
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
//			DMA1_Channel1->CNDTR = DATA_LENGTH;	 //���ô������ݳ���
//			DMA1_Channel1->CCR |= 0x00001;//Channel Enabled		
}

void TIM2_IRQHandler(void)
{	
			TIM2->SR &= ~0x000F;	//�����ʱ��2��ȫ���жϱ�־
			//TIM_ClearITPendingBit(TIM2, TIM_IT_CC4);
			//TIM_ClearITPendingBit(TIM2, TIM_IT_Update);  //���TIMx�����жϱ�־ 
}

// *********����ʱ��3���ӳٺ����STOPģʽ*************
void TIM3_IRQHandler(void)
{
			music_count += 1; 
			//Count++; 
			TIM_ClearITPendingBit(TIM3, TIM_IT_Update);  //���TIMx�����жϱ�־ 
}

// ******************�첽����1ģ��Ľ������ݴ���******************
void USART1_IRQHandler(void)
{
		if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
		{	
			rx_buf[rx_serial++] =  USART_ReceiveData(USART1);	//��ȡ���յ�������
			if(rx_serial>RX_BUF_LENGTH)
				rx_serial = 0;
		}	
} 

// ******************����2�����ݴ���******************************
void USART2_IRQHandler(void)
{
		if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET){}
} 

// *****************������*************************
int main(void)
{
			system_init();
			//uart_exti_init();
			LCD_PSB_LOW;	//0�����ô��нӿڷ�ʽ����Һ����ʾ				
			LCD_RST_HIGH;	
			lcd_port_init();
//			CLEARGDRAM(0x0000);
			Disp(1,0,16,"�㽭��ѧ���ѧԺ");//��ʾ���ݵ�LCD12864�ӳ���			
			Disp(2,1,12,"������·���");//��ʾ���ݵ�LCD12864�ӳ���
			Disp(3,2,8,"87951197");//��ʾ���ݵ�LCD12864�ӳ���
			Disp(4,1,12,"Һ����ʾ�ɹ�");//��ʾ���ݵ�LCD12864�ӳ���

//			lcd_cmd = 0x32;
//			write_com(0x01);//�����ʾ	
//			while(1)
//			{
//				write_com(0x02);//�α��λ		
//				systick_delay_us(4600);//4.6mS
//				write_com(0x04);//λַ��λ		
//				systick_delay_us(4600);//4.6mS
//				//write_com(0x32);//����ָ�����
//				write_com(lcd_cmd);//����ָ�����
//				systick_delay_us(4600);//4.6mS
////				write_com(0x03);//����ָ�-����ֱ����ַ
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
							CLEARGDRAM(0x0000);//���Һ����ʾ
							Disp(1,0,16,"�㽭��ѧ���ѧԺ");//��ʾ���ݵ�LCD12864�ӳ���			
							Disp(2,1,12,"������·���");//��ʾ���ݵ�LCD12864�ӳ���
							Disp(3,2,8,"87951197");//��ʾ���ݵ�LCD12864�ӳ���
							Disp(4,1,12,"Һ����ʾ�ɹ�");//��ʾ���ݵ�LCD12864�ӳ���
				}
				switch(ctrl_current_status)
				{
					case 0x00:		//������ȫ��
							CLEARGDRAM(0x0000);//Һ����ʼ���ӳ���
							Disp(1,0,16,"ϵͳ���а������");//��ʾ���ݵ�LCD12864�ӳ���
							Disp(3,2,8,"87951197");//��ʾ���ݵ�LCD12864�ӳ���
							Disp(4,0,16,"�㽭��ѧ���ѧԺ");//��ʾ���ݵ�LCD12864�ӳ���			
							do
							{
										sys_sw_led_check();
										key=scankey();
										switch(key)
										{
											case 0x01:Disp(2,1,12,"KEY_VALUE:01");	break;//��ʾ���ݵ�LCD12864�ӳ���
											case 0x02:Disp(2,1,12,"KEY_VALUE:02");	break;//��ʾ���ݵ�LCD12864�ӳ���
											case 0x03:Disp(2,1,12,"KEY_VALUE:03");	break;//��ʾ���ݵ�LCD12864�ӳ���
											case 0x04:Disp(2,1,12,"KEY_VALUE:04");	break;//��ʾ���ݵ�LCD12864�ӳ���
											case 0x05:Disp(2,1,12,"KEY_VALUE:05");	break;//��ʾ���ݵ�LCD12864�ӳ���
											case 0x06:Disp(2,1,12,"KEY_VALUE:06");	break;//��ʾ���ݵ�LCD12864�ӳ���
											case 0x07:Disp(2,1,12,"KEY_VALUE:07");	break;//��ʾ���ݵ�LCD12864�ӳ���
											case 0x08:Disp(2,1,12,"KEY_VALUE:08");	break;//��ʾ���ݵ�LCD12864�ӳ���
											case 0x09:Disp(2,1,12,"KEY_VALUE:09");	break;//��ʾ���ݵ�LCD12864�ӳ���
											case 0x0A:Disp(2,1,12,"KEY_VALUE:10");	break;//��ʾ���ݵ�LCD12864�ӳ���
											case 0x0B:Disp(2,1,12,"KEY_VALUE:11");	break;//��ʾ���ݵ�LCD12864�ӳ���
											case 0x0C:Disp(2,1,12,"KEY_VALUE:12");	break;//��ʾ���ݵ�LCD12864�ӳ���
											case 0x0D:Disp(2,1,12,"KEY_VALUE:13");	break;//��ʾ���ݵ�LCD12864�ӳ���
											case 0x0E:Disp(2,1,12,"KEY_VALUE:14");	break;//��ʾ���ݵ�LCD12864�ӳ���
											case 0x0F:Disp(2,1,12,"KEY_VALUE:15");	break;//��ʾ���ݵ�LCD12864�ӳ���
											case 0x10:Disp(2,1,12,"KEY_VALUE:16");	break;//��ʾ���ݵ�LCD12864�ӳ���
											default:break;											
										}
										ctrl_current_status = (ctrl_port_check()>>11)&0x03;
								}while(ctrl_current_status == 0x00);
								break;
					case 0x01:
								CLEARGDRAM(0x0000);//Һ����ʼ���ӳ���
								Disp(1,0,16,"ϵͳ�������ֲ���");//��ʾ���ݵ�LCD12864�ӳ���
								Disp(2,1,13,"Playing Music");//��ʾ���ݵ�LCD12864�ӳ���  
								Disp(3,2,8,"87951197");//��ʾ���ݵ�LCD12864�ӳ���
								Disp(4,0,16,"�㽭��ѧ���ѧԺ");//��ʾ���ݵ�LCD12864�ӳ���		
								//TIM3_Int_Init(100,3599);//10Khz�ļ���Ƶ��:8*9/2=36M;36000000/3600=10000
								Play_Song(0);			 
								break;
					case 0x02:
								CLEARGDRAM(0x0000);
								Disp(1,0,16,"ϵͳ���й�����");//��ʾ���ݵ�LCD12864�ӳ���				
								ShowQQChar(0x88,"                    ",16);
								ShowQQChar(0x90,"                    ",16);
								ShowQQChar(0x98,"                    ",16);
								do
								{
									 sys_sw_led_check();
									 if(OPT_CHECK&0x0100)
									 {
										 ShowQQChar(0x91,"1:��",2);
									 }
									 else
									 {
										 ShowQQChar(0x91,"1:��",2);							 
									 }
									 if(OPT_CHECK&0x0200)
									 {
										 ShowQQChar(0x93,"2:��",2);
									 }
									 else
									 {
										 ShowQQChar(0x93,"2:��",2);
									 }						 
									 if(OPT_CHECK&0x0400)
									 {
										 ShowQQChar(0x95,"3:��",2);
									 }
									 else
									 {
										 ShowQQChar(0x95,"3:��",2);
									 }
									 
									 if(OPT_CHECK&0x0800)
									 {
										 ShowQQChar(0x89,"4:��",2);
									 }
									 else
									 {
										 ShowQQChar(0x89,"4:��",2);
									 }						 
									 if(OPT_CHECK&0x1000)
									 {
										 ShowQQChar(0x8B,"5:��",2);
									 }
									 else
									 {
										 ShowQQChar(0x8B,"5:��",2);
									 }
									 
									 if(OPT_CHECK&0x2000)
									 {
										 ShowQQChar(0x8D,"6:��",2);
									 }
									 else
									 {
										 ShowQQChar(0x8D,"6:��",2);
									 }
									 
									 if(OPT_CHECK&0x4000)
									 {
										 ShowQQChar(0x9B,"7:��",2);
									 }
									 else
									 {
										 ShowQQChar(0x9B,"7:��",2);
									 }
									 systick_delay_us(100000);
									 ctrl_current_status = (ctrl_port_check()>>11)&0x03;
								}while(ctrl_current_status == 0x02);
								CLEARGDRAM(0x0000);//Һ����ʼ���ӳ���
								break;
					default:
						break;
				}
			}
}

/**************************��������*************************/
void Play_Song(uint8_t i)
{
			 uint16_t Temp1;//,temp2;
			 uint32_t Addr;
			 music_count = 0; 
			 Addr = i * 217;
			 TIM2->CCMR2 = 0x6000;	//����ʱ��2��CH4ͨ����Ϊ����PWMģʽ			 
			 do
			 {
					 //sys_sw_led_check();
					 Temp1 = SONG[Addr++];//����Ƶ�ʲ���
					 if ( Temp1 == 0xFF ) //��ֹ��        
					 {
									TIM2->CCMR2 &= 0x2000;	//ǿ�Ƶ͵�ƽTIM_OCMode_Inactive			
									systick_delay_us(100000);
									TIM2->CCMR2 |= 0x2000;	//��ԭΪTIM_PWM1 MODE
					 }
					 else if ( Temp1 == 0x00 ) //����������  
					 {
							TIM_Cmd(TIM2, DISABLE);	//�رն�ʱ��3
							TIM2->CCMR2 &= 0x2000;	//ǿ�Ƶ͵�ƽTIM_OCMode_Inactive			 
							return;
					 }
					 else
					 {
								//TIM2_CC4_10mS(Temp1*32);	
								//b_curr_que = 0;	
								TIM2->ARR = Temp1<<1;//��ʱ���������ź�
								TIM2->CCR4 = Temp1;//<<7;//PWMռ�ձ�Ϊ50%
								TIM2->CR1 |= TIM_CR1_CEN;				 
								TIM3->CR1 |= TIM_CR1_CEN;
						 		music_timers = SONG[Addr++];	//�������
						  //systick_delay_ms(Temp1);
								while(1)
								{
									if(music_count>=music_timers)
									{
										music_count = 0;
										break;
									}							
								}						 
								ctrl_current_status = ((GPIOA->IDR & 0x1800)>>11)&0x03;//��ؿ��ƶ˿ڵ�״̬�Ƿ��ǲ���ģʽ
								if(ctrl_current_status != 0x01) break;//���˳�
					 }
			 }while(ctrl_current_status == 0x01);
			 TIM3->CR1 &= ~TIM_CR1_CEN;		//�رն�ʱ��3				 
			 TIM2->CR1 &= ~TIM_CR1_CEN;		//�رն�ʱ��2				 
			 TIM2->CCMR2 &= ~0x2000;	//ǿ�Ƶ͵�ƽTIM_OCMode_Inactive			 
}


// *****************�����ƶ˿ڵ�״̬**************
uint16_t ctrl_port_check(void)
{
			return 	GPIO_ReadInputData(GPIOA) & 0x1800;		
}

// **********�����ƶ˿ڣ�����ϵͳ����ģʽ**********
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

// ********ɨ�谴������������ļ�ֵ******************
uint8_t scankey(void)//������̷�תɨ��
{
		uint keyvalue=0;
		GPIO_WriteBit(KEYS_GPIO_Port,0x000F,Bit_RESET);
		//GPIO_Write(KEYS_GPIO_Port,0x00F0);
		systick_delay_us(20);
		if(GPIO_ReadInputDataBit(KEYINT_GPIO_Port,KEYINT))		//����Ƿ��а�������
		{
			return 0x0000;
		}
		else	
		{
				GPIO_WriteBit(KEYS_GPIO_Port,0x00FF,Bit_SET);
				GPIO_ResetBits(KEYS_GPIO_Port,KEY4_PIN);
				systick_delay_us(200);//��ʱ200uS
			
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
				systick_delay_us(200);//��ʱ200uS
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
				systick_delay_us(200);//��ʱ200uS
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
				systick_delay_us(200);//��ʱ200uS
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

//  *********************ϵͳ��ʼ������*******************************
void system_init(void)
{
			GPIO_PinRemapConfig(GPIO_Remap_PD01, ENABLE); //PD1��PD0��Ϊ�ⲿʱ�Ӷ˿�
			//PD0 remapped on OSC_IN, PD1 remapped on OSC_OUT
		//Stm32_Clock_Init(RCC_PLLMul_16);//ʹ���ڲ�RC=8M����ʱ��;8/2*16=64MHz
			Stm32_Clock_HSE(9);//�ⲿʱ��PLL��ʼ��	//8*9=72M
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);	//ʹ��AFIOʱ�ӣ�UART3��ӳ��ʱ��	
			GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE); //ʹ�ܷ���SWģʽ
			NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
			systick_delay_init();//ϵͳsystick������ʼ��
			SYS_io_init();//ϵͳ��������ڳ�ʼ��
			//system_spi_init();	//ͬ�����пڳ�ʼ��������Һ����֧��ͬ�����ڣ����޷�ʹ��
			uart1_init(115200);
			//TIM3_Int_Init(10000/5,7199);
			systick_delay_init();		
			TIM2_CC4_43uS(100);	//����43uS��PWM��׼���壬������ַ���
			TIM3_Init_10ms();	//��ʱ��3��ʼ����10mS�����ڶ�ʱ�ж�
}

// *****************��ʱ��2��ʼ��********************
// ���ö�ʱ��2��CH4(PA3)�����ÿ���PWM1ģʽ��������
// ע�⣺����43uS�Ļ�������������ֵ�ƽ�ź�
void TIM2_CC4_43uS(uint16_t par_pwm)
{
			//TIM2CLK = SystemCoreClock / 2
			/* Compute the prescaler value */
			TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
			TIM_OCInitTypeDef  TIM_OCInitStructure = { 0 };
			NVIC_InitTypeDef NVIC_InitStructure;

			RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);    /*ʹ�ܶ�ʱ��2��ʱ�� */

			/* Time base configuration */
			TIM_TimeBaseStructure.TIM_Period = par_pwm;
			TIM_TimeBaseStructure.TIM_Prescaler = 307;//614;
			TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;//������ʱ��ֻ��1(0).2(1).4(2)��Ƶģʽ
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
			TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);  //ʹ��ָ����TIM2�ж�
			TIM_Cmd(TIM2, DISABLE);//ENABLE);//

			//�ж����ȼ�NVIC����
			NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
			NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;  //TIM2�ж�
			NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //��ռ���ȼ�0��
			NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;  //�����ȼ�2��
			NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
			NVIC_Init(&NVIC_InitStructure);  //��ʼ��NVIC�Ĵ���
}

// ***********���ڸ�Һ����֧��SPI�������ػ����½����������棬������********
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


//ͨ�ö�ʱ��3�жϳ�ʼ��
//����ʱ��ѡ��ΪAPB1��2������APB1Ϊ36M
//arr���Զ���װֵ��
//psc��ʱ��Ԥ��Ƶ��
//����ʹ�õ��Ƕ�ʱ��2!
void TIM3_Init_10ms(void)
{
			TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
			NVIC_InitTypeDef NVIC_InitStructure;

			RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); //ʱ��ʹ��
	
			TIM_TimeBaseStructure.TIM_Period = 5624; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	
			TIM_TimeBaseStructure.TIM_Prescaler =256; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ
			TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV4; //����ʱ�ӷָ�:TDTS = Tck_tim
			TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
			TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //����ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
		 
			TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE ); //ʹ��ָ����TIM2�ж�,��������ж�

			//�ж����ȼ�NVIC����
			NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
			NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  //TIM3�ж�
			NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //��ռ���ȼ�0��
			NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //�����ȼ�3��
			NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
			NVIC_Init(&NVIC_InitStructure);  //��ʼ��NVIC�Ĵ���
			TIM_Cmd(TIM3, DISABLE);  //ʹ��TIMx					 
}

void EXTI_Configuration(void)
{
			NVIC_InitTypeDef NVIC_InitStructure;	
			EXTI_InitTypeDef EXTI_InitStructure; 
			NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
			NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn|EXTI3_IRQn;	//����ͨ��
			NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;	//������ռ���ȼ�
			NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;	//��Ӧ���ȼ�
			NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
			NVIC_Init(&NVIC_InitStructure);		
			
			// ���EXTI��·����λ
			EXTI_ClearITPendingBit(EXTI_Line3|EXTI_Line10);
			// ѡ��GPIO�ܽ������ⲿ�ж���·
			GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource3|GPIO_PinSource10);	
			EXTI_InitStructure.EXTI_Line = EXTI_Line3|EXTI_Line10; 
			EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt; 
			EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling; 
			EXTI_InitStructure.EXTI_LineCmd = ENABLE;	
			// ��ʼ������EXTI�Ĵ���
			EXTI_Init(&EXTI_InitStructure);
	
}

void system_adc_init(uint8_t ch_n)//
{
			ADC_InitTypeDef ADC_InitStructure; 
			
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,ENABLE );	 //ʹ��ADC1ͨ��ʱ��		 

			RCC_ADCCLKConfig(RCC_PCLK2_Div6);   //����ADC��Ƶ����6 72M/6=12,ADC���ʱ�䲻�ܳ���14M
			ADC_DeInit(ADC1);  //��λADC1 

			ADC_RegularChannelConfig(ADC1, ch_n, 1, ADC_SampleTime_239Cycles5 );	//ADC1,ADCͨ��,����ʱ��Ϊ239.5����	  			    
			ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;	//ADC����ģʽ:ADC1��ADC2�����ڶ���ģʽ
			ADC_InitStructure.ADC_ScanConvMode = DISABLE;	//ģ��ת�������ڵ�ͨ��ģʽ
			ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;	//ģ��ת�������ڵ���ת��ģʽ
			ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;	//ת��������������ⲿ��������
			ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;	//ADC�����Ҷ���
			ADC_InitStructure.ADC_NbrOfChannel = 1;	//˳����й���ת����ADCͨ������Ŀ
			ADC_Init(ADC1, &ADC_InitStructure);	//����ADC_InitStruct��ָ���Ĳ�����ʼ������ADCx�ļĴ���   
			
			ADC_Cmd(ADC1, ENABLE);	//ʹ��ָ����ADC1			
			ADC_ResetCalibration(ADC1);	//ʹ�ܸ�λУ׼			 
			while(ADC_GetResetCalibrationStatus(ADC1));	//�ȴ���λУ׼����			
			ADC_StartCalibration(ADC1);	 //����ADУ׼		 
			while(ADC_GetCalibrationStatus(ADC1));	 //�ȴ�У׼����
 
//		ADC_SoftwareStartConvCmd(ADC1, ENABLE);		//ʹ��ָ����ADC1�����ת����������
}


/********************************END OF FILE***********************************/


