# include "IHM.h"

// Declaration de l'ecran
U8G2_ST7920_128X64_F_SW_SPI _u8g2(U8G2_R0,13,11,12,U8X8_PIN_NONE);

IHM::IHM(){
  pinMode(_pinDetection,INPUT_PULLUP);
  pinMode(_pinStrategie,INPUT_PULLUP);
  pinMode(_pinTypeRobot,INPUT_PULLUP);
  pinMode(_pinTirette,INPUT_PULLUP);
  _u8g2.begin();           //Init du LCD
}

IHM::~IHM(){}

//----------------GESTION DES BOUTTONS DE L'IHM----------------
void IHM::updateButtonIHM(){
  getDetection();
  getStrategie();
  getCheck();
  getTypeRobot();
  getEquipe();
}
bool IHM::getTirette(){
  _tirette = digitalRead(_pinTirette) ;
  return _tirette ;
}
bool IHM::getDetection(){
  _detection  = digitalRead(_pinDetection) ;
  return _detection ;
}
bool IHM::getStrategie(){
  _strategie  = digitalRead(_pinStrategie) ;
  return _strategie ;
}
bool IHM::getCheck(){
  if(analogRead(_pinCheck)>10)
  {
    delay(250);
    if(analogRead(_pinCheck)>10) _check = true ;
  }
  else _check = false ;
  return _check ;
}
bool IHM::getTypeRobot(){
  _typeRobot  = digitalRead(_pinTypeRobot) ;
  return _typeRobot ;
}
bool IHM::getEquipe(){
  if(analogRead(_pinEquipe)>10) _equipe = false ;
  else _equipe = true ;
  return _equipe ;
}

//----------------GESTION DES PAGES LCD-------------------
void IHM::prepare() {
  _u8g2.setFont(u8g2_font_4x6_tf);
  _u8g2.setFontRefHeightExtendedText();
  _u8g2.setDrawColor(1);
  _u8g2.setFontPosTop();
  _u8g2.setFontDirection(0);
}
void IHM::splashScreen() {
  _u8g2.clearBuffer();
  _u8g2.drawXBMP( 0, 15, _LOGO_KARIBOUS_width, _LOGO_KARIBOUS_height, _LOGO_KARIBOUS_bits);
  _u8g2.sendBuffer();
}
void IHM::menuScreen() {
  const int ligneDebut = 10;
  const int colonne1 = 14;
  const int colonne2 = 70;

  _u8g2.clearBuffer();
  prepare();

  _u8g2.setFont(u8g2_font_4x6_tf);
  // Affichages des titres :
  _u8g2.drawStr( colonne1, ligneDebut,    "      EQUIPE");
  _u8g2.drawStr( colonne1, ligneDebut+10, "   EVITEMENT");
  _u8g2.drawStr( colonne1, ligneDebut+20, "       ROBOT");
  _u8g2.drawStr( colonne1, ligneDebut+30, "ETAT TIRETTE");
  _u8g2.drawStr( colonne1, ligneDebut+40, "   STRATEGIE");
  // Ligne de séparation
  _u8g2.drawBox(colonne2-4,ligneDebut,1,ligneDebut+37);

  // Etat equipe :
  _u8g2.setCursor(colonne2,ligneDebut);
  if ( _equipe == _EQUIPE_JAUNE ) _u8g2.print("JAUNE");
  else _u8g2.print("VIOLET");
  // Etat detection:
  _u8g2.setCursor(colonne2,ligneDebut+10);
  if ( _detection ) _u8g2.print("SIMPLE");
  else _u8g2.print("COMPLET");
  // Etat type de robot :
  _u8g2.setCursor(colonne2,ligneDebut+20);
  if ( _typeRobot == _ROBOT_PRIMAIRE ) _u8g2.print("PRIMAIRE");
  else _u8g2.print("SECONDAIRE");
  // Etat tirette :
  _u8g2.setCursor(colonne2,ligneDebut+30);
  if ( _tirette ) _u8g2.print("ATTENTE");
  else _u8g2.print("OK");
  // Etat strategie :
  _u8g2.setCursor(colonne2,ligneDebut+40);
  if ( _strategie ) _u8g2.print("HOMOLOGATION");
  else _u8g2.print("MATCH");

  _u8g2.sendBuffer();
}
void IHM::initScreen(){
  _u8g2.clearBuffer();
  _u8g2.setFont(u8g2_font_logisoso58_tr);
  _u8g2.drawStr( 10, 2, "Init");
  _u8g2.sendBuffer();
  delay(1500);
}
void IHM::checkListScreen(){
  for(int i=0;i<6;i++)
  {
    int x = 128;    //On commence le texte à droite
    do
    {
      _u8g2.clearBuffer();

      _u8g2.drawBox(22,0,2,33);          //Ligne de séparation numéro
      for(int j=0;j<=128;j=j+4) _u8g2.drawBox(j,40,2,1);          //Ligne de séparation texte - tirets

      _u8g2.setFont(u8g2_font_logisoso32_tn); //Changer la font pour le numero
      _u8g2.setCursor(0,0);
      _u8g2.print(i+1);                        //Afficher le numero de l'action

      _u8g2.setFont(u8g2_font_courB08_tf);       //Changer la font pour la description
      _u8g2.drawStr(28, 0, "Action :");     //Afficher le titre de l'action

      _u8g2.setFont(u8g2_font_logisoso22_tf);       //Changer la font pour la description
      _u8g2.drawStr(25, 10, _titreList[i]);     //Afficher le titre de l'action

      _u8g2.setFont(u8g2_font_courB08_tf);  //Changer la font pour la description
      _u8g2.drawStr(x, 48, _list[i]);           //Afficher l'action

      _u8g2.sendBuffer();
      delay(100);
      x -= 10 ;                               //Scrolling
    } while(getCheck());
    delay(1000);
  }
}
void IHM::goScreen() {
  _u8g2.clearBuffer();
  _u8g2.setFont(u8g2_font_logisoso58_tr);
  _u8g2.drawStr( 18, 2, "GO!");
  _u8g2.sendBuffer();
}
void IHM::matchScreen(int score,int tempsRestant,int nbrBadCRC) {
	_u8g2.clearBuffer();
	prepare();
  _u8g2.setFont(u8g2_font_inr42_mn);
  _u8g2.setCursor(8, 9);
  _u8g2.print(score);
  _u8g2.setFont(u8g2_font_4x6_tf);
  _u8g2.drawStr( 0, 0, "Score:");
  _u8g2.drawStr( 68, 0, "Temps:      sec");
  _u8g2.setCursor(93, 0);
  _u8g2.print(tempsRestant);
  _u8g2.drawStr( 105, 57, "points");
  _u8g2.drawStr( 0, 57, "NOK:");
  _u8g2.setCursor(20, 57);
  _u8g2.print(nbrBadCRC);
	_u8g2.sendBuffer();
}
