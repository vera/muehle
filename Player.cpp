#include "Player.h"

Player::Player(int id) {
  pieces = 9;
  this->id = id;
}

int Player::getID() {
  return id;
}

int Player::getPieces()
{
  return pieces;
}

bool Player::isOutOfPieces() {
  return pieces == 0;
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

void Player::incPieces() {
  pieces++;
}

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

int Player::getPiecesOnBoard() {
  return piecesOnBoard.size();
}

vector<int> Player::getPiecesOnBoardVector() {
  return piecesOnBoard;
}

bool Player::hasUnprotectedPiecesOnBoard(vector<int> protectedPoints) {
  for(vector<int>::iterator it = piecesOnBoard.begin(); it != piecesOnBoard.end(); ++it)
  {
    if(std::find(protectedPoints.begin(), protectedPoints.end(), *it) == protectedPoints.end()) return true;
  }
  return false;
}

void Player::reset()
{
  piecesOnBoard.clear();
  pieces = 9;
}
