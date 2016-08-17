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
};
