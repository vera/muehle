#ifndef _AIPLAYER_H
#define _AIPLAYER_H

// Define player ID of the computer, default is 2
#define AI_PLAYER_ID 2

#include "Player.h"
#include "HumanPlayer.h"
#include "Exceptions.h"

#include <vector>
#include <ctime>
#include <array>

class AIPlayer : public Player {
  std::array<int, 24> vertices;
  std::array<std::array<int, 3>, 16> possibleMillPositions;
  std::array<std::array<int, 2>, 32> edges;

  vector<int> onePieceLeft, twoPiecesLeft, impossible;
  vector<int> onePieceLeftHuman, twoPiecesLeftHuman, impossibleHuman;

  public:
    int getID();

    AIPlayer();

    AIPlayer(std::array<int, 24> vertices, std::array<std::array<int, 3>, 16> possibleMillPositions, std::array<std::array<int, 2>, 32> edges);

    void updateHumanVectors(int pos1, int pos2);

    void updateAIVectors(int pos1, int pos2);

    int askPlacePosition();

    int askRemovePosition(vector<int> protectedPoints);

    std::pair<int,int> askMovePositions();

    int canBeReached(int pos2, std::array<std::array<int, 2>, 32> edges, std::array<int, 24> vertices);

    std::pair<int,int> askFreeMovePositions();

    void reset();
};

#endif // _AIPLAYER_H
