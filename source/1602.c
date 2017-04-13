#define _1602_C   //��仰������ͷ�ļ�����������
#include "config.h"
#include "1602.h" 
uint8 tmpp0; //�ݴ�P0
bit tmpAD0;	//�ݴ�ADDR0
bit tmpAD1;	//�ݴ�ADDR1
void LedScanPause()	//��ͣLed��ɨ��
{
  tmpp0 = P0;
  tmpAD0 = ADDR0;
  tmpAD1 = ADDR1;
  ENLED = 1;
  ET2 = 0;
}
/*Ӳ�������ȱ�ݣ���һ���ܹ������е�LED���ص�*/
void LedScanCon() //����Led��ɨ��
{   
   ADDR0 = tmpAD0;
   ADDR1 = tmpAD1;
   P0 = tmpp0;
   ENLED = 0;
   ET2 = 1;
}
/*Һ������ʱ����ͣLed��ɨ��*/
void busycheck()
{
   uint8 sta;
   LCD_DB = 0xff;  //����Ĵ���д1
   RS = 0;
   RW = 1; 
   do 
   {
     ENLCD = 1;
     sta = LCD_DB;	//��״̬Ҳ�Ƕ�����
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

void SetCusorPos(uint8 x,uint8 y)   //����д�����ݵĳ�ʼRAM��ַ
{
    uint8 addr; 
  	if(y)
	   addr = 0x40 + x;
	else
	   addr = 0x00 + x;
	 writecmd(addr|0x80);
}

void showstr(uint8 x,uint8 y,int8* ptr) //�������ͽ���ʾRAM
{
  	SetCusorPos(x, y);
	while(*ptr!='\0')   //һ����ʾ16���ַ�
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
