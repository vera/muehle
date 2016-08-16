#include "Board.h"

Board::Board(QWidget * parent) : QWidget(parent) {
  AIPlayer aiPlayer;
  HumanPlayer humanPlayer;
  turn = 1;
  gamePhase = 1;

  QSignalMapper *signalMapper = new QSignalMapper(this);

  connect(signalMapper, SIGNAL(mapped(int)), this, SLOT(pointSelected(int)));

  for(int i = 0; i < 24; i++)
  {
    vertices[i] = 0;

    buttons[i] = new QPushButton("", this);

    connect(buttons[i], SIGNAL(clicked()), signalMapper, SLOT(map()));
    signalMapper->setMapping(buttons[i], i);

    buttons[i]->setFixedHeight(50);
    buttons[i]->setFixedWidth(50);
    buttons[i]->setObjectName("empty");
  }

  qApp->setStyleSheet("QLabel#boldLabel { font-weight: bold; } QPushButton#empty { border: 0px; background-image: url(:/images/empty.png); } QPushButton#player1 { border: 0px; background-image: url(:/images/blue_stone.png); } QPushButton#player2 { border: 0px; background-image: url(:/images/red_stone.png); } QPushButton#empty:hover { border: 0px; background-image: url(:/images/blue_stone_selected.png); }");

  QGridLayout *boardLayout = new QGridLayout();
  QGridLayout *statusLayout = new QGridLayout();

  // Setup the status layout
  // Displays: status message, current turn, current game phase

  QLabel * statusTextLabel = new QLabel("Status messages:");
  statusTextLabel->setObjectName("boldLabel");
  statusLayout->addWidget(statusTextLabel, 2,0,1,2);

  statusList = new QListWidget;
  statusList->addItem("A new game has started.");
  statusLayout->addWidget(statusList, 3,0,1,2);

  QLabel * turnTextLabel = new QLabel("Current turn:");
  turnTextLabel->setObjectName("boldLabel");
  statusLayout->addWidget(turnTextLabel, 0,0);

  turnLabel = new QLabel(QString::number(turn));
  statusLayout->addWidget(turnLabel, 0, 1);

  QLabel * gamePhaseTextLabel = new QLabel("Current game phase:");
  gamePhaseTextLabel->setObjectName("boldLabel");
  statusLayout->addWidget(gamePhaseTextLabel, 1, 0);

  gamePhaseLabel = new QLabel(QString::number(gamePhase));
  statusLayout->addWidget(gamePhaseLabel, 1, 1);

  QSpacerItem *spacer = new QSpacerItem(400,300);
  statusLayout->addItem(spacer, 4, 0,1,2);

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

void Board::setHoverStylesheet()
{
  // qApp->setStyleSheet("QPushButton:hover { border: 0px; background-image: url(:/images/blue_stone.png); }");
}

void Board::updateGamePhaseLabel(QString str)
{
  gamePhaseLabel->setText(str);
}

void Board::incGamePhase() {
  gamePhase++;
  updateGamePhaseLabel(QString::number(gamePhase));
}

void Board::updateStatusLabel(QString str)
{
  statusList->insertItem(0, str);
  statusList->repaint();
}

void Board::pointSelected(int pos) {
  //qDebug()<<buttons[pos]->objectName();
  switch(gamePhase)
  {
  case 1:
    {
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
        } break;
      }
    } break;

  case 2:
    {
      static int moveFrom = 24;

      if(moveFrom == 24)
      {
        moveFrom = pos;
        buttons[pos]->setObjectName("selected");

        // Set stylesheet
        qApp->setStyleSheet("QLabel#boldLabel { font-weight: bold; } QPushButton#empty { border: 0px; background-image: url(:/images/empty.png); } QPushButton#player1 { border: 0px; background-image: url(:/images/blue_stone.png); } QPushButton#selected { border: 0px; background-image: url(:/images/blue_stone_selected.png); } QPushButton#player2 { border: 0px; background-image: url(:/images/red_stone.png); } QPushButton#empty:hover { border: 0px; background-image: url(:/images/blue_stone_selected.png); } QPushButton#selected:hover { border: 0px; background-image: url(:/images/blue_stone.png); }");

        return;
      }
      else if(pos != moveFrom)
      {
        // Attempt move
        try
        {
          movePiece(moveFrom, pos, &humanPlayer);
        }
        catch(const exception & e) {
          QMessageBox messageBox;
          messageBox.critical(0,"Error",e.what());
          messageBox.setFixedSize(500,200);
          return;
        }
        // Reset moveFrom
        moveFrom = 24;
        // Set stylesheet (*)
        qApp->setStyleSheet("QLabel#boldLabel { font-weight: bold; } QPushButton#empty { border: 0px; background-image: url(:/images/empty.png); } QPushButton#player1 { border: 0px; background-image: url(:/images/blue_stone.png); } QPushButton#player2 { border: 0px; background-image: url(:/images/red_stone.png); } QPushButton#player1:hover { border: 0px; background-image: url(:/images/blue_stone_selected.png); }");
      }
      else
      {
        // The button was clicked twice, so it will be reset
        moveFrom = 24;

        buttons[pos]->setObjectName("player"+QString::number(humanPlayer.getID()));

        // Set stylesheet (*)
        qApp->setStyleSheet("QLabel#boldLabel { font-weight: bold; } QPushButton#empty { border: 0px; background-image: url(:/images/empty.png); } QPushButton#player1 { border: 0px; background-image: url(:/images/blue_stone.png); } QPushButton#player2 { border: 0px; background-image: url(:/images/red_stone.png); } QPushButton#player1:hover { border: 0px; background-image: url(:/images/blue_stone_selected.png); }");
        return;
      }
    } break;
  }

  // Wait 1s
  //usleep(1000000);
  aiTurn();

  // Turn completed
  updateStatusLabel("----\nTurn " + QString::number(turn)+ " completed.");
  incTurn();

  if((gamePhase == 1 && (humanPlayer.isOutOfPieces() || aiPlayer.isOutOfPieces())))
  {
    updateStatusLabel("All pieces are placed. Phase 2 begins.");

    // Set stylesheet (*)
    qApp->setStyleSheet("QLabel#boldLabel { font-weight: bold; } QPushButton#empty { border: 0px; background-image: url(:/images/empty.png); } QPushButton#player1 { border: 0px; background-image: url(:/images/blue_stone.png); } QPushButton#player2 { border: 0px; background-image: url(:/images/red_stone.png); } QPushButton#player1:hover { border: 0px; background-image: url(:/images/blue_stone_selected.png); }");

    incGamePhase();
  }
}

