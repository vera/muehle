// Define player ID of the human player, default is 1
#define HUMAN_PLAYER_ID 1

#include <iostream>
#include "Player.h"
#include "Exceptions.h"

using namespace std;

class HumanPlayer : public Player {

  public:
  static constexpr int getID() { return HUMAN_PLAYER_ID; }

  HumanPlayer() : Player(HUMAN_PLAYER_ID) {

  }

};
