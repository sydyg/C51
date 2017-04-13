#ifndef _MAIN_H
#define _MAIN_H
#define SYS_MCLK (11059200/12)  //机器周期

enum StaSystem {Normal,Set_Time,Set_Alarm};

#ifndef _MAIN_C	 //看夹在if和endif之间的程序有没有编译的必要
extern void keyaction(uint8 keymap);

#else
void LeftMove();
void RightMove();
void InputTime(uint8 time);
void SwitchSta();
void configtimer0(uint8 ms);
void configtimer2(uint8 ms);
void CanselSet();
#endif

#endif

