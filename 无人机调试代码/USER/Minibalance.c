#include "sys.h"
/**************************************************************************
���ߣ�ƽ��С��֮��
�ҵ��Ա�С�꣺http://shop114407458.taobao.com/
�˴���Ŀ����㷨��ͨ����֤�ģ�������ͬѧ�ǲ�Ҫֱ��ʹ�õ��������棬
��Ϊ��ͬ�ĵ������ͬ�Ľⷨ���������İ�װ������Ӱ�쵽���յĿ���ϵͳ
����ݳ���ĵײ�Ϳ��ƴ���������Ӳ��һ��һ������ƥ��
**************************************************************************/
u8 Way_Angle=1;                             //��ȡ�Ƕȵ��㷨��1:DMP2�������� ���Ըĳ�3 �������˲���
u8 Flag_Show=0,Flag_Stop=1;                 //��ʾ��־λ ��ͣ��־λ ��ʾ���ǳ�Ӱ�촦���ٶ�
int Temperature;                            //��ʾ�¶�
float Moto1,Moto2,Moto3,Moto4;               //���PWM���� Ӧ��Motor�� ��Moto�¾�	
int PID_OK=0;
int PID_CONTROL=17500;
float PID_kp=8.85;//3.3
float PID_kpy=8.00;//4.4
float PID_ki=0;
float PID_kiy=0;
float PID_kd=2.00;//1.5
float PID_kdy=0;//2.0
int gyro_count=0;
int in_10ms=0;
long gyro_0_last=0;
long gyro_1_last=0;
short gyro_0[8]={0};
short gyro_1[8]={0};
int PID_limit=600;
int Compensate=10;
char pitch_store[1]={0};
int pitch_count=0;
int main(void)
{ 
	Stm32_Clock_Init(9);            //ϵͳʱ������
	delay_init(72);                 //��ʱ��ʼ��
	JTAG_Set(JTAG_SWD_DISABLE);     //=====�ر�JTAG�ӿ�
	JTAG_Set(SWD_ENABLE);           //=====��SWD�ӿ� �������������SWD�ӿڵ���

	//�����ĸ�50%ռ�ձȵĵ�ƽ�������
	KEY_Init();                     //������ʼ��
	MiniBalance_PWM_Init(19999,35); //�ɿ������ѹ��Ƶ���������á�  Ĭ�ϣ�999~200hz	   40000~50hz
	//PWMA=10000;		PWMB=10000;		PWMC=10000;			PWMD=10000;
	//delay_ms(50);
	//��ƽ��Χ��6.25%~10%   2500~4000 	��������Ϊ�ߵ�ƽ  1000~2000
	//PWMA=16000;		PWMB=16000; 		PWMC=16000;		PWMD=16000;
	//delay_ms(1000);delay_ms(1000);delay_ms(1000);delay_ms(1000);
	PWMA=18000;		PWMB=18000; 		PWMC=18000;		PWMD=18000;
	//PWMA=5000;		PWMB=10000; 		PWMC=15000;		PWMD=20000;  //���Կ�������PWM��Ӧ�ĸ�����
	
  Timer1_Init(49,14399);	 //ÿ10ms��һ���жϺ���

	LED_Init();                     //��ʼ���� LED ���ӵ�Ӳ���ӿ�
	KEY_Init();                     //������ʼ��
	OLED_Init();                    //OLED��ʼ��
	uart_init(72,115200);           //��ʼ������1	 ���͵����ԵĴ���
  uart2_init(36,9600);            //����2��ʼ��    ��������
	IIC_Init();                     //ģ��IIC��ʼ��
  MPU6050_initialize();           //=====MPU6050��ʼ��	
	DMP_Init();                     //��ʼ��DMP     
	

	while(1)
	{
	 	if(Way_Angle==1)                     //����������ȡ
		{
			Read_DMP();                      //===��ȡ���ٶȺ����
		}  
		if(Flag_Stop==1)//ʹ��OLED��ʾ����PC�˴��ڵ�������
		{
			Way_Angle=1;//ʹ��DMP
			if(Pitch<0)		OLED_ShowNumber(45,40,Pitch+360,3,12);
			else					OLED_ShowNumber(45,40,Pitch,3,12);
			
			OLED_ShowNumber(0,40,PID_kp*100,3,12);
			OLED_ShowNumber(0,50,PID_kd*100,3,12);
			
			//Temperature=Read_Temperature();  //===��ȡMPU6050�����¶ȴ��������ݣ����Ʊ�ʾ�����¶ȡ�	
			//oled_show(); //===��ʾ����
			printf("%4d\r\n",gyro[2]);
			//printf("X�����%f  Y�����%f   \r\n",Pitch,Roll);//����λ����������  x�� Pitch  y�� Roll
			//printf("GYRO[0]%6d     GYRO[1]%6d      GYRO[2]%6d   \r\n",gyro[0],gyro[1],gyro[2]);//����λ����������  x�� Pitch  y�� Roll
			//}
		}	
	} 
}
