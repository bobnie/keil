#include "control.h"	
#include "filter.h"	
#define PI 3.14159265
/**************************************************************************
���ߣ�Mini Balance 
�Ա����̣�http://shop114407458.taobao.com/
**************************************************************************/

/**************************************************************************
�������ܣ�5MS��ʱ�жϺ��� 5MS��������
��ڲ�������
����  ֵ����
��    �ߣ�Mini Balance
**************************************************************************/
void TIM1_UP_IRQHandler(void)  
{    
	if(TIM1->SR&0X0001)//10ms��ʱ�ж�
	{   
		TIM1->SR&=~(1<<0);                                       //===�����ʱ��1�жϱ�־λ		 
  		//Led_Flash(400);                                          //===LED��˸;

		Key();                                                  //===ɨ�谴��״̬

		if(PID_OK==1)
		{
		/*if(in_10ms<700)
		{
		 in_10ms++;
		}
		else
		{
			LED=~LED;
			PID_CONTROL+=3;
			//in_10ms=0;
		}*/
			if(gyro[0]>3000||gyro[0]<-3000)			{				gyro[0]=gyro_0_last;			}
			if(gyro[1]>3000||gyro[1]<-3000)			{				gyro[1]=gyro_1_last;			}
														gyro_0_last=gyro[0];
														gyro_1_last=gyro[1];
			
			gyro[0]/=16.40;gyro[1]/=16.40;gyro[2]/=16.40;//ת���ɽǶ�ֵ
			
			//�������޷�
			if(Pitch>5||Pitch<-5)			{				PID_ki=0;			}	
			if(Roll>5||Roll<-5)				{				PID_kiy=0;		}
			
	//������xģʽ
			//���õ�X��  3.3  1.5
//			Moto1=PID_CONTROL - PID_kp*Pitch - PID_ki*Pitch_integ - gyro[1]*PID_kd;//ƽ���PD����  
//			Moto2=PID_CONTROL + PID_kp*Pitch + PID_ki*Pitch_integ + gyro[1]*PID_kd;//�����м���Z��P������������ 
//			Moto3=PID_CONTROL + PID_kp*Pitch + PID_ki*Pitch_integ + gyro[1]*PID_kd;////////////// 
//			Moto4=PID_CONTROL - PID_kp*Pitch - PID_ki*Pitch_integ - gyro[1]*PID_kd;////////////// 
			//���õ�Y��  4.4 2.0
//			Moto1=PID_CONTROL + PID_kpy*Roll + PID_kiy*Roll_integ + gyro[0]*PID_kdy;//ƽ���PD����  
//			Moto2=PID_CONTROL - PID_kpy*Roll - PID_kiy*Roll_integ - gyro[0]*PID_kdy;//�����м���Z��P������������ 
//			Moto3=PID_CONTROL + PID_kpy*Roll + PID_kiy*Roll_integ + gyro[0]*PID_kdy;////////////// 
//			Moto4=PID_CONTROL - PID_kpy*Roll - PID_kiy*Roll_integ - gyro[0]*PID_kdy;////////////// 
			
			//�����������һ�λ��������Ĵ��룬ÿ���뼸��  +PID_CONTROL
			//����xģʽ����
//			Moto1=PID_CONTROL - PID_kp*Pitch - PID_ki*Pitch_integ - gyro[1]*PID_kd 
//							+ PID_kpy*Roll + PID_kiy*Roll_integ + gyro[0]*PID_kdy;//ƽ���PD����  
//			Moto2=PID_CONTROL + PID_kp*Pitch + PID_ki*Pitch_integ + gyro[1]*PID_kd
//							 - PID_kpy*Roll - PID_kiy*Roll_integ - gyro[0]*PID_kdy;//�����м���Z��P������������ 
//			Moto3=PID_CONTROL + PID_kp*Pitch + PID_ki*Pitch_integ + gyro[1]*PID_kd
//								 + PID_kpy*Roll + PID_kiy*Roll_integ + gyro[0]*PID_kdy;////////////// 
//			Moto4=PID_CONTROL - PID_kp*Pitch - PID_ki*Pitch_integ - gyro[1]*PID_kd
//									 - PID_kpy*Roll - PID_kiy*Roll_integ - gyro[0]*PID_kdy;////////////// 

			//PID_CONTROL=PID_CONTROL-gyro[2]*0.1;
			//���õ�ʮ��ģʽ
			Moto1=PID_CONTROL - PID_kp*(Pitch+0) - gyro[1]*PID_kd - PID_ki*Pitch_integ;//ƽ���PD����  
			Moto2=PID_CONTROL + PID_kp*(Pitch+0) + gyro[1]*PID_kd + PID_ki*Pitch_integ;//�����м���Z��P������������ 
			Moto3=PID_CONTROL + PID_kpy*(Roll-0) + gyro[0]*PID_kdy + PID_kiy*Roll_integ;////////////// 
			Moto4=PID_CONTROL - PID_kpy*(Roll-0) - gyro[0]*PID_kdy - PID_kiy*Roll_integ;////////////// 		
			
			Pitch_integ+=Pitch;    Roll_integ+=Roll;//�������ۼ�
			
			Xianfu_Pwm();                                            //===PWM�޷�
			Set_Pwm(Moto1,Moto2,Moto3,Moto4);                        //===��ֵ��PWM�Ĵ���
//	    if(Turn_Off()==0)                   //===����������쳣
//				Set_Pwm(Moto1,Moto2,Moto3,Moto4);                        //===��ֵ��PWM�Ĵ���
//			else
//			{
//				Set_Pwm(18000,18000,18000,18000);
//				PID_OK=0;
//			}
		}
		    		
	}       
} 


