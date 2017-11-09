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
#define Delay(n) while((n)--)
u32 STM32IdHigh = 0;
u32 STM32IdMed = 0;
u32 STM32IdLow = 0;
void RccInitialisation(void);
void GPIO_Configuration(void);
/**
  * @brief  主函数
  * @param  无
  * @retval 无
  */
int main(void)
{
	vu32 n =2000000;
	RccInitialisation();
	GPIO_Configuration();
	GPIO_SetBits(GPIOA , GPIO_Pin_2);
	GPIO_SetBits(GPIOA , GPIO_Pin_3);
	Delay(n);
	GPIO_ResetBits(GPIOA , GPIO_Pin_2);
	GPIO_ResetBits(GPIOA , GPIO_Pin_3);
  	  while(1);
	  /* add your code here ^_^. */
}
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
void GPIO_Configuration(void)
{ 
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin =GPIO_Pin_2 | GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOA , &GPIO_InitStructure);
} 
		
	
	

/*********************************************END OF FILE**********************/

