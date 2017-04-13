#ifndef	_I2C_H
#define	_I2C_H

#ifndef _I2C_C
extern void I2Cstart();
extern void I2Cstop();
extern uint8 I2CReadNAK();
extern uint8 I2CReadACK();
extern bit I2Cwritebus(uint8 dat);
#endif

#endif