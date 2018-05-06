/*
 * Camera.c
 *
 *  Created on: Feb 10, 2018
 *      Author: Avina
 */

#include "derivative.h" 
#include "Camera.h"

// Defines for LineScan Camera
#define    TAOS_DELAY              asm ("nop")              // minimal delay time
#define    TAOS_SI_HIGH            GPIOB_PDOR |= (1<<8)     // SI on PTB8
#define    TAOS_SI_LOW             GPIOB_PDOR &= ~(1<<8)    // SI on PTB8
#define    TAOS_CLK_HIGH           GPIOB_PDOR |= (1<<9)     // CLK on PTB9
#define    TAOS_CLK_LOW            GPIOB_PDOR &= ~(1<<9)    // CLK on PTB9

int ImageData [128];                        // array to store the LineScan image

void ImageCapture(void)
{
    unsigned char i;
    ADC0_CFG2 |= 0x10;                            // select B side of the MUX
    TAOS_SI_HIGH;
    TAOS_DELAY;
    TAOS_CLK_HIGH;
    TAOS_DELAY;
    TAOS_SI_LOW;
    TAOS_DELAY;
    // inputs data from camera (first pixel)
    ADC0_SC1A  =  11;                            // set ADC0 channel 11
    while((ADC0_SC1A & ADC_SC1_COCO_MASK) == 0);// wait until ADC is ready
    ImageData[0] = ADC0_RA;                        // return value
    TAOS_CLK_LOW;
    
    for(i=1;i<128;i++)
    {
        TAOS_DELAY;
        TAOS_DELAY;
        TAOS_CLK_HIGH;
        TAOS_DELAY;
        TAOS_DELAY;
        // inputs data from camera (one pixel each time through loop)
        ADC0_SC1A  =  11;                            // set ADC0 channel 11
        while((ADC0_SC1A & ADC_SC1_COCO_MASK) == 0);// wait until ADC is ready
        ImageData[i] = ADC0_RA;                        // return value
        TAOS_CLK_LOW;
    }
    
    TAOS_DELAY;
    TAOS_DELAY;
    TAOS_CLK_HIGH;
    TAOS_DELAY;
    TAOS_DELAY;
    TAOS_CLK_LOW;
}
