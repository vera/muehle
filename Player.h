#ifndef _PLAYER_h
#define _PLAYER_h
#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

class Player {
  protected:
    // Player ID
    int id;
    // Amount of pieces the player has left
    int pieces;
    // Vector that contains the positions of the player's pieces on board
    vector<int> piecesOnBoard;

  public:
    // Constructor
    Player(int id);

    // Getters
    int getID();
    int getPieces();
    int getPiecesOnBoard();
    vector<int> getPiecesOnBoardVector();

    // Setters
    void incPieces();

    /*!
     *  Update pieces and piecesOnBoard
     *  These methods are called when the player makes a turn (places a piece, removes a piece, moves a piece on the board)
     *
     *  Parameters: pos: The position where a piece was just placed/removed
     *              pos1: The position where a piece was just moved from
     *              pos2: The position where a piece was just moved to
     */

    void movePieceToBoard(int pos);
    void removePieceFromBoard(int pos);
    void movePieceOnBoard(int pos1, int pos2);

    /*!
     *  Checks if the player has unprotected pieces on board
     *  This method is called when the opposing player forms a mill
     *  If the player has no unprotected pieces on board, the opposing player will not be allowed to remove a piece
     *
     *  Parameters: protectedPoints: All protected points on the board that cannot be removed
     *
     *  Returns:    True if the player has unprotected pieces on board, false if not
     */

    bool hasUnprotectedPiecesOnBoard(vector<int> protectedPieces);

    /*!
     *  Checks if the player is out of pieces
     *  This method is called to determine when all pieces have been placed and game phase 1 ends
     *
     *  Returns:    True if the player is out of pieces, false if not
     */

    bool isOutOfPieces();

    /*!
     *  Resets the player
     *  This method is called when the "Restart game" button is pressed
     */

    void reset();
};

#endif // _PLAYER_h
