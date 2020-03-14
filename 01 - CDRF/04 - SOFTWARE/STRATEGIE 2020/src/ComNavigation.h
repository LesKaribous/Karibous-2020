#ifndef DEF_COMNAVIGATION
#define DEF_COMNAVIGATION

#include <Arduino.h>
#include <Wire.h>
#include <FastCRC.h>

class ComNavigation
{
  public:
    ComNavigation();
    ~ComNavigation();

    void setEquipe(bool equipe);
    void setDetection(bool detection);
    int getNbrBadCRC();

    //DEMANDE L'ETAT DU DEPLACEMENT
    int askNavigation();
    //ENVOI UNE COMMANDE TURN GO
    void turnGo(bool adversaire, bool recalage,bool ralentit,int turn, int go);
    //ENVOI UNE COMMANDE DE DEPLACEMENT ABSOLU
    void sendNavigation(byte fonction, int X, int Y, int rot);
    //ENVOI UNE COMMANDE DE DEPLACEMENT RELATIF
    void sendNavigation(byte fonction, int rot, int dist);


  private:
    // Adressage I2C pour les cartes esclaves
    const byte _CARTE_DEPLACEMENT = 60;
    //Etat de la position demandée
    const byte _TERMINEE = 0;  // Position validée et terminée
    const byte _RECU     = 1;  // Position reçu
    const byte _ERRONEE  = 2;  // Position erronée. CRC nok.
    const byte _BIZARRE  = 3;  // Reponse étrange à ne pas prendre en compte
    // Couleur Equipe
    const bool _EQUIPE_JAUNE = 1 ;
    const bool _EQUIPE_VIOLET = 0 ;

    bool _equipe = _EQUIPE_VIOLET, _detection = false ;

    int _nbrBadCRC = 0   ; // Nombre de CRC érronés
    int _nbrBizarre = 0  ; // Nombre de réponses bizarres

    FastCRC8 _CRC8;

    byte _bufNavRelatif[5]={0,0,0,0,0}; // Buffer d'envoi des ordres de navigation relatifs
    byte _crcNavRelatif = 0; // CRC de controle pour les ordres de navigation relatifs

    byte _bufNavAbsolu[6]={0,0,0,0,0,0}; // Buffer d'envoi des ordres de navigation absolus
    byte _crcNavAbsolu = 0 ; // CRC de controle pour les ordres de navigation absolus

};

#endif
