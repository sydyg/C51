#ifndef _MAIN_H
#define _MAIN_H
#define SYS_MCLK (11059200/12)  //��������

enum StaSystem {Normal,Set_Time,Set_Alarm};

#ifndef _MAIN_C	 //������if��endif֮��ĳ�����û�б���ı�Ҫ
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

