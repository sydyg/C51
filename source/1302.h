#ifndef _1302_H
#define _1302_H

struct sTime{
  uint8 year; //��ĺ���λ
  uint8 month;
  uint8 day;
  uint8 hour;
  uint8 minute;
  uint8 sec;
  uint8 week;
};

#ifndef _1302_C
extern struct sTime code InitTime;
extern struct sTime RealTime;
extern void GetRealTime(struct sTime* Realtime);
extern void SetRealTime(struct sTime* RealTime); //��1302д����
extern void init1302();
#endif

#endif