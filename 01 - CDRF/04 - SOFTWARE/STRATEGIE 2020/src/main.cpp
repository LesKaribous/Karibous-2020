#include "Strategie.h"


void setup(){
  Serial.begin(9600);       //Demarrage d'un liaison série pour le debug
  Wire.begin();             //Demarrage de la liaison I2C
  delay(500);               //Pause de demarrage avant lancement
  interface.splashScreen(); //Affichage du Logo des Karibous
  initActionneur();         //Initialisation des actionneurs
  delay(1000);              //Attente affichage logo

  waitLaunch();             //Attendre le début du match
}


void loop(){
  if (typeRobot == ROBOT_PRIMAIRE)
  {
    if (strategie == STRATEGIE_HOMOLOGATION) homologationPrimaire();
    else matchPrimaire(); // Match
  }
  else
  {
    if (strategie == STRATEGIE_HOMOLOGATION) homologationSecondaire();
    else matchSecondaire(); // Match
  }
}


void waitLaunch(){
  // Gestion tirette
  while (interface.getTirette())
  {
    interface.updateButtonIHM();
    if(!interface.getCheck())
    {
      interface.initScreen();
      sequenceRecalage();
      interface.checkListScreen();
    }
    interface.menuScreen();
  }
  while (!interface.getTirette())
  {
    interface.updateButtonIHM();
    if(!interface.getCheck())
    {
      interface.initScreen();
      sequenceRecalage();
      interface.checkListScreen();
    }
    interface.menuScreen();
  }

  // Lancement du Match
	timeInit = millis();
  interface.goScreen();
  // Enregistrement des paramètres de match
  typeRobot = interface.getTypeRobot();
  strategie = interface.getStrategie();
  detection = interface.getDetection();
  equipe = interface.getEquipe();
  // Delai pour affichage
	delay (100);
}

//----------------INIT ACTIONNEUR-------------
void initActionneur()
{
  // Check du type de robot :
    typeRobot = interface.getTypeRobot();
  // Initialisation des actionneurs en fonction du type de robot
    brasDroit.setPin(pinServoDroit,pinServoVentouseDroit,pinPompeDroit,pinEVDroit);
    brasGauche.setPin(pinServoGauche,pinServoVentouseGauche,pinPompeGauche,pinEVGauche,pinAscenseurGauche,PinPotardGauche);

    if (typeRobot == ROBOT_PRIMAIRE)
    {
      brasDroit.setLimit(45,140,10,90);
      brasGauche.setLimit(140,45,160,70,86,400,600);
    }
    else
    {
      brasDroit.setLimit(45,150,10,90);
      brasGauche.setLimit(140,45,160,70);
    }

    //--------------------------------------------------------
      brasGauche.setAscenseur(0,1000);
      brasGauche.setAscenseur(100,1000);
      brasGauche.setAscenseur(50,1000);
    //--------------------------------------------------------

    brasDroit.setPosition(0,0,LOW,LOW,0);
    brasGauche.setPosition(0,0,LOW,LOW,2000);

    brasDroit.setPosition(100,100,HIGH,LOW,0);
    brasGauche.setPosition(100,100,HIGH,LOW,2000);

    brasDroit.setPosition(0,0,LOW,LOW,0);
    brasGauche.setPosition(0,0,LOW,LOW,2000);

    // Specifique Robot primaire / secondaire
    if (typeRobot == ROBOT_PRIMAIRE)
    {
      // Si strategie pour le robot primaire selectionné :
      servoBrasDroit.detach();
      servoBrasGauche.detach();

      servoBrasDroit.attach(pinServoBrasDroit);
      servoBrasGauche.attach(pinServoBrasGauche);

      servoBrasDroit.write(75);
      servoBrasGauche.write(90);
      delay(2000);
      servoBrasDroit.write(170);
      servoBrasGauche.write(5);
      delay(2000);
    }
    else
    {
      servoDrapeau.detach();
      servoDrapeau.attach(pinServoDrapeau);
      servoDrapeau.write(30);
      delay(2000);
      servoDrapeau.write(10);
      delay(1000);
      servoDrapeau.detach();
    }
}

//----------------STRATEGIES----------------

