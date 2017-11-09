/******************** (C) COPYRIGHT 2008 STMicroelectronics ********************
* File Name          : main.c
* Author             : MCD Application Team
* Version            : V2.0.3
* Date               : 09/22/2008
* Description        : Main program body
********************************************************************************
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_lib.h"
typedef enum
{
	KeyScanState_0=0x00,
	KeyScanState_1=0x01,
	KeyScanState_2=0x02,
}KeyScanState_Typedef;
#define KEYPORT GPIOA
#define KEY0PIN GPIO_Pin_0
#define KEY1PIN GPIO_Pin_1
#define KEY2PIN GPIO_Pin_2
#define KEY3PIN GPIO_Pin_3
#define LEDPORT GPIOA
#define LED0PIN GPIO_Pin_4                                                                              
#define LED1PIN GPIO_Pin_5
#define LED2PIN GPIO_Pin_6
#define LED3PIN GPIO_Pin_7
void RccInitialisation(void);
void GpioInitialisation(void);
void SystickInitialisation(void);
/**
  * @brief  主函数
  * @param  无
  * @retval 无
  */
int main(void)
{
	vu16 KeyPortStatus=0;
	KeyScanState_Typedef KeyScanState;
	RccInitialisation();
	GpioInitialisation();
	SystickInitialisation();
  	  while(1)
			{
				if(SysTick_GetFlagStatus(SysTick_FLAG_COUNT)==SET)
				{
					KeyPortStatus=GPIO_ReadInputData(KEYPORT)&0x000f;
					switch(KeyScanState)
				{
					case KeyScanState_0:
					{
						if(KeyPortStatus!=0x000f)
						{
							KeyScanState=KeyScanState_1;
						}
						break;
					}
					case KeyScanState_1:
					{
						if(KeyPortStatus)
						{
							if(GPIO_ReadInputDataBit(KEYPORT,KEY0PIN)==0)
							{
								GPIO_WriteBit(LEDPORT,LED0PIN,
								(BitAction)(1-GPIO_ReadInputDataBit(LEDPORT,LED0PIN)));}
								else if(GPIO_ReadInputDataBit(KEYPORT,KEY1PIN)==0)
								{
									GPIO_WriteBit(LEDPORT,LED1PIN,
								(BitAction)(1-GPIO_ReadInputDataBit(LEDPORT,LED1PIN)));}
							else if(GPIO_ReadInputDataBit(KEYPORT,KEY2PIN)==0)
							{
									GPIO_WriteBit(LEDPORT,LED2PIN,
								(BitAction)(1-GPIO_ReadInputDataBit(LEDPORT,LED2PIN)));}
							else if(GPIO_ReadInputDataBit(KEYPORT,KEY3PIN)==0)
							{
								GPIO_WriteBit(LEDPORT,LED3PIN,
								(BitAction)(1-GPIO_ReadInputDataBit(LEDPORT,LED3PIN)));}
								KeyScanState=KeyScanState_2;
							}
							else{
								KeyScanState=KeyScanState_0;
							}
							break;
						}
						case KeyScanState_2:
						{
							if(KeyPortStatus==0x000f)
							{
								KeyScanState=KeyScanState_0;
							}
							break;
						}}}}}
		/* add your code here ^_^. */
void RccInitialisation(void)
{
	ErrorStatus HSEStartUpstatus;
	RCC_DeInit();
	RCC_HSEConfig(RCC_HSE_ON);
	HSEStartUpstatus = RCC_WaitForHSEStartUp();
	if(HSEStartUpstatus == SUCCESS)
	{
		RCC_HCLKConfig(RCC_SYSCLK_Div1);
		RCC_HCLKConfig(RCC_HCLK_Div1);
    RCC_HCLKConfig(RCC_HCLK_Div2);
    FLASH_SetLatency(FLASH_Latency_2);
		FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);
		RCC_PLLConfig(RCC_PLLSource_HSE_Div1,RCC_PLLMul_9);
		RCC_PLLCmd(ENABLE);
		while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET);
		RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
		while(RCC_GetSYSCLKSource() != 0x08);
	}
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
}	
void GpioInitialisation(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin=KEY0PIN|KEY1PIN|KEY2PIN|KEY3PIN;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU;
	GPIO_Init(KEYPORT,&GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin=LED0PIN|LED1PIN|LED2PIN|LED3PIN;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(LEDPORT , &GPIO_InitStructure);
}
void SystickInitialisation(void)
{
	SysTick_CounterCmd(SysTick_Counter_Disable);
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK);
	SysTick_CounterCmd(SysTick_Counter_Clear);
	SysTick_SetReload(72000*20);
	SysTick_CounterCmd(SysTick_Counter_Enable);
}

	
	
		
	
	

/*********************************************END OF FILE**********************/

