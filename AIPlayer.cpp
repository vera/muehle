#include "AIPlayer.h"

/*!
 *  Constructors
 */

AIPlayer::AIPlayer() : Player(AI_PLAYER_ID) {
  srand(time(NULL));
}

AIPlayer::AIPlayer(std::array<int, 24> vertices, std::array<std::array<int, 3>, 16> possibleMillPositions, std::array<std::array<int, 2>, 32> edges) : Player(AI_PLAYER_ID) {
  srand(time(NULL));
  this->vertices = vertices;
  this->possibleMillPositions = possibleMillPositions;
  this->edges = edges;
}

/*!
 *  Getters
 */

int AIPlayer::getID() {
  return AI_PLAYER_ID;
}

/*!
 *  Vector update methods
 */

void AIPlayer::updateHumanVectors(int pos1, int pos2)
{
  // pos1: Position of the human player's piece that was just placed
  // pos2: Position of the human player's piece that was just removed

  // Case 1: Human player has just placed a piece
  // => pos1 is set, pos2 is -1

  // Case 2: Human player has just moved a piece
  // => pos1 and pos2 are set

  // Case 3: AI player has just removed one of the human player's pieces
  // => pos2 is set, pos1 is -1

  // If pos2 is set
  // Check if existing or possible mills have been broken
  // Should be done first in case the removed piece is involved in a possible mill with the added piece
  if(pos2 != -1)
  {
    int millNr;
    int point1, point2, point3;

    // Check if existing mill has been broken
    for(int i = 0; i < 16; i++)
    {
      // Save the three points of the mill as point1-3
      point1 = possibleMillPositions[i][0];
      point2 = possibleMillPositions[i][1];
      point3 = possibleMillPositions[i][2];

      // Go through all possible mill positions involving the piece that is being moved
      if(point1 == pos1 || point2 == pos1 || point3 == pos1)
      {
        // Check if the mill involving the moved piece was previously formed
        if(vertices[point1] == HUMAN_PLAYER_ID && vertices[point2] == HUMAN_PLAYER_ID && vertices[point3] == HUMAN_PLAYER_ID)
        {
          // If yes, add the mill nr to onePieceLeftHuman
          onePieceLeftHuman.push_back(i);
        }
      }
    }

    // Go through onePieceLeftHuman
    for(vector<int>::iterator it = onePieceLeftHuman.begin(); it != onePieceLeftHuman.end();)
    {
      millNr = *it;

      // If pos2 was involved, move to twoPiecesLeftHuman
      if(possibleMillPositions[millNr][0] == pos2 || possibleMillPositions[millNr][1] == pos2 || possibleMillPositions[millNr][2] == pos2) {
        onePieceLeftHuman.erase(it);
        twoPiecesLeftHuman.push_back(millNr);
      } else {
        ++it;
      }
    }

    // Go through twoPiecesLeftHuman and impossibleHuman
    for(vector<int>::iterator it = twoPiecesLeftHuman.begin(); it != twoPiecesLeftHuman.end();)
    {
      millNr = *it;

      // If pos2 was involved, remove
      if(possibleMillPositions[millNr][0] == pos2 || possibleMillPositions[millNr][1] == pos2 || possibleMillPositions[millNr][2] == pos2) {
        twoPiecesLeftHuman.erase(it);
      } else {
        ++it;
      }
    }

    for(vector<int>::iterator it = impossibleHuman.begin(); it != impossibleHuman.end();)
    {
      millNr = *it;

      // If pos2 was involved, remove
      if(possibleMillPositions[millNr][0] == pos2 || possibleMillPositions[millNr][1] == pos2 || possibleMillPositions[millNr][2] == pos2) {
        impossibleHuman.erase(it);
      } else {
        ++it;
      }
    }

  }

  // If pos1 is set
  // Check if there are new possible mills
  if(pos1 != -1)
  {
    for(int j = 0; j < 16; j++)
    {
      // Go through all possible mill positions involving the piece that was just placed
      if(possibleMillPositions[j][0] == pos1 || possibleMillPositions[j][1] == pos1 || possibleMillPositions[j][2] == pos1)
      {
        if(vertices[possibleMillPositions[j][0]] == AI_PLAYER_ID || vertices[possibleMillPositions[j][1]] == AI_PLAYER_ID || vertices[possibleMillPositions[j][2]] == AI_PLAYER_ID)
        {
          impossible.push_back(j);

          // Check if placing this piece has made previously possible mill of the opposing player impossible
          if(std::find(onePieceLeft.begin(), onePieceLeft.end(), j) != onePieceLeft.end()) {
            onePieceLeft.erase(std::find(onePieceLeft.begin(), onePieceLeft.end(), j));
            impossible.push_back(j);
          } else if(std::find(twoPiecesLeft.begin(), twoPiecesLeft.end(), j) != twoPiecesLeft.end()) {
            twoPiecesLeft.erase(std::find(twoPiecesLeft.begin(), twoPiecesLeft.end(), j));
            impossible.push_back(j);
          }

        }
        else
        {
          if(std::find(onePieceLeftHuman.begin(), onePieceLeftHuman.end(), j) != onePieceLeftHuman.end())
          {
            // If j already is in onePieceLeft, it is already a completed mill, do nothing, just erase
            onePieceLeftHuman.erase(std::find(onePieceLeftHuman.begin(), onePieceLeftHuman.end(), j));
          }
          else if(std::find(twoPiecesLeftHuman.begin(), twoPiecesLeftHuman.end(), j) != twoPiecesLeftHuman.end())
          {
            // If j already is in twoPiecesLeft, move to onePieceLeft
            twoPiecesLeftHuman.erase(std::find(twoPiecesLeftHuman.begin(), twoPiecesLeftHuman.end(), j));
            onePieceLeftHuman.push_back(j);
          }
          else
          {
            // If j isn't already in onePieceLeft or twoPiecesLeft, add to twoPiecesLeft
            twoPiecesLeftHuman.push_back(j);
          }
        }
      }
    }
  }

  // Update vertices
  if(pos1 != -1) vertices[pos1] = HUMAN_PLAYER_ID;
  if(pos2 != -1) vertices[pos2] = 0;
}

