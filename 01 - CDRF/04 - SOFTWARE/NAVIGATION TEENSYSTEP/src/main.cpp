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
  digitalWrite(pinSleep, HIGH);
	// Init du type de robot
	changeTypeRobot(digitalRead(pinRobot));
	// Init des pins capteurs bordure
	for (int i=0;i<4;i++) pinMode(pinBorderSensor[i], INPUT_PULLUP)	;
	// Init des pins capteurs adversaire
	pinMode(pinAdversaireAvant, INPUT_PULLUP)	;
	pinMode(pinAdversaireArriere, INPUT_PULLUP)	;
	//------Initialisation des communications------
	Serial.begin(115200);

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
	getBorderState();
	getOpponentState();
	strategieNavigation();
}

void strategieNavigation()
{
	switch (stateNav)
	{
		case NAVIGATION_AVAILABLE:	navigationAvailable();	break;
	  case CONVERT_REL_TARGET:		convertRelTarget(); 		break;
		case CONVERT_ABS_TARGET: 		convertAbsTarget(); 		break;
		case SET_ROTATION: 					setRotation();					break;
		case WAIT_ROTATION: 				waitRotation();					break;
		case SET_DISTANCE:					setDistance();					break;
		case WAIT_DISTANCE:					waitDistance();					break;
		case STOP_OPPONENT:					stopOpponent();					break;
		case WAIT_OPPONENT:					waitOpponent();					break;
		case SET_ALPHA:							setAlpha();							break;
	  case WAIT_ALPHA:						waitAlpha();						break;
		case END_OF_MATCH:					endOfMatch();						break;
	  case WAIT_END_OF_MATCH:			waitEndOfMatch();				break;
	}
}

void navigationAvailable()
{
	// Navigation Disponible
	digitalWrite(pinSleep, LOW);
	if (stateCom == NEW_REL_TARGET) stateNav = CONVERT_REL_TARGET ;
	else if (stateCom == NEW_ABS_TARGET) stateNav = CONVERT_ABS_TARGET ;
}
void convertRelTarget()
{
	targetRot = relativeRequest[0]*FacteurRot;
	targetDis = relativeRequest[1]*FacteurX;

	stateNav = SET_ROTATION ;
}
void convertAbsTarget()
{
	double deltaY = absoluteRequest[1]-currentPos[1] ;
	double deltaX = absoluteRequest[0]-currentPos[0] ;

	targetRot = atan2(deltaY,deltaX);
	targetRot *= (180.0/PI);
	targetRot = currentPos[2]-targetRot;
	targetRot *= FacteurRot;

	targetDis = sqrt((deltaY*deltaY)+(deltaX*deltaX));
	targetDis *= FacteurX;

	targetAlpha = absoluteRequest[2]-targetRot;
	targetAlpha *= FacteurRot;

	stateNav = SET_ROTATION ;
}
void setRotation()
{
	// Lance la rotation du robot
	digitalWrite(pinSleep, HIGH);
	mGauche.setTargetRel(-targetRot);
	mDroit.setTargetRel(-targetRot);
	robot.moveAsync(mGauche,mDroit);
	stateNav = WAIT_ROTATION ;
}