void homologationPrimaire()
{
  finMatch();
}

void homologationSecondaire()
{
  turnGo(ADVERSAIRE_NON,false,true,0,180);
  turnGo(ADVERSAIRE_NON,false,true,0,-50);

  brasGauche.setPosition(50,0,LOW,LOW,0);
  brasDroit.setPosition(50,0,LOW,LOW,300);

  brasGauche.setPosition(100,100,HIGH,LOW,0);
  brasDroit.setPosition(100,100,HIGH,LOW,300);

  turnGo(ADVERSAIRE_NON,false,true,0,25);

  brasGauche.setPosition(100,0,HIGH,LOW,0);
  brasDroit.setPosition(100,0,HIGH,LOW,1000);

  brasGauche.setPosition(50,0,HIGH,LOW,0);
  brasDroit.setPosition(50,0,HIGH,LOW,1000);

  turnGo(ADVERSAIRE_NON,false,true,0,-80);
  turnGo(ADVERSAIRE_NON,false,true,90,500);

  brasGauche.setPosition(100,0,HIGH,LOW,0);
  brasDroit.setPosition(100,0,HIGH,LOW,300);

  brasGauche.setPosition(100,100,LOW,HIGH,0);
  brasDroit.setPosition(100,100,LOW,HIGH,1000);

  turnGo(ADVERSAIRE_NON,false,true,0,-30);

  brasGauche.setPosition(0,0,LOW,LOW,0);
  brasDroit.setPosition(0,0,LOW,LOW,500);

  finMatch();
}

void sequenceRecalage()
{
  bool recalage = true;
  //Recalage
  if (typeRobot == ROBOT_PRIMAIRE)
  {
    turnGo(ADVERSAIRE_NON,recalage,true,0,-250);
    turnGo(ADVERSAIRE_NON,false,true,0,-20);
    turnGo(ADVERSAIRE_NON,false,true,0,100);
    delay(500);
    turnGo(ADVERSAIRE_NON,recalage,true,90,-1000);
    turnGo(ADVERSAIRE_NON,false,true,0,-20);
    delay(500);
    turnGo(ADVERSAIRE_NON,false,true,0,650);
    turnGo(ADVERSAIRE_NON,recalage,true,-90,-250);
    turnGo(ADVERSAIRE_NON,false,true,0,-20);
    turnGo(ADVERSAIRE_NON,false,true,0,160);
    turnGo(ADVERSAIRE_NON,false,true,90,0);
  }
  else
  {
    turnGo(ADVERSAIRE_NON,recalage,true,0,-250);
    turnGo(ADVERSAIRE_NON,false,true,0,-20);
    turnGo(ADVERSAIRE_NON,false,true,0,100);
    delay(500);
    turnGo(ADVERSAIRE_NON,recalage,true,90,-1000);
    turnGo(ADVERSAIRE_NON,false,true,0,-20);
    delay(500);
    turnGo(ADVERSAIRE_NON,false,true,0,350);
    turnGo(ADVERSAIRE_NON,recalage,true,-90,-250);
    turnGo(ADVERSAIRE_NON,false,true,0,-20);
    turnGo(ADVERSAIRE_NON,false,true,0,250);
    turnGo(ADVERSAIRE_NON,false,true,-12,0);
    //turnGo(ADVERSAIRE_NON,0,false,0,-600);
  }
}

void matchPrimaire()
{
  finMatch();
}

void matchSecondaire()
{
  finMatch();
}

//----------------PROCEDURE D'ATTENTE----------------
void attente(int temps){

	int initTemps = millis();
	while( (millis()-initTemps) <= temps)
	{
		majTemps();
		interface.matchScreen(score,tempsRestant,nbrBadCRC);
	}
}

void majScore(int points, int multiplicateur){
	score = score + (points*multiplicateur);
}