void AIPlayer::updateAIVectors(int pos1, int pos2)
{
  // pos1: Position of the AI player's piece that was just placed
  // pos2: Position of the AI player's piece that was just removed

  // Case 1: AI player has just placed a piece
  // => pos1 is set, pos2 is -1

  // Case 2: AI player has just moved a piece
  // => pos1 and pos2 are set

  // Case 3: Human player has just removed one of the AI player's pieces
  // => pos2 is set, pos1 is -1

  // If pos2 is set
  // Check if existing or possible mills have been broken
  // Should be done first in case the removed piece is involved in a possible mill with the added piece
  if(pos2 != -1)
  {
    int millNr;
    int point1, point2, point3;

    // Check if existing mill has been broken
    for(int i = 0; i < 16; i++)
    {
      // Save the three points of the mill as point1-3
      point1 = possibleMillPositions[i][0];
      point2 = possibleMillPositions[i][1];
      point3 = possibleMillPositions[i][2];

      // Go through all possible mill positions involving the piece that is being moved
      if(point1 == pos1 || point2 == pos1 || point3 == pos1)
      {
        // Check if the mill involving the moved piece was previously formed
        if(vertices[point1] == AI_PLAYER_ID && vertices[point2] == AI_PLAYER_ID && vertices[point3] == AI_PLAYER_ID)
        {
          // If yes, add the mill nr to onePieceLeft
          onePieceLeft.push_back(i);
        }
      }
    }

    // Go through onePieceLeft
    for(vector<int>::iterator it = onePieceLeft.begin(); it != onePieceLeft.end();)
    {
      millNr = *it;

      // If pos2 was involved, move to twoPiecesLeft
      if(possibleMillPositions[millNr][0] == pos2 || possibleMillPositions[millNr][1] == pos2 || possibleMillPositions[millNr][2] == pos2) {
        onePieceLeft.erase(it);
        twoPiecesLeft.push_back(millNr);
      } else {
        ++it;
      }
    }

    // Go through twoPiecesLeft and impossible
    for(vector<int>::iterator it = twoPiecesLeft.begin(); it != twoPiecesLeft.end();)
    {
      millNr = *it;

      // If pos2 was involved, remove
      if(possibleMillPositions[millNr][0] == pos2 || possibleMillPositions[millNr][1] == pos2 || possibleMillPositions[millNr][2] == pos2) {
        twoPiecesLeft.erase(it);
      } else {
        ++it;
      }
    }

    for(vector<int>::iterator it = impossible.begin(); it != impossible.end();)
    {
      millNr = *it;

      // If pos2 was involved, remove
      if(possibleMillPositions[millNr][0] == pos2 || possibleMillPositions[millNr][1] == pos2 || possibleMillPositions[millNr][2] == pos2) {
        impossible.erase(it);
      } else {
        ++it;
      }
    }

  }

  // If pos1 is set
  // Check if there are new possible mills
  if(pos1 != -1)
  {
    for(int j = 0; j < 16; j++)
    {
      // Go through all possible mill positions involving the piece that was just placed
      if(possibleMillPositions[j][0] == pos1 || possibleMillPositions[j][1] == pos1 || possibleMillPositions[j][2] == pos1)
      {
        if(vertices[possibleMillPositions[j][0]] == HUMAN_PLAYER_ID || vertices[possibleMillPositions[j][1]] == HUMAN_PLAYER_ID || vertices[possibleMillPositions[j][2]] == HUMAN_PLAYER_ID)
        {
          impossible.push_back(j);

          // Check if placing this piece has made previously possible mill of the opposing player impossible
          if(std::find(onePieceLeftHuman.begin(), onePieceLeftHuman.end(), j) != onePieceLeftHuman.end()) {
            onePieceLeftHuman.erase(std::find(onePieceLeftHuman.begin(), onePieceLeftHuman.end(), j));
            impossibleHuman.push_back(j);
          } else if(std::find(twoPiecesLeftHuman.begin(), twoPiecesLeftHuman.end(), j) != twoPiecesLeftHuman.end()) {
            twoPiecesLeftHuman.erase(std::find(twoPiecesLeftHuman.begin(), twoPiecesLeftHuman.end(), j));
            impossibleHuman.push_back(j);
          }

        }
        else
        {
          if(std::find(onePieceLeft.begin(), onePieceLeft.end(), j) != onePieceLeft.end())
          {
            // If j already is in onePieceLeft, it is now a completed mill, do nothing, just erase
            onePieceLeft.erase(std::find(onePieceLeft.begin(), onePieceLeft.end(), j));
          }
          else if(std::find(twoPiecesLeft.begin(), twoPiecesLeft.end(), j) != twoPiecesLeft.end())
          {
            // If j already is in twoPiecesLeft, move to onePieceLeft
            twoPiecesLeft.erase(std::find(twoPiecesLeft.begin(), twoPiecesLeft.end(), j));
            onePieceLeft.push_back(j);
          }
          else
          {
            // If j isn't already in onePieceLeft or twoPiecesLeft, add to twoPiecesLeft
            twoPiecesLeft.push_back(j);
          }
        }
      }
    }
  }

  // Update vertices
  if(pos1 != -1) vertices[pos1] = AI_PLAYER_ID;
  if(pos2 != -1) vertices[pos2] = 0;
}

