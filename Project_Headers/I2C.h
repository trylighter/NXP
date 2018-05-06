/*
 * I2C.h
 *
 *  Created on: Mar 4, 2018
 *      Author: Avina
 */

#ifndef I2C_H_
#define I2C_H_

void Pause(int num);
void I2C_READ_MULTI(unsigned char Device, unsigned char Address, unsigned char n, unsigned char*r);
unsigned char I2C_READ_BYTE(unsigned char Device, unsigned char Address);
void I2C_WRITE_BYTE(unsigned char Device, unsigned char Address, unsigned char Data);

#endif /* I2C_H_ */
