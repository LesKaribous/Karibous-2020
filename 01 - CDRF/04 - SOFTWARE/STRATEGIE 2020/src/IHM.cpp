# include "IHM.h"

// Declaration de l'ecran
U8G2_ST7920_128X64_F_SW_SPI _u8g2(U8G2_R3,13,11,12,U8X8_PIN_NONE);

IHM::IHM(){
  //pinMode(_pinDetection,INPUT_PULLUP);
  //pinMode(_pinStrategie,INPUT_PULLUP);
  //pinMode(_pinTypeRobot,INPUT_PULLUP);
  pinMode(_pinTirette,INPUT_PULLUP);

  pinMode(_latchMux, OUTPUT);
  pinMode(_clockMux, OUTPUT);
  pinMode(_clockInhMux, OUTPUT);
  pinMode(_dataMux, INPUT);
  digitalWrite(_latchMux, HIGH);
  digitalWrite(_clockMux, LOW);

    _u8g2.begin();           //Init du LCD
}

IHM::~IHM(){}

//----------------GESTION DES BOUTTONS DE L'IHM----------------
void IHM::updateButtonIHM(){
  readButtonState();
  getDetection();
  getStrategie();
  getCheck();
  getTypeRobot();
  getEquipe();
}

void IHM::readButtonState(){
  digitalWrite(_latchMux, LOW);
  digitalWrite(_latchMux, HIGH);
  for (int i=0; i <= 7; i++)
  {
    // on note l'état de la sortie (pin 9) du HC165
    _buttonState[7-i] = digitalRead(_dataMux);
    // et on envoi un front montant sur la pin 2 pour décaler les valeurs
    digitalWrite(_clockMux, HIGH);
    digitalWrite(_clockMux, LOW);
  }
}
bool IHM::getTirette(){
  if(analogRead(_pinTirette)<10) _tirette = false ;
  else _tirette = true ;
  return _tirette ;
}
bool IHM::getDetection(){
  _detection  = _buttonState[7] ;
  return _detection ;
}
bool IHM::getStrategie(){
  _strategie  = _buttonState[6] ;
  return _strategie ;
}
bool IHM::getCheck(){
  _check = _buttonState[5];
  return _check ;
}
bool IHM::getTypeRobot(){
  _typeRobot  = digitalRead(_pinTypeRobot) ;
  return _typeRobot ;
}
bool IHM::getEquipe(){
  _equipe = _buttonState[4];
  return _equipe ;
}
void IHM::setRecalage(bool state){
  _recalage = state;
}
bool IHM::getRecalage(){
  return _recalage ;
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
  // Affichage de l'image K
  _u8g2.setDrawColor(1);
  _u8g2.setBitmapMode(0);
  _u8g2.drawXBMP( 0, 32, _LOGO_KARIBOUS_width, _LOGO_KARIBOUS_height, _LOGO_KARIBOUS_bits);

  // Affichages de la date de compilation
  _u8g2.setFont(u8g2_font_micro_mr);
  _u8g2.drawStr( 5, 118, __DATE__);
  _u8g2.drawStr( 5, 128, __TIME__);

  _u8g2.sendBuffer();
}
void IHM::menuScreen() {
  const int ligneEtat = 0;
  const int ligneStrategie = 64;
  const int colonne0 = 0;
  const int colonne1 = 2;
  const int colonne2 = 35;

  _u8g2.clearBuffer();
  prepare();

  // Affichages des titres :
  _u8g2.setFont(u8g2_font_5x7_mr);

  _u8g2.drawStr( colonne1, ligneEtat,    " -- ETAT --");
  _u8g2.drawStr( colonne1, ligneStrategie,   " -- STRAT --");

  // Affichages des sous-titres :
  _u8g2.setFont(u8g2_font_micro_mr);

  _u8g2.drawStr( colonne1 + 15, ligneEtat+10, "robot ");
  _u8g2.drawStr( colonne1 + 10, ligneEtat+20, "Tirette");
  _u8g2.drawStr( colonne1 + 10, ligneEtat+30, "Balise");
  _u8g2.drawStr( colonne1 + 10, ligneEtat+40, "Recalage");

  _u8g2.drawStr( colonne1, ligneStrategie+10, "Equipe");

  // Etat type de robot :
  _u8g2.setCursor(colonne1,ligneEtat+10);
  if ( _typeRobot == _ROBOT_PRIMAIRE ) _u8g2.print("1st");
  else _u8g2.print("2nd");

  // Symbols :
  _u8g2.setFont(u8g2_font_m2icon_9_tf);
  // Etat tirette :
  if ( _tirette ) _u8g2.drawGlyph(colonne1,ligneEtat+20-1,0x0045);
  else _u8g2.drawGlyph(colonne1,ligneEtat+20-1,0x0046);
  // Etat balise :
  if ( !_detection ) _u8g2.drawGlyph(colonne1,ligneEtat+30-1,0x0045);
  else _u8g2.drawGlyph(colonne1,ligneEtat+30-1,0x0046);
  // Etat recalage :
  if ( !_recalage ) _u8g2.drawGlyph(colonne1,ligneEtat+40-1,0x0045);
  else _u8g2.drawGlyph(colonne1,ligneEtat+40-1,0x0046);



  // Etat equipe :
  /*
  _u8g2.setCursor(colonne2,ligneDebut);
  if ( _equipe == _EQUIPE_JAUNE ) _u8g2.print("jaune");
  else _u8g2.print("bleu");

  // Etat strategie :
  _u8g2.setCursor(colonne2,ligneDebut+40);
  if ( _strategie ) _u8g2.print("homologation");
  else _u8g2.print("match");
  */


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
