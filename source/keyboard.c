#define _KEYBOARD_C
#include "config.h"
#include "keyboard.h"
uint8 pdata keysta[4][4]={{1,1,1,1},{1,1,1,1},{1,1,1,1},{1,1,1,1}};
uint8 pdata stabkp[4][4]={{1,1,1,1},{1,1,1,1},{1,1,1,1},{1,1,1,1}};
uint8 code keymap[4][4]={{0x31,0x32,0x33,0x26},	//��
                        {0x34,0x35,0x36,0x25},  //��
                        {0x37,0x38,0x39,0x28},	//��
                        {0x30,0x1b,0x0d,0x27}};	//��
void KeyDrive()
{
   uint8 i,j;
   for(i=0;i<4;i++)
   {
     for(j=0;j<4;j++)
	 {
	   if(stabkp[i][j]!=keysta[i][j])
	   {
	     stabkp[i][j] = keysta[i][j];
		 if(stabkp[i][j]==0)
		 {
		    KeyAction(keymap[i][j]);
		 }
	   }
	 }
   }
}
void keyscan()
{
  uint8 i;
  static uint8 keyout = 0;
  static uint8 keynum[4][4]={{0xff,0xff,0xff,0xff},{0xff,0xff,0xff,0xff},
                       {0xff,0xff,0xff,0xff},{0xff,0xff,0xff,0xff}};
  keynum[keyout][0] = (keynum[keyout][0]<<1) | KeyIn1;
  keynum[keyout][1] = (keynum[keyout][1]<<1) | KeyIn2;
  keynum[keyout][2] = (keynum[keyout][2]<<1) | KeyIn3;
  keynum[keyout][3] = (keynum[keyout][3]<<1) | KeyIn4;
  for(i=0;i<4;i++)
  {
    if((keynum[keyout][i]&0x0f)==0x0f)//��״̬����4��Ϊ1����Ϊ����
	{
	   keysta[keyout][i] = 1;
	}
    else if((keynum[keyout][i]&0x0f)==0x00)//��״̬����4��Ϊ0����Ϊ����
	{
	   keysta[keyout][i] = 0;		//�м�Ķ���ȫ���ܣ�ֻ���ȶ�״̬�Ľ��
	}
  }
  keyout++;
  keyout &= 0x03;
  switch(keyout)
  {
    case 0: KeyOut4 = 1;KeyOut1 = 0;break;
    case 1: KeyOut1 = 1;KeyOut2 = 0;break;
	case 2: KeyOut2 = 1;KeyOut3 = 0;break;
	case 3: KeyOut3 = 1;KeyOut4 = 0;break;
	default:break;
  }  
}

