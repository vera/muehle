#ifndef BOARD_H
#define BOARD_H

#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <typeinfo>

#include <unistd.h>

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
#include <QApplication>

#include "Player.h"
#include "HumanPlayer.cpp"
#include "AIPlayer.cpp"

using namespace std;

class Board : public QWidget {
  Q_OBJECT

  int edges [32][2] = {
    {0,1},
    {0,9},
    {1,2},
    {1,4},
    {2,14},
    {3,10},
    {3,4},
    {4,5},
    {4,7},
    {5,13},
    {6,7},
    {6,11},
    {7,8},
    {8,12},
    {9,10},
    {9,21},
    {10,11},
    {10,18},
    {11,15},
    {12,13},
    {12,17},
    {13,14},
    {13,20},
    {14,23},
    {15,16},
    {16,17},
    {16,19},
    {18,19},
    {19,20},
    {19,22},
    {21,22},
    {22,23}
  };

  int vertices [24];

  vector<int> protectedPoints;

  QPushButton * buttons[24];
  QLabel * rowLabels[7];
  QLabel * colLabels[7];
  QLabel * turnLabel;
  QLabel * gamePhaseLabel;
  QListWidget * statusList;

  HumanPlayer humanPlayer;
  AIPlayer aiPlayer;

  int turn;
  int gamePhase;
  int millDetected;

  int possibleMillPositions [16][3] = {
      {0,1,2},
      {0,9,21},
      {1,4,7},
      {2,14,23},
      {3,4,5},
      {3,10,18},
      {5,13,20},
      {6,7,8},
      {6,11,15},
      {8,12,17},
      {9,10,11},
      {12,13,14},
      {15,16,17},
      {16,19,22},
      {18,19,20},
      {21,22,23}
    };

public slots:
  void pointSelected(int pos);

public:
  Board(QWidget * parent = 0);

  void updateTurnLabel(QString str);
  void updateStatusLabel(QString str);
  void incTurn();
  void updateGamePhaseLabel(QString str);
  void incGamePhase();

  void setPlaceHoverStylesheet();
  void setMoveHoverStylesheet();
  void setRemoveHoverStylesheet();

  void addPiece(int pos, Player * player);

  bool isConnected(int pos1, int pos2);

  void movePiece(int pos1, int pos2, Player * player);

  void movePieceFreely(int pos1, int pos2, Player  * player);

  void removePiece(int pos, Player * player);

  void detectMill(int pos);

  void aiTurn();
};


#endif // BOARD_H
