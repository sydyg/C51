#define _INFRARED_C
#include "config.h"
#include "main.h"
uint8 code irmap[][2]={  //红外键码到标准PC键码的映射表
    {0x45,0x00}, {0x46,0x00}, {0x47,0x00}, //开关->无  Mode->无   静音->无
    {0x44,0x00}, {0x40,0x25}, {0x43,0x27}, //播放->无  后退->向左 前进->向右
    {0x07,0x00}, {0x15,0x28}, {0x09,0x26}, // EQ->无   减号->向下 加号->向上
    {0x16, '0'}, {0x19,0x1B}, {0x0D,0x0D}, //'0'->'0'  箭头->ESC  U/SD->回车
    {0x0C, '1'}, {0x18, '2'}, {0x5E, '3'}, //'1'->'1'  '2'->'2'   '3'->'3'
    {0x08, '4'}, {0x1C, '5'}, {0x5A, '6'}, //'4'->'4'  '5'->'5'   '6'->'6'
    {0x42, '7'}, {0x52, '8'}, {0x4A, '9'}, //'7'->'7'  '6'->'8'   '9'->'9'
};
uint8 rbuf[4];//红外接收缓冲区
bit irflag=0;

void IrDrive()
{
  uint8 i; 
  if(irflag)
  {
    irflag = 0;
	for(i=0;i<sizeof(irmap)/2;i++)
	{
	  if(rbuf[2]==irmap[i][0])//遍历所有的键码
	  {
	    keyaction(irmap[i][1]);
		return;
	  }
	}
  }
}

void InitIfrered()
{
  IR_Input = 1;//释放红外接收引脚
  TMOD &= 0x0f;
  TMOD |= 0x10;
  TR1 = 0;//停止T1计时
  ET1 = 0;//禁止T1中断
  EX1 = 1;//外部中断1使能
  IT1 = 1;//设置外部中断为下降沿触发
}

uint16 GetHighTime()//获取高电平
{
  TH1 = 0;
  TL1 = 0;
  TR1 = 1;
  while(IR_Input) //为高时，一直计时
  {
    if(TH1>0x40)//计时>0x4000,即大于18ms时
	{
	  break;   //退出，避免错误信号导致程序假死
	}
  }
  TR1 = 0;  //计时完毕，停止计时
  return (TH1*256 + TL1); //返回计时值
}

uint16 GetLowTime()//获取高电平
{
  TH1 = 0;
  TL1 = 0;
  TR1 = 1;
  while(!IR_Input) //为高时，一直计时
  {
    if(TH1>0x40)//计时>0x4000,即大于18ms时
	{
	  break;   //退出，避免错误信号导致程序假死
	}
  }
  TR1 = 0;  //计时完毕，停止计时
  return (TH1*256 + TL1); //返回计时值
}

void interruptINT1() interrupt 2
{
  uint16 Time;
  int8 i,j;
  uint8 byte=0;
  Time = GetLowTime();
  if((Time<7833)||(Time>8755)) //不在8.5ms-9.5ms,错误时间
  {
     IE1 = 0; //外部中断标志清0
	 return;
  }
  Time = GetHighTime();
  if((Time<3686)||(Time>4608)) //不在4ms-5ms,错误时间
  {
     IE1 = 0; //外部中断标志清0
	 return;
  }
  for(i=0;i<4;i++)
  {
    for(j=0;j<8;j++)
	{
	  Time = GetLowTime();
	  if((Time<313)||(Time>718)) //不在340us-780us,错误时间
	  {
	     IE1 = 0; //外部中断标志清0
		 return;
	  }
	  Time = GetHighTime();
	  if((Time>312)&&(Time<716)) //在340us-780us,比特值0
	  {
	     byte >>= 1;
	  }
	  else if((Time>1345)&&(Time<1751))	//在1460us-1900us，比特值1
	  {
	     byte >>= 1;
		 byte |= 0x80;
	  }
	  else 	//错误数据
	  {
	     IE1 = 0; //外部中断标志清0
		 return;
	  }
	}
	rbuf[i] = byte;	 //接收的数据写进缓冲区
  }
  irflag = 1; //接收完成标志
  IE1 = 0;
}