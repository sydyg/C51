#ifndef _E2PROM_H
#define _E2PROM_H

#ifndef _E2PROM_C
extern void E2Read(uint8 addr,struct sTime* inittime,struct Alarm* alarm);
extern void E2Write(uint8 addr,struct sTime* realtime,struct Alarm* alarm);
#endif

#endif