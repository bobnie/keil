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
u32 STM32IdHigh = 0;
u32 STM32IdMed = 0;
u32 STM32IdLow = 0;
void RccInitialisation(void);

/**
  * @brief  主函数
  * @param  无
  * @retval 无
  */
int main(void)
{
	RccInitialisation();
	STM32IdLow = *((u32*)0x1FFFF7E8);
	STM32IdMed = *((u32*)0x1FFFF7EC);
	STM32IdHigh = *((u32*)0x1FFFF7F0);
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
}	
		
	
	

/*********************************************END OF FILE**********************/

