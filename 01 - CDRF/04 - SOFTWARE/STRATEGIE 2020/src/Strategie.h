#include <Arduino.h>
//#include <Servo.h>
#include "Bras.h"
#include "IHM.h"
#include "ComNavigation.h"


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
const int pinPompeGauche=14,pinEVGauche=15,pinPompeDroit=16,pinEVDroit=17 ;
const int pinServoDroit=20,pinServoVentouseDroit=21,pinServoGauche=22,pinServoVentouseGauche=23 ;
const int pinAscenseurGauche = 31, PinPotardGauche = A13 ;

const int pinServoDrapeau = 8;
const int pinServoBrasDroit = 29 ;      // Pin Servo bras droit
const int pinServoBrasGauche = 30 ;     // Pin Servo bras gauche

int pinBalise ;

Bras brasDroit ;
Bras brasGauche ;

Servo servoDrapeau ;
Servo servoBrasDroit ;
Servo servoBrasGauche ;


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
//----- FIN DE MATCH -----
void finMatch();
//----- AUTRES -----
void majTemps();
void majScore();
void attente(int temps);


//DEMANDE L'ETAT DU DEPLACEMENT----------------
int askNavigation();

//ENVOI UNE COMMANDE DE DEPLACEMENT ABSOLU----------------
void sendNavigation(byte fonction, int X, int Y, int rot);

//ENVOI UNE COMMANDE DE DEPLACEMENT RELATIF----------------
void sendNavigation(byte fonction, int rot, int dist);

//ENVOI UNE COMMANDE TURN GO----------------
void turnGo(bool adversaire, bool recalage,bool ralentit,int turn, int go);
