/*
 * Moteur.h
 *
 *  Created on: Feb 10, 2018
 *      Author: Avina
 */

#ifndef MOTEUR_H_
#define MOTEUR_H_



int Saturateur(int val, double seuil_bas, double seuil_haut);
void Cal_vitesse(int diff, int diff_old, int arret, int ralentissement);

#endif /* MOTEUR_H_ */
