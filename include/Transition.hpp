#pragma once

#include <string>
#include <vector>

enum class Movement {
  LEFT,
  RIGHT,
  STAY
};

/**
 * @brief Representa una transición de la Máquina de Turing multicinta
 * 
 * Para multicinta: δ(q, [s1, s2, ..., sn]) = (q', [s1', s2', ..., sn'], [m1, m2, ..., mn])
 * Movimientos: L (izquierda), R (derecha), S (sin movimiento)
 * Escritura y movimientos simultáneos
 */
class Transition {
public:
  Transition(const std::string& current_state,
             const std::vector<char>& read_symbols,
             const std::string& next_state,
             const std::vector<char>& write_symbols,
             const std::vector<Movement>& movements);
  
  const std::string& getCurrentState() const;
  const std::vector<char>& getReadSymbols() const;
  const std::string& getNextState() const;
  const std::vector<char>& getWriteSymbols() const;
  const std::vector<Movement>& getMovements() const;
  
  bool matches(const std::string& state, const std::vector<char>& symbols) const;
  size_t getNumTapes() const;
  std::string toString() const;
  
  static Movement charToMovement(char c);
  static char movementToChar(Movement m);

private:
  std::string current_state_;
  std::vector<char> read_symbols_;
  std::string next_state_;
  std::vector<char> write_symbols_;
  std::vector<Movement> movements_;
};
