#include <Arduino.h>
#include <Wire.h>
#include <U8g2lib.h>
#include <SPI.h>
#include <FastCRC.h>
#include <Servo.h>

// Points pour chaque action

#define ADVERSAIRE_NON 1
#define ADVERSAIRE_OUI 0
// Adressage I2C pour les cartes esclaves
#define CARTE_DEPLACEMENT 60
// Couleur Equipe
#define EQUIPE_JAUNE 1
#define EQUIPE_VIOLET 0
// Strategie
#define STRATEGIE_HOMOLOGATION 1
#define STRATEGIE_MATCH 0
// type de Robot
#define ROBOT_PRIMAIRE 1
#define ROBOT_SECONDAIRE 0
// Autres
#define TEMPS_MATCH 101000
//Etat de la position demandée
#define TERMINEE 0  // Position validée et terminée
#define RECU 1      // Position reçu
#define ERRONEE 2   // Position erronée. CRC nok.
#define BIZARRE 3   // Reponse étrange à ne pas prendre en compte


// Logo Karibous
#define LOGO_KARIBOUS_width 128
#define LOGO_KARIBOUS_height 33
static unsigned char LOGO_KARIBOUS_bits[] = {
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1c, 0xfc, 0xf9, 0x03,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x1c, 0xfc, 0xfb, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x1c, 0xfc, 0xfb, 0x03, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1c, 0x04, 0x0b, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x1c, 0xfc, 0xf9, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x1c, 0x0c, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1c, 0xfc, 0x39, 0x03,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0xfc, 0xf9, 0xf9, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0xfc, 0x01, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xe0, 0x0f, 0xc0, 0x01,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfe, 0xe0, 0xc3, 0x1f,
   0xe0, 0xff, 0xc7, 0xf3, 0xff, 0x81, 0xff, 0x87, 0x0f, 0x38, 0xfc, 0x3f,
   0xfc, 0xf0, 0xc1, 0xff, 0xe1, 0xff, 0xcf, 0xf3, 0xff, 0xc3, 0xff, 0x8f,
   0x0f, 0x38, 0xfe, 0x3f, 0xfc, 0xf8, 0xc0, 0xff, 0xe3, 0xff, 0xdf, 0xf3,
   0xff, 0xe7, 0xff, 0x9f, 0x0f, 0x38, 0xfe, 0x3f, 0xfc, 0xf8, 0xc0, 0xff,
   0xe7, 0xff, 0xdf, 0xf3, 0xff, 0xe7, 0xff, 0x9f, 0x0f, 0x38, 0xff, 0x1f,
   0xfc, 0x7c, 0xc0, 0xff, 0xe7, 0xff, 0xdf, 0xf3, 0xff, 0xe7, 0xff, 0x9f,
   0x0f, 0x38, 0xff, 0x1f, 0xfc, 0x3e, 0xc0, 0xff, 0xe7, 0xff, 0xdf, 0xf3,
   0xff, 0xe7, 0xff, 0x9f, 0x0f, 0x38, 0xff, 0x1f, 0xfc, 0x1f, 0x00, 0xf0,
   0x67, 0x00, 0xde, 0xf3, 0x00, 0xe7, 0x01, 0x9e, 0x0f, 0x38, 0x0f, 0x00,
   0xfc, 0x0f, 0x00, 0x00, 0x67, 0x00, 0xde, 0xf3, 0x00, 0xe3, 0x01, 0x9e,
   0x0f, 0x38, 0x0f, 0x00, 0xfc, 0x0f, 0xe0, 0xff, 0xe7, 0xff, 0xdf, 0xf3,
   0xff, 0xe1, 0x01, 0x9e, 0x0f, 0x38, 0xfe, 0x1f, 0xfc, 0x1f, 0xf0, 0xff,
   0xe7, 0xff, 0xcf, 0xf3, 0xff, 0xe3, 0x01, 0x9e, 0x0f, 0x3c, 0xfc, 0x3f,
   0xfc, 0x1f, 0x70, 0xc0, 0xe7, 0xff, 0xc7, 0x71, 0x80, 0xe7, 0x01, 0x8e,
   0x0f, 0x3c, 0x00, 0x3f, 0xfc, 0x3f, 0x70, 0x00, 0xe7, 0xf9, 0xc1, 0x71,
   0x80, 0xe7, 0x01, 0x8e, 0x0f, 0x1c, 0x00, 0x38, 0x7c, 0x7e, 0xf0, 0x01,
   0xe7, 0xf1, 0xc0, 0xf1, 0xff, 0xe7, 0x01, 0x8e, 0xff, 0x1f, 0x1e, 0x38,
   0x3c, 0x7c, 0xf0, 0xff, 0xe7, 0xf1, 0xc0, 0xf1, 0xff, 0xe7, 0xff, 0x0f,
   0xff, 0x1f, 0xfe, 0x3f, 0x3c, 0xfc, 0xf0, 0xff, 0xe7, 0xe1, 0xc1, 0xf1,
   0xff, 0xc3, 0xff, 0x0f, 0xff, 0x1f, 0xfe, 0x3f, 0x3c, 0xf8, 0xe1, 0xff,
   0xe7, 0xe1, 0xc3, 0xf1, 0xff, 0x81, 0xff, 0x07, 0xfe, 0x0f, 0xfe, 0x1f,
   0x38, 0xf0, 0x03, 0x7f, 0xe7, 0xc1, 0xc7, 0xf1, 0xff, 0x00, 0xf8, 0x03,
   0xf8, 0x07, 0xf8, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };


