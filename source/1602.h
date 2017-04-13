#ifndef _1602_H
#define _1602_H

#ifndef _1602_C	 //.c文件中未定义_1602_C，编译后面的空语句
extern void init1602();
extern void LedScanPause();
extern void LedScanCon();
extern void SetCusorPos(uint8 x,uint8 y);
extern void showstr(uint8 x,uint8 y,int8* ptr);
extern void OpenCusor();
extern void CloseCusor();
extern void ClearFull();
void showchar(uint8 dat);
#endif

#endif
