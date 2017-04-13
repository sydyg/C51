#ifndef _LED_H
#define _LED_H

#ifndef _LED_C 
extern void ShowTimeLed();
extern void ShowTimeLcd(uint8 opt);
extern void FlowLight();
extern struct LedBuf ledbuf;
#endif
struct LedBuf {	  
	 uint8 array[8];	 //点阵
     uint8 led[6];	 //数码管
	 uint8 alone;	//独立LED	 
};

#endif