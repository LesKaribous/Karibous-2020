# include "ComNavigation.h"

ComNavigation::ComNavigation(){
  Wire.begin();             //Demarrage de la liaison I2C
}
ComNavigation::~ComNavigation(){}

void ComNavigation::setTeam(bool team){
  _team = team ;
}
void ComNavigation::setGlobalDetection(bool globalDetection){
  _globalDetection = globalDetection ;
}
void ComNavigation::setDetection(bool detection){
  _detectionState = detection ;
}
void ComNavigation::setRecalibration(bool recalibration){
  _recalibrationState = recalibration ;
}
void ComNavigation::setSpeed(bool speed){
  _speedState = speed ;
}
int ComNavigation::getNbrBadCRC(){
  return _nbrBadCRC;
}

//----------------DEMANDE L'ETAT DU DEPLACEMENT----------------
int ComNavigation::askNavigation()
{
  int etatNavigation ;
  char reponseNavigation ;
  Wire.requestFrom(_CARTE_DEPLACEMENT, 1);
  while(Wire.available())
  {
    reponseNavigation = Wire.read();
  }
  // Serial.print("repNav:");
  // Serial.println(reponseNavigation);
  if (reponseNavigation=='N') etatNavigation = _RECU ;
  else if (reponseNavigation=='O') etatNavigation = _TERMINEE ;
  else if (reponseNavigation=='E') etatNavigation = _ERRONEE ;
  else reponseNavigation = _BIZARRE ;
	return etatNavigation;
}

//----------------ENVOI UNE COMMANDE DE DEPLACEMENT ABSOLU----------------
void ComNavigation::sendNavigation(byte fonction, int X, int Y, int rot)
{
	// Stockage des valeurs à envoyer dans le buffer
  _bufNavAbsolu[0]=fonction;
	_bufNavAbsolu[1]=rot >> 8;
	_bufNavAbsolu[2]=rot & 255;
	_bufNavAbsolu[3]=X >> 8;
	_bufNavAbsolu[4]=X & 255;
	_bufNavAbsolu[5]=Y >> 8;
	_bufNavAbsolu[6]=Y & 255;

	// Calcul du CRC
	_crcNavRelatif = _CRC8.smbus(_bufNavAbsolu, sizeof(_bufNavAbsolu));
	//Serial.println(_crcNavRelatif);

	// Envoi des données
	Wire.beginTransmission(_CARTE_DEPLACEMENT);
	for(int i=0;i<=6;i++)
	{
		Wire.write(_bufNavRelatif[i]);
	}
	//Wire.write(crcNavRelatif);
	Wire.endTransmission();
}

//----------------ENVOI UNE COMMANDE DE DEPLACEMENT RELATIF----------------
void ComNavigation::sendNavigation(byte fonction, int rot, int dist)
{
	if ( _team == _EQUIPE_VIOLET ) rot = -rot ;
	// Stockage des valeurs à envoyer dans le buffer
	_bufNavRelatif[0]=fonction;
	_bufNavRelatif[1]=rot >> 8;
	_bufNavRelatif[2]=rot & 255;
	_bufNavRelatif[3]=dist >> 8;
	_bufNavRelatif[4]=dist & 255;
	// Calcul du CRC
	_crcNavRelatif = _CRC8.smbus(_bufNavRelatif, sizeof(_bufNavRelatif));
	//Serial.println(_crcNavRelatif);
	// Envoi des données
	Wire.beginTransmission(_CARTE_DEPLACEMENT);
	for(int i=0;i<=4;i++)
	{
		Wire.write(_bufNavRelatif[i]);
	}
	Wire.write(_crcNavRelatif);
	Wire.endTransmission();
}

//----------------ENVOI UNE COMMANDE TURN GO----------------
void ComNavigation::turnGo(int turn, int go)
{
  turnGo(_detectionState,_recalibrationState,_speedState,turn, go);
}

void ComNavigation::turnGo(bool detection, bool recalibration,bool speed,int turn, int go)
{
  bool optionDetection = _globalDetection || detection;
  byte optionNavigation = 0;

	bitWrite(optionNavigation,0,optionDetection);
	bitWrite(optionNavigation,1,recalibration);
	bitWrite(optionNavigation,2,speed);
	sendNavigation(optionNavigation, turn, go);

  setDetection(detection);
  setRecalibration(recalibration);
  setSpeed(speed);
}
