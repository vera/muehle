#include "Board.h"

Board::Board(QWidget * parent) : QWidget(parent) {
  AIPlayer aiPlayer;
  HumanPlayer humanPlayer;
  turn = 1;

  // edges = {{1,9}, {0,2,4}, {1,14}, {4,10}, {1,3,5,7}, {4,13}, {7,11}, {0,10,21}, {3,9,11,13}, {6,10,15}, {8,13,17}, {5,12,14,20}, {2,13,23}, {11,16}, {15,17,19}, {12,16}, {16,18,20,22}, {13,19}, {9,22}, {19,21,23}, {14,22}};

  QSignalMapper *signalMapper = new QSignalMapper(this);

  connect(signalMapper, SIGNAL(mapped(int)), this, SLOT(pointSelected(int)));

  for(int i = 0; i < 24; i++) {
    vertices[i] = 0;

    buttons[i] = new QPushButton("", this);

    connect(buttons[i], SIGNAL(clicked()), signalMapper, SLOT(map()));
    signalMapper->setMapping(buttons[i], i);

    buttons[i]->setFixedHeight(50);
    buttons[i]->setFixedWidth(50);
    buttons[i]->setStyleSheet("border: 0px; background-image: url(:/images/empty.png);");
  }

    /* edges[0] = [2,10];
    edges[1] = [1,3,5];
    edges[2] = [2,15];
    edges[3] = [5,11];
    edges[4] = [2,4,6,8];
    edges[5] = [5,14];
    edges[6] = [8,12];
    edges[7] = [5,7,9];
    edges[8] = [8,13];
    edges[9] = [1,11,22];
    edges[10] = [4,10,12,14];
    edges[11] = [7,11,16];
    edges[12] = [9,14,18];
    edges[13] = [6,13,15,21];
    edges[14] = [3,14,24];
    edges[15] = [12,17];
    edges[16] = [16,18,20];
    edges[17] = [13,17];
    edges[18] = [11,20];
    edges[19] = [17,19,21,23];
    edges[20] = [14,20];
    edges[21] = [10,23];
    edges[22] = [20,22,24];
    edges[23] = [15,23]; */

    QGridLayout *boardLayout = new QGridLayout();
    QGridLayout *statusLayout = new QGridLayout();

    // Setup the status layout
    // Displays: status message, current turn

    QLabel * statusTextLabel = new QLabel("Status messages:");
    statusTextLabel->setStyleSheet("QLabel { font-weight: bold; }");
    statusLayout->addWidget(statusTextLabel, 1,0,1,2);

    statusList = new QListWidget;
    statusList->addItem("A new game has started.");
    statusLayout->addWidget(statusList, 2,0,1,2);

    QLabel * turnTextLabel = new QLabel("Current turn:");
    turnTextLabel->setStyleSheet("QLabel { font-weight: bold; }");
    statusLayout->addWidget(turnTextLabel, 0,0);

    turnLabel = new QLabel(QString::number(turn));
    statusLayout->addWidget(turnLabel, 0, 1);

    QSpacerItem *spacer = new QSpacerItem(400,300);
    statusLayout->addItem(spacer, 3, 0,1,2);

   // Setup the board layout

    boardLayout->setSpacing(0);

    // Step 1/4: Add the row and column labels

    for(int i = 7; i > 0; i--) {
      rowLabels[i-1] = new QLabel(QString::number(i));
      rowLabels[i-1]->setStyleSheet("color: grey; text-align: center; padding: 5px;");
      boardLayout->addWidget(rowLabels[i-1], (7-i)*2, 0);
    }

    for(int i = 65; i < 72; i++) {
      char ch = i;
      QString str = QString(ch);
      colLabels[i-65] = new QLabel(str);
      colLabels[i-65]->setAlignment(Qt::AlignCenter);
      colLabels[i-65]->setStyleSheet("color: grey; text-align: center; padding: 5px;");
      boardLayout->addWidget(colLabels[i-65], 13, (i-64)*2-1);
    }

    // Step 2/4: Add the buttons

    boardLayout->addWidget(buttons[0], 0, 1);
    boardLayout->addWidget(buttons[1], 0, 7);
    boardLayout->addWidget(buttons[2], 0, 13);

    boardLayout->addWidget(buttons[3], 2, 3);
    boardLayout->addWidget(buttons[4], 2, 7);
    boardLayout->addWidget(buttons[5], 2, 11);

    boardLayout->addWidget(buttons[6], 4, 5);
    boardLayout->addWidget(buttons[7], 4, 7);
    boardLayout->addWidget(buttons[8], 4, 9);

    boardLayout->addWidget(buttons[9], 6, 1);
    boardLayout->addWidget(buttons[10], 6, 3);
    boardLayout->addWidget(buttons[11], 6, 5);
    boardLayout->addWidget(buttons[12], 6, 9);
    boardLayout->addWidget(buttons[13], 6, 11);
    boardLayout->addWidget(buttons[14], 6, 13);

    boardLayout->addWidget(buttons[15], 8, 5);
    boardLayout->addWidget(buttons[16], 8, 7);
    boardLayout->addWidget(buttons[17], 8, 9);

    boardLayout->addWidget(buttons[18], 10, 3);
    boardLayout->addWidget(buttons[19], 10, 7);
    boardLayout->addWidget(buttons[20], 10, 11);

    boardLayout->addWidget(buttons[21], 12, 1);
    boardLayout->addWidget(buttons[22], 12, 7);
    boardLayout->addWidget(buttons[23], 12, 13);

    // Step 3/4: Add the vertical lines

    QLabel *verticalLine [40];

    for(int i = 0; i < 40; i++) {
      verticalLine[i] = new QLabel();
      verticalLine[i]->setPixmap(QPixmap(":/images/vertical.png"));
    }

    boardLayout->addWidget(verticalLine[0],1,1);
    boardLayout->addWidget(verticalLine[1],1,7);
    boardLayout->addWidget(verticalLine[2],1,13);

    boardLayout->addWidget(verticalLine[3],2,1);
    boardLayout->addWidget(verticalLine[4],2,13);

    boardLayout->addWidget(verticalLine[5],3,1);
    boardLayout->addWidget(verticalLine[6],3,3);
    boardLayout->addWidget(verticalLine[7],3,7);
    boardLayout->addWidget(verticalLine[8],3,11);
    boardLayout->addWidget(verticalLine[9],3,13);

    boardLayout->addWidget(verticalLine[10],4,1);
    boardLayout->addWidget(verticalLine[11],4,3);
    boardLayout->addWidget(verticalLine[12],4,11);
    boardLayout->addWidget(verticalLine[13],4,13);

    boardLayout->addWidget(verticalLine[14],5,1);
    boardLayout->addWidget(verticalLine[15],5,3);
    boardLayout->addWidget(verticalLine[16],5,5);
    boardLayout->addWidget(verticalLine[17],5,9);
    boardLayout->addWidget(verticalLine[18],5,11);
    boardLayout->addWidget(verticalLine[19],5,13);

    boardLayout->addWidget(verticalLine[20],7,1);
    boardLayout->addWidget(verticalLine[21],7,3);
    boardLayout->addWidget(verticalLine[22],7,5);
    boardLayout->addWidget(verticalLine[23],7,9);
    boardLayout->addWidget(verticalLine[24],7,11);
    boardLayout->addWidget(verticalLine[25],7,13);

    boardLayout->addWidget(verticalLine[26],8,1);
    boardLayout->addWidget(verticalLine[27],8,3);
    boardLayout->addWidget(verticalLine[28],8,11);
    boardLayout->addWidget(verticalLine[29],8,13);

    boardLayout->addWidget(verticalLine[30],9,1);
    boardLayout->addWidget(verticalLine[31],9,3);
    boardLayout->addWidget(verticalLine[32],9,7);
    boardLayout->addWidget(verticalLine[33],9,11);
    boardLayout->addWidget(verticalLine[34],9,13);

    boardLayout->addWidget(verticalLine[35],10,1);
    boardLayout->addWidget(verticalLine[36],10,13);

    boardLayout->addWidget(verticalLine[37],11,1);
    boardLayout->addWidget(verticalLine[38],11,7);
    boardLayout->addWidget(verticalLine[39],11,13);

    // Step 4/4: Add the horizontal lines

    QLabel *horizontalLine [40];

    for(int i = 0; i < 40; i++) {
      horizontalLine[i] = new QLabel();
      horizontalLine[i]->setPixmap(QPixmap(":/images/horizontal.png"));
    }

    boardLayout->addWidget(horizontalLine[0],0,2);
    boardLayout->addWidget(horizontalLine[1],0,3);
    boardLayout->addWidget(horizontalLine[2],0,4);
    boardLayout->addWidget(horizontalLine[3],0,5);
    boardLayout->addWidget(horizontalLine[4],0,6);
    boardLayout->addWidget(horizontalLine[5],0,8);
    boardLayout->addWidget(horizontalLine[6],0,9);
    boardLayout->addWidget(horizontalLine[7],0,10);
    boardLayout->addWidget(horizontalLine[8],0,11);
    boardLayout->addWidget(horizontalLine[9],0,12);

    boardLayout->addWidget(horizontalLine[10],2,4);
    boardLayout->addWidget(horizontalLine[11],2,5);
    boardLayout->addWidget(horizontalLine[12],2,6);
    boardLayout->addWidget(horizontalLine[13],2,8);
    boardLayout->addWidget(horizontalLine[14],2,9);
    boardLayout->addWidget(horizontalLine[15],2,10);

    boardLayout->addWidget(horizontalLine[16],4,6);
    boardLayout->addWidget(horizontalLine[17],4,8);

    boardLayout->addWidget(horizontalLine[18],6,2);
    boardLayout->addWidget(horizontalLine[19],6,4);
    boardLayout->addWidget(horizontalLine[20],6,10);
    boardLayout->addWidget(horizontalLine[21],6,12);

    boardLayout->addWidget(horizontalLine[22],8,6);
    boardLayout->addWidget(horizontalLine[23],8,8);

    boardLayout->addWidget(horizontalLine[24],10,4);
    boardLayout->addWidget(horizontalLine[25],10,5);
    boardLayout->addWidget(horizontalLine[26],10,6);
    boardLayout->addWidget(horizontalLine[27],10,8);
    boardLayout->addWidget(horizontalLine[28],10,9);
    boardLayout->addWidget(horizontalLine[29],10,10);

    boardLayout->addWidget(horizontalLine[30],12,2);
    boardLayout->addWidget(horizontalLine[31],12,3);
    boardLayout->addWidget(horizontalLine[32],12,4);
    boardLayout->addWidget(horizontalLine[33],12,5);
    boardLayout->addWidget(horizontalLine[34],12,6);
    boardLayout->addWidget(horizontalLine[35],12,8);
    boardLayout->addWidget(horizontalLine[36],12,9);
    boardLayout->addWidget(horizontalLine[37],12,10);
    boardLayout->addWidget(horizontalLine[38],12,11);
    boardLayout->addWidget(horizontalLine[39],12,12);

    // Setup the main layout

    QHBoxLayout *mainLayout = new QHBoxLayout();

    QSpacerItem *spacer2 = new QSpacerItem(50,100);

    mainLayout->addLayout(boardLayout);
    mainLayout->addSpacerItem(spacer2);
    mainLayout->addLayout(statusLayout);

    mainLayout->setSizeConstraint(QLayout::SetFixedSize);

    setLayout(mainLayout);

    setWindowTitle("Mühle");
}

