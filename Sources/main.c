/* FRDM-KL25 Freedom Board NXP Cup Minimal Program
* (c) THD Technische Hochschule Deggendorf
* (c) DIT Deggendorf Institute of Technology
* Version 2.1, 09.01.2018
* Change: ported to NXP Cup and Alamank Car
*/

#include "derivative.h" 
#include "Declaration.h"

// Function declaration
int abs (int);

// Main program
int main(void){
    
	initialisation();
	
    // Main loop
    for(;;) {                        // endless loop
       /* if ( (BlackLineLeft > 38)&&(BlackLineRight < 90))
        {       
            for(;;)
            {
                TPM0_C2V = 1;                    // TPM0 channel1 left Motor 1 In 1 slow forward
                TPM0_C5V = 1;                    // TPM0 channel5 right Motor 2 In 2 slow forward            
                GPIOB_PDOR &= ~GPIO_PDOR_PDO(1<<18);  // red LED on  
            }
        }
        else
        {
            TPM0_C2V = 100;                    // TPM0 channel1 left Motor 1 In 1 slow forward
            TPM0_C5V = 100;                    // TPM0 channel5 right Motor 2 In 2 slow forward
            GPIOB_PDOR |= GPIO_PDOR_PDO(1<<18);  // red LED off
        }*/
    }
    return 0;
}


