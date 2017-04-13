#define _LED_C
#include "config.h"
#include "1302.h"
#include "1602.h"
#include "Led.h"

struct LedBuf ledbuf;
uint8 code Ledchar[]={0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,
                 0x80,0x90,0x88,0x83,0xc6,0xa1,0x86,0x8e};
uint8 code WeekMod[]={
             0xFF,0xC3,0x81,0x81,0x81,0xC3,0xE7,0xFF, //��
			 0xFF,0xE7,0xE7,0xE7,0xE7,0xE7,0xE7,0xFF, //1
			 0xE3,0xDD,0xDD,0xEF,0xF7,0xFB,0xFD,0xC1, //2
			 0xFF,0xC3,0xBD,0xDF,0xE7,0xDF,0xBD,0xC3,//3
			 0xCF,0xD7,0xDB,0xDD,0xDE,0x00,0xDF,0xDF,//4
			 0xFF,0xC3,0xFB,0xFB,0xC3,0xDF,0xDF,0xC3,//5
			 0xFF,0xFB,0xFB,0xFB,0xC3,0xDB,0xDB,0xC3,//6
};
void FlowLight()	//������ʾ������
{
   static uint8 i=0;
   uint8 code tab[]={	//������
         0x7f,0x3f,0x1f,0x0f,0x87,0xc3,0xe1,0xf0,0xf8,0xfc,0xfe,0xff};
   ledbuf.alone = tab[i];
   i++;
   if(i>=sizeof(tab))
     i = 0;
}

void ShowArry(uint8 week)
{
   int8 i;
   for(i=0;i<8;i++)
   {
     ledbuf.array[i] = WeekMod[week*8 + i];
   } 
}

void ShowLed(uint8 index,uint8 num,bit point)
{						   //indexλ������num��ʾ����,pointС����
   ledbuf.led[index] = Ledchar[num];
   if(point)
   {
     ledbuf.led[index] &= 0x7f;
   }
}

void ShowTimeLed()
{
    static uint8 secbkp=0xaa;
	GetRealTime(&RealTime);
	if(secbkp != RealTime.sec)
	{	    
		ShowLed(5,RealTime.hour>>4,0);
		ShowLed(4,RealTime.hour&0x0f,1);  //ʱ
		ShowLed(3,RealTime.minute>>4,0);
		ShowLed(2,RealTime.minute&0x0f,1);  //��
		ShowLed(1,RealTime.sec>>4,0);
		ShowLed(0,RealTime.sec&0x0f,0);  //��
		ShowArry(RealTime.week&0x0f);//����
		secbkp = RealTime.sec;
	}
}

              /*LCD����*/
void ShowTimeLcd(uint8 opt)	 //opt==1,������������
{
  uint8 str[11];
  static uint8 daybkp=0xaa;
  /*if(opt)
  {
    GetRealTime(&RealTime);
  }*/
  if((daybkp!=RealTime.day)||(opt==1))
  {
	  str[0] = '2';
	  str[1] = '0';
	  str[2] = (RealTime.year>>4) + '0'; //��
	  str[3] = (RealTime.year&0x0f) + '0';
	  str[4] = '-';
	  str[5] = (RealTime.month>>4) + '0'; //��
	  str[6] = (RealTime.month&0x0f) + '0';
	  str[7] = '-';
	  str[8] = (RealTime.day>>4) + '0'; //��
	  str[9] = (RealTime.day&0x0f) + '0';
	  str[10] = '\0';
	  LedScanPause();//��ͣLED��ˢ��
	  showstr(0,0,str);
	  LedScanCon();	 //����LED��ˢ��
	  daybkp = RealTime.day;
  }
}

void interrupttimer2() interrupt 5	 //ÿ1ms�л�һ�������
{
   static uint8 i=0;   
   if(ENLED==0)
   {  
     P0 = 0xff;  //����
     P1 = (P1 & 0xf0) | i;  //ѡ������� 
	 P0 = *((uint8 data*)&ledbuf + i); //ת��Ϊdata����ָ��
     /*if(i<8)
	 {     
	  P0 = ledbuf.array[i];
	 }
	 else if(i>=8&&i<14)
	   P0 = ledbuf.led[i-8];
	 else if(i==14)
	   P0 = ledbuf.alone;*/
	 i++;
	 if(i>14)
	   i = 0;
   }
   TF2 = 0;//T2�жϱ�־ֻ���������0
}