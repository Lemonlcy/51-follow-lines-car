#include <REGX52.H>

#define LOW 20
#define MID 40
#define NOM 60
#define HIGH 80
#define FULL 100

sbit L1=P0^0;	//����̽ͷ����
sbit L2=P0^1;	//����̽ͷ����
sbit R1=P2^1;	//����̽ͷ����
sbit R2=P2^0;	//����̽ͷ����

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
