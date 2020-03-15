
                                                                                   
/* ,--.                     ,--. ,--.               ,--.,--.                          
*  |  |    ,---.  ,---.     |  .'   / ,--,--.,--.--.`--'|  |-.  ,---. ,--.,--. ,---.  
*  |  |   | .-. :(  .-'     |  .   ' ' ,-.  ||  .--',--.| .-. '| .-. ||  ||  |(  .-'  
*  |  '--.\   --..-'  `)    |  |\   \\ '-'  ||  |   |  || `-' |' '-' ''  ''  '.-'  `) 
*  `-----' `----'`----'     `--' '--' `--`--'`--'   `--' `---'  `---'  `----' `----'  
*
*   author : Adrien Bracq, Thomas Duvinage, Yann Paulmier, Jules Topart
*   organization : Les Karibous
*   license : void
*   description : Stratedy firmware for CDFR 2020
*/                                                            
                                                                        
#include "strategy.h"

void setup(){
  Serial.begin(9600);       //Demarrage d'un liaison série pour le debug
  Wire.begin();             //Demarrage de la liaison I2C

  delay(500);               //Pause de demarrage avant lancement

  interface.splashScreen(); //Affichage du Logo des Karibous
  initActionneur();         //Initialisation des actionneurs

  delay(1000);              //Attente affichage logo

  prepareMatch();
  waitLaunch();             //Attendre le début du match
}


void loop()
{
    updateClock();
    processBlocks();
}