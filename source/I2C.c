#include<reg52.h>
#define  _I2C_C
#include "config.h"

#define I2Cdelay() {_nop_();_nop_();_nop_();_nop_();}

void I2Cstart()
{
  	I2C_SDA = 1;
	I2C_SCL = 1;
	I2Cdelay();
	I2C_SDA = 0;
	I2Cdelay();
	I2C_SCL = 0;
}

void I2Cstop()
{
   I2C_SDA = 0;
   I2C_SCL = 0;
   I2Cdelay();
   I2C_SCL = 1;
   I2Cdelay();
   I2C_SDA = 1;
   I2Cdelay();
}

bit I2Cwritebus(uint8 dat)
{
  uint8 mask;
  bit ack=1;
  for(mask=0x80;mask!=0;mask>>=1)
  {
    if(mask&dat)
	  I2C_SDA = 1;
	else
	  I2C_SDA = 0;
	I2Cdelay();
	I2C_SCL = 1;
	I2Cdelay();
	I2C_SCL = 0;
  }
  I2C_SDA = 1;
  I2Cdelay();
  I2C_SCL = 1;
  ack = I2C_SDA;
  I2Cdelay();
  I2C_SCL = 0;
  return ack;
}

uint8 I2CReadNAK()
{
   uint8 dat;
   uint8 mask; 
   I2C_SDA = 1;
   for(mask=0x80;mask!=0;mask>>=1)
   {
      I2Cdelay();
	  I2C_SCL = 1;
      if(I2C_SDA)
	    dat |= mask;
	  else
	    dat &= ~mask;
	 I2Cdelay();
	 I2C_SCL = 0;
   }
   I2C_SDA = 1;
   I2Cdelay();
   I2C_SCL = 1;
   I2Cdelay();
   I2C_SCL = 0;
   return dat;
}

uint8 I2CReadACK()
{
   uint8 dat;
   uint8 mask; 
   I2C_SDA = 1;
   for(mask=0x80;mask!=0;mask>>=1)
   {
      I2Cdelay();
	  I2C_SCL = 1;
      if(I2C_SDA)
	    dat |= mask;
	  else
	    dat &= ~mask;
	 I2Cdelay();
	 I2C_SCL = 0;
   }
   I2C_SDA = 0;
   I2Cdelay();
   I2C_SCL = 1;
   I2Cdelay();
   I2C_SCL = 0;
   return dat;
}
