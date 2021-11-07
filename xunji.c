#include <REGX52.H>

#define LOW 20
#define MID 40
#define NOM 60
#define HIGH 80
#define FULL 100

sbit L1=P0^0;	//红外探头边左
sbit L2=P0^1;	//红外探头中左
sbit R1=P2^1;	//红外探头中右
sbit R2=P2^0;	//红外探头边右

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
