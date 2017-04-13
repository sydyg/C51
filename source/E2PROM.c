#define _E2PROM_C
#include "config.h"
#include "Alarm.h"
#include "1302.h"
#include "I2C.h"
void E2Read(uint8 addr,struct sTime* inittime,struct Alarm* alarm)
{
   do
   {
      I2Cstart();
	  if(I2Cwritebus(0x50<<1))
	  {
	    break;
	  }
	 I2Cstop();
   }while(1);
   I2Cwritebus(addr);
   I2Cstart();
   I2Cwritebus((0x50<<1)|0x01);
   inittime->year = I2CReadACK();
   inittime->month = I2CReadACK();
   inittime->day = I2CReadACK();
   inittime->hour = I2CReadACK();
   inittime->minute = I2CReadACK();
   inittime->sec = I2CReadACK();
   inittime->week = I2CReadACK();
   alarm->hour = I2CReadACK();
   alarm->minute = I2CReadNAK();  
}

void E2Write(uint8 addr,struct sTime* realtime,struct Alarm* alarm)
{
  uint8 len = 9;//日期加上闹钟9个字节
  uint8 pdata buf[9];
  buf[0] = realtime->year;
  buf[1] = realtime->month;
  buf[2] = realtime->day;
  buf[3] = realtime->hour;
  buf[4] = realtime->minute;
  buf[5] = realtime->sec;
  buf[6] = realtime->week;
  buf[7] = alarm->hour;
  buf[8] = alarm->minute;
  while(len>0)
  {
  	do
   {
      I2Cstart();
	  if(I2Cwritebus(0x50<<1))	 //未响应就一直建立联系
	  {
	    break;
	  }
	 I2Cstop();
   }while(1);
   I2Cwritebus(addr);
   while(len>0)
   {
      I2Cwritebus(buf[9-len]);
	  addr++;
	  len--;
	  if(addr&0x07)	  //写到了页边界，停止
	  {
	     break;
	  }
    }
   I2Cstop();  //数据写完，停止
  }
}