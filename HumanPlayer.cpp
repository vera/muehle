#include <iostream>
#include "Player.h"
#include "Exceptions.h"

using namespace std;

class HumanPlayer : public Player {

  public:
  HumanPlayer() : Player(1) {

  }

};
