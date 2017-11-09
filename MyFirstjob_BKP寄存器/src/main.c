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
#include "stdio.h"
#include "stm32f10x_it.h"
void RccInitialisation(void);
void GpioInitialisation(void);
void NvicInitialisation(void);
void UsartInitialisation(void);
void PrintBackupReg(void);
void BkpInitialisation(void);
void WriteToBackupReg(u16 FirstBackupData);
u8 CheckBackupReg(u16 FirstBackupData);
/**
  * @brief  主函数
  * @param  无
  * @retval 无
  */
int main(void)
{
	RccInitialisation();
	GpioInitialisation();
	NvicInitialisation();
	UsartInitialisation();
	BkpInitialisation();
	if(CheckBackupReg(0xA53C)==0x00)
	{
		printf("\r\nThe datas are as their initial status. \r\n");
		printf("\n\r\n\r");
		PrintBackupReg();
	}
	else
	{
		printf("\r\nThe datas have been changed. \r\n");
		BKP_ClearFlag();
		WriteToBackupReg(0xA53C);
		printf("\r\nRecover the datas of DRX to their initial status. \r\n");
		PrintBackupReg();
	}
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
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA,ENABLE);
}	
void GpioInitialisation(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
}
void NvicInitialisation(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel=TAMPER_IRQChannel;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0;
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}
void BkpInitialisation(void)
{
	PWR_BackupAccessCmd(ENABLE);
	BKP_ClearFlag();
	BKP_TamperPinLevelConfig(BKP_TamperPinLevel_Low);
	BKP_ITConfig(ENABLE);
	BKP_TamperPinCmd(ENABLE);
}
void WriteToBackupReg(u16 FirstBackupData)
{
	BKP_WriteBackupRegister(BKP_DR1  , FirstBackupData);
	BKP_WriteBackupRegister(BKP_DR2  , BKP->DR1 + 0x5A);
	BKP_WriteBackupRegister(BKP_DR2  , BKP->DR2 + 0x5A);
	BKP_WriteBackupRegister(BKP_DR2  , BKP->DR3 + 0x3C);
	BKP_WriteBackupRegister(BKP_DR2  , BKP->DR4 + 0x06);
	BKP_WriteBackupRegister(BKP_DR2  , BKP->DR5 + 0x78);
	BKP_WriteBackupRegister(BKP_DR2  , BKP->DR6 + 0xFF);
	BKP_WriteBackupRegister(BKP_DR2  , BKP->DR7 + 0xB4);
	BKP_WriteBackupRegister(BKP_DR2  , BKP->DR8 + 0x1E);
	BKP_WriteBackupRegister(BKP_DR2  , BKP->DR9 + 0xD4);
}
u8 CheckBackupReg(u16 FirstBackupData)
{
	if(BKP_ReadBackupRegister(BKP_DR1)  !=FirstBackupData) return 1;
	if(BKP_ReadBackupRegister(BKP_DR2)  !=(BKP->DR1 + 0x5A))return 2;
	if(BKP_ReadBackupRegister(BKP_DR3)  !=(BKP->DR1 + 0x5A))return 3;
	if(BKP_ReadBackupRegister(BKP_DR4)  !=(BKP->DR1 + 0x5A))return 4;
	if(BKP_ReadBackupRegister(BKP_DR5)  !=(BKP->DR1 + 0x5A))return 5;
	if(BKP_ReadBackupRegister(BKP_DR6)  !=(BKP->DR1 + 0x5A))return 6;
	if(BKP_ReadBackupRegister(BKP_DR7)  !=(BKP->DR1 + 0x5A))return 7;
	if(BKP_ReadBackupRegister(BKP_DR8)  !=(BKP->DR1 + 0x5A))return 8;
	if(BKP_ReadBackupRegister(BKP_DR9)  !=(BKP->DR1 + 0x5A))return 9;
	if(BKP_ReadBackupRegister(BKP_DR10)  !=(BKP->DR1 + 0x5A))return 10;
	return 0;
}
u8 IsBackupRegReset(void)
{
	if(BKP_ReadBackupRegister(BKP_DR1)  != 0x0000)return 1;
	if(BKP_ReadBackupRegister(BKP_DR2)  != 0x0000)return 2;
	if(BKP_ReadBackupRegister(BKP_DR3)  != 0x0000)return 3;
	if(BKP_ReadBackupRegister(BKP_DR4)  != 0x0000)return 4;
	if(BKP_ReadBackupRegister(BKP_DR5)  != 0x0000)return 5;
	if(BKP_ReadBackupRegister(BKP_DR6)  != 0x0000)return 6;
	if(BKP_ReadBackupRegister(BKP_DR7)  != 0x0000)return 7;
	if(BKP_ReadBackupRegister(BKP_DR8)  != 0x0000)return 8;
	if(BKP_ReadBackupRegister(BKP_DR9)  != 0x0000)return 9;
	if(BKP_ReadBackupRegister(BKP_DR10)  != 0x0000)return 10;
	return 0;
}
void PrintBackupReg(void)
{
	printf("\nNow the data in DRx are:\r\n");
	printf("DR1  =0x%04X\t"  , BKP_ReadBackupRegister(BKP_DR1));
	printf("DR2  =0x%04X\t"  , BKP_ReadBackupRegister(BKP_DR2));
	printf("DR3  =0x%04X\t"  , BKP_ReadBackupRegister(BKP_DR3));
	printf("DR4  =0x%04X\t"  , BKP_ReadBackupRegister(BKP_DR4));
	printf("DR5  =0x%04X\t"  , BKP_ReadBackupRegister(BKP_DR5));
	printf("DR6  =0x%04X\t"  , BKP_ReadBackupRegister(BKP_DR6));
	printf("DR7  =0x%04X\t"  , BKP_ReadBackupRegister(BKP_DR7));
	printf("DR8  =0x%04X\t"  , BKP_ReadBackupRegister(BKP_DR8));
	printf("DR9  =0x%04X\t"  , BKP_ReadBackupRegister(BKP_DR9));
	printf("DR10=0x%04X\t"  , BKP_ReadBackupRegister(BKP_DR10));
}
void UsartInitialisation(void)
{
	USART_InitTypeDef USART_InitStructure;
	USART_ClockInitTypeDef USART_ClockInitStructure;
        USART_InitStructure.USART_BaudRate=9600;
	USART_InitStructure.USART_WordLength=USART_WordLength_8b;
	USART_InitStructure.USART_Parity=USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl=USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode=USART_Mode_Rx|USART_Mode_Tx;
	USART_Init(USART1,&USART_InitStructure);
	USART_Cmd(USART1,ENABLE);
}
int fputc(int ch,FILE *f)	
{
	USART_SendData(USART1, (u8)ch);
	while(USART_GetFlagStatus(USART1,USART_FLAG_TC)==RESET);
	return ch;
}


		
	
	

/*********************************************END OF FILE**********************/