/*!
 *  AI logic methods
 */

int AIPlayer::askPlacePosition()
{
  // The AI's priorities are:
  // #1 Form a mill when one piece is left
  // #2 Block a mill of the opposing player when one piece is left
  // #3 Start forming a mill when two pieces are left
  // #4 Place at one of the central four positions (7, 11, 12, 16)
  // #5 Block a mill of the opposing player when two pieces are left
  // #6 Place at a random position

  // STEPS

  // #1 Form a mill when one piece is left
  // #1.1 If onePieceLeft isn't empty, pick a random mill out of the vector and form it

  if(!(onePieceLeft.empty()))
  {
    int r = rand() % onePieceLeft.size();
    int millNr = onePieceLeft.at(r);
    for(int i = 0; i < 3; i++)
    {
      if(vertices[possibleMillPositions[millNr][i]] == 0) return possibleMillPositions[millNr][i];
    }
  }

  // #2 Block a mill of the opposing player when one piece is left
  // #2.1 If onePieceLeftHuman isn't empty, pick a random mill out of the vector and block it

  if(!(onePieceLeftHuman.empty()))
  {
    int r = rand() % onePieceLeftHuman.size();
    int millNr = onePieceLeftHuman.at(r);
    for(int i = 0; i < 3; i++)
    {
      if(vertices[possibleMillPositions[millNr][i]] == 0) return possibleMillPositions[millNr][i];
    }
  }

  // #3 Start forming a mill when two pieces are left
  // #3.1 If twoPiecesLeft isn't empty, pick a random mill out of the vector and start forming it

  if(!(twoPiecesLeft.empty()))
  {
    int r = rand() % twoPiecesLeft.size();
    int millNr = twoPiecesLeft.at(r);
    for(int i = 0; i < 3; i++)
    {
      if(vertices[possibleMillPositions[millNr][i]] == 0) return possibleMillPositions[millNr][i];
    }
  }

  // #4 Place at one of the central four positions (7, 11, 12, 16)

  int centralPositions [4] = {7, 11, 12, 16};

  if(vertices[centralPositions[0]] == 0 || vertices[centralPositions[1]] == 0 || vertices[centralPositions[2]] == 0)
  {
    int r;

    do
    {
      r = rand() % 3;
    } while(vertices[centralPositions[r]] != 0);

    return centralPositions[r];
  }

  // #5 Block a mill of the opposing player when two pieces are left
  // #5.1 If onePieceLeftHuman isn't empty, pick a random mill out of the vector and block it

  if(!(twoPiecesLeftHuman.empty()))
  {
    int r = rand() % twoPiecesLeftHuman.size();
    int millNr = twoPiecesLeftHuman.at(r);
    for(int i = 0; i < 3; i++)
    {
      if(vertices[possibleMillPositions[millNr][i]] == 0) return possibleMillPositions[millNr][i];
    }
  }

  // #6 Place at a random position

  int r;

  do
  {
    r = rand() % 24;
  } while(vertices[r] != 0);

  return r;
}

