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
	delay_init(72);	          //ÑÓÊ±³õÊ¼»¯
	NVIC_Configuration();
	uart_init(9600);	 //´®¿Ú³õÊ¼»¯
 	LED_Init();		            //LED¶Ë¿Ú³õÊ¼»¯
	KEY_Init();               //°´¼ü¶Ë¿Ú³õÊ¼»¯
	Timerx_Init(500,7199);   //10KhzµÄ¼ÆÊıÆµÂÊ£¬¼ÆÊıµ½5000Îª500ms 
	UltrasonicWave_Configuration();	 //¶Ô³¬Éù²¨Ä£¿é³õÊ¼»
	 
	while(1)
	{
		UltrasonicWave_StartMeasure();		//¿ªÊ¼²â¾à£¬·¢ËÍÒ»¸ö>10usµÄÂö³å£¬È»ºó²âÁ¿·µ»ØµÄ¸ßµçÆ½Ê±¼ä
		beeph_Init();
	}
	
 }

