#include "../include/FileParser.hpp"
#include <algorithm>
#include <cctype>

/**
 * @brief Parsea un archivo de definición de MT y retorna la máquina configurada
 * @param filename Ruta al archivo de definición
 * @return TuringMachine configurada según el archivo
 * @throws std::runtime_error si el archivo no existe o el formato es inválido
 * 
 * Formato esperado (comentarios solo al inicio):
 * 1. Conjunto Q (estados)
 * 2. Conjunto Σ (alfabeto de entrada)
 * 3. Conjunto Γ (alfabeto de cinta)
 * 4. Estado inicial s
 * 5. Símbolo blanco b
 * 6. Conjunto F (estados finales)
 * 7. Número de cintas (opcional, para MT multicinta)
 * 8. Transiciones δ (una por línea)
 */
TuringMachine FileParser::parseMachineDefinition(const std::string& filename) {
  std::ifstream file(filename);
  if (!file.is_open()) {
    throw std::runtime_error("No se pudo abrir el archivo: " + filename);
  }

  try {
    // 1. Leer conjunto Q (estados)
    std::string line = readNextLine(file);
    std::vector<std::string> states = tokenize(line);
    if (states.empty()) {
      throw std::runtime_error("El conjunto Q no puede estar vacío");
    }

    // 2. Leer conjunto Σ (alfabeto de entrada)
    line = readNextLine(file);
    std::vector<std::string> sigma_tokens = tokenize(line);
    if (sigma_tokens.empty()) {
      throw std::runtime_error("El conjunto Σ no puede estar vacío");
    }

    // 3. Leer conjunto Γ (alfabeto de cinta)
    line = readNextLine(file);
    std::vector<std::string> gamma_tokens = tokenize(line);
    if (gamma_tokens.empty()) {
      throw std::runtime_error("El conjunto Γ no puede estar vacío");
    }

    // 4. Leer estado inicial s
    line = readNextLine(file);
    std::string initial_state = trim(line);
    if (initial_state.empty()) {
      throw std::runtime_error("El estado inicial no puede estar vacío");
    }

    // 5. Leer símbolo blanco b
    line = readNextLine(file);
    std::string blank_str = trim(line);
    if (blank_str.empty()) {
      throw std::runtime_error("El símbolo blanco no puede estar vacío");
    }
    char blank_symbol = blank_str[0];

    // 6. Leer conjunto F (estados finales)
    line = readNextLine(file);
    std::vector<std::string> final_states = tokenize(line);
    // F puede estar vacío (MT que no acepta nada)

    // 7. Leer número de cintas (opcional para MT multicinta)
    line = readNextLine(file);
    int num_tapes = 1; // Por defecto, 1 cinta
    std::vector<std::string> tape_tokens = tokenize(line);
    
    // Verificar si es un número (para multicinta) o una transición
    bool is_transition = false;
    if (!tape_tokens.empty()) {
      // Intentar convertir a número
      try {
        num_tapes = std::stoi(tape_tokens[0]);
        if (num_tapes < 1) {
          throw std::runtime_error("El número de cintas debe ser >= 1");
        }
      } catch (const std::invalid_argument&) {
        // No es un número, es la primera transición
        is_transition = true;
        num_tapes = 1;
      }
    }

    // 8. Leer transiciones δ
    std::vector<std::string> transitions;
    
    // Si la línea anterior era una transición, procesarla primero
    if (is_transition) {
      transitions.push_back(line);
    }

    // Leer el resto de transiciones
    while (std::getline(file, line)) {
      line = trim(line);
      if (!isCommentOrEmpty(line)) {
        transitions.push_back(line);
      }
    }

    file.close();

    // TODO: Crear y configurar la TuringMachine con los datos parseados
    // Por ahora retornamos una instancia vacía
    TuringMachine tm;
    
    return tm;

  } catch (const std::exception& e) {
    file.close();
    throw std::runtime_error("Error al parsear " + filename + ": " + e.what());
  }
}

/**
 * @brief Lee cadenas de entrada desde un archivo (una por línea)
 * @param filename Ruta al archivo de entrada
 * @return Vector de cadenas de entrada
 * @throws std::runtime_error si el archivo no existe
 */
std::vector<std::string> FileParser::parseInputStrings(const std::string& filename) {
  std::ifstream file(filename);
  if (!file.is_open()) {
    throw std::runtime_error("No se pudo abrir el archivo de entrada: " + filename);
  }

  std::vector<std::string> input_strings;
  std::string line;
  
  // Leer todas las líneas como cadenas de entrada
  while (std::getline(file, line)) {
    input_strings.push_back(line);
  }

  file.close();
  return input_strings;
}

/**
 * @brief Lee la siguiente línea no vacía y no comentario del archivo
 * @param file Stream del archivo
 * @return Línea leída (sin espacios al inicio/final)
 * @throws std::runtime_error si llega al final del archivo
 */
std::string FileParser::readNextLine(std::ifstream& file) {
  std::string line;
  while (std::getline(file, line)) {
    line = trim(line);
    // Ignorar líneas vacías y comentarios
    if (!isCommentOrEmpty(line)) {
      return line;
    }
  }
  throw std::runtime_error("Fin de archivo inesperado");
}

/**
 * @brief Divide una línea en tokens separados por espacios
 * @param line Línea a dividir
 * @return Vector de tokens
 */
std::vector<std::string> FileParser::tokenize(const std::string& line) {
  std::vector<std::string> tokens;
  std::istringstream iss(line);
  std::string token;
  
  while (iss >> token) {
    tokens.push_back(token);
  }
  
  return tokens;
}

/**
 * @brief Elimina espacios en blanco al inicio y final de una cadena
 * @param str Cadena a procesar
 * @return Cadena sin espacios al inicio/final
 */
std::string FileParser::trim(const std::string& str) {
  if (str.empty()) return str;
  
  // Encontrar el primer carácter no espacio
  size_t start = 0;
  while (start < str.length() && std::isspace(static_cast<unsigned char>(str[start]))) {
    ++start;
  }
  
  // Encontrar el último carácter no espacio
  size_t end = str.length();
  while (end > start && std::isspace(static_cast<unsigned char>(str[end - 1]))) {
    --end;
  }
  
  return str.substr(start, end - start);
}

/**
 * @brief Verifica si una línea es un comentario o está vacía
 * @param line Línea a verificar
 * @return true si es comentario o vacía, false en caso contrario
 */
bool FileParser::isCommentOrEmpty(const std::string& line) {
  return line.empty() || line[0] == '#';
}