int AIPlayer::askRemovePosition(vector<int> protectedPoints)
{
  // The AI's priorities are:
  // #1 Free a blocked mill
  // #2 Remove a piece of the opposing player in a mill with one piece left
  // #3 Remove the piece of the opposing player that is involved in the most possible mills
  // #4 Remove a random piece

  // STEPS

  // #1 Free a blocked mill
  // #1.1 Go through the impossible vector (contains all impossible mills that are blocked in some way)

  int millNr;
  int points [3];

  // #1.2 Check if there is a mill that is only blocked a single piece
  for(vector<int>::iterator it = impossible.begin(); it != impossible.end(); it++)
  {
    millNr = *it;

    points[0] = possibleMillPositions[millNr][0];
    points[1] = possibleMillPositions[millNr][1];
    points[2] = possibleMillPositions[millNr][2];

    // Uses XOR: (a ^ b ^ c) && !(a && b && c)
    if(((vertices[points[0]] == HUMAN_PLAYER_ID) ^ (vertices[points[1]] == HUMAN_PLAYER_ID) ^ (vertices[points[2]] == HUMAN_PLAYER_ID)) && !(vertices[points[0]] == HUMAN_PLAYER_ID && vertices[points[1]] == HUMAN_PLAYER_ID && vertices[points[2]] == HUMAN_PLAYER_ID))
    {
      for(int i = 0; i < 3; i++)
      {
        // and return the position of the blocking piece
        if(vertices[points[i]] == HUMAN_PLAYER_ID) return points[i];
      }
    }
  }

  // #1.3 Else pick a random mill

  int r = rand() % impossible.size();
  millNr = impossible.at(r);

  points[0] = possibleMillPositions[millNr][0];
  points[1] = possibleMillPositions[millNr][1];
  points[2] = possibleMillPositions[millNr][2];

  for(int i = 0; i < 3; i++)
  {
    // and return the position of the blocking piece
    if(points[i] == HUMAN_PLAYER_ID) return points[i];
  }

  // #2 Remove a piece of the opposing player in a mill with one piece left

  for(vector<int>::iterator it = onePieceLeftHuman.begin(); it != onePieceLeftHuman.end(); it++)
  {
    millNr = *it;

    points[0] = possibleMillPositions[millNr][0];
    points[1] = possibleMillPositions[millNr][1];
    points[2] = possibleMillPositions[millNr][2];

    for(int i = 0; i < 3; i++)
    {
      if(points[i] == HUMAN_PLAYER_ID) return points[i];
    }
  }

  // #3 Remove the piece of the opposing player that is involved in the most possible mills

  int involvedInPotentialMills [24] = {0};
  int involvedInMostPotentialMills = 24;

  // #3.1 Go through all vertices == HUMAN_PLAYER_ID
  // and check if they are part of a possible mill; if yes, increase count involvedInPotentialMills
  for(int i = 0; i < 24; i++)
  {
    if(vertices[i] == HUMAN_PLAYER_ID)
    {
      for(int j = 0; j < 16; j++)
      {
        if(possibleMillPositions[j][0] == i || possibleMillPositions[j][1] == i || possibleMillPositions[j][2] == i)
        {
          if(vertices[possibleMillPositions[j][0]] != 2 && vertices[possibleMillPositions[j][1]] != 2 && vertices[possibleMillPositions[j][2]] != 2)
          {
            // Piece i is involved in a possible Muehle
            involvedInPotentialMills[i]++;
            if(involvedInPotentialMills[i] > involvedInPotentialMills[involvedInMostPotentialMills])
            {
              involvedInMostPotentialMills = i;
            }
          }
        }
      }
    }
  }

  // #3.2 If it exists, return the one that is part of the most possible mills

  if(involvedInMostPotentialMills != 24)
  {
    return involvedInMostPotentialMills;
  }

  // #4 Remove a random piece

  do
  {
    r = rand() % 24;
  } while(vertices[r] != 1 || std::find(protectedPoints.begin(), protectedPoints.end(), r) != protectedPoints.end());

  return r;
}

