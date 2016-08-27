#ifndef _EXCEPTIONS_h
#define _EXCEPTIONS_h

#include <exception>

using namespace std;

class OutOfPiecesException : public exception {
  const char * what () const throw () {
    return "You are out of pieces.";
  }
};

class VertixNotEmptyException : public exception
{
  const char * what () const throw () {
    return "A piece has already been placed in this position.";
  }
};

class IllegalMoveException : public exception {
  const char * what() const throw() {
    return "This move isn't possible.";
  }
};

class IllegalRemoveException : public exception {
  const char * what() const throw() {
    return "This piece cannot be removed.";
  }
};

#endif // _EXCEPTIONS_h
