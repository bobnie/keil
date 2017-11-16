
#include "usart2.h"
#include "show.h"


/**************************************************************************
�������ܣ�����2��ʼ��
��ڲ�����pclk2:PCLK2 ʱ��Ƶ��(Mhz)    bound:������
����  ֵ����
**************************************************************************/
void uart2_init(u32 pclk2,u32 bound)
{  	 
	float temp;
	u16 mantissa;
	u16 fraction;	   
	temp=(float)(pclk2*1000000)/(bound*16);//�õ�USARTDIV
	mantissa=temp;				 //�õ���������
	fraction=(temp-mantissa)*16; //�õ�С������	 
  mantissa<<=4;
	mantissa+=fraction; 
	RCC->APB2ENR|=1<<2;   //ʹ��PORTA��ʱ��  
	RCC->APB1ENR|=1<<17;  //ʹ�ܴ���ʱ�� 
	GPIOA->CRL&=0XFFFF00FF; 
	GPIOA->CRL|=0X00008B00;//IO״̬����
	GPIOA->ODR|=1<<2;	  
	RCC->APB1RSTR|=1<<17;   //��λ����1
	RCC->APB1RSTR&=~(1<<17);//ֹͣ��λ	   	   
	//����������
 	USART2->BRR=mantissa; // ����������	 
	USART2->CR1|=0X200C;  //1λֹͣ,��У��λ.
	//ʹ�ܽ����ж�
	USART2->CR1|=1<<8;    //PE�ж�ʹ��
	USART2->CR1|=1<<5;    //���ջ������ǿ��ж�ʹ��	    	
	MY_NVIC_Init(3,3,USART2_IRQn,2);//��2��������ȼ� 
}

/**************************************************************************
�������ܣ�����2�����ж�
��ڲ�������
����  ֵ����
**************************************************************************/
void USART2_IRQHandler(void)
{	
	if(USART2->SR&(1<<5))//���յ�����
	{	  
	  unsigned char uart_receive;//����������ر���
	  uart_receive=USART2->DR; 
	  //printf("%c",uart_receive);//����λ���������
		//OLED_ShowString(105,0,&uart_receive);
	  switch(uart_receive)
	  {
	  case 'a'://PWM +=��ռ�ձȱ�С��-=��ռ�ձȱ��
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
	  case 'p':  //����PID
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


