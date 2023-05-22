#ifndef CURSOR_POSITION_H
#define CURSOR_POSITION_H

class CursorPosition {
public:
  int index;
  int relPosition;
  int line;
  CursorPosition() {
    index = -1;
    relPosition = -1;
    line = -1;
  }

  CursorPosition(int _index, int _relPos, int _line) {
    index = _index;
    relPosition = _relPos;
    line = _line;
  }

  CursorPosition(CursorPosition &curPos) {
    index = curPos.index;
    relPosition = curPos.relPosition;
    line = curPos.line;
  }

  CursorPosition(const CursorPosition &curPos) {
    index = curPos.index;
    relPosition = curPos.relPosition;
    line = curPos.line;
  }

  void advance(char currentChar = '\0') {
    index ++;
    relPosition++;

    if (currentChar == '\n') {
      relPosition = 0;
      line++;
    }
  }
};

#endif