#pragma once

#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <stdexcept>
#include "TuringMachine.hpp"


/**
 * @brief Clase de utilidad para analizar archivos de configuración y entrada de Máquinas de Turing.
 * 
 * Proporciona métodos estáticos para leer y analizar definiciones de Máquinas de Turing y cadenas de entrada desde archivos.
 */
class FileParser {
public:
  static TuringMachine parseMachineDefinition(const std::string& filename);
  static std::vector<std::string> parseInputStrings(const std::string& filename);

private:
  static std::string readNextLine(std::ifstream& file);
  static std::vector<std::string> tokenize(const std::string& line);
  static std::string trim(const std::string& str);
  static bool isCommentOrEmpty(const std::string& line);
  static Transition parseTransition(const std::vector<std::string>& tokens, size_t num_tapes);
};