// Declaration des pins IHM
const int pinTirette = 39 ;         // Pin tirette
const int pinDetection = 37 ;       // Pin activation de la detection
const int pinStrategie = 38 ;       // Pin choix de strategie
const int pinEquipe = A21;          // Pin choix d'équipe
const int pinCheck = A22;           // Pin bouton checklist
const int pinRobot = 16;            // Pin jumper type de robot

// Declaration des pins E/S
const int pinServoGauche = 14 ;     // Pin Servo en bas à gauche
const int pinPompeGauche = 22 ;     // Pin de la pompe gauche
const int pinEVGauche = 23 ;        // Pin de l'EV gauche

const int pinServoDroit = 15 ;      // Pin Servo en bas à droit
const int pinPompeDroit = 21 ;      // Pin de la pompe droite
const int pinEVDroit = 20 ;         // Pin de l'EV Droite

const int pinServoBrasDroit = 14 ;      // Pin Servo bras droit
const int pinServoBrasGauche = 15 ;     // Pin Servo bras gauche

const int pinServoAvant = 17 ;      // Pin servo avant robot secondaire
const int pinPompeAvant = 22 ;      // Pin de la pompe avant sur robot secondaire
const int pinEVAvant = 23 ;         // Pin de l'EV avant sur robot secondaire

const int pinBalisePrimaire = 17 ;
const int pinBaliseSecondaire = 21 ;

int pinBalise ;

// Declaration des servomoteurs
Servo servoGauche;
Servo servoDroit;

Servo servoBrasDroit;
Servo servoBrasGauche;

Servo servoAvant;

// Declaration des positions servo
// Bras ventouse
const int sgHaut    = 95 ;
const int sgMilieu  = 57 ;
const int sgBas     = 10 ;

const int sdHaut    = 50 ;
const int sdMilieu  = 90 ;
const int sdBas     = 130 ;

const int avHaut    = 160 ;
const int avQuart    = 140 ;
const int avBas     = 80 ;

// Bras lateraux
const int sgHaut_bras = 50;
const int sgBas_bras  = 155;

const int sdHaut_bras = 130;
const int sdBas_bras  = 30;

// Declaration des variables IHM
bool tirette = false , detection = false , strategie = false , check = false , changeStrat = true , typeRobot = ROBOT_PRIMAIRE;

// Liste d'action de la check list:
char* titreList[6] = {
                  "Robot",
                  "Experi.",
                  "Palets",
                  "Table",
                  "Balise",
                  "GO !"};
// Liste d'action de la check list:
char* list[6] = {
                  "Installer le robot secondaire ",
                  "Installer l'experience        ",
                  "Verifier les palets           ",
                  "Nettoyer la table             ",
                  "Poser la balise adversaire    ",
                  "Bon match !                   "};

// Declaration de l'ecran
U8G2_ST7920_128X64_F_SW_SPI u8g2(U8G2_R0,13,11,12,U8X8_PIN_NONE);

int nbrBadCRC = 0   ; // Nombre de CRC érronés
int nbrBizarre = 0  ; // Nombre de réponses bizarres

FastCRC8 CRC8;
byte bufNavRelatif[5]={0,0,0,0,0}; // Buffer d'envoi des ordres de navigation relatifs
byte crcNavRelatif = 0; // CRC de controle pour les ordres de navigation relatifs

byte bufNavAbsolu[6]={0,0,0,0,0,0}; // Buffer d'envoi des ordres de navigation absolus
byte crcNavAbsolu = 0 ; // CRC de controle pour les ordres de navigation absolus

bool equipe = EQUIPE_VIOLET ;
byte optionNavigation = 0;

int score = 0;

double tempsRestant = TEMPS_MATCH;
double timeInit=0;

void setup();
void loop();

//GESTION DES BOUTTONS DE L'IHM----------------
void bouttonIHM();

//INITIALISATION ROBOT----------------
void initRobot();
void initActionneur();
void sequenceRecalage();

//STRATEGIE DEPLACEMENT----------------
void testLigneDroite();
void homologationPrimaire();
void homologationSecondaire();
void matchPrimaire();
void matchSecondaire();
void testRotation();
void demoRotation();
void testActionneur();

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

//GESTION DES PAGES LCD-------------------
void u8g2_prepare();

void u8g2_splash_screen();

void u8g2_menu_pendant_match();

void u8g2_menu_avant_match();

void u8g2_splash_screen_GO();

void majScore(int points, int multiplicateur);