void waitRotation()
{
	// Attend la fin de la rotation
	if (!robot.isRunning()) stateNav = SET_DISTANCE ;
}
void setDistance()
{
	// Lance le déplacement en distance du robot
	digitalWrite(pinSleep, HIGH);
	mGauche.setTargetRel(-targetDis);
	mDroit.setTargetRel(targetDis);
	//Enregistre la position de départ
	startPositionLeft		=	mGauche.getPosition();
	startPositionRight	= mDroit.getPosition();

	robot.moveAsync(mGauche,mDroit);
	//stateCom = WAITING_TARGET;
	stateNav = WAIT_DISTANCE;
}
void waitDistance()
{
	// Attend la fin du déplacement en distance
	// Si recalage demandé et marche arriere
	if (optionRecalage && targetDis < 0)
	{
		if 			(borderState[2]) pinMode(pinStepDroit,INPUT);
		else if (borderState[3]) pinMode(pinStepGauche,INPUT);
	}
	// Si check adversaire demandé
	if (optionAdversaire)
	{
		if 			(targetDis > 0 && presenceAvant) 		stateNav = STOP_OPPONENT ;
		else if (targetDis < 0 && presenceArriere) 	stateNav = STOP_OPPONENT ;
	}

	if (!robot.isRunning())
	{
		if(stateCom == NEW_REL_TARGET)
		{
			stateNav = NAVIGATION_AVAILABLE ;
			stateCom = WAITING_TARGET ;
			pinMode(pinStepDroit,OUTPUT);
			pinMode(pinStepGauche,OUTPUT);
		}
		else if(stateCom == NEW_ABS_TARGET)
		{
			stateNav = SET_ALPHA ;
		}
	}
}
void stopOpponent()
{
	// Arrêt du robot en cas d'adversaire
	// Enregistre la position actuelle (getPosition)
	// Enregistre la psoition désirée de fin
	Serial.print("stop à :");
	Serial.println(mDroit.getPosition());
	Serial.print("Distance demandée :");
	Serial.println(targetDis);
	Serial.print("Start position :");
	Serial.println(startPositionRight);
	// Met la position du stepper à la position actuelle + decelleration (setPosition)
	// va a la position actuelle - decelleration
	robot.stopAsync();
	// Va à WAIT_OPPONENT
	stateNav = WAIT_OPPONENT ;
}
void waitOpponent()
{
	// Attendre la fin de la deceleration (vitesse à zero)
	if (targetDis > 0 && !presenceAvant)
	{
		targetDis = targetDis-(mDroit.getPosition()-startPositionRight);
		stateNav = SET_DISTANCE ;
	}
	else if (targetDis < 0 && !presenceArriere)
	{
		targetDis = targetDis+(startPositionRight-mDroit.getPosition());
		stateNav = SET_DISTANCE ;
	}
}
void setAlpha()
{
	// Lance la rotation du robot
	digitalWrite(pinSleep, HIGH);
	mGauche.setTargetRel(-targetAlpha);
	mDroit.setTargetRel(-targetAlpha);
	robot.moveAsync(mGauche,mDroit);
	stateNav = WAIT_ALPHA ;
}
void waitAlpha()
{
	// Attend la fin de la rotation
	if (!robot.isRunning())
	{
		stateNav = NAVIGATION_AVAILABLE ;
		pinMode(pinStepDroit,OUTPUT);
		pinMode(pinStepGauche,OUTPUT);
	}
}
void endOfMatch()
{
	// Fin du match
	digitalWrite(pinSleep, HIGH);
	robot.stopAsync();
	stateNav = WAIT_END_OF_MATCH ;
}
void waitEndOfMatch()
{
	// Attendre la fin de l'arrêt de fin de match
	if (!robot.isRunning()) stateNav = NAVIGATION_AVAILABLE ;
}

void getBorderState()
{
	for(int i = 0;i<4;i++) borderState[i] = digitalRead(pinBorderSensor[i]);
}

void getOpponentState()
{
	// Adversaire Avant
	if (digitalReadFast(pinAdversaireAvant))
	{
		if (!presenceAvant) presenceAvant = true ;
		//Demarre ou reset le comptage de temps
		avantTimeInit = millis();
	}
	else if((millis()-avantTimeInit)>=sensorTime) presenceAvant = false;

	// Adversaire Arriere
	if (digitalReadFast(pinAdversaireArriere))
	{
		if (!presenceAvant) presenceArriere = true ;
		//Demarre ou reset le comptage de temps
		arriereTimeInit = millis();
	}
	else if((millis()-arriereTimeInit)>=sensorTime) presenceArriere = false;
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
		//Serial.println(crcNavRelatif);
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
				optionRalentit 		= 	bitRead(bufNavRelatif[0], 2);
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
			absoluteRequest[0]= bufNavAbsolu[1] << 8 | bufNavAbsolu[2]; // NewX
			absoluteRequest[1]= bufNavAbsolu[3] << 8 | bufNavAbsolu[4]; // NewY
			absoluteRequest[2]= bufNavAbsolu[5] << 8 | bufNavAbsolu[6]; // NewAlpha

			if (bufNavAbsolu[0] == END_OF_MATCH) stateNav = END_OF_MATCH ;
			else
			{
				optionAdversaire 	= 	bitRead(bufNavAbsolu[0], 0);
				optionRecalage 		= 	bitRead(bufNavAbsolu[0], 1);
				optionRalentit 		= 	bitRead(bufNavAbsolu[0], 2);
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
