#define _ALARM_C
#include "config.h"
#include "Alarm.h"
#include "1602.h"
#include "1302.h"
#include "I2C.h"

bit buzzon=0;
uint8 pdata tbuf[6]; //����ʱ�仺����
struct Alarm data alarm={0x06,0x30};
void ShowAlarm(bit choise)
{   
   tbuf[0] = (alarm.hour>>4) + '0';
   tbuf[1] = (alarm.hour&0x0f) + '0';
   tbuf[2] = ':';
   tbuf[3] = (alarm.minute>>4) + '0';
   tbuf[4] = (alarm.minute&0x0f) + '0';
   tbuf[5] = '\0';
   LedScanPause();//��ͣLED��ˢ��
   if(choise)
   {	
	 showstr(10,1,tbuf);
   }
   else
     showstr(0,1,tbuf);
   LedScanCon();//LED��ˢ��
}
void MonitorAlarm()
{
  if((RealTime.hour==alarm.hour)&&(RealTime.minute==alarm.minute))
  {
	 buzzon = 1;
  }	
  else
	 buzzon = 0; 
}

void E2Read(uint8 addr,struct Alarm* alarm)
{
   do
   {
      I2Cstart();
	  if(I2Cwritebus(0x50<<1)==0)
	  {
	    break;
	  }
	 I2Cstop();
   }while(1);
   I2Cwritebus(addr);
   I2Cstart();
   I2Cwritebus((0x50<<1)|0x01);
   alarm->hour = I2CReadACK();
   alarm->minute = I2CReadNAK();
   I2Cstop();  
}

void E2Write(uint8 addr,struct Alarm* alarm)
{
  int8 len = 2;//����2���ֽ�
  uint8 pdata buf[2];
  buf[0] = alarm->hour;
  buf[1] = alarm->minute;
  while(len>0)
  {
  	do
   {
      I2Cstart();
	  if(I2Cwritebus(0x50<<1)==0)	 //δ��Ӧ��һֱ������ϵ,0����Ӧ
	  {
	    break;
	  }
	 I2Cstop();
   }while(1);
   I2Cwritebus(addr);
   while(len>0)
   {
      I2Cwritebus(buf[2-len]);
	  addr++;
	  len--;
	  if(addr&0x07)	  //д����ҳ�߽磬ֹͣ
	  {
	     break;
	  }
    }
   I2Cstop();  //����д�ֹ꣬ͣ
  }
}

