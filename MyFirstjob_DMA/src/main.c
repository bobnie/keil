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
#include "string.h"
#include "stm32f10x_it.h"
#define BufferSize  32
vu16 CurrDataCounter=0;
vu32 Tick=0;
u32 SRC_Const_Buffer[BufferSize]=
{
	0x01020304,0x05060708,0x090A0B0C,0x0D0E0F10,
	0x11121314,0x15161718,0x191A1B1C,0x1D1E1F20,
	0x21222324,0x25262728,0x292A2B2C,0x2D2E2F30,
	0x31323334,0x35363738,0x393A3B3C,0x3D3E3F40,
	0x41424244,0x45464748,0x494A4B4C,0x4D4E4F50,
	0x51525254,0x55565758,0x595A5B5C,0x5D5E5F60,
	0x61626264,0x65666768,0x696A6B6C,0x6D6E6F70,
	0x71727374,0x75767778,0x797A7b7C,0x7D76EF80
};
u32 DST_Buffer[BufferSize];
void RccInitialisation(void);
void NvicInitialisation(void);
void GpioInitialisation(void);
void UsartInitialisation(void);
void DmaInitialisation(void);
void SystickInitialisation(void);
/**
  * @brief  主函数
  * @param  无
  * @retval 无
  */
int main(void)
{
	u8 i=0;
	u8 TickCntCPU=0;
  u8 TickCntDMA=0;
	RccInitialisation();
	NvicInitialisation();
	GpioInitialisation();
	UsartInitialisation();
	DmaInitialisation();
	SystickInitialisation();
	Tick=0;
	for(i=0;i<BufferSize;i++)
	{
		DST_Buffer[i]=SRC_Const_Buffer[i];
	}
	TickCntCPU=Tick;
	for(i=0;i<BufferSize;i++)
	{
		DST_Buffer[i]=0;
	}
	Tick=0;
	DMA_Cmd(DMA1_Channel6,ENABLE);
	while(CurrDataCounter !=0);
	TickCntDMA=Tick;
	if(strncmp((const char *)SRC_Const_Buffer,(const char *)DST_Buffer,BufferSize)==0)
	{
		printf("\r\nTransmit Success! \r\n");
	}
	else
	{
		printf("\r\nTransmit Fault! \r\n");}
		printf("\r\nThe CPU transfer,time consume:%dus! \n\r",TickCntCPU);
		printf("\r\nThe DMA transfer,time consume:%dus! \n\r",TickCntCPU);
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
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA,ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);
}
void GpioInitialisation()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP; 
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
}
void NvicInitialisation()
{
	NVIC_InitTypeDef NVIC_InitStructure;
	# ifdef VECT_TAB_RAM
	NVIC_SetVectorTable(NVIC_VectTab_RAM,0x0);
	# else
	NVIC_SetVectorTable(NVIC_VectTab_FLASH,0x0);
	# endif
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
	NVIC_InitStructure.NVIC_IRQChannel=DMA1_Channel6_IRQChannel;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0;
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init (&NVIC_InitStructure);
}
void DmaInitialisation()
{
	DMA_InitTypeDef DMA_InitStructure;
	DMA_DeInit(DMA1_Channel6);
	DMA_InitStructure.DMA_PeripheralBaseAddr=(u32)SRC_Const_Buffer;
	DMA_InitStructure.DMA_MemoryBaseAddr=(u32)DST_Buffer;
	DMA_InitStructure.DMA_DIR=DMA_DIR_PeripheralSRC;
	DMA_InitStructure.DMA_BufferSize=BufferSize;
	DMA_InitStructure.DMA_PeripheralInc=DMA_PeripheralInc_Enable;
	DMA_InitStructure.DMA_MemoryInc=DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_MemoryDataSize=DMA_MemoryDataSize_Word;
	DMA_InitStructure.DMA_Mode=DMA_Mode_Normal;
	DMA_InitStructure.DMA_Priority=DMA_Priority_High;
	DMA_InitStructure.DMA_M2M=DMA_M2M_Enable;
	DMA_Init(DMA1_Channel6,&DMA_InitStructure);
	DMA_ITConfig(DMA1_Channel6,DMA_IT_TC,ENABLE);
	CurrDataCounter=DMA_GetCurrDataCounter(DMA1_Channel6);
}
void SystickInitialisation()
{
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);
	SysTick_SetReload(9);
	SysTick_CounterCmd(SysTick_Counter_Enable);
	SysTick_ITConfig(ENABLE);
}
void UsartInitialisation()
{
	USART_InitTypeDef USART_InitStructure;
	USART_ClockInitTypeDef USART_ClockInitStructure;
  USART_InitStructure.USART_BaudRate=9600;
	USART_InitStructure.USART_WordLength=USART_WordLength_8b;
	USART_InitStructure.USART_StopBits=USART_StopBits_1;
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

