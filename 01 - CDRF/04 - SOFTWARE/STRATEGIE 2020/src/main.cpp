#include "Strategie.h"

void setup()
{
  pinMode(pinTirette,INPUT_PULLUP);
  pinMode(pinDetection,INPUT_PULLUP);
  pinMode(pinStrategie,INPUT_PULLUP);
  pinMode(pinRobot,INPUT_PULLUP);

  Serial.begin(9600);     //Demarrage d'un liaison série pour le debug
  Wire.begin();           //Demarrage de la liaison I2C

  delay(500);             //Pause de demarrage avant lancement

  u8g2.begin();           //Init du LCD
  u8g2_splash_screen();   //Affichage du Logo des Karibous
  initActionneur();       //Initialisation des actionneurs
  delay(1000);            //Attente affichage logo

  // Gestion tirette
  while (digitalRead(pinTirette))
  {
    //Menu d'avant Match
    bouttonIHM();
    u8g2_menu_avant_match();
  }
  while (!digitalRead(pinTirette))
  {
    //Menu d'avant Match
    bouttonIHM();
    u8g2_menu_avant_match();
  }

  // Lancement du Match
	timeInit = millis();
  u8g2_splash_screen_GO();
	delay (100);
  //digitalWrite(pinBalise,HIGH); //Lancement Balise
}

