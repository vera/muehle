#include "HumanPlayer.h"

// Constructor
HumanPlayer::HumanPlayer() : Player(HUMAN_PLAYER_ID) { }

// Getter
int HumanPlayer::getID() {
  return HUMAN_PLAYER_ID;
}
