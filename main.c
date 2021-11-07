#include <REGX52.H>
#include <string.h>
#include <intrins.h>

/*头文件*/

/*宏常量*/
#define uchar unsigned char
#define uint  unsigned int
	
#define LOW 20
#define MID 40
#define NOM 60
#define HIGH 80
#define FULL 100


sbit L1=P2^7;	//红外探头边左
sbit L2=P2^6;	//红外探头中左
sbit R1=P2^1;	//红外探头中右
sbit R2=P2^0;	//红外探头边右

sbit M1=P1^0;	    
sbit M2=P1^1;	    
sbit M3=P1^3;	   
sbit M4=P1^2;

sbit gan=P1^7;
sbit feng=P1^4;

/*全局变量区*/
uint PWM_L=0,PWM_R=0,t=0;//右轮，左轮，时间（左轮和右轮都是小于100的数，越大转速越高）
uchar mode=0,sign=0,begin=0;//状态指示：遥控/循迹，干簧管，发车指示
uchar XJ=0;

/*函数声明区*/
void stop(void);
void Timer0Init(void);
void Uart_Init(void);
void delay1ms(int num);
int XunJi(void);
/*检测函数*/
/*开车函数*/

/*主函数*/
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
			while(begin!=1)//重复检测出发信号
			{
				if(sign==1)
				{
					delay1ms(20);
					if(sign==1) begin=1;
				}
			}
			if(begin==1)
			{
				/*检测函数*/
				/*开车函数*/
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
*                        延时1ms
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
*                        循迹模块
**********************************************************************
*/
int XunJi()
{
	if(L1==1 && R2==1)
		return 1;//NOM NOM 前进
	else if(L1==0 && L2==0 && R1==0 && R2==0)
		return 1;//NOM NOM 前进
	else if(L1==1 && R1==0 && R2==0)
		return 2;//LOW NOM 大左转
	else if(L1==0 && L2==1 && R1==0 && R2==0)
		return 3;//MID NOM 小左转
	else if(L1==0 && L2==0 && R2==1)
		return 4;//NOM LOW 大右转
	else if(L1==0 && L2==0 && R1==1 && R2==0)
		return 5;//NOM MID 小右转
	else if(L1==0 && L2==1 && R1==1 && R2==0)
		return 1;//NOM NOM 直走
	else
		return 1;//NOM NOM 直走
}

/*定时器0*/
void Timer0Init()//初始化
{
	TMOD|=0X01;//选择为定时器0模式，工作方式1，仅用TR0打开启动。

	TH0=(65536-100)/256;	//F=f/12=11.0592M/12=0.9216MHZ   T=1/F=1.085us 
	TL0=(65536-100)%256;	//定时108.5us=0.1ms
	ET0=1;//打开定时器0中断允许
	TR0=1;//打开定时器
	EA=1;
}

void Timer0() interrupt 1
{
	TH0=(65536-100)/256; //重新赋初值
	TL0=(65536-100)%256;
	if(t<PWM_L)
	{
		M3=1;
		M4=0;	//左轮正转
	}
	else
	{
		M3=0;
		M4=0;	//左轮不转
	}
	if(t<PWM_R)
	{
		M1=1;
		M2=0;	//右轮正转
	}
	else
	{
		M1=0;
		M2=0;	//右轮不转
	}
	t++;
	if(t>=100)//100*0.1ms=10ms PWM周期10ms 用PWM_L/R控制高电平占空比
		t=0;	
}

/*蓝牙定时器*/
void UART_Routine()  interrupt 4
{	
	if(RI==1)
	{	
	  P0 =SBUF;					   //发送的数据SBUF，赋给P0
	 if(P0==0X01)				   //0x01这个值可以通过蓝牙调试器编辑模式设置
	 {
	   PWM_L=NOM;PWM_R=NOM;
	 }
	 if(P0==0X02)				   //0x02这个值可以通过蓝牙调试器编辑模式设置
	 {
	   PWM_L=LOW;PWM_R=NOM;//大左转
	 }
	 if(P0==0X03)				   //0x03这个值可以通过蓝牙调试器编辑模式设置
	 {
	   PWM_L=MID;PWM_R=NOM;//小左转
	 }
	 if(P0==0X04)				   //0x03这个值可以通过蓝牙调试器编辑模式设置
	 {
	   PWM_L=NOM;PWM_R=LOW;//大右转
	 }
	 if(P0==0X05)				   //0x03这个值可以通过蓝牙调试器编辑模式设置
	 {
	   PWM_L=NOM;PWM_R=MID;//小右转
	 }
//	 if(P0==0X06)				   //0x03这个值可以通过蓝牙调试器编辑模式设置
//	 {
//	   PWM_count=5;
//	 }
//	 if(P0==0X07)				   //0x03这个值可以通过蓝牙调试器编辑模式设置
//	 {
//	   PWM_count=6;
//	 }
	 if(P0==0Xff)				   //0xff这个值可以通过蓝牙调试器编辑模式设置
	 {
	   mode=1;
	 }
	 if(P0==0X00)				   //0x00这个值可以通过蓝牙调试器编辑模式设置
	 {
	   PWM_L=0;PWM_R=0;
	 }

	  RI=0;
	}
}

void Uart_Init()   //9600bps@11.0592MHz
{
	PCON |= 0x80;		//使能波特率加倍	
	SCON = 0x50;		//8位数据
	TMOD &= 0x0F;		//清除定时器1模式
	TMOD |= 0x20;		//设定定时器8为自动
	TL1 = 0xFA;			//设定初始值
	TH1 = 0xFA;			//设定初始值
	ET1 = 0;		
    TR1 = 1;
    EA = 1;
    ES =1 ; 	
}