//----------------DEMANDE L'ETAT DU DEPLACEMENT----------------
int askNavigation()
{
  int etatNavigation ;
  char reponseNavigation ;
  Wire.requestFrom(CARTE_DEPLACEMENT, 1);
  while(Wire.available())
  {
    reponseNavigation = Wire.read();
  }
  // Serial.print("repNav:");
  // Serial.println(reponseNavigation);
  if (reponseNavigation=='N') etatNavigation = RECU ;
  else if (reponseNavigation=='O') etatNavigation = TERMINEE ;
  else if (reponseNavigation=='E') etatNavigation = ERRONEE ;
  else reponseNavigation = BIZARRE ;
	return etatNavigation;
}

//----------------ENVOI UNE COMMANDE DE DEPLACEMENT ABSOLU----------------
void sendNavigation(byte fonction, int X, int Y, int rot)
{
	// Stockage des valeurs à envoyer dans le buffer
  bufNavAbsolu[0]=fonction;
	bufNavAbsolu[1]=rot >> 8;
	bufNavAbsolu[2]=rot & 255;
	bufNavAbsolu[3]=X >> 8;
	bufNavAbsolu[4]=X & 255;
	bufNavAbsolu[5]=Y >> 8;
	bufNavAbsolu[6]=Y & 255;

	// Calcul du CRC
	crcNavRelatif = CRC8.smbus(bufNavAbsolu, sizeof(bufNavAbsolu));
	//Serial.println(crcNavRelatif);

	// Envoi des données
	Wire.beginTransmission(CARTE_DEPLACEMENT);
	for(int i=0;i<=6;i++)
	{
		Wire.write(bufNavRelatif[i]);
	}
	//Wire.write(crcNavRelatif);
	Wire.endTransmission();

}

//----------------ENVOI UNE COMMANDE DE DEPLACEMENT RELATIF----------------
void sendNavigation(byte fonction, int rot, int dist)
{
	if ( equipe == EQUIPE_VIOLET ) rot = -rot ;
	// Stockage des valeurs à envoyer dans le buffer
	bufNavRelatif[0]=fonction;
	bufNavRelatif[1]=rot >> 8;
	bufNavRelatif[2]=rot & 255;
	bufNavRelatif[3]=dist >> 8;
	bufNavRelatif[4]=dist & 255;
	// Calcul du CRC
	crcNavRelatif = CRC8.smbus(bufNavRelatif, sizeof(bufNavRelatif));
	//Serial.println(crcNavRelatif);
	// Envoi des données
	Wire.beginTransmission(CARTE_DEPLACEMENT);
	for(int i=0;i<=4;i++)
	{
		Wire.write(bufNavRelatif[i]);
	}
	Wire.write(crcNavRelatif);
	Wire.endTransmission();
}

//----------------ENVOI UNE COMMANDE TURN GO----------------
void turnGo(bool adversaire, bool recalage,bool ralentit,int turn, int go)
{
  int reponseNavigation ;
  bool optionDetection = detection || adversaire; //

	bitWrite(optionNavigation,0,optionDetection); // false -> la detection adverse est active
	bitWrite(optionNavigation,1,recalage);
	bitWrite(optionNavigation,2,ralentit);
	sendNavigation(optionNavigation, turn, go);
	//attente(100);
  reponseNavigation = askNavigation();
	while(reponseNavigation!=TERMINEE)
	{
    if (reponseNavigation==ERRONEE)
    {
      sendNavigation(optionNavigation, turn, go);
      nbrBadCRC++;
      // Serial.print("nbrBadCRC:");
      // Serial.println(nbrBadCRC);
    }
    attente(100);
    reponseNavigation = askNavigation();
	}
}

//----------------MISE A JOUR DU TEMPS DE MATCH----------------
void majTemps(){
  tempsRestant = ( TEMPS_MATCH - (millis() - timeInit) ) / 1000;
  if ( tempsRestant <= 0 )
  {
    finMatch();
  }
}

//----------------PROCEDURE DE FIN DE MATCH----------------
void finMatch(){
	// Stopper les moteurs
	sendNavigation(255, 0, 0);
  servoDrapeau.detach();
  servoDrapeau.attach(pinServoDrapeau);
  servoDrapeau.write(30);
  delay(500);
  servoDrapeau.detach();
	// Boucle infinie
	while(1)
	{
		// Stopper les moteurs
		sendNavigation(255, 0, 0);
    // Stoppe la Balise
    //digitalWrite(pinBalise,LOW);
	}
}
