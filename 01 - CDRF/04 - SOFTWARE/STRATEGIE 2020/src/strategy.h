#ifndef STRATEGY_H
#define STRATEGY_H


#include <Arduino.h>
#include <Wire.h>

#include "IHM.h"
#include "planner.h"
#include "communication.h"
  

/***************************************
 *             Variables               *
 ***************************************/

// Adressage I2C pour les cartes esclaves
#define CARTE_DEPLACEMENT 60
//Etat de la position demandée
#define TERMINEE 0  // Position validée et terminée
#define RECU 1      // Position reçu
#define ERRONEE 2   // Position erronée. CRC nok.
#define BIZARRE 3   // Reponse étrange à ne pas prendre en compte

// Adversaire
const bool ADVERSAIRE_NON = 1,
           ADVERSAIRE_OUI = 0;

// Couleur Equipe
const bool EQUIPE_JAUNE   = 1,
           EQUIPE_VIOLET  = 0;

// Strategie
const bool STRATEGIE_HOMOLOGATION = 1,
           STRATEGIE_MATCH        = 0;

// type de Robot
const bool ROBOT_PRIMAIRE   = 1,
           ROBOT_SECONDAIRE = 0;

// Autres
const int TEMPS_MATCH = 101000;

// Declaration des pins E/S
const int pinPompeGauche            = 14,
          pinEVGauche               = 15,
          pinPompeDroit             = 16,
          pinEVDroit                = 17,
          pinServoDroit             = 20,
          pinServoVentouseDroit     = 21,
          pinServoGauche            = 22,
          pinServoVentouseGauche    = 23,
          pinServoDrapeau           = 8;
        //pinServoBrasDroit         = 9;      // Pin Servo bras droit
        //pinServoBrasGauche        = 10;     // Pin Servo bras gauche

int pinBalise ;

Bras brasDroit ;
Bras brasGauche ;

Servo servoDrapeau ;

int nbrBadCRC = 0   ; // Nombre de CRC érronés
int nbrBizarre = 0  ; // Nombre de réponses bizarres

FastCRC8 CRC8;
byte bufNavRelatif[5]={0,0,0,0,0}; // Buffer d'envoi des ordres de navigation relatifs
byte crcNavRelatif = 0; // CRC de controle pour les ordres de navigation relatifs

byte bufNavAbsolu[6]={0,0,0,0,0,0}; // Buffer d'envoi des ordres de navigation absolus
byte crcNavAbsolu = 0 ; // CRC de controle pour les ordres de navigation absolus

byte optionNavigation = 0;

int score = 0;

double tempsRestant = TEMPS_MATCH;
double timeInit=0;

// Declaration des variables IHM
IHM interface ;
bool detection = false , strategie = false , typeRobot = ROBOT_PRIMAIRE , equipe = EQUIPE_VIOLET ;



/***************************************
 *             Strategy                *
 ***************************************/
void homologationPrimaire   ();
void homologationSecondaire ();
void sequenceRecalage       ();
void matchPrimaire          ();
void matchSecondaire        ();
void testLancerGobelet      ();

/***************************************
 *            Main Methods             *
 ***************************************/
void initActionneur         ();  //Initialisation des actionneurs
void prepareMatch           ();  //Initialise la stratégie séléctionnée
void waitLaunch             ();  //Attendre le début du match

void majTemp                ();
void majScore               ();
void attente                (int temps);


int askNavigation           ();//DEMANDE L'ETAT DU DEPLACEMENT

//ENVOI UNE COMMANDE DE DEPLACEMENT ABSOLU
void sendNavigation         ( byte fonction, 
                              int X, 
                              int Y,
                              int rot        );

;//ENVOI UNE COMMANDE DE DEPLACEMENT RELATIF
void sendNavigation         ( byte fonction,
                              int rot, 
                              int dist        );

//ENVOI UNE COMMANDE TURN GO
void turnGo(bool adversaire, bool recalage,bool ralentit,int turn, int go);

#endif 