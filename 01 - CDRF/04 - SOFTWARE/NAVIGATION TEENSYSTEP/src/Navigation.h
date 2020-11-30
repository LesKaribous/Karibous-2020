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
typedef enum {
  NAVIGATION_AVAILABLE,
  CONVERT_REL_TARGET,
  CONVERT_ABS_TARGET,
  SET_ROTATION,
  WAIT_ROTATION,
  SET_DISTANCE,
  WAIT_DISTANCE,
  STOP_OPPONENT,
  WAIT_OPPONENT,
  SET_ALPHA,
  WAIT_ALPHA,
  END_OF_MATCH = 255,
  WAIT_END_OF_MATCH
} Navigation ;

// Etats de la position demandée
typedef enum {
  WAITING_TARGET,
  NEW_REL_TARGET,
  NEW_ABS_TARGET,
  ERROR_BAD_CRC,
} Communication ;

Communication stateCom = WAITING_TARGET;
Navigation stateNav = NAVIGATION_AVAILABLE;

// -------------------- PARAMETRES RELATIFS AUX MOTEURS ------------------------

//Variables Pin Moteur
const int pinStepGauche = 5;
const int pinDirGauche = 6;
const int pinStepDroit = 2;
const int pinDirDroit = 3;

const int pinSleep = 4;

const int pinM0 = 7;
const int pinM1 = 0;
const int pinM2 = 1;

// Declaration des Stepper et du controller
Stepper mGauche(pinStepGauche,pinDirGauche);   //STEP pin =  2, DIR pin = 3
Stepper mDroit(pinStepDroit, pinDirDroit);   //STEP pin =  5, DIR pin = 6
StepControl robot;

// Variable par defaut pour le réglage des déplacements
float FacteurX      = 1.0;
float FacteurDroit  = 1.0;
float FacteurGauche = 1.0;
float FacteurRot    = 1.0;

uint32_t VitesseMaxDroite = 20000;
uint32_t VitesseMaxGauche = 20000;
uint32_t AccelRot         = 20000;
uint32_t AccelMaxDroite   = 20000;
uint32_t AccelMaxGauche   = 20000;
uint32_t AccelStop        = 20000;

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

//------------------------- GESTION DE CAPTEURS BORDURE ------------------------

// AV_DROIT , AV_GAUCHE , AR_DROIT , AR_GAUCHE
const int pinBorderSensor[4] = {13,14,15,16};
bool borderState[4] = {0,0,0,0};

//---------------------- GESTION DES CAPTEURS D'ADVERSAIRE  --------------------

const int pinAdversaireArriere = 11;
const int pinAdversaireAvant = 12;

//------------------------ COMMUNICATION de NAVIGATION -------------------------

FastCRC8 CRC8;
byte bufNavRelatif[6]={0,0,0,0,0,0};    // Buffer de reception relatifs + le CRC
byte crcNavRelatif = 0;                 // CRC relatif
byte bufNavAbsolu[8]={0,0,0,0,0,0,0,0}; // Buffer de reception absolu + le CRC
byte crcNavAbsolu = 0;                  // CRC absolu

int16_t relativeRequest[2] ;  // rotation, distance
int16_t absoluteRequest[3] ;  // X, Y, Alpha

long currentPos[3]={0.0,0.0,0.0};        // X, Y, Alpha

double targetDis    = 0.0 ;
double targetRot    = 0.0 ;
double targetAlpha  = 0.0;

bool optionAdversaire = false;
bool optionRecalage   = false;
bool optionRalentit   = false;

bool presenceAvant    = false;
bool presenceArriere  = false;

unsigned long avantTimeInit   = 0;
unsigned long arriereTimeInit = 0;
unsigned long sensorTime      = 1000;

long startPositionLeft  = 0;
long startPositionRight = 0;

// --------------------------------------------------

void changeTypeRobot(bool type);
void getBorderState();
void getOpponentState();

// ------- MACHINE D'ETAT -------
void strategieNavigation();

void navigationAvailable();
void convertRelTarget();
void convertAbsTarget();
void setRotation();
void waitRotation();
void setDistance();
void waitDistance();
void stopOpponent();
void waitOpponent();
void setAlpha();
void waitAlpha();
void endOfMatch();
void waitEndOfMatch();

//----- Communications -----
void receiveEvent(int howMany);
void requestEvent();