std::pair<int,int> AIPlayer::askMovePositions()
{
  int pos1, pos2;

  // The AI's priorities are:
  // #1 Form a mill when onePieceLeft and canBeReached
  // #2 Block a mill of the opposing player when onePieceLeft and canBeReached
  // #3 Move one piece in a mill
  // #4 Move a random piece

  // STEPS

  // #1 Form a mill when onePieceLeft and canBeReached
  // #1.1 Check if onePieceLeft is not empty and canBeReached

  int millNr;

  if(!onePieceLeft.empty())
  {
    for(vector<int>::iterator it = onePieceLeft.begin(); it != onePieceLeft.end(); it++)
    {
      // Get number of the mill
      millNr = *it;
      // Go through all three piece positions of the mill to figure out which one is empty
      for(int i = 0; i < 3; i++)
      {
        pos2 = possibleMillPositions[millNr][i];
        if(vertices[pos2] == 0)
        {
          // Check if the empty position can be reached
          // canBeReached returns position from where pos2 can be reached if it exists, otherwise it returns -1
          pos1 = canBeReached(pos2);
          if(pos1 != -1) return std::make_pair(pos1, pos2);
        }
      }
    }
  }

  // #2 Block a mill of the opposing player when onePieceLeft and canBeReached
  // #2.1 Check if onePieceLeftHuman is not empty and canBeReached

  if(!onePieceLeftHuman.empty())
  {
    for(vector<int>::iterator it = onePieceLeftHuman.begin(); it != onePieceLeftHuman.end(); it++)
    {
      // Get number of the mill
      millNr = *it;
      // Go through all three piece positions of the mill to figure out which one is empty
      for(int i = 0; i < 3; i++)
      {
        pos2 = possibleMillPositions[millNr][i];
        if(vertices[pos2] == 0)
        {
          // Check if the empty position can be reached
          // canBeReached returns position from where pos2 can be reached if it exists, otherwise it returns -1
          pos1 = canBeReached(pos2);
          if(pos1 != -1) return std::make_pair(pos1, pos2);
        }
      }
    }
  }

  // #3 Move one piece in a mill

  // #3.1 Find formed mills

  int points [3];

  for(int i = 0; i < 16; i++)
  {
    // Save the three points of the mill as point1-3
    points[0] = possibleMillPositions[i][0];
    points[1] = possibleMillPositions[i][1];
    points[2] = possibleMillPositions[i][2];

    // Check if the mill is formed
    if(vertices[points[0]] == AI_PLAYER_ID && vertices[points[1]] == AI_PLAYER_ID && vertices[points[2]] == AI_PLAYER_ID)
    {
      // #3.2 If yes, move a piece at a random position of the mill
      int r = rand() % 3;
      pos1 = points[r];

      // #3.3 Find a empty position that is connected to that position

      for(int j = 0; j < 32; j++)
      {
        if(edges[j][0] == pos1)
        {
          if(vertices[edges[j][1]] == 0)
          {
            pos2 = edges[j][1];
            return std::make_pair(pos1, pos2);
          }
        }
        else if(edges[j][1] == pos1)
        {
          if(vertices[edges[j][0]] == 0) {
            pos2 = edges[j][0];
            return std::make_pair(pos1, pos2);
          }
        }
      }
    }
  }

  // #4 Move a random piece

  do
  {
    pos2 = rand() % 24;
  } while(vertices[pos2] != 0 || canBeReached(pos2) == -1 || vertices[canBeReached(pos2)] == HUMAN_PLAYER_ID);

  pos1 = canBeReached(pos2);

  return std::make_pair(pos1, pos2);
}

