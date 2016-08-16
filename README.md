# Mühle

The popular boardgame ["Nine Men's Morris"](https://en.wikipedia.org/wiki/Nine_Men%27s_Morris) or "Mühle" in German, created using C++ and Qt5.

## How to setup

Clone the repository:

`git clone https://github.com/veracl/muehle.git`

Change directory:

`cd muehle`

Create the Makefile:

`qmake muehle.pro`

Now run:

`make`

Done! You can start the game:

`./muehle`

**Alternatively**: Download and install Qt Creator ([link](https://www.qt.io/download/)), open the project and build & run from there.

## To-Do

- [ ] Implement game phase 2 and 3

- [ ] Figure out how to deploy properly

## Code issues

- [ ] detectMuehle[1] may be unneccessary

- [ ] mill/Muehle naming issue

- [ ] Crashes for unknown reasons

- [x] No delay before AI makes a move feels weird, especially when it removes a piece you just placed

## Additional features

- [ ] "Restart game" button

- [ ] Let AI make the first move

- [ ] Timer (how long has this game been going)

- [ ] Save permanent stats such as Games won, Games lost, etc.
