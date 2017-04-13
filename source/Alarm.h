#ifndef _ALARM_H
#define _ALARM_H

struct Alarm {
  uint8 hour;
  uint8 minute;
};

#ifndef _ALARM_C
extern bit buzzon;
extern void ShowAlarm(bit choise);
extern struct Alarm data alarm;
extern void MonitorAlarm();
extern void E2Read(uint8 addr,struct Alarm* alarm);
extern void E2Write(uint8 addr,struct Alarm* alarm);
#endif


#endif