void loop()
{
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

//----------------INIT ACTIONNEUR-------------
void initActionneur()
{
  // Check du type de robot :
  typeRobot = digitalRead(pinRobot) ;
  // Initialisation des actionneurs en fonction du type de robot
    brasDroit.setPin(pinServoDroit,pinServoVentouseDroit,pinPompeDroit,pinEVDroit);
    brasGauche.setPin(pinServoGauche,pinServoVentouseGauche,pinPompeGauche,pinEVGauche);

    brasDroit.setLimit(45,150,10,90);
    brasGauche.setLimit(140,45,160,70);

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

//----------------GESTION DES BOUTTONS DE L'IHM----------------
void bouttonIHM()
{
  detection = digitalRead(pinDetection) ;
  tirette   = digitalRead(pinTirette)   ;
  strategie = digitalRead(pinStrategie) ;
  typeRobot = digitalRead(pinRobot) ;

  if(analogRead(pinEquipe)>10) equipe = false ;
  else equipe = true ;

  if(analogRead(pinCheck)>10)
  {
    delay(250);
    if(analogRead(pinCheck)>10) check = true ;
  }
  else check = false ;

  if(!check) initRobot();
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
    turnGo(ADVERSAIRE_NON,false,true,0,650);          // Centre du robot à 743
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

void demoRotation()
{
  // Rotation
  for(int i=0;i<=10;i++)
  {
    turnGo(ADVERSAIRE_OUI,false,true,3600,0);
  }
  // Fin de match
  finMatch();
}

// Test nouvel actionneur
void testActionneur()
{
  brasGauche.setPosition(50,0,LOW,LOW,1000);
  brasGauche.setPosition(50,100,LOW,LOW,1000);
  brasGauche.setPosition(100,100,HIGH,LOW,1000);
  brasGauche.setPosition(50,100,HIGH,LOW,2000);
  brasGauche.setPosition(50,100,LOW,HIGH,500);
  brasGauche.setPosition(50,100,LOW,LOW,500);
  brasGauche.setPosition(50,0,LOW,LOW,0);
  brasGauche.setPosition(0,0,LOW,LOW,0);

  brasDroit.setPosition(50,0,LOW,LOW,1000);
  brasDroit.setPosition(50,100,LOW,LOW,1000);
  brasDroit.setPosition(100,100,HIGH,LOW,1000);
  brasDroit.setPosition(50,100,HIGH,LOW,2000);
  brasDroit.setPosition(50,100,LOW,HIGH,500);
  brasDroit.setPosition(50,100,LOW,LOW,500);
  brasDroit.setPosition(50,0,LOW,LOW,0);
  brasDroit.setPosition(0,0,LOW,LOW,0);
}

// Test nouvel actionneur
void testLancerGobelet()
{
  // Prise du gobelet
}



//----------------PROCEDURE D'ATTENTE----------------
void attente(int temps)
{
	int initTemps = millis();
	while( (millis()-initTemps) <= temps)
	{
		majTemps();
		u8g2_menu_pendant_match();
	}
}

//----------------GESTION DES PAGES LCD-------------------
void u8g2_prepare() {
  u8g2.setFont(u8g2_font_4x6_tf);
  u8g2.setFontRefHeightExtendedText();
  u8g2.setDrawColor(1);
  u8g2.setFontPosTop();
  u8g2.setFontDirection(0);
}

void u8g2_splash_screen() {
  u8g2.clearBuffer();
  u8g2.drawXBMP( 0, 15, LOGO_KARIBOUS_width, LOGO_KARIBOUS_height, LOGO_KARIBOUS_bits);
  u8g2.sendBuffer();
}

void u8g2_splash_screen_GO() {
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_logisoso58_tr);
  u8g2.drawStr( 18, 2, "GO!");
  u8g2.sendBuffer();
}

void u8g2_menu_avant_match() {
  const int ligneDebut = 10;
  const int colonne1 = 14;
  const int colonne2 = 70;

  u8g2.clearBuffer();
  u8g2_prepare();

  u8g2.setFont(u8g2_font_4x6_tf);
  // Affichages des titres :
  u8g2.drawStr( colonne1, ligneDebut,    "      EQUIPE");
  u8g2.drawStr( colonne1, ligneDebut+10, "   EVITEMENT");
  u8g2.drawStr( colonne1, ligneDebut+20, "       ROBOT");
  u8g2.drawStr( colonne1, ligneDebut+30, "ETAT TIRETTE");
  u8g2.drawStr( colonne1, ligneDebut+40, "   STRATEGIE");
  // Ligne de séparation
  u8g2.drawBox(colonne2-4,ligneDebut,1,ligneDebut+37);

  // Etat equipe :
  u8g2.setCursor(colonne2,ligneDebut);
  if ( equipe == EQUIPE_JAUNE ) u8g2.print("JAUNE");
  else u8g2.print("VIOLET");
  // Etat detection:
  u8g2.setCursor(colonne2,ligneDebut+10);
  if ( detection ) u8g2.print("SIMPLE");
  else u8g2.print("COMPLET");
  // Etat type de robot :
  u8g2.setCursor(colonne2,ligneDebut+20);
  if ( typeRobot == ROBOT_PRIMAIRE ) u8g2.print("PRIMAIRE");
  else u8g2.print("SECONDAIRE");
  // Etat tirette :
  u8g2.setCursor(colonne2,ligneDebut+30);
  if ( tirette ) u8g2.print("ATTENTE");
  else u8g2.print("OK");
  // Etat strategie :
  u8g2.setCursor(colonne2,ligneDebut+40);
  if ( strategie ) u8g2.print("HOMOLOGATION");
  else u8g2.print("MATCH");

  u8g2.sendBuffer();
}

void u8g2_menu_pendant_match() {
	u8g2.clearBuffer();
	u8g2_prepare();
  u8g2.setFont(u8g2_font_inr42_mn);
  u8g2.setCursor(8, 9);
  u8g2.print(score);
  u8g2.setFont(u8g2_font_4x6_tf);
  u8g2.drawStr( 0, 0, "Score:");
  u8g2.drawStr( 68, 0, "Temps:      sec");
  u8g2.setCursor(93, 0);
  u8g2.print(tempsRestant);
  u8g2.drawStr( 105, 57, "points");
  u8g2.drawStr( 0, 57, "NOK:");
  u8g2.setCursor(20, 57);
  u8g2.print(nbrBadCRC);
	u8g2.sendBuffer();
}

void majScore(int points, int multiplicateur){
	score = score + (points*multiplicateur);
}

//----------------INITIALISATION DU ROBOT----------------
void initRobot()
{
  //-------Affichage Init-------
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_logisoso58_tr);
  u8g2.drawStr( 10, 2, "Init");
  u8g2.sendBuffer();
  attente(1000);
  //-------Recalage bordure-------
  sequenceRecalage();
  //-------Checklist-------
  // A FINIR !
  /*
  for(int i=0;i<6;i++)
  {
    int x = 128;    //On commence le texte à droite
    do
    {
      u8g2.clearBuffer();

      u8g2.drawBox(22,0,2,33);          //Ligne de séparation numéro
      for(int j=0;j<=128;j=j+4) u8g2.drawBox(j,40,2,1);          //Ligne de séparation texte - tirets

      u8g2.setFont(u8g2_font_logisoso32_tn); //Changer la font pour le numero
      u8g2.setCursor(0,0);
      u8g2.print(i+1);                        //Afficher le numero de l'action

      u8g2.setFont(u8g2_font_courB08_tf);       //Changer la font pour la description
      u8g2.drawStr(28, 0, "Action :");     //Afficher le titre de l'action

      u8g2.setFont(u8g2_font_logisoso22_tf);       //Changer la font pour la description
      u8g2.drawStr(25, 10, titreList[i]);     //Afficher le titre de l'action

      u8g2.setFont(u8g2_font_courB08_tf);  //Changer la font pour la description
      u8g2.drawStr(x, 48, list[i]);           //Afficher l'action

      u8g2.sendBuffer();
      attente(100);
      x -= 10 ;                               //Scrolling
    } while(analogRead(pinCheck)>10);
    attente(1000);
  }
  */
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
void majTemps()
{
  tempsRestant = ( TEMPS_MATCH - (millis() - timeInit) ) / 1000;
  if ( tempsRestant <= 0 )
  {
    finMatch();
  }
}

//----------------PROCEDURE DE FIN DE MATCH----------------
void finMatch()
{
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
