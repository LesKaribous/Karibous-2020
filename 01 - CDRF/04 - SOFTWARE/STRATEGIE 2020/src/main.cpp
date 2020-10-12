#include "Strategie.h"


void setup(){
  Serial.begin(115200);     //Demarrage d'un liaison série pour le debug
  Wire.begin();             //Demarrage de la liaison I2C
  delay(500);               //Pause de demarrage avant lancement
  interface.splashScreen(); //Affichage du Logo des Karibous
  setupActionneur();        //Setup des actionneurs
  //initActionneur();         //Initialisation des actionneurs
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

void interfaceLink(){

    byte bufReceive[16];
    bool newCommand = false;
    for (int i = 0; i<16;i++)
    {
      bufReceive[i]=0;
    }
    if (Serial.available()>0)
    {
        Serial.readBytes(bufReceive, 16);
        if (bufReceive[0] == 'b' && bufReceive[15] == '\n' )
        {
          newCommand = true;
        }
    }
    if (newCommand)
    {
      brasGauche.setPosition(bufReceive[1],bufReceive[2],bufReceive[3],bufReceive[4],0);
      brasDroit.setPosition(bufReceive[6],bufReceive[7],bufReceive[8],bufReceive[9],0);
      brasGauche.setAscenseur(bufReceive[5]);
      //brasDroit.setAscenseur(bufReceive[10]);

      int turnValue = bufReceive[11] << 8 | bufReceive[12];
      int goValue = bufReceive[13] << 8 | bufReceive[14];

      turnGo(ADVERSAIRE_NON,false,true,turnValue,goValue);
    }
}

void waitLaunch(){
  // Gestion tirette
  while (interface.getTirette())
  {
    interface.updateButtonIHM();
    interfaceLink();
    if(!interface.getCheck())
    {
      interface.initScreen();
      sequenceRecalage();
      //interface.checkListScreen();
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
      //interface.checkListScreen();
    }
    interface.menuScreen();
  }

  // Lancement du Match
  matchEnCours = true;
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

void setupActionneur(){
  // Check du type de robot :
    typeRobot = interface.getTypeRobot();
  // Initialisation des actionneurs en fonction du type de robot
    brasDroit.setPin(pinServoDroit,pinServoVentouseDroit,pinPompeDroit,pinEVDroit);
    brasGauche.setPin(pinServoGauche,pinServoVentouseGauche,pinPompeGauche,pinEVGauche);

    if (typeRobot == ROBOT_PRIMAIRE)
    {
      brasDroit.setLimit(35,140,10,90);
      brasGauche.setLimit(110,10,160,70);
      brasDroit.setPosition(0,0,LOW,LOW,0);
      brasGauche.setPosition(0,0,LOW,LOW,2000);

      servoBrasDroit.detach();
      servoBrasGauche.detach();

      servoBrasDroit.attach(pinServoBrasDroit);
      servoBrasGauche.attach(pinServoBrasGauche);

      servoBrasDroit.write(170);
      servoBrasGauche.write(5);
    }
    else
    {
      brasDroit.setLimit(45,150,10,90);
      brasGauche.setLimit(140,45,160,70);
      brasDroit.setPosition(0,0,LOW,LOW,0);
      brasGauche.setPosition(0,0,LOW,LOW,2000);
    }

}

void initActionneur(){

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
  //TO DO
  finMatch();
}

void homologationSecondaire()
{
  //TO DO
  finMatch();
}

void sequenceRecalage(){
  //Recalage
  if (typeRobot == ROBOT_PRIMAIRE)
  {
    turnGo(false,true,false,0,-100);
    attente(1000);
    //TO DO
  }
  else
  {
    //TO DO
  }
}

void matchPrimaire(){
  turnGo(true,false,false,0,100);
  turnGo(true,false,false,45,0);
  turnGo(true,false,false,-45,0);
  turnGo(true,true,false,0,-100);
  turnGo(true,false,false,0,100);
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
		if(matchEnCours)
    {
      majTemps();
		  interface.matchScreen(score,tempsRestant,navError);
    }
	}
}

void majScore(int points, int multiplicateur){
	score = score + (points*multiplicateur);
}

//----------------ENVOI UNE COMMANDE TURN GO----------------
void turnGo(bool adversaire, bool recalage,bool ralentit,int turn, int go)
{
	nav.turnGo(adversaire,recalage,ralentit,turn,go);

  int reponseNavigation = nav.askNavigation();
	while(reponseNavigation!=TERMINEE)
	{
    if (reponseNavigation==ERRONEE)
    {
      nav.turnGo(adversaire,recalage,ralentit,turn,go);
      navError++;
    }
    attente(100);
    reponseNavigation = nav.askNavigation();
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
	nav.sendNavigation(255, 0, 0);
  servoDrapeau.detach();
  servoDrapeau.attach(pinServoDrapeau);
  servoDrapeau.write(30);
  delay(500);
  servoDrapeau.detach();
	// Boucle infinie
	while(1)
	{
		// Stopper les moteurs
		nav.sendNavigation(255, 0, 0);
    // Stoppe la Balise
    //digitalWrite(pinBalise,LOW);
	}
}
