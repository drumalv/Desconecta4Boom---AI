#ifndef PLAYER_H
#define PLAYER_H

#include "environment.h"

class Player{
    public:
      Player(int jug);
      Environment::ActionType Think();
      void Perceive(const Environment &env);
      double Poda_AlfaBeta(Environment actual_, int jugador_, int PROFUNDIDAD_ALFABETA, int profundidad ,Environment::ActionType &accion,double alfa, double beta);

    private:
      int jugador_;
      Environment actual_;
};
#endif
