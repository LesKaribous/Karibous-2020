# include "ComNavigation.h"

ComNavigation::ComNavigation(){
  Wire.begin();             //Demarrage de la liaison I2C
}
ComNavigation::~ComNavigation(){}

void ComNavigation::setEquipe(bool equipe){
  _equipe = equipe ;
}
void ComNavigation::setDetection(bool detection){
  _detection = detection ;
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
	//Serial.println(crcNavRelatif);

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
	if ( _equipe == _EQUIPE_VIOLET ) rot = -rot ;
	// Stockage des valeurs à envoyer dans le buffer
	_bufNavRelatif[0]=fonction;
	_bufNavRelatif[1]=rot >> 8;
	_bufNavRelatif[2]=rot & 255;
	_bufNavRelatif[3]=dist >> 8;
	_bufNavRelatif[4]=dist & 255;
	// Calcul du CRC
	_crcNavRelatif = _CRC8.smbus(_bufNavRelatif, sizeof(_bufNavRelatif));
	//Serial.println(crcNavRelatif);
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
void ComNavigation::turnGo(bool adversaire, bool recalage,bool ralentit,int turn, int go)
{
  int reponseNavigation ;
  bool optionDetection = _detection || adversaire; //
  byte optionNavigation = 0;

	bitWrite(optionNavigation,0,optionDetection); // false -> la detection adverse est active
	bitWrite(optionNavigation,1,recalage);
	bitWrite(optionNavigation,2,ralentit);
	sendNavigation(optionNavigation, turn, go);
	//attente(100);
  reponseNavigation = askNavigation();
	while(reponseNavigation!=_TERMINEE)
	{
    if (reponseNavigation==_ERRONEE)
    {
      sendNavigation(optionNavigation, turn, go);
      _nbrBadCRC++;
      // Serial.print("nbrBadCRC:");
      // Serial.println(nbrBadCRC);
    }


    


    //------------------!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    //attente(100); //A corriger et à remettre !!!!!
    //------------------!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!




    reponseNavigation = askNavigation();
	}
}
