#define _MAIN_C  //在.c文件中预定义一个_C,表示头文件的相关部分要编译
#include "config.h"
#include "main.h"
#include "1602.h"
#include "Led.h"
#include "1302.h"
#include "keyboard.h"
#include "18B20.h"
#include "Alarm.h"
#include "I2C.h"
#include "Infrared.h"

bit flag200ms=0;//200ms标志
bit flag2s;//2s标志
bit flag5s;
uint16 tmpt0; //定时器0的暂存值
enum StaSystem stasystem;
int8 Setindex=255;	//设置位索引
struct sTime SetTime;//设置时间缓冲区

void main()
{
  EA = 1; 
  configtimer0(1);
  configtimer2(1);
  init1602();
  E2Read(0x08,&alarm); 	//读取闹钟时间
  init1302(); //从E2中读取数据出来
  Start18B20();
  InitIfrered();//初始化红外
  while(!flag2s);//上电延时2s

  LedScanPause();//暂停LED的刷新
  ShowTemp();//显示温度
  showstr(0,1,"Alarm at: ");  
  ShowAlarm(1);//显示闹钟
  LedScanCon();//暂停LED的刷新

  while(1)
  {
    if(flag200ms)  //每200ms把需要显示的内容更新一遍
	{
	  flag200ms = 0; 
	  FlowLight();//流水灯显示更新
	  ShowTimeLed(); //时间显示更新
	 	  
	  if(flag2s)
	  {
	    flag2s = 0;
		if(stasystem==Normal)
		{  
		   LedScanPause();//暂停LED的刷新
		   ShowTimeLcd(0);
		   ShowTemp();//显示温度
		   //showstr(0,1,"Alarm at: ");	  
		   LedScanCon();//LED的刷新
		   MonitorAlarm();		  
		}
	  }		 	 
	keydrive();
	IrDrive();
  }
}
}

void keyaction(uint8 keymap)
{
  if(keymap>=0x30 && keymap<=0x39)	//数字键
  {
  	 InputTime(keymap-0x30);  //1302不使用ascii码运算，不然错
  }
  else if(keymap==0x27)//右键
  {
  	RightMove();
  }
  else if(keymap==0x25)//左键
  {
     LeftMove();
  }
  else if(keymap==0x0d)	 //enter
  {
      SwitchSta();
  }
  else if(keymap==0x1b)	//esc键
  {
     CanselSet();
  }

}

void CanselSet()
{
  Setindex = 255;
  if(stasystem!=Normal)
  {
	  LedScanPause();//暂停LED的刷新
	  CloseCusor();
	  ClearFull();
	  ShowTimeLcd(1);
	  ShowTemp();//显示温度
	  showstr(0,1,"Alarm at: ");
	  ShowAlarm(1);//显示闹钟
	  LedScanCon();//LED的刷新
	  stasystem = Normal;
  }
}

