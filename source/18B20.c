#define _18B20_C
#include "config.h"
#include "18B20.h"
#include "1602.h"

void DelayX10us(uint8 time)
{
  while(time--)	//time--ռ��һ���������ڣ�����8����ʱ��������10us
  {
     _nop_();
	 _nop_();
	 _nop_();
	 _nop_();
	 _nop_();
	 _nop_();
	 _nop_();
	 _nop_();
  }
}
bit Get18B20Ack()
{
   bit ack=1;
   EA = 0;
   IO_18B20 = 0;//��������
   DelayX10us(50);//����500us
   IO_18B20 = 1;//�ͷ�����
   DelayX10us(6);//�ȴ�60us
   ack = IO_18B20;//��ȡACK
   while(!IO_18B20); //�ȴ������������
   EA = 1;
   return ack;
}

uint8 Read18B20bus()
{
   uint8 dat=0;
   uint8 mask;
   EA = 0;
   for(mask=0x01;mask!=0;mask<<=1)
   {
     IO_18B20 = 0;
	 _nop_();
	 _nop_();
	 IO_18B20 = 1;
	 _nop_();
	 _nop_();
	 if(IO_18B20==1)
	   dat |= mask;
	 else
	   dat &= ~mask;
	 IO_18B20 = 1;
	 DelayX10us(6);//�ȴ�60us
	 
   }
   EA = 1;
   return dat;
}

void Write18B20bus(uint8 dat)
{
  	uint8 mask;
	EA = 0;//��18B20ͨ�ţ������ֹ�жϵĸ���
	for(mask=0x01;mask!=0;mask<<=1)
	{
	   IO_18B20 = 0;
	   _nop_();
	   _nop_();
	   if(dat&mask)
	   {
	     IO_18B20 = 1;//�ͷ�����
	   }
	   else
		 IO_18B20 = 0;
	  DelayX10us(6);//1�ĳ���ʱ�����60us,DS18B20һ���ܲɵ���
	  IO_18B20 = 1;
	}
	EA = 1;
}

bit GetTemp(int16* temp)	 //tempΪ�¶�ֵ
{
   uint8 LSB,MSB;
   bit ack=1;
   ack = Get18B20Ack();
   if(ack==0)
   {
       Write18B20bus(0xcc);
	   Write18B20bus(0xbe);
	   LSB = Read18B20bus();
	   MSB = Read18B20bus();
	   *temp = ((uint16)MSB<<8) + LSB;
   }
   return ack;
}

bit Start18B20()  //�¶�ת����ʼ
{
   bit ack = 1;
   ack = Get18B20Ack();
   if(ack==0)
   {
      Write18B20bus(0xcc);
	  Write18B20bus(0x44);
   }
   return ack;
}

void int2str(uint8*ptr,int16 num)
{
  uint8 pdata buf[3];//�������ݵĻ�����
  int8 i=0;
  if(num<0)
  {
    *ptr++ = '-';
	 num = -num;
  }
  while(num!=0)
  {
    buf[i++] = num%10;
	num /= 10;
  }
  i--;
  for(;i>=0;i--)
  {
    *ptr++ = buf[i]+'0';
  }
  *ptr++ = '\'';
  *ptr++ = 'C';
  *ptr = '\0';
}

void ShowTemp()
{
  int16 temp;//�洢�¶�ֵ
  static bit reg=1;
  //uint8 decT; //T��С������
  int16 intT; //T����������
  uint8 pdata str[5];
   reg = GetTemp(&temp);
   if(reg==0)  //�����¶���
   {
     intT = temp>>4;
	 //decT = (uint8)(temp & 0x0f);
	 int2str(str,intT);
	 LedScanPause();//��ͣLED��ˢ��
	 showstr(11,0,str);
	 LedScanCon();//LED��ˢ��
   }
   Start18B20();
}

