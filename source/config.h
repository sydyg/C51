#ifndef _CONFIG_H
#define _CONFIG_H

/*通用头文件的声明*/
#include<reg52.h>
#include<intrins.h>

	   /*类型别名*/
typedef unsigned char uint8;
typedef unsigned int uint16;
typedef unsigned long uint32;
typedef signed char int8;
typedef signed int int16;
typedef signed long int32;

/*Led模块引脚分配*/
sbit ADDR0 = P1^0;
sbit ADDR1 = P1^1;
sbit ADDR2 = P1^2;
sbit ADDR3 = P1^3;
sbit ENLED = P1^4;

/*1602引脚分配*/
#define LCD_DB P0
sbit RS = P1^0;
sbit RW = P1^1;
sbit ENLCD = P1^5;

/*1302引脚分配*/
sbit SIO = P3^4;
sbit SCK = P3^5;
sbit S1302 = P1^7;
/*按键引脚*/
sbit KeyIn1 = P2^4;
sbit KeyIn2 = P2^5;
sbit KeyIn3 = P2^6;
sbit KeyIn4 = P2^7;
sbit KeyOut1 = P2^3;
sbit KeyOut2 = P2^2;
sbit KeyOut3 = P2^1;
sbit KeyOut4 = P2^0;
/*1302引脚*/
sbit IO_18B20 = P3^2;
/*蜂鸣器*/
sbit Buzz = P1^6;

/*I2C*/
sbit I2C_SDA = P3^6;
sbit I2C_SCL = P3^7;
/*红外引脚*/
sbit IR_Input = P3^3;
#endif

