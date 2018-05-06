/*
 * Declaration.c
 *
 *  Created on: Feb 10, 2018
 *      Author: Avina
 */

#include "derivative.h"
#include "Declaration.h"
#include "I2C.h"
#include "Acce.h"

int temp_reg;                               // temporary register

void initialisation (void){
	int i;
	// configure clock to 48 MHz from a 8 MHz crystal
	MCG_C2 = (MCG_C2_RANGE0(1) | MCG_C2_EREFS0_MASK);     // configure the oscillator settings
    MCG_C1 = (MCG_C1_CLKS(2) | MCG_C1_FRDIV(3));        // divider for 8 MHz clock
    for (i = 0 ; i < 24000 ; i++);                        // wait for OSCINIT to set
    	// now in FBE mode
	MCG_C6 |= MCG_C6_CME0_MASK;        // enable the clock monitor
	MCG_C5 |= MCG_C5_PRDIV0(1);     // set PLL ref divider to divide by 2
	temp_reg = MCG_C6;                 // store present C6 value (as CME0 bit was previously set)
	temp_reg &= ~MCG_C6_VDIV0_MASK; // clear VDIV settings
	temp_reg |= MCG_C6_PLLS_MASK | MCG_C6_VDIV0(0);     // write new VDIV and enable PLL
	MCG_C6 = temp_reg;                 // update MCG_C6
	for (i = 0 ; i < 4000 ; i++);     // wait for PLLST status bit to set
		// now in PBE mode
	SIM_CLKDIV1 = (SIM_CLKDIV1_OUTDIV1(1) | SIM_CLKDIV1_OUTDIV4(1));    // core clock, bus clock div by 2
	MCG_C1 &= ~MCG_C1_CLKS_MASK;    // switch CLKS mux to select the PLL as MCGCLKOUT
	for (i = 0 ; i < 2000 ; i++);    // Wait for clock status bits to update
		// now in PEE mode, core and system clock 48 MHz, bus and flash clock 24 MHz
	      
	// turn on all port clocks
	SIM_SCGC5 = SIM_SCGC5_PORTA_MASK | SIM_SCGC5_PORTB_MASK | SIM_SCGC5_PORTC_MASK | SIM_SCGC5_PORTD_MASK | SIM_SCGC5_PORTE_MASK;
	    
	// turn on all UART clocks - not needed in this project
	SIM_SCGC4 = SIM_SCGC4_UART0_MASK | SIM_SCGC4_UART1_MASK | SIM_SCGC4_UART2_MASK;
	    
	// turn on all TPM clocks
	SIM_SCGC6 |= SIM_SCGC6_TPM0_MASK | SIM_SCGC6_TPM1_MASK | SIM_SCGC6_TPM2_MASK;
	    
	// turn on ADC0 clock
	SIM_SCGC6 |= SIM_SCGC6_ADC0_MASK;
	
	//I2C Accelerometer 
	// turn on I2C0 clock
	SIM_SCGC4 |= SIM_SCGC4_I2C0_MASK;  //Module
	SIM_SCGC5 |= SIM_SCGC5_PORTE_MASK; // Data
	SIM_SCGC5 |= SIM_SCGC5_PORTA_MASK; //Interruption
	
	// set ADC inputs to ADC inputs
	PORTC_PCR2  |= PORT_PCR_MUX(0);        // Camera 1 PTC2 ADC0_SE11
	    
	// set GPIOs to GPIO function
	PORTC_PCR0  |= PORT_PCR_MUX(1);    // PTC0 Push Button S2
	PORTB_PCR18 |= PORT_PCR_MUX(1);    // PTB18 LED red
	PORTB_PCR19 |= PORT_PCR_MUX(1);    // PTB19 LED green
	PORTD_PCR1  |= PORT_PCR_MUX(1);    // PTD1  LED blue
	PORTB_PCR8  |= PORT_PCR_MUX(1);    // PTB8 Camera SI
	PORTB_PCR9  |= PORT_PCR_MUX(1);    // PTB9 Camera Clock
	PORTA_PCR4  = 0;                   // Set PTA4 Pin Control Register to Zero
	PORTA_PCR4  |= PORT_PCR_MUX(1);    // PTA4 Motor 1 In 1 (speed) PTA4 TPM0_CH1
	PORTA_PCR5  |= PORT_PCR_MUX(3);    // PTA5 Motor 1 In 2 (direction)
	PORTC_PCR8  |= PORT_PCR_MUX(1);    // PTC8 Motor 2 In 1 (direction)
	PORTC_PCR9  |= PORT_PCR_MUX(3);    // PTC9 Motor 2 In 2 (speed) PTC8 TPM0_CH5
	PORTE_PCR24 |= PORT_PCR_MUX(5);    // PTE24 Set to I2C Function (SCL)
	PORTE_PCR25 |= PORT_PCR_MUX(5);    // PTE25 Set to I2C Function (SDA)
	PORTA_PCR14 |= (0|PORT_PCR_ISF_MASK|     // Clear the interrupt flag
	                  PORT_PCR_MUX(0x1)|     // PTA14 is configured as GPIO
	                  PORT_PCR_IRQC(0xA));   // PTA14 is configured for falling edge interrupts 
	
	// set GPIO outputs to outputs
	GPIOB_PDDR |= (1<<18);           // PTB18 LED red
	GPIOB_PDDR |= (1<<19);           // PTB19 LED green
	GPIOD_PDDR |= (1<<1);            // PTD1  LED blue
	GPIOB_PDDR |= (1<<8);            // PTB8 Camera SI
	GPIOB_PDDR |= (1<<9);            // PTB9 Camera Clock
	GPIOA_PDDR |= (1<<4);            // PTA4 Motor 1 In 1
	GPIOA_PDDR |= (1<<5);            // PTA5 Motor 1 In 2 (direction)
	GPIOC_PDDR |= (1<<8);            // PTC9 Motor 2 In 1 (direction)
	GPIOC_PDDR |= (1<<9);            // PTC9 Motor 2 In 2
	    
	// all LEDs off
	GPIOB_PDOR |= GPIO_PDOR_PDO(1<<18);   // red LED off
	GPIOB_PDOR |= GPIO_PDOR_PDO(1<<19);   // green LED off
	GPIOD_PDOR |= GPIO_PDOR_PDO(1<<1);    // blue LED off
	    
	// set GPIO input to input
	GPIOC_PDDR &= ~ (1<<0);            // PTC0 Push Button S2
	GPIOA_PDDR &= ~ (1<<14);
	
	// set PWM outputs
	PORTA_PCR12 |= PORT_PCR_MUX(3);    // Servo Motor 1 Out  PTA12 TPM1_CH0
	    
	// configure TPM clock source to be 48 MHz
	SIM_SOPT2 |= SIM_SOPT2_TPMSRC(1);        // MCGFLLCLK clock or MCGPLLCLK/2
	SIM_SOPT2 |= SIM_SOPT2_PLLFLLSEL_MASK;    // use MCGPLLCLK/2 clock when running from 8 MHz and PLL
	    
	// set TPM prescaler before enabling the timer
	TPM0_SC |= 3;                    // prescaler for TPM0 (Motor) is 8
	TPM1_SC |= 3;                    // prescaler for TPM1 (Servo) is 8
	    
	// TPM modulo register, set frequency
	TPM0_MOD = 600;                    // modulo TPM0 (Motor), periode = 0.10 ms (10000 Hz)
	TPM1_MOD = 60000;                // modulo TPM0 (Servo), periode = 10 ms (100 Hz)
	    
	// set TPM clock mode to enable timer
	TPM0_SC |= TPM_SC_CMOD(1);        // enable TPM0 (Motor)
	TPM1_SC |= TPM_SC_CMOD(1);        // enable TPM1 (Servo)
	    
	// configure TPM channel outputs high-true pulses
	TPM0_C2SC = 0x28;                // TPM0 channel1 Motor 1 In 1 speed left
	TPM0_C5SC = 0x28;                // TPM0 channel5 Motor 2 In 2 speed right
	TPM1_C0SC = 0x28;                // TPM1 channel0 Servo 1
	    
	// TPM channel value registers, sets duty cycle
	TPM1_C0V = 8000;                // TPM1 channel0 Servo 1 ca. 1.5 ms (middle)
	    
	// initial configuration of motors
	TPM0_C2V = 50;                    // TPM0 channel1 left Motor 1 In 1 slow forward
	TPM0_C5V = 50;                    // TPM0 channel5 right Motor 2 In 2 slow forward
	GPIOA_PDOR &= ~(1<<4);            // Set PTA4 left Motor 1 In 2 forward
	GPIOC_PDOR &= ~(1<<8);            // Set PTC8 right Motor 2 In 1 forward
	    
	// configure interrupts in TPM0
	TPM1_SC |= TPM_SC_TOIE_MASK;    // enable overflow interrupt in TPM1 (10 ms rate)
	    
	// ADC0 clock configuration
	ADC0_CFG1 |= 0x01;                // clock is bus clock divided by 2 = 12 MHz
	    
	// ADC0 resolution
	ADC0_CFG1 |= 0x08;                // resolution 10 bit, max. value is 1023
	    
	// ADC0 conversion mode
	ADC0_SC3 = 0x00;                // single conversion mode
	
	// I2C0 clock configuration
	I2C0_F = I2C_F_MULT(0) | I2C_F_ICR(0x14) ;  // clock is bus clock divided by 48 = 500 KHz
	
	// I2C0 mode
	I2C0_C1 |= I2C_C1_IICEN_MASK; // I2C0 is enabled

	// enable interrupts 18 (TPM = FTM1)  in NVIC, no interrupt levels
	NVIC_ICPR |= (1 << 18);            // clear pending interrupt 18
	NVIC_ISER |= (1 << 18);            // enable interrupt 18
	NVIC_ICPR |= 1 << ((INT_PORTA - 16)%32);
	NVIC_ISER |= 1 << ((INT_PORTA - 16)%32); 
	// enable interrupts globally
	asm (" CPSIE i ");                // enable interrupts on core level
	
	Init_Acce();
	Calibration();
	
}
