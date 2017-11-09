#include "control.h"	
#include "filter.h"	
#define PI 3.14159265
/**************************************************************************
作者：Mini Balance 
淘宝店铺：http://shop114407458.taobao.com/
**************************************************************************/

/**************************************************************************
函数功能：5MS定时中断函数 5MS控制周期
入口参数：无
返回  值：无
作    者：Mini Balance
**************************************************************************/
void TIM1_UP_IRQHandler(void)  
{    
	if(TIM1->SR&0X0001)//5ms定时中断
	{   
		  TIM1->SR&=~(1<<0);                                        //===清除定时器1中断标志位		 
  		Led_Flash(400);                                          //===LED闪烁;	      
			Key();                                                  //===扫描按键状态
 			Moto1=2000-5*Pitch+gyro[1]*8/100  -5*Roll+gyro[0]*8/100  ;//平衡的PD控制 可自行加入Z轴P控制抑制自旋 
			Moto2=2000+5*Pitch-gyro[1]*8/100  +5*Roll-gyro[0]*8/100  ;////////////// 
			Moto3=2000-5*Pitch+gyro[1]*8/100  +5*Roll-gyro[0]*8/100  ;////////////// 
			Moto4=2000+5*Pitch-gyro[1]*8/100  -5*Roll+gyro[0]*8/100  ;////////////// 
   		Xianfu_Pwm();                                            //===PWM限幅
      if(Turn_Off()==0)                   //===如果不存在异常
 			Set_Pwm(Moto1,Moto2,Moto3,Moto4);                        //===赋值给PWM寄存器    		
	}       
} 


/**************************************************************************
函数功能：赋值给PWM寄存器
入口参数：PWM
返回  值：无
作    者：Mini Balance
**************************************************************************/
void Set_Pwm(int moto1,int moto2,int moto3,int moto4)
{
			PWMA=myabs(moto1);
			PWMB=myabs(moto2);	
	   	PWMC=myabs(moto3);
			PWMD=myabs(moto4);	
}

/**************************************************************************
函数功能：限制PWM赋值 
入口参数：无
返回  值：无
作    者：Mini Balance
**************************************************************************/
void Xianfu_Pwm(void)
{	
	  int Amplitude=4000; 
    if(Moto1<0) Moto1=0;	
		if(Moto1>Amplitude)  Moto1=Amplitude;	
	  if(Moto2<0) Moto2=0;	
		if(Moto2>Amplitude)  Moto2=Amplitude;		
	  if(Moto3<0) Moto3=0;	
		if(Moto3>Amplitude)  Moto3=Amplitude;	
	  if(Moto4<0) Moto4=0;	
		if(Moto4>Amplitude)  Moto4=Amplitude;		
	
}

/**************************************************************************
函数功能：异常关闭电机
入口参数：倾角和电压
返回  值：1：异常  0：正常
作    者：Mini Balance
**************************************************************************/
u8 Turn_Off(void)
{
	    u8 temp;
			if(Pitch<-40||Pitch>40||1==Flag_Stop||Roll<-40||Roll>40)
			{	                                                 //===倾角大于40度关闭电机
      temp=1;                                            //===Flag_Stop置1关闭电机
      }
			else
      temp=0;
      return temp;			
}
	

/**************************************************************************
函数功能：绝对值函数
入口参数：int
返回  值：unsigned int
**************************************************************************/
int myabs(int a)
{ 		   
	  int temp;
		if(a<0)  temp=-a;
	  else temp=a;
	  return temp;
}
/**************************************************************************
函数功能：按键修改小车运行状态 
入口参数：无
返回  值：无
**************************************************************************/
void Key(void)
{	
	u8 tmp;
	tmp=click(); 
	if(tmp==1)
	Flag_Stop=!Flag_Stop;
//	if(tmp==2)Flag_Show=~Flag_Show;
}

