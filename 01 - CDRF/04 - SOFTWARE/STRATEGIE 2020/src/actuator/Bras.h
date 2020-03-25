#ifndef DEF_BRAS
#define DEF_BRAS

#include <Arduino.h>
#include <Servo.h>

class Bras
{
    public:

    Bras();
    ~Bras();
    void setPin(int pinServoBras, int pinServoVentouse, int pinPompe, int pinEv);

    void setLimit(int minServoBras, int maxServoBras, int minServoVentouse, int maxServoVentouse);

    int calcPositionBras(byte posServoBras);
    int calcPositionVentouse(byte posServoVentouse);

    void setPosition(byte posServoBras, byte posServoVentouse, bool pompe, bool ev, int wait);

    void setBras(byte posServoBras);
    void setBras(byte posServoBras, int wait);

    void setVentouse(byte posServoVentouse);
    void setVentouse(byte posServoVentouse, int wait);

    void setPompe(bool state);
    void setPompe(bool state, int wait);
    void setEV(bool state);
    void setEV(bool state, int wait);

    private:

    Servo _servoBras ;
    Servo _servoVentouse ;

    int _pinServoBras;
    int _pinServoVentouse;

    int _pinPompe ;
    int _pinEv ;

    int _minServoBras ;
    int _maxServoBras ;
    int _minServoVentouse ;
    int _maxServoVentouse ;


};

#endif