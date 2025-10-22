#include "../include/Alphabet.hpp"
#include <sstream>

/**
 * @brief Constructor por defecto (alfabeto vacío)
 */
Alphabet::Alphabet() : symbols_() {}

/**
 * @brief Constructor a partir de un vector de símbolos
 * @param symbols Vector de strings (cada string debe ser un símbolo de un carácter)
 */
Alphabet::Alphabet(const std::vector<std::string>& symbols) : symbols_() {
  for (const auto& symbol : symbols) {
    if (!symbol.empty()) {
      symbols_.insert(symbol[0]);
    }
  }
}

/**
 * @brief Constructor a partir de un set de caracteres
 * @param symbols Set de caracteres
 */
Alphabet::Alphabet(const std::set<char>& symbols) : symbols_(symbols) {}

/**
 * @brief Añade un símbolo al alfabeto
 * @param symbol Símbolo a añadir
 */
void Alphabet::addSymbol(char symbol) {
  symbols_.insert(symbol);
}

/**
 * @brief Verifica si un símbolo pertenece al alfabeto
 * @param symbol Símbolo a verificar
 * @return true si el símbolo pertenece al alfabeto, false en caso contrario
 */
bool Alphabet::contains(char symbol) const {
  return symbols_.find(symbol) != symbols_.end();
}

/**
 * @brief Obtiene el conjunto de símbolos
 * @return Set de caracteres del alfabeto
 */
const std::set<char>& Alphabet::getSymbols() const {
  return symbols_;
}

/**
 * @brief Obtiene el tamaño del alfabeto
 * @return Número de símbolos
 */
size_t Alphabet::size() const {
  return symbols_.size();
}

/**
 * @brief Verifica si el alfabeto está vacío
 * @return true si está vacío, false en caso contrario
 */
bool Alphabet::isEmpty() const {
  return symbols_.empty();
}

/**
 * @brief Representación en string del alfabeto
 * @return String con los símbolos del alfabeto
 */
std::string Alphabet::toString() const {
  std::ostringstream oss;
  oss << "{";
  bool first = true;
  for (char symbol : symbols_) {
    if (!first) {
      oss << ", ";
    }
    oss << symbol;
    first = false;
  }
  oss << "}";
  return oss.str();
}
