#define _1302_C
#include "config.h"
#include"1302.h"

struct sTime RealTime;
struct sTime code InitTime={0x16,0x02,0x19,0x12,0x23,0x30,0x05};
void Writebus(uint8 dat)
{
  uint8 mask;
  //SCK = 0;
  for(mask=0x01;mask!=0;mask<<=1)
  {
    if(dat&mask)
	  SIO = 1;
	else 
	  SIO = 0;
	SCK = 1;
	SCK = 0;
  }
  SIO = 1;
}

uint8 Readbus()
{
  uint8 dat=0;
  uint8 mask;
  for(mask=0x01;mask!=0;mask<<=1)
  {
    if(SIO)
	  dat |= mask;
	SCK = 1;
	SCK = 0;
  }
  return dat;
}

uint8 SingleByteRead(uint8 reg)
{
  uint8 dat;
  S1302 = 1;
  Writebus((reg<<1)|0x81);
  dat = Readbus();
  S1302 = 0;
  return dat;
}
void SingleByteWrite(uint8 reg,uint8 dat)
{
  S1302 = 1;
  Writebus((reg<<1)|0x80);
  Writebus(dat);
  S1302 = 0;
}

void BurstRead(uint8* buf)
{
   int8 i;
   S1302 = 1;
   Writebus(0xbf);
   for(i=0;i<8;i++)
   {
     buf[i] = Readbus();
   }
   S1302 = 0;
}

void BurstWrite(uint8* buf)
{
   int8 i; 
   S1302 = 1;
   Writebus(0xbe);
   for(i=0;i<8;i++)
   {
     Writebus(buf[i]);
   }
   S1302 = 0;
}

void SetRealTime(struct sTime* RealTime) //向1302写数据
{
    uint8 buf[8];
	buf[7] = 0;
	buf[0] = RealTime -> sec;
	buf[1] = RealTime -> minute;
	buf[2] = RealTime -> hour;
	buf[3] = RealTime -> day;
	buf[4] = RealTime -> month;
	buf[5] = RealTime -> week;
	buf[6] = RealTime -> year;
	BurstWrite(buf);
}

void GetRealTime(struct sTime* RealTime) //读1302时间
{
   uint8 buf[8];
   BurstRead(buf);
   RealTime -> sec = buf[0];
   RealTime -> minute = buf[1];
   RealTime -> hour = buf[2];
   RealTime -> day = buf[3];
   RealTime -> month = buf[4];
   RealTime -> year = buf[6];
   RealTime -> week = buf[5];
}

void init1302()
{
  uint8 dat;
  S1302 = 0;
  SCK = 0; 
  dat = SingleByteRead(0);
  if((dat&0x80)!=0)
  {
      SingleByteWrite(7,0x00);
	  SetRealTime(&InitTime);
  }
}