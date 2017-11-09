#include "delay.h"
#include "sys.h"
#include "key.h"
#include "usart.h"
#include "exti.h"
#include "timer.h"
#include "UltrasonicWave.h"
#include "beep.h"
#include "led.h"
void BEEP_Init(void);
uint8_t BeepFlag=1;
 int main(void)
 {
 // SystemInit();
	delay_init(72);	          //��ʱ��ʼ��
	NVIC_Configuration();
	uart_init(9600);	 //���ڳ�ʼ��
 	LED_Init();		            //LED�˿ڳ�ʼ��
	KEY_Init();               //�����˿ڳ�ʼ��
	Timerx_Init(500,7199);   //10Khz�ļ���Ƶ�ʣ�������5000Ϊ500ms 
	UltrasonicWave_Configuration();	 //�Գ�����ģ���ʼ�
	 
	while(1)
	{
		UltrasonicWave_StartMeasure();		//��ʼ��࣬����һ��>10us�����壬Ȼ��������صĸߵ�ƽʱ��
		beeph_Init();
	}
	
 }

