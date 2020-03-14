#include <Arduino.h>
#include <Wire.h>
#include <FastCRC.h>
#include <Servo.h>
#include "Bras.h"
#include "IHM.h"


// Adressage I2C pour les cartes esclaves
#define CARTE_DEPLACEMENT 60
//Etat de la position demandée
#define TERMINEE 0  // Position validée et terminée
#define RECU 1      // Position reçu
#define ERRONEE 2   // Position erronée. CRC nok.
#define BIZARRE 3   // Reponse étrange à ne pas prendre en compte

// Adversaire
const bool ADVERSAIRE_NON = 1 ;
const bool ADVERSAIRE_OUI = 0 ;
// Couleur Equipe
const bool EQUIPE_JAUNE = 1 ;
const bool EQUIPE_VIOLET = 0 ;
// Strategie
const bool STRATEGIE_HOMOLOGATION = 1 ;
const bool STRATEGIE_MATCH = 0 ;
// type de Robot
const bool ROBOT_PRIMAIRE = 1 ;
const bool ROBOT_SECONDAIRE = 0 ;
// Autres
const int TEMPS_MATCH = 101000 ;

// Declaration des pins E/S
const int pinPompeGauche = 14 ;     // Pin de la pompe gauche
const int pinEVGauche = 15 ;        // Pin de l'EV gauche
const int pinPompeDroit = 16 ;      // Pin de la pompe droite
const int pinEVDroit = 17 ;         // Pin de l'EV Droite

const int pinServoDroit = 20 ;       // Pin Servo en bas à gauche
const int pinServoVentouseDroit = 21;// Pin Servo ventouse gauche
const int pinServoGauche = 22 ;        // Pin Servo en bas à droite
const int pinServoVentouseGauche = 23 ;// Pin Servo ventouse droite

const int pinServoDrapeau = 8;
//const int pinServoBrasDroit = 9 ;      // Pin Servo bras droit
//const int pinServoBrasGauche = 10 ;     // Pin Servo bras gauche

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

//----- FONCTIONS OBLIGATOIRES -----
void setup();
void loop();

//----- INITIALISATION ROBOT -----
void initActionneur();
void sequenceRecalage();
void waitLaunch();

//----- STRATEGIE DEPLACEMENT -----
void homologationPrimaire();
void homologationSecondaire();
void matchPrimaire();
void matchSecondaire();
void testLancerGobelet();

//DEMANDE L'ETAT DU DEPLACEMENT----------------
int askNavigation();

//ENVOI UNE COMMANDE DE DEPLACEMENT ABSOLU----------------
void sendNavigation(byte fonction, int X, int Y, int rot);

//ENVOI UNE COMMANDE DE DEPLACEMENT RELATIF----------------
void sendNavigation(byte fonction, int rot, int dist);

//MISE A JOUR DU TEMPS DE MATCH----------------
void majTemps();

//PROCEDURE D'ATTENTE----------------
void attente(int temps);

//PROCEDURE DE FIN DE MATCH----------------
void finMatch();

//ENVOI UNE COMMANDE TURN GO----------------
void turnGo(bool adversaire, bool recalage,bool ralentit,int turn, int go);