void Board::updateTurnLabel(QString str)
{
  turnLabel->setText(str);
}

void Board::incTurn() {
  turn++;
  updateTurnLabel(QString::number(turn));
}

void Board::updateStatusLabel(QString str)
{
  statusList->insertItem(0, str);
}

void Board::pointSelected(int pos) {
  switch(muehleDetected[0])
  {
  // No Muehle is detected
  case 0:
    {
      try
      {
        addPiece(pos, &humanPlayer);
      }
      catch(const exception & e)
      {
        QMessageBox messageBox;
        messageBox.critical(0,"Error",e.what());
        messageBox.setFixedSize(500,200);
        return;
      }

      if(muehleDetected[0] == 1)
      {
        // If the player places a Muehle, the computer does not get to place a piece and the turn ends
        return;
      }

      aiTurn();
    } break;

  // A Muehle is detected
  case 1:
    {
      try
      {
        removePiece(pos, &aiPlayer);
      }
      catch(const exception & e) {
        QMessageBox messageBox;
        messageBox.critical(0,"Error",e.what());
        messageBox.setFixedSize(500,200);
        return;
      }

      aiTurn();
    } break;
  }

  updateStatusLabel("----\nTurn " + QString::number(turn)+ " completed.");
  incTurn();

  if(humanPlayer.isOutOfPieces() || aiPlayer.isOutOfPieces())
  {
    updateStatusLabel("Someone is out of pieces.");
  }
}