int AIPlayer::canBeReached(int pos2)
{
  int pos1;
  // #1 Go through all edges
  for(int i = 0; i < 32; i++)
  {
    // #2 Check if they involve positon pos2
    if(edges[i][0] == pos2)
    {
      pos1 = edges[i][1];
      // #3 Check if they also involve a position where the AI player has placed a piece
      // If yes, return that position
      if(vertices[pos1] == AI_PLAYER_ID) return pos1;
    }
    // #2 Check if they involve positon pos2
    else if(edges[i][1] == pos2)
    {
      pos1 = edges[i][0];
      // #3 Check if they also involve a position where the AI player has placed a piece
      // If yes, return that position
      if(vertices[pos1] == AI_PLAYER_ID) return pos1;
    }
  }

  // #4 If none are, return -1
  return -1;
}

std::pair<int,int> AIPlayer::askFreeMovePositions()
{
  int pos1, pos2;

  // The AI's priorities are:
  // #1 Form a mill when onePieceLeft
  // #2 Block a mill of the opposing player when onePieceLeft
  // #3 Start forming a mill when twoPiecesLeft
  // #4 Block a mill of the opposing player when twoPiecesLeft

  // STEPS

  // #1 Form a mill when onePieceLeft
  // #1.1 Check if onePieceLeft is not empty

  int millNr;

  if(!onePieceLeft.empty())
  {
    for(vector<int>::iterator it = onePieceLeft.begin(); it != onePieceLeft.end(); it++)
    {
      // Get number of the mill
      millNr = *it;
      // Go through all three piece positions of the mill to figure out which one is empty
      for(int i = 0; i < 3; i++)
      {
        // pos2 is the empty position of the mill
        pos2 = possibleMillPositions[millNr][i];
        if(vertices[pos2] == 0)
        {
          // Return position of random AI player's piece
          int randomPosition = rand() % piecesOnBoard.size();
          pos1 = piecesOnBoard.at(randomPosition);
          return std::make_pair(pos1, pos2);
        }
      }
    }
  }

  // #2 Block a mill of the opposing player when onePieceLeft
  // #2.1 Check if onePieceLeftHuman is not empty

  if(!onePieceLeftHuman.empty())
  {
    for(vector<int>::iterator it = onePieceLeftHuman.begin(); it != onePieceLeftHuman.end(); it++)
    {
      // Get number of the mill
      millNr = *it;
      // Go through all three piece positions of the mill to figure out which one is empty
      for(int i = 0; i < 3; i++)
      {
        // pos2 is the empty position of the mill
        pos2 = possibleMillPositions[millNr][i];
        if(vertices[pos2] == 0)
        {
          // Return position of random AI player's piece
          int randomPosition = rand() % piecesOnBoard.size();
          pos1 = piecesOnBoard.at(randomPosition);
          return std::make_pair(pos1, pos2);
        }
      }
    }
  }

  // #3 Start forming a mill when twoPiecesLeft
  // #3.1 Check if twoPiecesLeft is not empty

  if(!twoPiecesLeft.empty())
  {
    for(vector<int>::iterator it = twoPiecesLeft.begin(); it != twoPiecesLeft.end(); it++)
    {
      // Get number of the mill
      millNr = *it;
      // Go through all three piece positions of the mill to figure out which one is empty
      for(int i = 0; i < 3; i++)
      {
        // pos2 is the first empty position of the mill
        pos2 = possibleMillPositions[millNr][i];
        if(vertices[pos2] == 0)
        {
          // Return position of random AI player's piece
          int randomPosition = rand() % piecesOnBoard.size();
          pos1 = piecesOnBoard.at(randomPosition);
          return std::make_pair(pos1, pos2);
        }
      }
    }
  }

  // #4 Block a mill of the opposing player when twoPiecesLeft
  // #4.1 Check if twoPiecesLeftHuman is not empty

  if(!twoPiecesLeftHuman.empty())
  {
    for(vector<int>::iterator it = twoPiecesLeftHuman.begin(); it != twoPiecesLeftHuman.end(); it++)
    {
      // Get number of the mill
      millNr = *it;
      // Go through all three piece positions of the mill to figure out which one is empty
      for(int i = 0; i < 3; i++)
      {
        // pos2 is the first empty position of the mill
        pos2 = possibleMillPositions[millNr][i];
        if(vertices[pos2] == 0)
        {
          // Return position of random AI player's piece
          int randomPosition = rand() % piecesOnBoard.size();
          pos1 = piecesOnBoard.at(randomPosition);
          return std::make_pair(pos1, pos2);
        }
      }
    }
  }

  // #5 Move random piece

  int randomPosition1 = rand() % piecesOnBoard.size();
  pos1 = piecesOnBoard.at(randomPosition1);

  do
  {
    pos2 = rand() % 24;
  } while(vertices[pos2] != 0);

  return std::make_pair(pos1, pos2);
}

/*!
 *  Reset method
 */

void AIPlayer::reset()
{
  Player::reset();
  onePieceLeft.clear();
  twoPiecesLeft.clear();
  impossible.clear();
  onePieceLeftHuman.clear();
  twoPiecesLeftHuman.clear();
  impossibleHuman.clear();

  for(int i = 0; i < 24; i++) {
    vertices[i] = 0;
  }
}
