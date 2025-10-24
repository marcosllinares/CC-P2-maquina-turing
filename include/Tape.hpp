#pragma once

#include <string>
#include <vector>
#include <deque>

/**
 * @brief Representa la cinta de una máquina de Turing con capacidad de expansión infinita.
 * 
 * La cinta permite lectura, escritura y movimiento del cabezal en ambas direcciones.
 * Se expande automáticamente cuando es necesario usando un símbolo en blanco configurable.
 */
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
  char blank_symbol_;
  int head_position_;
  
  void expandLeft();
  void expandRight();
};