void Board::aiTurn() {
  int aiPos = aiPlayer.askPlacePosition(vertices, possibleMuehlePositions);

  try
  {
    /*//debug
    QMessageBox messageBox;
    messageBox.critical(0,"Error",QString::number(aiPos));
    messageBox.setFixedSize(500,200);
    //enddebug*/
    addPiece(aiPos, &aiPlayer);
  }
  catch(const exception & e) {
    QMessageBox messageBox;
    messageBox.critical(0,"Error",e.what());
    messageBox.setFixedSize(500,200);
  }

  if(muehleDetected[0] == 2)
  {
    aiPos = aiPlayer.askRemovePosition(vertices, possibleMuehlePositions, protectedPoints);
    // AI removes one of the players pieces
    removePiece(aiPos, &humanPlayer);
  }
}

void Board::addPiece(int pos, Player * player) {
  if(vertices[pos] != 0) throw VertixNotEmptyException();
  if(player->isOutOfPieces()) throw OutOfPiecesException();

  if(player->getID() == 1) {
    buttons[pos]->setStyleSheet("border: 0px; background-image: url(:/images/blue_stone.png);");
    updateStatusLabel("You have placed a piece. You have " + QString::number(player->getPieces()) + " piece(s) left.");
  } else if(player->getID() == 2) {
    buttons[pos]->setStyleSheet("border: 0px; background-image: url(:/images/red_stone.png);");
    updateStatusLabel("The computer has placed a piece.");
  }

  vertices[pos] = player->getID();

  player->movePieceToBoard(pos);

  detectMuehle();
}

