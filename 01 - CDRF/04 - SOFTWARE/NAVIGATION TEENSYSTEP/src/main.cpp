#include "Navigation.h"

void setup()
{
	//------Déclaration des I/O------
	pinMode(pinSleep, OUTPUT);

  pinMode(pinM0, OUTPUT);
  pinMode(pinM1, OUTPUT);
  pinMode(pinM2, OUTPUT);

	pinMode(pinRobot,INPUT_PULLUP);

  //------Initilisation des I/O------
  // Passage en mode 1/16 de pas
  digitalWrite(pinM0, LOW);
  digitalWrite(pinM1, LOW);
  digitalWrite(pinM2, HIGH);
  // Pause des drivers avant le premier mouvement
  digitalWrite(pinSleep, LOW);
	// Init du type de robot
	changeTypeRobot(digitalRead(pinRobot));

	//------Initialisation des communications------
	Serial.begin(9600);
	Wire.begin(ADRESSE);
	Wire.onReceive(receiveEvent);
	Wire.onRequest(requestEvent);

	//------Initialisation Moteur------
	mGauche
    .setMaxSpeed(VitesseMaxGauche)      // steps/s
    .setAcceleration(AccelMaxGauche); 	// steps/s^2
  mDroit
    .setMaxSpeed(VitesseMaxDroite)      // steps/s
    .setAcceleration(AccelMaxDroite); 	// steps/s^2
}

void loop()
{
	strategieNavigation();
	Serial.println(stateNav);
}

void strategieNavigation()
{
	switch (stateNav)
	{
		case NAVIGATION_AVAILABLE :
			// Navigation Disponible
			digitalWrite(pinSleep, LOW);
			if (stateCom == NEW_REL_TARGET) stateNav = SET_ROTATION ;
			break;
		case SET_ROTATION  	:
			// Lance la rotation du robot
			digitalWrite(pinSleep, HIGH);
			targetRot = relativeRequest[0]*FacteurRot;
			mGauche.setTargetRel(-targetRot);
    	mDroit.setTargetRel(-targetRot);
			robot.move(mGauche,mDroit);
			stateNav = WAIT_ROTATION ;
			break;
		case WAIT_ROTATION 	:
			// Attend la fin de la rotation
			if (!robot.isRunning()) stateNav = SET_DISTANCE ;
			break;
		case SET_DISTANCE  	:
			// Lance le déplacement en distance du robot
			digitalWrite(pinSleep, HIGH);
			targetDis = relativeRequest[1]*FacteurX;
			mGauche.setTargetRel(targetDis);
			mDroit.setTargetRel(-targetDis);
			robot.move(mGauche,mDroit);
			stateCom = WAITING_TARGET;
			stateNav = WAIT_DISTANCE;
			break;
		case WAIT_DISTANCE 	:
			// Attend la fin du déplacement en distance
			if (!robot.isRunning()) stateNav = NAVIGATION_AVAILABLE ;
			break;
		case STOP_OPPONENT 	:
			// Arrêt du robot en cas d'adversaire
			break;
		case WAIT_OPPONENT 	:
			// Attente du passage de l'adversaire
			break;
		case END_OF_MATCH 	:
			// Fin du match
			finMatch();
			break;
	}
}

void changeTypeRobot(bool type)
{
	if(type == ROBOT_PRIMAIRE)
	{
		FacteurX			= primaireFacteurX ;
		FacteurDroit 	= primaireFacteurDroit ;
		FacteurGauche = primaireFacteurGauche ;
		FacteurRot 		= primaireFacteurRot ;
	} else {
		FacteurX			= secondaireFacteurX ;
		FacteurDroit 	= secondaireFacteurDroit ;
		FacteurGauche = secondaireFacteurGauche ;
		FacteurRot 		= secondaireFacteurRot ;
	}
}

//Fin de match
void finMatch()
{

	//TO DO

   	while(1)
   	{
			digitalWrite(pinSleep, LOW);
   	}
}

void receiveEvent(int howMany)
{

	if(howMany == 6)
	{
		// Si un déplacement relatif est demandé
		// On receptionne les données
		for (int i=0;i<=5;i++)
		{
			bufNavRelatif[i]=Wire.read();
		}

		// On calcul le CRC
		crcNavRelatif = CRC8.smbus(bufNavRelatif, sizeof(bufNavRelatif)-1); //On enleve le CRC
		Serial.println(crcNavRelatif);
		// On regarde si le CRC calculé correspond à celui envoyé
		if (crcNavRelatif==bufNavRelatif[sizeof(bufNavRelatif)-1])
		{
			// CRC ok
			// On traite les données
			relativeRequest[0]= bufNavRelatif[1] << 8 | bufNavRelatif[2];
			relativeRequest[1]= bufNavRelatif[3] << 8 | bufNavRelatif[4];

			if (bufNavRelatif[0] == END_OF_MATCH) stateNav = END_OF_MATCH ;
			else
			{
				optionAdversaire 	= 	bitRead(bufNavRelatif[0], 0);
				optionRecalage 		= 	bitRead(bufNavRelatif[0], 1);
				optionRalentit 		= 	bitRead(bufNavRelatif[0],2);
				// On indique qu'une nouvelle position est disponible
				stateCom = NEW_REL_TARGET;
			}
		}
		else
		{
			// CRC nok - la donnée est erronée
			// On indique que la prochaine position est erronée pour en renvois eventuel
			stateCom = ERROR_BAD_CRC;
		}
	}

	if(howMany == 8)
	{
		// Si un déplacement absolu est demandé
		// On receptionne les données
		for (int i=0;i<=7;i++)
		{
			bufNavAbsolu[i]=Wire.read();
		}

		// On calcul le CRC
		crcNavAbsolu = CRC8.smbus(bufNavAbsolu, sizeof(bufNavAbsolu)-1); //On enleve le CRC
		//Serial.println(crcNavRelatif);
		// On regarde si le CRC calculé correspond à celui envoyé
		if (crcNavAbsolu==bufNavAbsolu[sizeof(bufNavAbsolu)-1])
		{
			// CRC ok
			// On traite les données
			absoluteRequest[0]= bufNavAbsolu[1] << 8 | bufNavAbsolu[2];
			absoluteRequest[1]= bufNavAbsolu[3] << 8 | bufNavAbsolu[4];
			absoluteRequest[2]= bufNavAbsolu[5] << 8 | bufNavAbsolu[6];

			if (bufNavAbsolu[0] == END_OF_MATCH) stateNav = END_OF_MATCH ;
			else
			{
				optionAdversaire 	= 	bitRead(bufNavAbsolu[0], 0);
				optionRecalage 		= 	bitRead(bufNavAbsolu[0], 1);
				optionRalentit 		= 	bitRead(bufNavAbsolu[0],2);
				// On indique qu'une nouvelle position est disponible
				stateCom = NEW_ABS_TARGET;
			}
		}
		else
		{
			// CRC nok - la donnée est erronée
			// On indique que la prochaine position est erronée pour en renvois eventuel
			stateCom = ERROR_BAD_CRC;
		}
	}
}

void requestEvent()
{

	if (stateNav == NAVIGATION_AVAILABLE)
  {
    // Mouvement terminé
		Wire.write('O');
		//Serial.println('O');
	}
	else if (stateCom == ERROR_BAD_CRC)
	{
    // Commande non validé
		Wire.write('E');
		//Serial.println('N');
	}
	else
	{
    // Mouvement non terminé
		Wire.write('N');
		//Serial.println('N');
	}
}
