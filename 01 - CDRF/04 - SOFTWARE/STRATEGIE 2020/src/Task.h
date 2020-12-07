#ifndef DEF_TASK
#define DEF_TASK

#include <Arduino.h>

class Task
{
  public:
    Task();
    ~Task();

  private:
    // Type de tache
    typedef enum {
      TURN_GO,
      GOTO,
      PARAMETRE,
      ACTION,
      ATTENTE
    } TaskType ;

    // Etat de la tache
    typedef enum {
      PLANNED,  // Planifiée
      ONGOING,  // En cours
      PENDING,  // En attente
      ABORDED,  // Annulée
      DONE,     // Terminée
    } TaskState ;

    typedef struct
    {
      TaskType type;
      TaskState state;
      union
      {
          struct
          {               // Cas d'un Turn Go
              int rot;
              int dist;
          };
          struct
          {               // Cas d'un Go To
              int X;
              int Y;
              int alpha;
          };
          struct
          {               // Cas d'un Parametre
              bool detection;
              bool recalibration;
              bool speed;
          };
          struct
          {               // Cas d'une action
              byte posServoBras;
              byte posServoVentouse;
              bool pompe;
              bool ev;
              int wait;
          };
          struct
          {               // Cas d'une attente
              int pauseTime;
          };
          /* etc. */
      };
    } task ;

};
#endif
