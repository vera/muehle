#ifndef _BOARD_H
#define _BOARD_H

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
#include "HumanPlayer.h"
#include "AIPlayer.h"

using namespace std;

class Board : public QWidget {
  Q_OBJECT

  /*!
   *  Model of the game board
   *  The game board is represented as a graph structure consisting of:
   *  - Vertices (the intersecting points on the board, numbered from 0 to 23, going in rows from top to bottom and left to right)
   *  - Edges (the connections between the points)
   *
   *  For example, the array {0,1} in edges means that the points 0 and 1 are connected
   *
   *  An entry in the array vertices can be:
   *  - 0: The point on the board is empty
   *  - 1: The point on the board is occupied by one of the human player's pieces
   *  - 2: The point on the board is occupied by one of the AI player's pieces
   */

  std::array<int, 24> vertices = {{0}};

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

  /*!
   *  Model of the mills
   *  possibleMillPositions stores all combinations of three points on the board that can form a mill
   *
   *  This array is used to detect mills during the game
   *
   *  For example, the array {0,1,2} in possibleMillPositions means that the points 0, 1 and 2 form a mill
   */

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

  /*!
   *  The vector protectedPoints saves all points that are involved in mills and therefore cannot be removed
   */

  vector<int> protectedPoints;

  /*!
   *  UI elements
   */

  QPushButton * buttons[24];
  QLabel * rowLabels[7];
  QLabel * colLabels[7];
  QLabel * turnLabel;
  QLabel * gameRulesLabel;
  QLabel * gamesWonLabel;
  QLabel * gamesLostLabel;
  QListWidget * statusList;

  QString gameRules [3] = {
    {"The game has begun!\nYou have blue pieces, the computer has red pieces. You take turns placing pieces on the intersections of the lines on the board.\n\nYour goal is to form a mill (= three pieces in a horizontal or vertical line). If you have formed a mill, you can remove one of the computer's pieces off the board.\n\nThis phase ends when you have placed all of your nine pieces."},
    {"Phase 2 has begun!\nYou can now move your pieces to connected free points on the board.\n\nYour goal is to form a mill (= three pieces in a horizontal or vertical line). If you have formed a mill, you can remove one of the computer's pieces off the board.\n\nThis phase ends when you have three pieces left on the board."},
    {"Phase 3 has begun!\nYou can now move your pieces freely to any free points on the board.\n\nYour goal is to form a mill (= three pieces in a horizontal or vertical line). If you have formed a mill, you can remove one of the computer's pieces off the board.\n\nThe game ends when one of the players has no more legal moves or less than three pieces left on the board."}
  };

  /*!
   *  The players
   */

  HumanPlayer * humanPlayer;
  AIPlayer * aiPlayer;

  /*!
   *  Variables that capture the game state
   *
   *  turn:          The current turn
   *  gamePhase:     The current game phase (1, 2 or 3)
   *  millDetected:
   *                 - 0 if no mill is currently detected
   *                 - 1 if a mill formed by the human player's pieces is currently detected
   *                 - 2 if a mill formed by the AI player's pieces is currently detected
   *  gamesWon:      The amount of games the human player has won
   *  gamesLost:     The amount of games the human player has lost
   */

  int turn;
  int gamePhase;
  int millDetected;
  int gamesWon, gamesLost;

public slots:
  /*!
   *  This method is connected to the intersecting points on the game board (the points where game pieces can be placed)
   *  It contains the logic for what happens when the user clicks on one of the points
   *
   *  If a mill is detected, the human player will be allowed to remove one of the AI player's pieces
   *  If no mill is detected and the game phase is 1, the player will be allowed to place a piece
   *                                               2, the player will be allowed to move a piece
   *                                               3, the player will be allowed to move a piece freely
   */

  void pointSelected(int pos);

  /*!
   *  Resets the game board
   *  This method is connected to the "Restart game" button
   */

  void resetGame();

public:
  // Constructor
  Board(QWidget * parent = 0);

  // Setters
  void incGamePhase();
  void incTurn();

  /*!
   *  Set stylesheet methods
   *  The stylesheets create the hover effects and make the UI feel more responsive
   */

  void setPlaceHoverStylesheet();
  void setMoveHoverStylesheet();
  void setRemoveHoverStylesheet();

  /*!
   *  UI update methods
   *  These methods are called when any of the game state variables change (for example the game phase)
   */

  void updateTurnLabel(QString str);
  void updateStatusLabel(QString str);
  void updateGamePhaseLabel(QString str);
  void updateGameRulesLabel(QString str);
  void updateGamesWonLabel(QString str);
  void updateGamesLostLabel(QString str);

  /*!
   *  Turn methods
   *
   *  Updates the vertices array, calls the appropriate player update method, updates the UI, creates a status message
   *  These methods are called when placing, removing or moving a piece
   *
   *  Parameters: pos: The position where a piece was just placed/removed
   *              pos1: The position where a piece was just moved from
   *              pos2: The position where a piece was just moved to
   *              player: The player that just placed/removed/moved a piece
   */

  void addPiece(int pos, Player * player);
  void removePiece(int pos, Player * player);
  void movePiece(int pos1, int pos2, Player * player);
  void movePieceFreely(int pos1, int pos2, Player  * player);

  /*!
   *  Contains the logic for the AI's turn
   *  Calls the AI's logic methods that determine its next turn and then calls the turn method (for example askPlacePosition and then addPiece)
   */

  void aiTurn();

  /*!
   *  Checks if two positions on the board are connected (if an edge exists between them)
   *
   *  Parameters: pos1: A position on the board
   *              pos2: Another position on the board
   *
   *  Returns: True if pos1 and pos2 are connected, false if not
   */

  bool isConnected(int pos1, int pos2);

  /*!
   *  Checks if a player has legal moves left
   *  This method is called to determine whether the game has ended
   *
   *  Parameters: player: The player that is being checked
   */

  bool hasLegalMove(Player * player);

  /*!
   *  Checks if a mill has been formed
   *  If yes, updates the millDetected variable
   *  This method is called everytime a piece is placed
   *
   *  Parameters: pos: The position where a piece was just placed
   */

  void detectMill(int pos);

  /*!
   *  Ends the game
   *  Creates a status message, updates the gamesWon/gamesLost variable, disables all buttons on the board
   *
   *  Parameters: losingPlayer: The player that has lost the game
   */

  void endGame(Player * losingPlayer);
};


#endif // _BOARD_H
