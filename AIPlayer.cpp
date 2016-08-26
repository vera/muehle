// Define player ID of the computer, default is 2
#define AI_PLAYER_ID 2

#include "Player.h"
#include "Exceptions.h"
#include <vector>
#include <ctime>
#include <array>

class AIPlayer : public Player {
  std::array<int, 24> vertices;
  std::array<std::array<int, 3>, 16> possibleMillPositions;
  std::array<std::array<int, 2>, 32> edges;

  public:
    static constexpr int getID() { return AI_PLAYER_ID; }

    AIPlayer() : Player(AI_PLAYER_ID) {
      srand(time(NULL));
    }

    AIPlayer(std::array<int, 24> vertices, std::array<std::array<int, 3>, 16> possibleMillPositions, std::array<std::array<int, 2>, 32> edges) : Player(AI_PLAYER_ID) {
      srand(time(NULL));
      this->vertices = vertices;
      this->possibleMillPositions = possibleMillPositions;
      this->edges = edges;
    }

    // TODO: Block mills by human player
    int askPlacePosition()
    {
      // Goal: Create a mill
      // Go through vertices == 2
      // Go through possible mills, update vectors "1 left", "2 left", "impossible" with # of mill
      // If "1 left" isn't empty: pick random
      // Else "2 left"
      // Else "impossible"

      int i;
      vector<int> onePieceLeft, twoPiecesLeft, impossible;
      vector<int> piecesOnBoard = getPiecesOnBoardVector();

      for(vector<int>::iterator it = piecesOnBoard.begin(); it != piecesOnBoard.end(); it++)
      {
        for(int j = 0; j < 16; j++)
        {
          i = *it;
          if(possibleMillPositions[j][0] == i || possibleMillPositions[j][1] == i || possibleMillPositions[j][2] == i)
          {
            if(vertices[possibleMillPositions[j][0]] == 1 || vertices[possibleMillPositions[j][1]] == 1 || vertices[possibleMillPositions[j][2]] == 1)
            {
              impossible.push_back(j);
            }
            else
            {
              if(std::find(onePieceLeft.begin(), onePieceLeft.end(), j) != onePieceLeft.end())
              {
                // If j already is in onePieceLeft, it is already a completed mill, do nothing, just erase
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

      if(!(onePieceLeft.empty()))
      {
        int r = rand() % onePieceLeft.size();
        int millNr = onePieceLeft.at(r);
        for(int i = 0; i < 3; i++)
        {
          if(vertices[possibleMillPositions[millNr][i]] == 0) return possibleMillPositions[millNr][i];
        }
      }
      else if(!(twoPiecesLeft.empty()))
      {
        int r = rand() % twoPiecesLeft.size();
        int millNr = twoPiecesLeft.at(r);
        for(int i = 0; i < 3; i++)
        {
          if(vertices[possibleMillPositions[millNr][i]] == 0) return possibleMillPositions[millNr][i];
        }
      }
      else if(!(impossible.empty()))
      {
        int r = rand() % impossible.size();
        int millNr = impossible.at(r);
        for(int i = 0; i < 3; i++)
        {
          if(vertices[possibleMillPositions[millNr][i]] == 0) return possibleMillPositions[millNr][i];
        }
      }

      // If all three vectors are empty, for example at the beginning of the game when the board is empty, return a random position
      int r;

      do
      {
        r = rand() % 24;
      } while(vertices[r] != 0);

      return r;
    }

    int askRemovePosition(vector<int> protectedPoints)
    {
      // Go through all vertices == 1
      // Check if they are part of a possible mill; if yes, increase count involvedInPotentialMills
      // If it exists, return the one that is part of the most possible mills
      // If not, return a random
      // TODO: Free blocked mills in game phase 2 and 3

      int involvedInPotentialMills [24] = {0};
      int involvedInMostPotentialMills = 24;

      // TODO: Iterate through the piecesOnBoard vector instead of checking every single vertix
      for(int i = 0; i < 24; i++)
      {
        if(vertices[i] == 1)
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

      if((involvedInMostPotentialMills = 24))
      {
        int r;
        do
        {
          r = rand() % 24;
        } while(vertices[r] != 1 || std::find(protectedPoints.begin(), protectedPoints.end(), r) != protectedPoints.end());

        return r;
      }

      return involvedInMostPotentialMills;
    }

    std::pair<int,int> askMovePositions(vector<int> piecesOnBoardHuman)
    {
      int pos1, pos2;

      // The AI's priorities are:
      // #1 Form a mill when onePieceLeft and canBeReached
      // #2 Block a mill of the opposing player when onePieceLeft and canBeReached
      // #3 Move one piece in a mill if !canBeReached by opposing player
      // #4 Start moving to block a mill of the opposing player when onePieceLeft
      // #5 Move a random piece

      // STEPS
      // #1.1 Create onePieceLeft vector

      vector<int> onePieceLeft, twoPiecesLeft, impossible;
      vector<int> piecesOnBoard = getPiecesOnBoardVector();
      int i;

      for(vector<int>::iterator it = piecesOnBoard.begin(); it != piecesOnBoard.end(); it++)
      {
        for(int j = 0; j < 16; j++)
        {
          i = *it;
          if(possibleMillPositions[j][0] == i || possibleMillPositions[j][1] == i || possibleMillPositions[j][2] == i)
          {
            if(vertices[possibleMillPositions[j][0]] == 1 || vertices[possibleMillPositions[j][1]] == 1 || vertices[possibleMillPositions[j][2]] == 1)
            {
              impossible.push_back(j);
            }
            else
            {
              if(std::find(onePieceLeft.begin(), onePieceLeft.end(), j) != onePieceLeft.end())
              {
                // If j already is in onePieceLeft, it is already a completed mill, do nothing, just erase
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

      // #1.2 Check if onePieceLeft is not empty and canBeReached

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
              pos1 = canBeReached(pos2, edges, vertices);
              if(pos1 != -1) return std::make_pair(pos1, pos2);
            }
          }
        }
      }

      // #2.1 Create onePieceLeft vector for human player's pieces
      // TODO: Überlegen ob impossible notwendig ist

      vector<int> onePieceLeftHuman, twoPiecesLeftHuman, impossibleHuman;

      for(vector<int>::iterator it = piecesOnBoardHuman.begin(); it != piecesOnBoardHuman.end(); it++)
      {
        for(int j = 0; j < 16; j++)
        {
          i = *it;
          if(possibleMillPositions[j][0] == i || possibleMillPositions[j][1] == i || possibleMillPositions[j][2] == i)
          {
            if(vertices[possibleMillPositions[j][0]] == AI_PLAYER_ID || vertices[possibleMillPositions[j][1]] == AI_PLAYER_ID || vertices[possibleMillPositions[j][2]] == AI_PLAYER_ID)
            {
              impossible.push_back(j);
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

      // #2.2 Check if onePieceLeftHuman is not empty and canBeReached

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
              pos1 = canBeReached(pos2, edges, vertices);
              if(pos1 != -1) return std::make_pair(pos1, pos2);
            }
          }
        }
      }

      // #3 Move one piece in a mill if !canBeReached by opposing player

      // Find mills?

      // #4 Start moving to block a mill of the opposing player when onePieceLeft

      // #5 Move a random piece

      do
      {
        pos2 = rand() % 24;
      } while(vertices[pos2] != 0 || canBeReached(pos2, edges, vertices) == -1);

      pos1 = canBeReached(pos2, edges, vertices);

      return std::make_pair(pos1, pos2);
    }

    int canBeReached(int pos2, std::array<std::array<int, 2>, 32> edges, std::array<int, 24> vertices)
    {
      int pos1;
      // #1 Go through all pieces on board by AI
      for(int i = 0; i < 32; i++)
      {
        // #2 Check if they are connected to argument pos2
        if(edges[i][0] == pos2)
        {
          pos1 = edges[i][1];
          // #3 If yes, return piece position
          if(vertices[pos1] == AI_PLAYER_ID) return pos1;
        }
        else if(edges[i][1] == pos2)
        {
          pos1 = edges[i][0];
          // #3 If yes, return piece position
          if(vertices[pos1] == AI_PLAYER_ID) return pos1;
        }
      }

      // #4 If none are, return -1
      return -1;
    }

    std::pair<int,int> askFreeMovePositions(vector<int> piecesOnBoardHuman)
    {
      int pos1, pos2;

      // The AI's priorities are:
      // #1 Form a mill when onePieceLeft
      // #2 Block a mill of the opposing player when onePieceLeft
      // #3 Start forming a mill when twoPiecesLeft
      // #4 Block a mill of the opposing player when twoPiecesLeft

      // STEPS

      // #1 Form a mill when onePieceLeft
      // #1.1 Create onePieceLeft vector

      vector<int> onePieceLeft, twoPiecesLeft, impossible;
      vector<int> piecesOnBoard = getPiecesOnBoardVector();
      int i;

      for(vector<int>::iterator it = piecesOnBoard.begin(); it != piecesOnBoard.end(); it++)
      {
        for(int j = 0; j < 16; j++)
        {
          i = *it;
          if(possibleMillPositions[j][0] == i || possibleMillPositions[j][1] == i || possibleMillPositions[j][2] == i)
          {
            if(vertices[possibleMillPositions[j][0]] == 1 || vertices[possibleMillPositions[j][1]] == 1 || vertices[possibleMillPositions[j][2]] == 1)
            {
              impossible.push_back(j);
            }
            else
            {
              if(std::find(onePieceLeft.begin(), onePieceLeft.end(), j) != onePieceLeft.end())
              {
                // If j already is in onePieceLeft, it is already a completed mill, do nothing, just erase
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

      // #1.2 Check if onePieceLeft is not empty

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
      // #2.1 Create onePieceLeft vector for human player's pieces
      // TODO: Überlegen ob impossible notwendig ist

      vector<int> onePieceLeftHuman, twoPiecesLeftHuman, impossibleHuman;

      for(vector<int>::iterator it = piecesOnBoardHuman.begin(); it != piecesOnBoardHuman.end(); it++)
      {
        for(int j = 0; j < 16; j++)
        {
          i = *it;
          if(possibleMillPositions[j][0] == i || possibleMillPositions[j][1] == i || possibleMillPositions[j][2] == i)
          {
            if(vertices[possibleMillPositions[j][0]] == AI_PLAYER_ID || vertices[possibleMillPositions[j][1]] == AI_PLAYER_ID || vertices[possibleMillPositions[j][2]] == AI_PLAYER_ID)
            {
              impossible.push_back(j);
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

      // #2.2 Check if onePieceLeftHuman is not empty

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
};
