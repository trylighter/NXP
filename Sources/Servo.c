/*
 * Servo.c
 *
 *  Created on: Feb 11, 2018
 *      Author: Avina
 */

#include "derivative.h" 
#include "Servo.h"

void Servo_Position(int servo_position){
	int Commande, Diff, Seuil;
	Commande = 8000 - servo_position;
	
	Diff = Commande - TPM1_C0V;
	Seuil = 170;
	
	if(Diff>Seuil)
        TPM1_C0V = TPM1_C0V + Seuil;
	else if(Diff<-Seuil)
		TPM1_C0V = TPM1_C0V - Seuil;
	else
    	TPM1_C0V = Commande;
	
}
