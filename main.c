#include <REGX52.H>
#include <string.h>
#include <intrins.h>

/*ͷ�ļ�*/

/*�곣��*/
#define uchar unsigned char
#define uint  unsigned int
	
#define LOW 20
#define MID 40
#define NOM 60
#define HIGH 80
#define FULL 100


sbit L1=P2^7;	//����̽ͷ����
sbit L2=P2^6;	//����̽ͷ����
sbit R1=P2^1;	//����̽ͷ����
sbit R2=P2^0;	//����̽ͷ����

sbit M1=P1^0;	    
sbit M2=P1^1;	    
sbit M3=P1^3;	   
sbit M4=P1^2;

sbit gan=P1^7;
sbit feng=P1^4;

/*ȫ�ֱ�����*/
uint PWM_L=0,PWM_R=0,t=0;//���֣����֣�ʱ�䣨���ֺ����ֶ���С��100������Խ��ת��Խ�ߣ�
uchar mode=0,sign=0,begin=0;//״ָ̬ʾ��ң��/ѭ�����ɻɹܣ�����ָʾ
uchar XJ=0;

/*����������*/
void stop(void);
void Timer0Init(void);
void Uart_Init(void);
void delay1ms(int num);
int XunJi(void);
/*��⺯��*/
/*��������*/

/*������*/
void main()
{
	Timer0Init();
	Uart_Init();
	while(1)
	{
//		if(mode==0)
//		{
//			_nop_();
//		}
		if(mode==1)
		{
			while(begin!=1)//�ظ��������ź�
			{
				if(sign==1)
				{
					delay1ms(20);
					if(sign==1) begin=1;
				}
			}
			if(begin==1)
			{
				/*��⺯��*/
				/*��������*/
				XJ=XunJi();
				switch(XJ)
				{
					case 1:PWM_L=NOM;PWM_R=NOM;break;
					case 2:PWM_L=LOW;PWM_R=NOM;break;
					case 3:PWM_L=MID;PWM_R=NOM;break;
					case 4:PWM_L=NOM;PWM_R=LOW;break;
					case 5:PWM_L=NOM;PWM_R=MID;break;
					
				}
				if(sign==1)
				{
					delay1ms(20);
					if(sign==1)
					{
						begin=0;
						mode=0;
					}
				}
			}
			
		}
	}
}

/*
**********************************************************************
*                        ��ʱ1ms
**********************************************************************
*/
void delay1ms(int num)//@11.0592MHz
{
	unsigned char i, j, n;
	for(n=0;n<num;n++)
	{
		_nop_();
		i = 2;
		j = 199;
		do
		{
			while (--j);
		} while (--i);
	}
}
/*
**********************************************************************
*                        ѭ��ģ��
**********************************************************************
*/
int XunJi()
{
	if(L1==1 && R2==1)
		return 1;//NOM NOM ǰ��
	else if(L1==0 && L2==0 && R1==0 && R2==0)
		return 1;//NOM NOM ǰ��
	else if(L1==1 && R1==0 && R2==0)
		return 2;//LOW NOM ����ת
	else if(L1==0 && L2==1 && R1==0 && R2==0)
		return 3;//MID NOM С��ת
	else if(L1==0 && L2==0 && R2==1)
		return 4;//NOM LOW ����ת
	else if(L1==0 && L2==0 && R1==1 && R2==0)
		return 5;//NOM MID С��ת
	else if(L1==0 && L2==1 && R1==1 && R2==0)
		return 1;//NOM NOM ֱ��
	else
		return 1;//NOM NOM ֱ��
}

/*��ʱ��0*/
void Timer0Init()//��ʼ��
{
	TMOD|=0X01;//ѡ��Ϊ��ʱ��0ģʽ��������ʽ1������TR0��������

	TH0=(65536-100)/256;	//F=f/12=11.0592M/12=0.9216MHZ   T=1/F=1.085us 
	TL0=(65536-100)%256;	//��ʱ108.5us=0.1ms
	ET0=1;//�򿪶�ʱ��0�ж�����
	TR0=1;//�򿪶�ʱ��
	EA=1;
}

void Timer0() interrupt 1
{
	TH0=(65536-100)/256; //���¸���ֵ
	TL0=(65536-100)%256;
	if(t<PWM_L)
	{
		M3=1;
		M4=0;	//������ת
	}
	else
	{
		M3=0;
		M4=0;	//���ֲ�ת
	}
	if(t<PWM_R)
	{
		M1=1;
		M2=0;	//������ת
	}
	else
	{
		M1=0;
		M2=0;	//���ֲ�ת
	}
	t++;
	if(t>=100)//100*0.1ms=10ms PWM����10ms ��PWM_L/R���Ƹߵ�ƽռ�ձ�
		t=0;	
}

/*������ʱ��*/
void UART_Routine()  interrupt 4
{	
	if(RI==1)
	{	
	  P0 =SBUF;					   //���͵�����SBUF������P0
	 if(P0==0X01)				   //0x01���ֵ����ͨ�������������༭ģʽ����
	 {
	   PWM_L=NOM;PWM_R=NOM;
	 }
	 if(P0==0X02)				   //0x02���ֵ����ͨ�������������༭ģʽ����
	 {
	   PWM_L=LOW;PWM_R=NOM;//����ת
	 }
	 if(P0==0X03)				   //0x03���ֵ����ͨ�������������༭ģʽ����
	 {
	   PWM_L=MID;PWM_R=NOM;//С��ת
	 }
	 if(P0==0X04)				   //0x03���ֵ����ͨ�������������༭ģʽ����
	 {
	   PWM_L=NOM;PWM_R=LOW;//����ת
	 }
	 if(P0==0X05)				   //0x03���ֵ����ͨ�������������༭ģʽ����
	 {
	   PWM_L=NOM;PWM_R=MID;//С��ת
	 }
//	 if(P0==0X06)				   //0x03���ֵ����ͨ�������������༭ģʽ����
//	 {
//	   PWM_count=5;
//	 }
//	 if(P0==0X07)				   //0x03���ֵ����ͨ�������������༭ģʽ����
//	 {
//	   PWM_count=6;
//	 }
	 if(P0==0Xff)				   //0xff���ֵ����ͨ�������������༭ģʽ����
	 {
	   mode=1;
	 }
	 if(P0==0X00)				   //0x00���ֵ����ͨ�������������༭ģʽ����
	 {
	   PWM_L=0;PWM_R=0;
	 }

	  RI=0;
	}
}

void Uart_Init()   //9600bps@11.0592MHz
{
	PCON |= 0x80;		//ʹ�ܲ����ʼӱ�	
	SCON = 0x50;		//8λ����
	TMOD &= 0x0F;		//�����ʱ��1ģʽ
	TMOD |= 0x20;		//�趨��ʱ��8Ϊ�Զ�
	TL1 = 0xFA;			//�趨��ʼֵ
	TH1 = 0xFA;			//�趨��ʼֵ
	ET1 = 0;		
    TR1 = 1;
    EA = 1;
    ES =1 ; 	
}


