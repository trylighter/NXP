/*
 * Moteur.c
 *
 *  Created on: Feb 10, 2018
 *      Author: Avina
 */

#include "derivative.h" 
#include "Moteur.h"
#include "Acce.h"
#include "I2C.h"
#include "Servo.h"
#include <math.h>



#define SEUIL_ACCELZ 100
#define ACCELERATION sqrt(pow(Xout_g,2) + pow(Yout_g,2))

extern float Xout_g,Yout_g,Zout_g; // Données accéléromètre
int nouv_d,nouv_g;
//int abs (int);
int montee = 0;
float depassement = 0;


int Saturateur(int val, double seuil_bas, double seuil_haut){
	if ( val > seuil_haut)
		return seuil_haut;
	else if ( val < seuil_bas)
		return seuil_bas;
	else 
		return val;
}

void Cal_vitesse(int diff, int diff_old ,int arret, int ralentissement){
	int nouv;
	
	double tau_descente = 0;
	double tau_montee = 0.1;
	double tau_moyen = 0.03;
	float KP_DC = 0.5;
	float K_ACCEL = 100000;
	int val_PWM = (TPM0_C2V + TPM0_C5V)/2;
	int seuil_haut = 205, seuil_moyen = 125, seuil_bas = 85;
	if (arret != 1)
	{
		/*if( abs(Zout_g) > SEUIL_ACCELZ)
		{
			d;
		}*/
		depassement = ( abs((int)(10000 * Zout_g)) > SEUIL_ACCELZ)?1:0;
		depassement = 0;
		if(montee > 0)
		{
			if((montee > 20) && (depassement == 0))
			{
				montee = 0;
			}
			else
			{
				montee++;
			}
		}
		else
		{
			montee = (depassement==1)?1:0;
		}
		if ((abs(diff)> 10) || (montee != 0) ) //|| (ralentissement == 1))
			nouv =  seuil_bas -  17*abs(diff-diff_old) - 1.5*diff - K_ACCEL * ACCELERATION;
		else if((abs(diff)> 6))
			nouv = seuil_moyen - 4* abs(diff-diff_old) + (seuil_moyen - val_PWM)/(100*tau_moyen);
		else
			nouv = val_PWM +(seuil_haut - val_PWM)/(100*tau_montee);
		
		if ( nouv > seuil_haut)
			nouv = seuil_haut;
		if ( nouv < seuil_bas)
			nouv = seuil_bas;
		TPM0_C2V = nouv + 1.15*diff ;
		TPM0_C5V = nouv - 1.15*diff;
	}
	else
	{
		TPM0_C2V =  0 ;
		TPM0_C5V =  0;
		//Servo_Position(0);
		//for(;;);
	}
		
	
}
