
#include "usart2.h"
#include "show.h"


/**************************************************************************
函数功能：串口2初始化
入口参数：pclk2:PCLK2 时钟频率(Mhz)    bound:波特率
返回  值：无
**************************************************************************/
void uart2_init(u32 pclk2,u32 bound)
{  	 
	float temp;
	u16 mantissa;
	u16 fraction;	   
	temp=(float)(pclk2*1000000)/(bound*16);//得到USARTDIV
	mantissa=temp;				 //得到整数部分
	fraction=(temp-mantissa)*16; //得到小数部分	 
  mantissa<<=4;
	mantissa+=fraction; 
	RCC->APB2ENR|=1<<2;   //使能PORTA口时钟  
	RCC->APB1ENR|=1<<17;  //使能串口时钟 
	GPIOA->CRL&=0XFFFF00FF; 
	GPIOA->CRL|=0X00008B00;//IO状态设置
	GPIOA->ODR|=1<<2;	  
	RCC->APB1RSTR|=1<<17;   //复位串口1
	RCC->APB1RSTR&=~(1<<17);//停止复位	   	   
	//波特率设置
 	USART2->BRR=mantissa; // 波特率设置	 
	USART2->CR1|=0X200C;  //1位停止,无校验位.
	//使能接收中断
	USART2->CR1|=1<<8;    //PE中断使能
	USART2->CR1|=1<<5;    //接收缓冲区非空中断使能	    	
	MY_NVIC_Init(3,3,USART2_IRQn,2);//组2，最低优先级 
}

/**************************************************************************
函数功能：串口2接收中断
入口参数：无
返回  值：无
**************************************************************************/
void USART2_IRQHandler(void)
{	
	if(USART2->SR&(1<<5))//接收到数据
	{	  
	  unsigned char uart_receive;//蓝牙接收相关变量
	  uart_receive=USART2->DR; 
	  //printf("%c",uart_receive);//向上位机发送数�
		//OLED_ShowString(105,0,&uart_receive);
	  switch(uart_receive)
	  {
	  case 'a'://PWM +=是占空比变小，-=是占空比变大
			PWMA+=20; PWMD+=20;
		  break;
	  case 'b':
			PWMA-=20; PWMD-=20;
		  break;
	  case 'c':
			
	  	break;
		
		case 'd'://+KP   PID_kpy+=0.1;
		  if(PID_kpy<100)		  {			PID_kp+=1;	  }		  break;
	  case 'g'://-KP   PID_kpy-=0.1;
		  if(PID_kpy>0)		  {			PID_kp-=1;	  }		  break;
			
		case 'e'://+I
			if(PID_ki<20)			{			PID_ki+=0.1;			}	  	break;
		case 'h'://-I
			if(PID_ki>0)			{			PID_ki-=0.1;			}	  	break;
		case 'f'://+D   PID_kdy+=0.1;
			if(PID_kdy<100)			{			PID_kd+=0.01;			}	  	break;
		case 'i'://-D      
			if(PID_kdy>0)			{			PID_kd-=0.01;			}	  	break;
		
		case 'j'://+PWMA
			PWMA+=10;
			break;
		case 'k'://-PWMA
			PWMA-=10;
			break;
		case 'l'://+PWMB
			PWMB+=10;
			break;
		case 'm'://-PWMB
			PID_CONTROL+=1;
			break;
		case 'u'://+PWMC
			PID_CONTROL-=1;
			break;
		case 'v'://-PWMC
			PID_CONTROL+=10;
			break;
		case 'o':
			if(PID_OK==0)
			{
				if(PWMA<=17960)
				{
					PWMA+=40;
					PWMB+=40;
					PWMC+=40;
					PWMD+=40;
				}
			}
			if(PID_OK==1)
			{
				if(PID_CONTROL<=17960)
				{
					PID_CONTROL+=20;
				}
			}
	    break;
	  case 'p':  //启动PID
	  	PID_OK=1;
			//PID_CONTROL=17400;
	  	break;
		case 'q':
			if(PID_OK==0)
			{
				if(PWMA>=16040)
				{
					PWMA-=40;
					PWMB-=40;
					PWMC-=40;
					PWMD-=40;
				}
			}
			if(PID_OK==1)
			{
				if(PID_CONTROL>=16040)
				{
						PID_CONTROL-=20;
				}
			}
		  break;
			
	  case 's':
	    PID_CONTROL-=10;		break;	
		
		case 'z':
			PID_CONTROL+=5;		break;
		
		case 'y':
			PID_CONTROL-=5; 	break;
		case 'x':
      PID_OK=0;
      PWMA=18000;		PWMB=18000; 		PWMC=18000;		PWMD=18000;		
			break;
	  default:
	    break;
	  }
	}  											 
} 


