#include <Arduino.h>
#include <TeensyStep.h>
#include <Wire.h>
#include <math.h>
#include <FastCRC.h>

//Adresse I2C du module de navigation
#define ADRESSE 60

// Types de ROBOT
#define ROBOT_PRIMAIRE 1
#define ROBOT_SECONDAIRE 0

// -------------------- ETATS DE LA STRATEGIE DE NAVIGATION --------------------

// Etats de la navigation
#define NAVIGATION_AVAILABLE 0  // Navigation Disponible
#define SET_ROTATION 1          // Lance la rotation du robot
#define WAIT_ROTATION 2         // Attend la fin de la rotation
#define SET_DISTANCE 3          // Lance le déplacement en distance
#define WAIT_DISTANCE 4         // Attend la fin du déplacement en distance
#define STOP_OPPONENT 5         // Arrêt du robot en cas d'adversaire
#define WAIT_OPPONENT 6         // Attente du passage de l'adversaire

#define END_OF_MATCH 255        // Etat particulier de fin de match

// Etats de la position demandée
#define WAITING_TARGET 0   // Disponible pour nouvelle position
#define NEW_REL_TARGET 1   // Nouvelle position relative enregistrée
#define NEW_ABS_TARGET 2   // Nouvelle position relative enregistrée
#define ERROR_BAD_CRC 3    // Nouvelle position erronée. CRC nok.

// -------------------- PARAMETRES RELATIFS AUX MOTEURS ------------------------

//Variables Pin Moteur
const int pinStep1 = 2;
const int pinDir1 = 3;
const int pinStep2 = 5;
const int pinDir2 = 6;

const int pinSleep = 4;

const int pinM0 = 7;
const int pinM1 = 0;
const int pinM2 = 1;

// Declaration des Stepper et du controller
Stepper mGauche(pinStep1,pinDir1);   //STEP pin =  2, DIR pin = 3
Stepper mDroit(pinStep2, pinDir2);   //STEP pin =  5, DIR pin = 6
StepControl robot;

// Variable par defaut pour le réglage des déplacements
float FacteurX      = 1.0;
float FacteurDroit  = 1.0;
float FacteurGauche = 1.0;
float FacteurRot    = 1.0;

uint32_t VitesseMaxDroite = 10000;
uint32_t VitesseMaxGauche = 10000;
uint32_t AccelRot         = 5000;
uint32_t AccelMaxDroite   = 10000;
uint32_t AccelMaxGauche   = 10000;
uint32_t AccelStop        = 10000;

// Paramètres liés aux robots
// Primaire :
const float primaireFacteurX= 80.0;
const float primaireFacteurDroit = 80.0;
const float primaireFacteurGauche = 80.0;
const float primaireFacteurRot = 30.0;
// Secondaire :
const float secondaireFacteurX= 8.0;
const float secondaireFacteurDroit = 8.0;
const float secondaireFacteurGauche = 8.0;
const float secondaireFacteurRot = 14.0;

//------------------------- GESTION DU TYPE DE ROBOT ---------------------------

const int pinRobot = 17;

//------------------------ COMMUNICATION de NAVIGATION -------------------------

FastCRC8 CRC8;
byte bufNavRelatif[6]={0,0,0,0,0,0};    // Buffer de reception relatifs + le CRC
byte crcNavRelatif = 0;                 // CRC relatif
byte bufNavAbsolu[8]={0,0,0,0,0,0,0,0}; // Buffer de reception absolu + le CRC
byte crcNavAbsolu = 0;                  // CRC absolu

int16_t relativeRequest[2] ;  // rotation, distance
int16_t absoluteRequest[3] ;  // rotation, X, Y
int16_t currentPos[3];        //rotation, X, Y

int targetDis = 0 ;
int targetRot = 0 ;

byte stateCom = WAITING_TARGET;
byte stateNav = NAVIGATION_AVAILABLE;

bool optionAdversaire = false;
bool optionRecalage   = false;
bool optionRalentit   = false;

// --------------------------------------------------


void changeTypeRobot(bool type);
void strategieNavigation();
//----- Fin de match -----
void finMatch();
//----- Communications -----
void receiveEvent(int howMany);
void requestEvent();
