// Define player ID of the computer, default is 2
#define AI_PLAYER_ID 2

#include "Player.h"
#include "Exceptions.h"
#include <vector>
#include <ctime>

// To-Do:

// Possibly eliminate all these arguments, instead use Board as only argument
// and implement getters in Board

// Iterate through the piecesOnBoard vector instead of checking every single vertix

class AIPlayer : public Player {
  public:
    static constexpr int getID() { return AI_PLAYER_ID; }

    AIPlayer() : Player(AI_PLAYER_ID) {
      srand(time(NULL));
    }

    int askPlacePosition(int vertices [24], int possibleMuehlePositions [16][4])
    {
      // Goal: Create a muehle
      // Go through vertices == 2
      // Go through possible Muehlen, update vectors "1 left", "2 left", "impossible" with # of Muehle
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
            if(possibleMuehlePositions[j][0] == i || possibleMuehlePositions[j][1] == i || possibleMuehlePositions[j][2] == i)
            {
              if(vertices[possibleMuehlePositions[j][0]] == 1 || vertices[possibleMuehlePositions[j][1]] == 1 || vertices[possibleMuehlePositions[j][2]] == 1)
              {
                impossible.push_back(j);
              }
              else
              {
                if(std::find(onePieceLeft.begin(), onePieceLeft.end(), j) != onePieceLeft.end())
                {
                  onePieceLeft.erase(std::find(onePieceLeft.begin(), onePieceLeft.end(), j));
                }
                else if(std::find(twoPiecesLeft.begin(), twoPiecesLeft.end(), j) != twoPiecesLeft.end())
                {
                  twoPiecesLeft.erase(std::find(twoPiecesLeft.begin(), twoPiecesLeft.end(), j));
                  onePieceLeft.push_back(j);
                }
                else
                {
                  twoPiecesLeft.push_back(j);
                }
                // If j is already in onePieceLeft, it is already a completed Muehle, do nothing
              }
            }
          }
        //}
      }

      int muehleNr;

      if(!(onePieceLeft.empty()))
      {
        r = rand() % onePieceLeft.size();
        muehleNr = onePieceLeft.at(r);
        for(int i = 0; i < 3; i++)
        {
          if(vertices[possibleMuehlePositions[muehleNr][i]] == 0) return possibleMuehlePositions[muehleNr][i];
        }
      }
      else if(!(twoPiecesLeft.empty()))
      {
        r = rand() % twoPiecesLeft.size();
        muehleNr = twoPiecesLeft.at(r);
        for(int i = 0; i < 3; i++)
        {
          if(vertices[possibleMuehlePositions[muehleNr][i]] == 0) return possibleMuehlePositions[muehleNr][i];
        }
      }
      else if(!(impossible.empty()))
      {
        r = rand() % impossible.size();
        muehleNr = impossible.at(r);
        for(int i = 0; i < 3; i++)
        {
          if(vertices[possibleMuehlePositions[muehleNr][i]] == 0) return possibleMuehlePositions[muehleNr][i];
        }
      }
    }

    int askRemovePosition(int vertices [24], int possibleMuehlePositions [16][4], vector<int> protectedPoints)
    {
      // Go through all vertices == 1
      // Check if two of them are part of a possible Muehle
      // If yes, return the one that is part of the most possible Muehlen
      // If no, return a random

      int involvedInPotentialMuehlen [24] = {0};
      int involvedInMostPotentialMuehlen = 24;

      for(int i = 0; i < 24; i++)
      {
        if(vertices[i] == 1)
        {
          for(int j = 0; j < 16; j++)
          {
            if(possibleMuehlePositions[j][0] == i || possibleMuehlePositions[j][1] == i || possibleMuehlePositions[j][2] == i)
            {
              if(vertices[possibleMuehlePositions[j][0]] != 2 && vertices[possibleMuehlePositions[j][1]] != 2 && vertices[possibleMuehlePositions[j][2]] != 2)
              {
                // Piece i is involved in a possible Muehle
                involvedInPotentialMuehlen[i]++;
                if(involvedInPotentialMuehlen[i] > involvedInPotentialMuehlen[involvedInMostPotentialMuehlen])
                {
                  involvedInMostPotentialMuehlen = i;
                }
              }
            }
          }
        }
      }

      if((involvedInMostPotentialMuehlen = 24))
      {
        int r;
        do
        {
          r = rand() % 24;
        } while(vertices[r] != 1 || std::find(protectedPoints.begin(), protectedPoints.end(), r) != protectedPoints.end());

        return r;
      }

      return involvedInMostPotentialMuehlen;
    }
};
