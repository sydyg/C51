#define _INFRARED_C
#include "config.h"
#include "main.h"
uint8 code irmap[][2]={  //������뵽��׼PC�����ӳ���
    {0x45,0x00}, {0x46,0x00}, {0x47,0x00}, //����->��  Mode->��   ����->��
    {0x44,0x00}, {0x40,0x25}, {0x43,0x27}, //����->��  ����->���� ǰ��->����
    {0x07,0x00}, {0x15,0x28}, {0x09,0x26}, // EQ->��   ����->���� �Ӻ�->����
    {0x16, '0'}, {0x19,0x1B}, {0x0D,0x0D}, //'0'->'0'  ��ͷ->ESC  U/SD->�س�
    {0x0C, '1'}, {0x18, '2'}, {0x5E, '3'}, //'1'->'1'  '2'->'2'   '3'->'3'
    {0x08, '4'}, {0x1C, '5'}, {0x5A, '6'}, //'4'->'4'  '5'->'5'   '6'->'6'
    {0x42, '7'}, {0x52, '8'}, {0x4A, '9'}, //'7'->'7'  '6'->'8'   '9'->'9'
};
uint8 rbuf[4];//������ջ�����
bit irflag=0;

void IrDrive()
{
  uint8 i; 
  if(irflag)
  {
    irflag = 0;
	for(i=0;i<sizeof(irmap)/2;i++)
	{
	  if(rbuf[2]==irmap[i][0])//�������еļ���
	  {
	    keyaction(irmap[i][1]);
		return;
	  }
	}
  }
}

void InitIfrered()
{
  IR_Input = 1;//�ͷź����������
  TMOD &= 0x0f;
  TMOD |= 0x10;
  TR1 = 0;//ֹͣT1��ʱ
  ET1 = 0;//��ֹT1�ж�
  EX1 = 1;//�ⲿ�ж�1ʹ��
  IT1 = 1;//�����ⲿ�ж�Ϊ�½��ش���
}

uint16 GetHighTime()//��ȡ�ߵ�ƽ
{
  TH1 = 0;
  TL1 = 0;
  TR1 = 1;
  while(IR_Input) //Ϊ��ʱ��һֱ��ʱ
  {
    if(TH1>0x40)//��ʱ>0x4000,������18msʱ
	{
	  break;   //�˳�����������źŵ��³������
	}
  }
  TR1 = 0;  //��ʱ��ϣ�ֹͣ��ʱ
  return (TH1*256 + TL1); //���ؼ�ʱֵ
}

uint16 GetLowTime()//��ȡ�ߵ�ƽ
{
  TH1 = 0;
  TL1 = 0;
  TR1 = 1;
  while(!IR_Input) //Ϊ��ʱ��һֱ��ʱ
  {
    if(TH1>0x40)//��ʱ>0x4000,������18msʱ
	{
	  break;   //�˳�����������źŵ��³������
	}
  }
  TR1 = 0;  //��ʱ��ϣ�ֹͣ��ʱ
  return (TH1*256 + TL1); //���ؼ�ʱֵ
}

void interruptINT1() interrupt 2
{
  uint16 Time;
  int8 i,j;
  uint8 byte=0;
  Time = GetLowTime();
  if((Time<7833)||(Time>8755)) //����8.5ms-9.5ms,����ʱ��
  {
     IE1 = 0; //�ⲿ�жϱ�־��0
	 return;
  }
  Time = GetHighTime();
  if((Time<3686)||(Time>4608)) //����4ms-5ms,����ʱ��
  {
     IE1 = 0; //�ⲿ�жϱ�־��0
	 return;
  }
  for(i=0;i<4;i++)
  {
    for(j=0;j<8;j++)
	{
	  Time = GetLowTime();
	  if((Time<313)||(Time>718)) //����340us-780us,����ʱ��
	  {
	     IE1 = 0; //�ⲿ�жϱ�־��0
		 return;
	  }
	  Time = GetHighTime();
	  if((Time>312)&&(Time<716)) //��340us-780us,����ֵ0
	  {
	     byte >>= 1;
	  }
	  else if((Time>1345)&&(Time<1751))	//��1460us-1900us������ֵ1
	  {
	     byte >>= 1;
		 byte |= 0x80;
	  }
	  else 	//��������
	  {
	     IE1 = 0; //�ⲿ�жϱ�־��0
		 return;
	  }
	}
	rbuf[i] = byte;	 //���յ�����д��������
  }
  irflag = 1; //������ɱ�־
  IE1 = 0;
}