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
    std::vector<std::string> final_states_vec = tokenize(line);
    std::set<std::string> final_states(final_states_vec.begin(), final_states_vec.end());

    // 7. Leer número de cintas (opcional para MT multicinta)
    line = readNextLine(file);
    size_t num_tapes = 1; // Por defecto 1 cinta
    std::vector<std::string> tape_tokens = tokenize(line);
    
    // Verificar si es número de cintas o ya es una transición
    bool is_transition = false;
    if (!tape_tokens.empty()) {
      // Intentar convertir el primer token a un número
      try {
        int tapes = std::stoi(tape_tokens[0]);
        if (tapes < 1) {
          throw std::runtime_error("El número de cintas debe ser >= 1");
        }
        num_tapes = static_cast<size_t>(tapes);
      } catch (const std::invalid_argument&) {
        // No es un número, es una transición
        is_transition = true;
        num_tapes = 1;
      }
    }

    std::set<std::string> states_set(states.begin(), states.end());
    Alphabet input_alpha(sigma_tokens);
    Alphabet tape_alpha(gamma_tokens);
    
    TuringMachine tm(states_set, input_alpha, tape_alpha, 
                     initial_state, blank_symbol, final_states, num_tapes);

    // 8. Leer transiciones δ
    // Si la línea anterior era una transición, procesarla primero
    if (is_transition) {
      std::vector<std::string> trans_tokens = tokenize(line);
      Transition t = parseTransition(trans_tokens, num_tapes);
      tm.addTransition(t);
    }

    // Leer el resto de transiciones
    while (std::getline(file, line)) {
      line = trim(line);
      if (!isCommentOrEmpty(line)) {
        std::vector<std::string> trans_tokens = tokenize(line);
        Transition t = parseTransition(trans_tokens, num_tapes);
        tm.addTransition(t);
      }
    }

    file.close();
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

/**
 * @brief Parsea una transición desde tokens
 * @param tokens Vector de tokens de la línea de transición
 * @param num_tapes Número de cintas de la máquina
 * @return Transición parseada
 * @throws std::runtime_error si el formato es inválido
 * 
 * Formato esperado:
 * - 1 cinta: q0 a q1 X R (5 tokens)
 * - 2 cintas: q0 a b q1 X Y R L (8 tokens)
 * - n cintas: q0 s1..sn q1 w1..wn m1..mn (3n+2 tokens)
 */
Transition FileParser::parseTransition(const std::vector<std::string>& tokens, 
                                       size_t num_tapes) {
  size_t expected_tokens = 3 * num_tapes + 2;
  
  if (tokens.size() != expected_tokens) {
    throw std::runtime_error(
      "Formato de transición inválido. Esperados " + 
      std::to_string(expected_tokens) + " tokens, encontrados " + 
      std::to_string(tokens.size())
    );
  }
  
  std::string current_state = tokens[0];
  
  std::vector<char> read_symbols;
  for (size_t i = 0; i < num_tapes; ++i) {
    if (tokens[1 + i].empty()) {
      throw std::runtime_error("Símbolo de lectura vacío en la transición");
    }
    read_symbols.push_back(tokens[1 + i][0]);
  }
  
  std::string next_state = tokens[1 + num_tapes];
  
  std::vector<char> write_symbols;
  for (size_t i = 0; i < num_tapes; ++i) {
    if (tokens[2 + num_tapes + i].empty()) {
      throw std::runtime_error("Símbolo de escritura vacío en la transición");
    }
    write_symbols.push_back(tokens[2 + num_tapes + i][0]);
  }
  
  std::vector<Movement> movements;
  for (size_t i = 0; i < num_tapes; ++i) {
    if (tokens[2 + 2 * num_tapes + i].empty()) {
      throw std::runtime_error("Movimiento vacío en la transición");
    }
    movements.push_back(Transition::charToMovement(tokens[2 + 2 * num_tapes + i][0]));
  }
  
  return Transition(current_state, read_symbols, next_state, write_symbols, movements);
}
