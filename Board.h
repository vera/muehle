#ifndef BOARD_H
#define BOARD_H

#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <typeinfo>

#include "Exceptions.h"
#include <QPushButton>
#include <QLabel>
#include <QLayout>
#include <QGridLayout>
#include <QSpacerItem>
#include <QPixmap>
#include <QSignalMapper>
#include <QMessageBox>
#include <QScrollArea>
#include <QListWidget>

#include "Player.h"
#include "HumanPlayer.cpp"
#include "AIPlayer.cpp"

using namespace std;

class Board : public QWidget {
  Q_OBJECT

  // vector<int> edges [24];
  int edges [24][4] = {
    {1,9},
    {0,2,4},
    {1,14},
    {4,10},
    {1,3,5,7},
    {4,13},
    {7,11},
    {0,10,21},
    {3,9,11,13},
    {6,10,15},
    {8,13,17},
    {5,12,14,20},
    {2,13,23},
    {11,16},
    {15,17,19},
    {12,16},
    {16,18,20,22},
    {13,19},
    {9,22},
    {19,21,23},
    {14,22}
  };

  int vertices [24];

  vector<int> protectedPoints;

  QPushButton * buttons[24];
  QLabel * rowLabels[7];
  QLabel * colLabels[7];
  QLabel * turnLabel;
  QListWidget * statusList;

  HumanPlayer humanPlayer;
  AIPlayer aiPlayer;

  int turn;
  int muehleDetected [2];

  int possibleMuehlePositions [20][4] = {
      {0,1,2,0},
      {0,9,21,0},
      {0,3,6,0},
      {1,4,7,0},
      {2,5,8,0},
      {2,14,23,0},
      {3,4,5,0},
      {3,10,18,0},
      {5,13,20,0},
      {6,7,8,0},
      {6,11,15,0},
      {8,12,17,0},
      {9,10,11,0},
      {12,13,14,0},
      {15,18,21,0},
      {15,16,17,0},
      {16,19,22,0},
      {17,20,23,0},
      {18,19,20,0},
      {21,22,23,0}
    };

public slots:
  void pointSelected(int pos);

public:
  Board(QWidget * parent = 0);

  void updateTurnLabel(QString str);
  void updateStatusLabel(QString str);
  void incTurn();

  void addPiece(int pos, Player * player);

  bool isConnected(int pos, int pos2);

  void movePiece(int pos, int pos2, int player);

  void movePieceFreely(int pos, int pos2, int player);

  void removePiece(int pos, Player * player);

  void detectMuehle();

  void aiTurn();
};


#endif // BOARD_H
