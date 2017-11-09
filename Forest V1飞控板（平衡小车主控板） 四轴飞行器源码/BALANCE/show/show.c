#include "show.h"
  /**************************************************************************
作者：平衡小车之家
我的淘宝小店：http://shop114407458.taobao.com/
**************************************************************************/
unsigned char i;          //计数变量
unsigned char Send_Count; //串口需要发送的数据个数
/**************************************************************************
函数功能：OLED显示
入口参数：无
返回  值：无
**************************************************************************/
void oled_show(void)
{
		OLED_Display_On();  //显示屏打开
		//=============显示滤波器=======================//	
		                      OLED_ShowString(00,0,"WAY-");
		                      OLED_ShowNumber(30,0, Way_Angle,1,12);
	       if(Way_Angle==1)	OLED_ShowString(45,0,"DMP");
		else if(Way_Angle==2)	OLED_ShowString(45,0,"Kalman");
		else if(Way_Angle==3)	OLED_ShowString(45,0,"Hubu");
		//=============显示温度=======================//	
		                      OLED_ShowString(00,10,"Wendu");
		                      OLED_ShowNumber(45,10,Temperature/10,2,12);
		                      OLED_ShowNumber(68,10,Temperature%10,1,12);
		                      OLED_ShowString(58,10,".");
		                      OLED_ShowString(80,10,"`C");
		
		                      OLED_ShowString(0,40,"Pitch");
		if(Pitch<0)		OLED_ShowNumber(75,40,Pitch+360,3,12);
		else					        OLED_ShowNumber(75,40,Pitch,3,12);
			//=============显示角度=======================//
			                      OLED_ShowString(0,50,"Roll");
		if(Roll<0)		OLED_ShowNumber(75,50,Roll+360,3,12);
		else					        OLED_ShowNumber(75,50,Roll,3,12);
		//=============刷新=======================//
		OLED_Refresh_Gram();	
	}
/**************************************************************************
函数功能：向APP发送数据
入口参数：无
返回  值：无
作    者：平衡小车之家
**************************************************************************/
void APP_Show(void)
{    
	  int app_2,app_3,app_4;
	
		printf("Z%d:%d:%d:%dL$",(u8)app_2,(u8)app_3,(u8)app_4,(int)Pitch);
}

