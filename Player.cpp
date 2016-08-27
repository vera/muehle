#include "Player.h"

/*!
 *  Constructors
 */

Player::Player(int id) {
  pieces = 9;
  this->id = id;
}

/*!
 *  Getters
 */

int Player::getID() {
  return id;
}

int Player::getPieces()
{
  return pieces;
}

int Player::getPiecesOnBoard() {
  return piecesOnBoard.size();
}

vector<int> Player::getPiecesOnBoardVector() {
  return piecesOnBoard;
}

/*!
 *  Setters
 */

void Player::incPieces() {
  pieces++;
}

/*!
 *  Update methods
 */

void Player::removePieceFromBoard(int pos) {
  if(getPiecesOnBoard() > 0)
  {
    piecesOnBoard.erase(std::find(piecesOnBoard.begin(), piecesOnBoard.end(), pos));
  }
  else
  {
    throw OutOfPiecesException();
  }
}

void Player::movePieceOnBoard(int pos1, int pos2)
{
  removePieceFromBoard(pos1);
  pieces++;
  movePieceToBoard(pos2);
}

void Player::movePieceToBoard(int pos) {
  if(pieces > 0)
  {
    pieces--;
    piecesOnBoard.push_back(pos);
  }
  else
  {
    throw OutOfPiecesException();
  }
}

/*!
 *  Logic methods
 */

bool Player::isOutOfPieces() {
  return pieces == 0;
}

bool Player::hasUnprotectedPiecesOnBoard(vector<int> protectedPoints) {
  for(vector<int>::iterator it = piecesOnBoard.begin(); it != piecesOnBoard.end(); ++it)
  {
    if(std::find(protectedPoints.begin(), protectedPoints.end(), *it) == protectedPoints.end()) return true;
  }
  return false;
}

/*!
 *  Reset method
 */

void Player::reset()
{
  piecesOnBoard.clear();
  pieces = 9;
}
