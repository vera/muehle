#ifndef _PLAYER_h
#define _PLAYER_h
#include <iostream>
#include <vector>
#include <algorithm>
#include "Exceptions.h"

using namespace std;

class Player {
  protected:
    int id, pieces;
    vector<int> piecesOnBoard;

  public:
    Player(int id);
    int getID();
    int getPieces();
    bool isOutOfPieces();
    void movePieceToBoard(int pos);
    void incPieces();
    void removePieceFromBoard(int pos);
    void movePieceOnBoard(int pos1, int pos2);
    int getPiecesOnBoard();
    vector<int> getPiecesOnBoardVector();
    bool hasUnprotectedPiecesOnBoard(vector<int> protectedPieces);
    void reset();
};

#endif // _PLAYER_h