void Board::removePiece(int pos, Player * player)
{
  // Check if piece can be removed (is not protected)
  if(player->hasUnprotectedPiecesOnBoard(protectedPoints) && std::find(protectedPoints.begin(), protectedPoints.end(), pos) != protectedPoints.end())
  {
    throw IllegalRemoveException();
  }

  if(vertices[pos] != player->getID())
  {
    throw IllegalRemoveException();
  }

  player->decPiecesOnBoard(pos);
  vertices[pos] = 0;
  buttons[pos]->setStyleSheet("border: 0px; background-image: url(:/images/empty.png);");

  switch(player->getID())
  {
  case 1:
    updateStatusLabel("The computer has removed a piece.");
    break;
  case 2:
    updateStatusLabel("You have removed a piece.");
    break;
  }

  // Update muehleDetected
  detectMuehle();
}

bool Board::isConnected(int pos, int pos2) {
  return (std::find(std::begin(edges[pos]), std::end(edges[pos]), pos2) != std::end(edges[pos]));
}

void Board::movePiece(int pos, int pos2, int player) {

  if(vertices[pos] == player && vertices[pos2] == 0 && isConnected(pos, pos2)) {
    vertices[pos] = 0;
    vertices[pos2] = player;
  } else {
    throw IllegalMoveException();
  }
}

void Board::movePieceFreely(int pos, int pos2, int player) {

  if(vertices[pos] == player && vertices[pos2] == 0) {
    vertices[pos] = 0;
    vertices[pos2] = player;
  } else {
    throw IllegalMoveException();
  }
}

void Board::detectMuehle()
{
  int p = 0;

  for(int i = 0; i < 20; i++)
  {
    // Check if the points of a possible Muehle position are all occupied by a single player's pieces ([0] - [2]) and the Muehle hasn't been detected before ([3])
    if(vertices[possibleMuehlePositions[i][0]] != 0 && vertices[possibleMuehlePositions[i][0]] == vertices[possibleMuehlePositions[i][1]] && vertices[possibleMuehlePositions[i][1]] == vertices[possibleMuehlePositions[i][2]] && possibleMuehlePositions[i][3] == 0)
    {
      // Set Muehle as detected
      possibleMuehlePositions[i][3] = 1;

      // Add points involved in Muehle to protectedPoints
      protectedPoints.push_back(possibleMuehlePositions[i][0]);
      protectedPoints.push_back(possibleMuehlePositions[i][1]);
      protectedPoints.push_back(possibleMuehlePositions[i][2]);

      // Check: Does the opposing player have >3 pieces, are any non-protected

      // Set muehleDetected to which player's pieces created the Muehle ([0]) and at which position ([1])
      p = vertices[possibleMuehlePositions[i][0]];
    }
  }

  muehleDetected[0] = p;
  muehleDetected[1] = 0; // not used so I just set it to 0, may remove at some point

  switch(p)
  {
  case 1:
    updateStatusLabel("You have formed a mill and may remove a piece.");
    break;
  case 2:
    updateStatusLabel("The computer has formed a mill and may remove a piece.");
    break;
  }

  // If no Muehle was detected, set muehleDetected to 0
  //muehleDetected[0] = 0;
}
