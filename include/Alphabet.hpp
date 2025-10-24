#pragma once

#include <set>
#include <string>
#include <vector>

/**
 * @brief Clase que representa un alfabeto de símbolos para una máquina de Turing.
 * 
 * Gestiona un conjunto de símbolos únicos representados como caracteres.
 */
class Alphabet {
public:
  Alphabet();
  Alphabet(const std::vector<std::string>& symbols);
  Alphabet(const std::set<char>& symbols);
  
  void addSymbol(char symbol);
  bool contains(char symbol) const;
  const std::set<char>& getSymbols() const;
  size_t size() const;
  bool isEmpty() const;
  std::string toString() const;

private:
  std::set<char> symbols_;
};
