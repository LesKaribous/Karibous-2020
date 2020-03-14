#ifndef STRATEGY_H
#define STRATEGY_H


#include <Arduino.h>
#include "IHM.h"
#include "communication.h"
  
/***************************************
 *             Strategy                *
 ***************************************/
void homologationPrimaire   ();
void homologationSecondaire ();
void sequenceRecalage       ();
void matchPrimaire          ();
void matchSecondaire        ();

/***************************************
 *            Main Methods             *
 ***************************************/
  void initActionneur       ();  //Initialisation des actionneurs
  void prepareMatch         ();  //Initialise la stratégie séléctionnée
  void waitLaunch           ();  //Attendre le début du match



  #endif