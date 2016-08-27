#include "HumanPlayer.h"

HumanPlayer::HumanPlayer() : Player(HUMAN_PLAYER_ID) { }

int HumanPlayer::getID() {
  return HUMAN_PLAYER_ID;
}
