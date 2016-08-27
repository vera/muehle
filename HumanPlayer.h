#ifndef _HUMANPLAYER_H
#define _HUMANPLAYER_H

// Define player ID of the human player, default is 1
#define HUMAN_PLAYER_ID 1

#include <iostream>
#include "Player.h"
#include "Exceptions.h"

using namespace std;

class HumanPlayer : public Player {

  public:
  // Constructor
  HumanPlayer();

  // ID getter
  int getID();
};

#endif // _HUMANPLAYER_H
