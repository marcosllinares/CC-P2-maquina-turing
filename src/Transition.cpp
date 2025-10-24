#include "../include/Transition.hpp"
#include <sstream>
#include <stdexcept>

/**
 * @brief Constructor de una transición multicinta
 * @param current_state Estado actual de la transición
 * @param read_symbols Vector de símbolos leídos (uno por cinta)
 * @param next_state Estado siguiente de la transición
 * @param write_symbols Vector de símbolos a escribir (uno por cinta)
 * @param movements Vector de movimientos (uno por cinta)
 * @throws std::invalid_argument si los vectores no tienen el mismo tamaño
 */
Transition::Transition(const std::string& current_state,
                       const std::vector<char>& read_symbols,
                       const std::string& next_state,
                       const std::vector<char>& write_symbols,
                       const std::vector<Movement>& movements)
  : current_state_(current_state),
    read_symbols_(read_symbols),
    next_state_(next_state),
    write_symbols_(write_symbols),
    movements_(movements) {
  
  if (read_symbols_.size() != write_symbols_.size() || 
      read_symbols_.size() != movements_.size()) {
    throw std::invalid_argument(
      "Los vectores de símbolos y movimientos deben tener el mismo tamaño"
    );
  }
  
  if (read_symbols_.empty()) {
    throw std::invalid_argument(
      "Una transición debe tener al menos una cinta"
    );
  }
}

/**
 * @brief Obtiene el estado actual de la transición
 * @return Estado actual
 */
const std::string& Transition::getCurrentState() const {
  return current_state_;
}

/**
 * @brief Obtiene los símbolos leídos de todas las cintas
 * @return Vector de símbolos leídos
 */
const std::vector<char>& Transition::getReadSymbols() const {
  return read_symbols_;
}

/**
 * @brief Obtiene el estado siguiente de la transición
 * @return Estado siguiente
 */
const std::string& Transition::getNextState() const {
  return next_state_;
}

/**
 * @brief Obtiene los símbolos a escribir en todas las cintas
 * @return Vector de símbolos a escribir
 */
const std::vector<char>& Transition::getWriteSymbols() const {
  return write_symbols_;
}

/**
 * @brief Obtiene los movimientos para todas las cintas
 * @return Vector de movimientos
 */
const std::vector<Movement>& Transition::getMovements() const {
  return movements_;
}

/**
 * @brief Verifica si la transición aplica para un estado y símbolos dados
 * @param state Estado actual de la máquina
 * @param symbols Símbolos actuales en las cintas
 * @return true si la transición aplica, false en caso contrario
 */
bool Transition::matches(const std::string& state, 
                        const std::vector<char>& symbols) const {
  if (state != current_state_) {
    return false;
  }
  
  if (symbols.size() != read_symbols_.size()) {
    return false;
  }
  
  for (size_t i = 0; i < symbols.size(); ++i) {
    if (symbols[i] != read_symbols_[i]) {
      return false;
    }
  }
  
  return true;
}

/**
 * @brief Obtiene el número de cintas que maneja esta transición
 * @return Número de cintas
 */
size_t Transition::getNumTapes() const {
  return read_symbols_.size();
}

/**
 * @brief Representación en string de la transición
 * @return String en formato "q0 a b -> q1 X Y R L"
 */
std::string Transition::toString() const {
  std::ostringstream oss;
  
  oss << current_state_ << " ";
  
  for (char symbol : read_symbols_) {
    oss << symbol << " ";
  }
  
  oss << "-> " << next_state_ << " ";
  
  for (char symbol : write_symbols_) {
    oss << symbol << " ";
  }
  
  for (Movement move : movements_) {
    oss << movementToChar(move) << " ";
  }
  
  return oss.str();
}

/**
 * @brief Convierte un carácter en un movimiento
 * @param c Carácter ('L', 'R', o 'S')
 * @return Movimiento correspondiente
 * @throws std::invalid_argument si el carácter no es válido
 */
Movement Transition::charToMovement(char c) {
  switch (c) {
    case 'L':
    case 'l':
      return Movement::LEFT;
    case 'R':
    case 'r':
      return Movement::RIGHT;
    case 'S':
    case 's':
      return Movement::STAY;
    default:
      throw std::invalid_argument(
        std::string("Movimiento inválido: ") + c
      );
  }
}

/**
 * @brief Convierte un movimiento en un carácter
 * @param m Movimiento
 * @return Carácter correspondiente ('L', 'R', o 'S')
 */
char Transition::movementToChar(Movement m) {
  switch (m) {
    case Movement::LEFT:
      return 'L';
    case Movement::RIGHT:
      return 'R';
    case Movement::STAY:
      return 'S';
    default:
      return '?';
  }
}
