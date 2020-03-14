#include "strategy.h"

/***************************************
 *             Strategy                *
 ***************************************/

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



/***************************************
 *            Main methods             *
 ***************************************/

//Initializing strategy blocks
void prepareMatch(){
    
}

//Waiting match to start
void waitLaunch()
{
    // Gestion tirette
    while (interface.getTirette())
    {
        interface.updateButtonIHM();
        if (!interface.getCheck())
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
        if (!interface.getCheck())
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
    delay(100);
}


//Initializing actuators
void initActionneur()
{
  // Check du type de robot :
    typeRobot = interface.getTypeRobot();
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