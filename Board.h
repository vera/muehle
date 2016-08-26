#ifndef BOARD_H
#define BOARD_H

#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <typeinfo>
#include <array>

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

  std::array<std::array<int, 2>, 32> edges = {{
    {{0,1}},
    {{0,9}},
    {{1,2}},
    {{1,4}},
    {{2,14}},
    {{3,10}},
    {{3,4}},
    {{4,5}},
    {{4,7}},
    {{5,13}},
    {{6,7}},
    {{6,11}},
    {{7,8}},
    {{8,12}},
    {{9,10}},
    {{9,21}},
    {{10,11}},
    {{10,18}},
    {{11,15}},
    {{12,13}},
    {{12,17}},
    {{13,14}},
    {{13,20}},
    {{14,23}},
    {{15,16}},
    {{16,17}},
    {{16,19}},
    {{18,19}},
    {{19,20}},
    {{19,22}},
    {{21,22}},
    {{22,23}}
  }};

  std::array<int, 24> vertices;

  vector<int> protectedPoints;

  QPushButton * buttons[24];
  QLabel * rowLabels[7];
  QLabel * colLabels[7];
  QLabel * turnLabel;
  QLabel * gamePhaseLabel;
  QLabel * gameRulesLabel;
  QListWidget * statusList;

  HumanPlayer humanPlayer;
  AIPlayer aiPlayer;

  int turn;
  int gamePhase;
  int millDetected;

  std::array<std::array<int, 3>, 16> possibleMillPositions = {{
      {{0,1,2}},
      {{0,9,21}},
      {{1,4,7}},
      {{2,14,23}},
      {{3,4,5}},
      {{3,10,18}},
      {{5,13,20}},
      {{6,7,8}},
      {{6,11,15}},
      {{8,12,17}},
      {{9,10,11}},
      {{12,13,14}},
      {{15,16,17}},
      {{16,19,22}},
      {{18,19,20}},
      {{21,22,23}}
    }};

  QString gameRules [3] = {
    {"The game has begun!\nYou have blue pieces, the computer has red pieces. You take turns placing pieces on the intersections of the lines on the board.\n\nYour goal is to form a mill (= three pieces in a horizontal or vertical line). If you have formed a mill, you can remove one of the computer's pieces off the board.\n\nThis phase ends when you have placed all of your nine pieces."},
    {"Phase 2 has begun!\nYou can now move your pieces to connected free points on the board.\n\nYour goal is to form a mill (= three pieces in a horizontal or vertical line). If you have formed a mill, you can remove one of the computer's pieces off the board.\n\nThis phase ends when you have three pieces left on the board."},
    {"Phase 3 has begun!\nYou can now move your pieces freely to any free points on the board.\n\nYour goal is to form a mill (= three pieces in a horizontal or vertical line). If you have formed a mill, you can remove one of the computer's pieces off the board.\n\nThe game ends when one of the players has no more legal moves or less than three pieces left on the board."}
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
  void updateGameRulesLabel(QString str);

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

  void endGame(Player * losingPlayer);
};


#endif // BOARD_H
