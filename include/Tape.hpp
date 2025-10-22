#pragma once

#include <string>
#include <vector>
#include <deque>

class Tape {
public:
  Tape(char blank_symbol = '.');
  Tape(const std::string& input, char blank_symbol = '.');
  
  char read() const;
  void write(char symbol);
  void moveLeft();
  void moveRight();
  void moveStay();
  
  int getHeadPosition() const;
  std::string getContent() const;
  std::string getContentWithHead() const;
  void reset(const std::string& input);
  
private:
  std::deque<char> tape_;
  int head_position_;
  char blank_symbol_;
  
  void expandLeft();
  void expandRight();
};