void Board::aiTurn() {
  switch(gamePhase)
  {
  case 1:
    {
      int aiPos = aiPlayer.askPlacePosition(vertices, possibleMuehlePositions);

      try
      {
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
    } break;
  }
}

void Board::addPiece(int pos, Player * player) {
  if(vertices[pos] != 0) throw VertixNotEmptyException();
  if(player->isOutOfPieces()) throw OutOfPiecesException();

  if(player->getID() == 1) {
    buttons[pos]->setObjectName("player1");
    updateStatusLabel("You have placed a piece. You have " + QString::number(player->getPieces()) + " piece(s) left.");
  } else if(player->getID() == 2) {
    buttons[pos]->setObjectName("player2");
    updateStatusLabel("The computer has placed a piece.");
  }

  buttons[pos]->setStyle(qApp->style());

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
  buttons[pos]->setObjectName("empty");


  switch(player->getID())
  {
  case 1:
    updateStatusLabel("The computer has removed a piece.");
    break;
  case 2:
    updateStatusLabel("You have removed a piece.");
    break;
  }

  buttons[pos]->setStyle(qApp->style());
  statusList->repaint();

  // Update muehleDetected
  detectMuehle();
  // Reset stylesheet
  qApp->setStyleSheet("QLabel#boldLabel { font-weight: bold; } QPushButton#empty { border: 0px; background-image: url(:/images/empty.png); } QPushButton#player1 { border: 0px; background-image: url(:/images/blue_stone.png); } QPushButton#player2 { border: 0px; background-image: url(:/images/red_stone.png); } QPushButton#empty:hover { border: 0px; background-image: url(:/images/blue_stone_selected.png); }");
}

bool Board::isConnected(int pos1, int pos2) {
  for(int i = 0; i < 32; i++)
  {
    if(edges[i][0] == min(pos1, pos2) && edges[i][1] == max(pos1, pos2))
    {
      return true;
    }
  }
  return false;
}

void Board::movePiece(int pos1, int pos2, Player * player) {
  bool connected = isConnected(pos1, pos2);

  if(vertices[pos1] == player->getID() && vertices[pos2] == 0 && connected) {
    int p = player->getID();

    vertices[pos1] = 0;
    vertices[pos2] = p;

    buttons[pos1]->setObjectName("empty");
    buttons[pos2]->setObjectName("player"+QString::number(p));
  } else {
    throw IllegalMoveException();
  }

  buttons[pos1]->setStyle(qApp->style());
  buttons[pos2]->setStyle(qApp->style());
  detectMuehle();
}

void Board::movePieceFreely(int pos1, int pos2, Player * player) {

  if(vertices[pos1] == player->getID() && vertices[pos2] == 0) {
    vertices[pos1] = 0;
    vertices[pos2] = player->getID();
  } else {
    throw IllegalMoveException();
  }

  detectMuehle();
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
      // Actually don't think thats necessary

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
    // Set stylesheet
    qApp->setStyleSheet("QLabel#boldLabel { font-weight: bold; } QPushButton#empty { border: 0px; background-image: url(:/images/empty.png); } QPushButton#player1 { border: 0px; background-image: url(:/images/blue_stone.png); } QPushButton#player2 { border: 0px; background-image: url(:/images/red_stone.png); } QPushButton#player2:hover { border: 0px; background-image: url(:/images/red_stone_selected.png); }");
    break;
  case 2:
    updateStatusLabel("The computer has formed a mill and may remove a piece.");
    break;
  }

  // If no Muehle was detected, set muehleDetected to 0
  //muehleDetected[0] = 0;
}