void InputTime(uint8 time)
{
  if(stasystem==Set_Time)
  {
   switch(Setindex)	//要是还用RealTime的话，这里刚刷新完，还未写入1302，200ms到了
   {				//又从1302中读出数据，把刚刚刷新好的RealTime覆盖了
    case 0: SetTime.year=(SetTime.year&0x0f)|(time<<4);	 //原来time用成了ascii码
	        showchar(time);								 //导致传进1302的数据错误
	        SetCusorPos(1,1);
			Setindex = 1;
			break;
	case 1:	SetTime.year=(SetTime.year&0xf0)|time;
	        showchar(time);
	        SetCusorPos(3,1);
			Setindex = 2;
			break;
	case 2: SetTime.month=(SetTime.month&0x0f)|(time<<4);
	        showchar(time);
	        SetCusorPos(4,1);
			Setindex = 3;
			break;
	case 3:	SetTime.month=(SetTime.month&0xf0)|time;
	        showchar(time);
	        SetCusorPos(6,1);
			Setindex = 4;
			break;
	case 4: SetTime.day=(SetTime.day&0x0f)|(time<<4);
	        showchar(time);
	        SetCusorPos(7,1);
			Setindex = 5;
			break;
	case 5:	SetTime.day=(SetTime.day&0xf0)|time;
	        showchar(time);
	        SetCusorPos(9,1);
			Setindex = 6;
			break;
	case 6: SetTime.week=(SetTime.week&0xf0)|time;
	        showchar(time);
	        SetCusorPos(11,1);
			Setindex = 7;
			break;
	case 7:	SetTime.hour=(SetTime.hour&0x0f)|(time<<4);
	        showchar(time);
	        SetCusorPos(12,1);
			Setindex = 8;
			break;
	case 8: SetTime.hour=(SetTime.hour&0xf0)|time;
	        showchar(time);
	        SetCusorPos(14,1);
			Setindex = 9;
			break;
	case 9:	SetTime.minute=(SetTime.minute&0x0f)|(time<<4);
	        showchar(time);
	        SetCusorPos(15,1);
			Setindex = 10;
			break;
	case 10: SetTime.minute=(SetTime.minute&0xf0)|time;
	         showchar(time);
	         SetCusorPos(0,1);
			 Setindex = 0;
			 break;
	default :break;
   }
  }
  else if(stasystem==Set_Alarm)
  {
   switch(Setindex)
   {
    case 0: alarm.hour = (alarm.hour&0x0f)|(time<<4);
	        showchar(time);
			SetCusorPos(1,1);
			Setindex = 1;
			break;
	case 1: alarm.hour = (alarm.hour&0xf0)|time;
	        showchar(time);
			SetCusorPos(3,1);
			Setindex = 2; 
			break;
	case 2: alarm.minute = (alarm.minute&0x0f)|(time<<4);
	        showchar(time);
			SetCusorPos(4,1);
			Setindex = 3;
	        break;
	case 3:	alarm.minute = (alarm.minute&0xf0)|time;
			showchar(time);
			SetCusorPos(0,1);
			Setindex = 0;
	        break;
	default: break;
   }
  }
}

void LeftMove()
{
  Setindex--;
  if(stasystem==Set_Time)
  {
  if(Setindex<0)
    Setindex = 10;
   switch(Setindex)
   {
    case 0:	SetCusorPos(0,1);break;
	case 1:	SetCusorPos(1,1);break;
	case 2:	SetCusorPos(3,1);break;
	case 3:	SetCusorPos(4,1);break;
	case 4:	SetCusorPos(6,1);break;
	case 5:	SetCusorPos(7,1);break;
	case 6:	SetCusorPos(9,1);break;
	case 7:	SetCusorPos(11,1);break;
	case 8: SetCusorPos(12,1);break;
	case 9:	SetCusorPos(14,1);break;
	case 10: SetCusorPos(15,1);break;
	default :break;
   }
  }
  else if(stasystem==Set_Alarm)
  {
    if(Setindex<0)
      Setindex = 3;
    switch(Setindex)
	{
     case 0: SetCusorPos(0,1);break;
	 case 1: SetCusorPos(1,1);break;
	 case 2: SetCusorPos(3,1);break;
	 case 3: SetCusorPos(4,1);break;
	 default :break;
	}
  }  
}

void RightMove()
{
  Setindex++;
  if(stasystem==Set_Time)
  {
   if(Setindex>=11)
    Setindex = 0;
  switch(Setindex)
  {
    case 0:	SetCusorPos(0,1);break;
	case 1:	SetCusorPos(1,1);break;
	case 2:	SetCusorPos(3,1);break;
	case 3:	SetCusorPos(4,1);break;
	case 4:	SetCusorPos(6,1);break;
	case 5:	SetCusorPos(7,1);break;
	case 6:	SetCusorPos(9,1);break;
	case 7:	SetCusorPos(11,1);break;
	case 8: SetCusorPos(12,1);break;
	case 9:	SetCusorPos(14,1);break;
	case 10: SetCusorPos(15,1);break;
	default :break;
  } 
 }
 else if(stasystem==Set_Alarm)
 {
   if(Setindex>=4)
    Setindex = 0;
   switch(Setindex)
   {
   	case 0:	SetCusorPos(0,1);break;
	case 1:	SetCusorPos(1,1);break;
	case 2:	SetCusorPos(3,1);break;
	case 3:	SetCusorPos(4,1);break;
	default :break;
   }
  }
}