/**************************************************************************
�������ܣ���ֵ��PWM�Ĵ���
��ڲ�����PWM
����  ֵ����
��    �ߣ�Mini Balance
**************************************************************************/
void Set_Pwm(int moto1,int moto2,int moto3,int moto4)
{
			PWMA=myabs(moto1);
			PWMB=myabs(moto2);	
	   	PWMC=myabs(moto3);
			PWMD=myabs(moto4);	
}

/**************************************************************************
�������ܣ�����PWM��ֵ 
��ڲ�������
����  ֵ����
��    �ߣ�Mini Balance
**************************************************************************/
void Xianfu_Pwm(void)
{	
	  int high_throttle=PID_CONTROL+PID_limit;
	  int low_throllte=PID_CONTROL-PID_limit;

    if(Moto1<low_throllte) Moto1=low_throllte;	
		if(Moto1>high_throttle)  Moto1=high_throttle;
			
	  if(Moto2<low_throllte) Moto2=low_throllte;	
		if(Moto2>high_throttle)  Moto2=high_throttle;
				
	  if(Moto3<low_throllte) Moto3=low_throllte;	
		if(Moto3>high_throttle)  Moto3=high_throttle;
			
	  if(Moto4<low_throllte) Moto4=low_throllte;	
		if(Moto4>high_throttle)  Moto4=high_throttle;			
}

/**************************************************************************
�������ܣ��쳣�رյ��
��ڲ�������Ǻ͵�ѹ
����  ֵ��1���쳣  0������
��    �ߣ�Mini Balance
**************************************************************************/
u8 Turn_Off(void)
{
	  u8 temp;
	  //if(Pitch<-40||Pitch>40||1==Flag_Stop||Roll<-40||Roll>40)
		if(Pitch<-80||Pitch>80||Roll<-80||Roll>80)
	  {	                                                 //===��Ǵ���40�ȹرյ��
      	temp=1;                                            //===Flag_Stop��1�رյ��
      }
	  else
      	temp=0;
    return temp;			
}
	

/**************************************************************************
�������ܣ�����ֵ����
��ڲ�����int
����  ֵ��unsigned int
**************************************************************************/
int myabs(int a)
{ 		   
	  int temp;
		if(a<0)  temp=-a;  
	  else temp=a;
	  return temp;
}
/**************************************************************************
�������ܣ������޸�С������״̬ 
��ڲ�������
����  ֵ����
**************************************************************************/
void Key(void)
{	
	u8 tmp;
	tmp=click(); 
	if(tmp==1)
	
		//delay_ms(1000);delay_ms(1000);delay_ms(1000);delay_ms(1000);delay_ms(1000);
		//PWMA=(40000-2500)/2;		PWMB=(40000-2500)/2; 		PWMC=(40000-2500)/2;		PWMD=(40000-2500)/2;
		//PWMA=17000;		PWMB=17000; 		PWMC=17000;		PWMD=17000;
		//PID_OK=1;
		Flag_Stop=!Flag_Stop;
	
//	if(tmp==2)Flag_Show=~Flag_Show;
}

