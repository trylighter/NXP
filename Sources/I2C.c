/*
 * I2C.c
 *
 *  Created on: Mar 4, 2018
 *      Author: Avina
 */

#include "derivative.h"
#include "I2C.h"

#define I2C_DisableAck()       I2C0_C1 |= I2C_C1_TXAK_MASK
#define I2C_EnableAck()        I2C0_C1 &= ~I2C_C1_TXAK_MASK
#define I2C_RepeatedStart()    I2C0_C1 |= I2C_C1_RSTA_MASK
#define I2C_EnterRxMode()      I2C0_C1 &= ~I2C_C1_TX_MASK

#define I2C_START()            I2C0_C1 |= I2C_C1_TX_MASK;\
                               I2C0_C1 |= I2C_C1_MST_MASK

#define I2C_STOP()             I2C0_C1 &= ~I2C_C1_MST_MASK;\
                               I2C0_C1 &= ~I2C_C1_TX_MASK

#define I2C_WAIT()             while((I2C0_S & I2C_S_IICIF_MASK)==0) {} \
                               I2C0_S |= I2C_S_IICIF_MASK;

void Pause(int num){
	int i;
	for(i=0; i<num; i++)
		asm("nop");
}

void I2C_READ_MULTI(unsigned char Device, unsigned char Address, unsigned char n, unsigned char *r){
	char i;	
	I2C_START();	          
	I2C0_D = Device<<1;									/* Send I2C device address with W/R bit = 0 */
	I2C_WAIT();										

	I2C0_D = Address;										/* Send register address */
	I2C_WAIT();

	I2C_RepeatedStart();
		
	I2C0_D = ((Device<<1)| 0x01);							/* Send I2C device address this time with W/R bit = 1 */ 
	I2C_WAIT();	

	I2C_EnterRxMode();
	I2C_EnableAck();
	
	i = I2C0_D;
	I2C_WAIT();
	
	for(i=0; i<n-2; i++){
	    *r = I2C0_D;
	    r++;
	    I2C_WAIT();
	};
	
	I2C_DisableAck();
	*r = I2C0_D;
	r++;
	I2C_WAIT();
	I2C_STOP();
	*r = I2C0_D; 
	Pause(50);		
}

unsigned char I2C_READ_BYTE(unsigned char Device, unsigned char Address){
	unsigned char Data;
	I2C_START();					/*set to transmit mode */
	I2C0_D = Device<<1; 				/*1. send device address	*/
	I2C_WAIT();						/*wait for completion */
	I2C0_D = Address;		        /*2. send read address	*/
	I2C_WAIT();						/*wait for completion */
			
	I2C_RepeatedStart();            // Select Repeat Start
	I2C0_D = ((Device<<1)|0x1);	        /*3. send device address (read)	*/
	I2C_WAIT();						/*wait for completion */
	
	I2C_EnterRxMode();              //I2C bus to receive
	I2C_DisableAck();               //Acknowledgement of Data NOT received
		
	Data = I2C0_D;			  	    
	I2C_WAIT();						/*wait for completion */
	I2C_STOP();                     // Select Master Mode (Stop)
	Data = I2C0_D;			        /*read Data	*/
	Pause(50);
	return Data; 	
};

void I2C_WRITE_BYTE(unsigned char Device, unsigned char Address, unsigned char Data){
	I2C_START();				/*set to transmit mode & send start*/
	I2C0_D = Device<<1;			/*send device address	*/
	I2C_WAIT();					/*wait for acknowledgement */	
	I2C0_D = Address;			/*send write address	*/
	I2C_WAIT();					/*wait for acknowledgement */		
	I2C0_D = Data;				/*send data	*/
	I2C_WAIT();					/*wait for acknowledgement */
	I2C_STOP();                 /* (Stop) */
	Pause(50);
};

