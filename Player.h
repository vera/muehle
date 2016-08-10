#ifndef _PLAYER_h
#define _PLAYER_h
#include <vector>
#include <algorithm>
#include "Exceptions.h"

using namespace std;

class Player {
  int id, pieces;
  vector<int> piecesOnBoard;

  public:
    Player(int id);
    int getID();
    int getPieces();
    bool isOutOfPieces();
    void movePieceToBoard(int pos);
    void incPieces();
    void decPiecesOnBoard(int pos);
    int getPiecesOnBoard();
    vector<int> getPiecesOnBoardVector();
    bool hasUnprotectedPiecesOnBoard(vector<int> protectedPieces);
};

#endif
