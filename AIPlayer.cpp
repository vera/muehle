// Define player ID of the computer, default is 2
#define AI_PLAYER_ID 2

#include "Player.h"
#include "Exceptions.h"
#include <vector>
#include <ctime>

class AIPlayer : public Player {
  public:
    static constexpr int getID() { return AI_PLAYER_ID; }

    AIPlayer() : Player(AI_PLAYER_ID) {
      srand(time(NULL));
    }

    // TODO: Possibly eliminate all these arguments, instead use Board as only argument and implement getters in Board
    int askPlacePosition(int vertices [24], int possibleMillPositions [16][3])
    {
      // Goal: Create a mill
      // Go through vertices == 2
      // Go through possible mills, update vectors "1 left", "2 left", "impossible" with # of mill
      // If "1 left" isn't empty: pick random
      // Else "2 left"
      // Else "impossible"

      vector<int> onePieceLeft, twoPiecesLeft, impossible;

      int r;

      if(getPiecesOnBoard() == 0)
      {
        do
        {
          r = rand() % 24;
        } while(vertices[r] != 0);

        return r;
      }

      int i;
      vector<int> piecesOnBoard = getPiecesOnBoardVector();

      //for(int i = 0; i < 24; i++)
      for(vector<int>::iterator it = piecesOnBoard.begin(); it != piecesOnBoard.end(); it++)
      {
        //if(vertices[i] == 2)
        //{
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
        //}
      }

      int millNr;

      if(!(onePieceLeft.empty()))
      {
        r = rand() % onePieceLeft.size();
        millNr = onePieceLeft.at(r);
        for(int i = 0; i < 3; i++)
        {
          if(vertices[possibleMillPositions[millNr][i]] == 0) return possibleMillPositions[millNr][i];
        }
      }
      else if(!(twoPiecesLeft.empty()))
      {
        r = rand() % twoPiecesLeft.size();
        millNr = twoPiecesLeft.at(r);
        for(int i = 0; i < 3; i++)
        {
          if(vertices[possibleMillPositions[millNr][i]] == 0) return possibleMillPositions[millNr][i];
        }
      }
      else if(!(impossible.empty()))
      {
        r = rand() % impossible.size();
        millNr = impossible.at(r);
        for(int i = 0; i < 3; i++)
        {
          if(vertices[possibleMillPositions[millNr][i]] == 0) return possibleMillPositions[millNr][i];
        }
      }
    }

    int askRemovePosition(int vertices [24], int possibleMillPositions [16][3], vector<int> protectedPoints)
    {
      // Go through all vertices == 1
      // Check if they are part of a possible mill; if yes, increase count involvedInPotentialMills
      // If it exists, return the one that is part of the most possible mills
      // If not, return a random

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

    std::pair<int,int> askMovePositions(int vertices [24], int possibleMillPositions [16][3], int edges [32][2])
    {
      int pos1, pos2;

      // The AI's priorities are:
      // #1 Form a mill when onePieceLeft and canBeReached
      // #2 Block a mill of the opposing player when onePieceLeft and canBeReached
      // #3 Move one piece in a mill if !canBeReached by opposing player
      // #4 Start moving to block a mill of the opposing player when onePieceLeft
      // #5 Move a random piece

      // STEPS
      // #1 Create onePieceLeft vector

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

      // #2 Check if onePieceLeft is not empty and canBeReached

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
              if(pos2 != -1) return std::make_pair(pos1, pos2);
            }
          }
        }
      }

      // Move a random piece

      do
      {
        pos2 = rand() % 24;
      } while(vertices[pos2] != 0 || canBeReached(pos2, edges, vertices) == -1);

      pos1 = canBeReached(pos2, edges, vertices);

      return std::make_pair(pos1, pos2);
    }

    int canBeReached(int pos2, int edges [32][2], int vertices [24])
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

    std::pair<int,int> askFreeMovePositions()
    {
      // The AI's priorities are:
      // #1 Form a mill when onePieceLeft
      // #2 Block a mill of the opposing player when onePieceLeft
      // #3 Start forming a mill when twoPiecesLeft
      // #4 Block a mill of the opposing player when twoPiecesLeft

      int pos1, pos2;
      pos1 = 0;
      pos2 = 1;
      return std::make_pair(pos1, pos2);
    }
};
