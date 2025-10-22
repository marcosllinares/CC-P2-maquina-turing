#include "../include/Tape.hpp"
#include <sstream>

/**
 * @brief Constructor con símbolo blanco (cinta vacía)
 * @param blank_symbol Símbolo blanco de la cinta
 */
Tape::Tape(char blank_symbol) 
  : tape_(1, blank_symbol), head_position_(0), blank_symbol_(blank_symbol) {}

/**
 * @brief Constructor con cadena de entrada
 * @param input Cadena inicial en la cinta
 * @param blank_symbol Símbolo blanco de la cinta
 */
Tape::Tape(const std::string& input, char blank_symbol)
  : blank_symbol_(blank_symbol), head_position_(0) {
  
  if (input.empty()) {
    tape_.push_back(blank_symbol_);
  } else {
    for (char c : input) {
      tape_.push_back(c);
    }
  }
}

/**
 * @brief Lee el símbolo en la posición actual del cabezal
 * @return Símbolo leído
 */
char Tape::read() const {
  return tape_[head_position_];
}

/**
 * @brief Escribe un símbolo en la posición actual del cabezal
 * @param symbol Símbolo a escribir
 */
void Tape::write(char symbol) {
  tape_[head_position_] = symbol;
}

/**
 * @brief Mueve el cabezal a la izquierda (L)
 * Expande la cinta si es necesario
 */
void Tape::moveLeft() {
  if (head_position_ == 0) {
    expandLeft();
  }
  head_position_--;
}

/**
 * @brief Mueve el cabezal a la derecha (R)
 * Expande la cinta si es necesario
 */
void Tape::moveRight() {
  head_position_++;
  if (head_position_ >= static_cast<int>(tape_.size())) {
    expandRight();
  }
}

/**
 * @brief Mantiene el cabezal en la posición actual (S)
 */
void Tape::moveStay() {
  // No hace nada, el cabezal permanece en su posición
}

/**
 * @brief Obtiene la posición actual del cabezal
 * @return Posición del cabezal
 */
int Tape::getHeadPosition() const {
  return head_position_;
}

/**
 * @brief Obtiene el contenido de la cinta como string
 * @return Contenido de la cinta
 */
std::string Tape::getContent() const {
  std::string content;
  for (char c : tape_) {
    content += c;
  }
  return content;
}

/**
 * @brief Obtiene el contenido de la cinta con la posición del cabezal marcada
 * @return String en formato "ab[c]de" donde [c] indica la posición del cabezal
 */
std::string Tape::getContentWithHead() const {
  std::ostringstream oss;
  for (int i = 0; i < static_cast<int>(tape_.size()); ++i) {
    if (i == head_position_) {
      oss << '[' << tape_[i] << ']';
    } else {
      oss << tape_[i];
    }
  }
  return oss.str();
}

/**
 * @brief Reinicia la cinta con una nueva cadena de entrada
 * @param input Nueva cadena de entrada
 */
void Tape::reset(const std::string& input) {
  tape_.clear();
  head_position_ = 0;
  
  if (input.empty()) {
    tape_.push_back(blank_symbol_);
  } else {
    for (char c : input) {
      tape_.push_back(c);
    }
  }
}

/**
 * @brief Expande la cinta hacia la izquierda
 * Añade un símbolo blanco al inicio y ajusta la posición del cabezal
 */
void Tape::expandLeft() {
  tape_.push_front(blank_symbol_);
  head_position_++;
}

/**
 * @brief Expande la cinta hacia la derecha
 * Añade un símbolo blanco al final
 */
void Tape::expandRight() {
  tape_.push_back(blank_symbol_);
}
