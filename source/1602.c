#define _1602_C   //这句话，控制头文件的条件编译
#include "config.h"
#include "1602.h" 
uint8 tmpp0; //暂存P0
bit tmpAD0;	//暂存ADDR0
bit tmpAD1;	//暂存ADDR1
void LedScanPause()	//暂停Led的扫描
{
  tmpp0 = P0;
  tmpAD0 = ADDR0;
  tmpAD1 = ADDR1;
  ENLED = 1;
  ET2 = 0;
}
/*硬件设计有缺陷，不一定能够把所有的LED都关掉*/
void LedScanCon() //继续Led的扫描
{   
   ADDR0 = tmpAD0;
   ADDR1 = tmpAD1;
   P0 = tmpp0;
   ENLED = 0;
   ET2 = 1;
}
/*液晶操作时，暂停Led的扫描*/
void busycheck()
{
   uint8 sta;
   LCD_DB = 0xff;  //先向寄存器写1
   RS = 0;
   RW = 1; 
   do 
   {
     ENLCD = 1;
     sta = LCD_DB;	//读状态也是读数据
	 ENLCD = 0;
   }while(sta&0x80);
}

void writecmd(uint8 cmd)
{
   busycheck();
   RS = 0;
   RW = 0;
   LCD_DB = cmd;
   ENLCD = 1;
   ENLCD = 0;
}

void writedat(uint8 dat)
{
   busycheck();
   RS = 1;
   RW = 0;
   LCD_DB = dat;
   ENLCD = 1;
   ENLCD = 0;
}

void SetCusorPos(uint8 x,uint8 y)   //设置写入数据的初始RAM地址
{
    uint8 addr; 
  	if(y)
	   addr = 0x40 + x;
	else
	   addr = 0x00 + x;
	 writecmd(addr|0x80);
}

void showstr(uint8 x,uint8 y,int8* ptr) //将数据送进显示RAM
{
  	SetCusorPos(x, y);
	while(*ptr!='\0')   //一次显示16个字符
	{
	  writedat(*ptr++);
	}
}

void showchar(uint8 dat)
{
   writedat(dat+'0');
}

void OpenCusor()
{
  writecmd(0x0f);
}

void CloseCusor()
{
  writecmd(0x0c);
}
void ClearFull()
{
  writecmd(0x01);
}
void init1602()
{
   writecmd(0x38);
   writecmd(0x0c);
   writecmd(0x06);
   writecmd(0x01);
}