void SwitchSta()
{
    uint8 pdata str[17]; 
  	if(stasystem==Normal)
	{
	  stasystem = Set_Time;
	  Setindex = 0;
	  SetTime.year = RealTime.year;
	  SetTime.month = RealTime.month;
	  SetTime.day = RealTime.day;
	  SetTime.hour = RealTime.hour;
	  SetTime.minute = RealTime.minute;
	  SetTime.week = RealTime.week;
	  str[0] = (SetTime.year>>4) + '0';	//年
	  str[1] = (SetTime.year&0x0f) + '0';
	  str[2] = '-';
	  str[3] = (SetTime.month>>4) + '0';	//月
	  str[4] = (SetTime.month&0x0f) + '0';
	  str[5] = '-';
	  str[6] = (SetTime.day>>4) + '0';	 //日
	  str[7] = (SetTime.day&0x0f) + '0';
	  str[8] = '-';	 
	  str[9] = (SetTime.week&0x0f) + '0'; //星期
	  str[10] = ' ';
	  str[11] = (SetTime.hour>>4) + '0';	 //时
	  str[12] = (SetTime.hour&0x0f) + '0';
	  str[13] = ':';
	  str[14] = (SetTime.minute>>4) + '0';  //分
	  str[15] = (SetTime.minute&0x0f) + '0';
	  str[16] = '\0';
	  LedScanPause();//暂停LED的刷新
	  ClearFull();
	  showstr(0,0,"Set Time...");
	  showstr(0,1,str);	  
	  SetCusorPos(0,1);
	  OpenCusor(); //打开光标 
	  LedScanCon();	 //继续LED的刷新
	}
	else if(stasystem==Set_Time)
	{
	   stasystem = Set_Alarm;
	   Setindex = 0; 
	   SetRealTime(&SetTime);//时间写进1302	   
	   LedScanPause();//暂停LED的刷新
	   ClearFull();
	   showstr(0,0,"Set Alarm...");
	   ShowAlarm(0);//显示设置闹钟时间
	   SetCusorPos(0,1);	   
	   LedScanCon();	 //继续LED的刷新	   
	}
	else if(stasystem == Set_Alarm)
	{
	  stasystem = Normal;	  
	  Setindex = 255;//正常显示时，Setindex=255	  	  
	  LedScanPause();//暂停LED的刷新	  
	  ClearFull();
	  ShowTimeLcd(1);  //显示日期
	  ShowTemp();//显示温度
	  showstr(0,1,"Alarm at: ");
	  ShowAlarm(1); 
	  CloseCusor();	//关闭光标
	  E2Write(0x08,&alarm);
	  LedScanCon();	 //继续LED的刷新	  
	}
}

void configtimer0(uint8 ms)
{
  tmpt0 = SYS_MCLK/1000*ms;
  tmpt0 = 65536 - tmpt0;
  tmpt0 += 18;
  TMOD &= 0xf0;
  TMOD |= 0x01;
  TH0 = (uint8)(tmpt0>>8);
  TL0 = (uint8)(tmpt0);
  ET0 = 1;
  TR0 = 1;
}

void configtimer2(uint8 ms)
{
  uint16 tmpt2;
  ENLED = 0;
  tmpt2 = SYS_MCLK/1500*ms;
  tmpt2 = 65536 - tmpt2;
  tmpt2 += 18;
  T2CON = 0x00;//T2 16位重装模式
  RCAP2H = (uint8)(tmpt2>>8);
  RCAP2L = (uint8)(tmpt2);
  TH2 = RCAP2H;
  TL2 = RCAP2L;
  PT2 = 1;//设置T2为高优先级
  ET2 = 1;
  TR2 = 1;
}

void interrupttimer0() interrupt 1
{
   static uint8 cnt=0;//200ms计数器
   static uint8 cnt2s=0;//2s计数器
   cnt++;
   if(cnt>=200)
    {
	  cnt = 0;
	  cnt2s++;
	  flag200ms = 1;
	  if(cnt2s>=10)
	  {
	    cnt2s = 0;
		flag2s = 1;
	  }
	}
    if(buzzon)
     Buzz = ~Buzz;
    else
     Buzz = 1;
  keyscan();
  TH0 = (uint8)(tmpt0>>8);
  TL0 = (uint8)(tmpt0);
}
/*在向1602写数据时，必须暂停Led的更新，不然Led的高优先级会使写入1602的数据错误，
同时写可能会在向1602写数据时，Led中断，导致写入Led的数据也是错的*/