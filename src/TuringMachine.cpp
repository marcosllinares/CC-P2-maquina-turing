#include "../include/TuringMachine.hpp"
#include <iostream>
#include <stdexcept>
#include <algorithm>

/**
 * @brief Constructor de la Máquina de Turing multicinta
 * @param states Conjunto de estados Q
 * @param input_alphabet Alfabeto de entrada Σ
 * @param tape_alphabet Alfabeto de cinta Γ
 * @param initial_state Estado inicial s
 * @param blank_symbol Símbolo blanco b
 * @param final_states Conjunto de estados finales F
 * @param num_tapes Número de cintas (por defecto 1)
 * @throws std::invalid_argument si la definición es inválida
 */
TuringMachine::TuringMachine(const std::set<std::string>& states,
                             const Alphabet& input_alphabet,
                             const Alphabet& tape_alphabet,
                             const std::string& initial_state,
                             char blank_symbol,
                             const std::set<std::string>& final_states,
                             size_t num_tapes)
  : states_(states),
    input_alphabet_(input_alphabet),
    tape_alphabet_(tape_alphabet),
    initial_state_(initial_state),
    blank_symbol_(blank_symbol),
    final_states_(final_states),
    num_tapes_(num_tapes),
    current_state_(initial_state),
    step_count_(0),
    halted_(false) {
  
  if (states_.empty()) {
    throw std::invalid_argument("El conjunto de estados no puede estar vacío");
  }
  
  if (states_.find(initial_state_) == states_.end()) {
    throw std::invalid_argument("El estado inicial debe pertenecer a Q");
  }
  
  for (const auto& state : final_states_) {
    if (states_.find(state) == states_.end()) {
      throw std::invalid_argument("Los estados finales deben pertenecer a Q");
    }
  }
  
  if (!tape_alphabet_.contains(blank_symbol_)) {
    throw std::invalid_argument("El símbolo blanco debe pertenecer a Γ");
  }
  
  if (num_tapes_ < 1) {
    throw std::invalid_argument("Debe haber al menos una cinta");
  }
  
  for (size_t i = 0; i < num_tapes_; ++i) {
    tapes_.emplace_back(blank_symbol_);
  }
}

/**
 * @brief Añade una transición a la función de transición δ
 * @param transition Transición a añadir
 * @throws std::invalid_argument si la transición es inválida
 */
void TuringMachine::addTransition(const Transition& transition) {
  if (states_.find(transition.getCurrentState()) == states_.end()) {
    throw std::invalid_argument(
      "El estado actual de la transición debe pertenecer a Q"
    );
  }
  
  if (states_.find(transition.getNextState()) == states_.end()) {
    throw std::invalid_argument(
      "El estado siguiente de la transición debe pertenecer a Q"
    );
  }
  
  if (transition.getNumTapes() != num_tapes_) {
    throw std::invalid_argument(
      "La transición debe tener el mismo número de cintas que la máquina"
    );
  }
  
  for (char symbol : transition.getReadSymbols()) {
    if (!tape_alphabet_.contains(symbol)) {
      throw std::invalid_argument(
        "Los símbolos leídos deben pertenecer a Γ"
      );
    }
  }
  
  for (char symbol : transition.getWriteSymbols()) {
    if (!tape_alphabet_.contains(symbol)) {
      throw std::invalid_argument(
        "Los símbolos escritos deben pertenecer a Γ"
      );
    }
  }

  // Comprobación de que el símbolo blanco no está incluido en el alfabeto de entrada
  for (char symbol : input_alphabet_.getSymbols()) {
    if (symbol == blank_symbol_) {
      throw std::invalid_argument(
        "El símbolo blanco no puede pertenecer al alfabeto de entrada Σ"
      );
    }
  }
  
  transitions_.push_back(transition);
}

/**
 * @brief Ejecuta la máquina de Turing con una cadena de entrada
 * @param input Cadena de entrada
 * @param max_steps Número máximo de pasos antes de abortar
 * @return true si la ejecución terminó (con o sin aceptación), false si se excedió max_steps o si no hay transición aplicable
 */
bool TuringMachine::run(const std::string& input, size_t max_steps) {
  for (char c : input) {
    if (!input_alphabet_.contains(c)) {
      throw std::invalid_argument(
        "La cadena de entrada contiene símbolos que no pertenecen a Σ"
      );
    }
  }
  
  initializeTapes(input);
  current_state_ = initial_state_;
  step_count_ = 0;
  halted_ = false;
  
  // Descomentar esta parte si quieres activar el modo traza
  // Imprimir configuración inicial
  // std::cout << "\n╔════════════════════════════════════════════════════╗\n";
  // std::cout << "║           TRAZA DE EJECUCIÓN - MT                  ║\n";
  // std::cout << "╚════════════════════════════════════════════════════╝\n\n";
  // printTrace();
  
  while (!halted_ && step_count_ < max_steps) {
    if (!executeStep()) {
      halted_ = true;
    }
    step_count_++;
  }
  
  // Imprimir configuración final
  // if (halted_) {
  //   std::cout << "\n╔════════════════════════════════════════════════════╗\n";
  //   std::cout << "║           CONFIGURACIÓN FINAL                      ║\n";
  //   std::cout << "╚════════════════════════════════════════════════════╝\n\n";
  //   std::cout << "Estado final: " << current_state_ << "\n";
  //   std::cout << "Total de pasos: " << step_count_ << "\n";
  //   std::cout << "Resultado: " << (isAccepted() ? "ACEPTADA" : "RECHAZADA") << "\n\n";
  // }
  
  return halted_;
}

/**
 * @brief Verifica si la máquina ha aceptado la cadena de entrada
 * @return true si el estado actual es un estado final
 */
bool TuringMachine::isAccepted() const {
  return final_states_.find(current_state_) != final_states_.end();
}

/**
 * @brief Obtiene el estado actual de la máquina
 * @return Estado actual
 */
std::string TuringMachine::getCurrentState() const {
  return current_state_;
}

/**
 * @brief Obtiene el contenido de todas las cintas
 * @return Vector con el contenido de cada cinta
 */
std::vector<std::string> TuringMachine::getTapesContent() const {
  std::vector<std::string> contents;
  for (const auto& tape : tapes_) {
    contents.push_back(tape.getContent());
  }
  return contents;
}

/**
 * @brief Obtiene el contenido de todas las cintas con la posición del cabezal
 * @return Vector con el contenido de cada cinta marcando el cabezal
 */
std::vector<std::string> TuringMachine::getTapesContentWithHead() const {
  std::vector<std::string> contents;
  for (const auto& tape : tapes_) {
    contents.push_back(tape.getContentWithHead());
  }
  return contents;
}

/**
 * @brief Obtiene el número de pasos ejecutados
 * @return Número de pasos
 */
size_t TuringMachine::getStepCount() const {
  return step_count_;
}

/**
 * @brief Reinicia la máquina al estado inicial
 */
void TuringMachine::reset() {
  current_state_ = initial_state_;
  step_count_ = 0;
  halted_ = false;
  
  for (auto& tape : tapes_) {
    tape.reset("");
  }
}

/**
 * @brief Imprime la configuración actual de la máquina
 */
void TuringMachine::printConfiguration() const {
  std::cout << "Estado: " << current_state_ << "\n";
  std::cout << "Pasos: " << step_count_ << "\n";
  
  for (size_t i = 0; i < tapes_.size(); ++i) {
    std::cout << "Cinta " << (i + 1) << ": " 
              << tapes_[i].getContentWithHead() << "\n";
  }
  
  std::cout << "Aceptado: " << (isAccepted() ? "Sí" : "No") << "\n";
}

/**
 * @brief Obtiene el resultado de la primera cinta (sin símbolo blanco al inicio/final)
 * @return Contenido de la primera cinta sin blancos en los extremos
 */
std::string TuringMachine::getResultFromFirstTape() const {
  if (tapes_.empty()) {
    return "";
  }
  
  std::string content = tapes_[0].getContent();
  
  size_t start = content.find_first_not_of(blank_symbol_);
  if (start == std::string::npos) {
    return std::string(1, blank_symbol_);
  }
  
  size_t end = content.find_last_not_of(blank_symbol_);
  return content.substr(start, end - start + 1);
}

/**
 * @brief Imprime la traza de ejecución paso a paso
 * @param transition Transición que se va a aplicar (nullptr si es el estado inicial)
 */
void TuringMachine::printTrace(const Transition* transition) const {
  std::cout << "────────────────────────────────────────────────────\n";
  std::cout << "Paso: " << step_count_ << "\n";
  std::cout << "Estado: " << current_state_ << "\n";
  
  // Mostrar las cintas con el cabezal
  for (size_t i = 0; i < tapes_.size(); ++i) {
    std::cout << "Cinta " << (i + 1) << ": " 
              << tapes_[i].getContentWithHead() << "\n";
  }
  
  // Si hay transición, mostrarla
  if (transition != nullptr) {
    std::cout << "\nTransición aplicada: " << transition->toString() << "\n";
  } else {
    std::cout << "\nConfiguración inicial\n";
  }
  
  std::cout << "────────────────────────────────────────────────────\n\n";
}

/**
 * @brief Busca una transición aplicable al estado y símbolos actuales
 * @param state Estado actual
 * @param symbols Símbolos actuales en las cintas
 * @return Puntero a la transición encontrada, nullptr si no existe
 */
const Transition* TuringMachine::findTransition(
    const std::string& state, 
    const std::vector<char>& symbols) const {
  
  for (const auto& transition : transitions_) {
    if (transition.matches(state, symbols)) {
      return &transition;
    }
  }
  
  return nullptr;
}

/**
 * @brief Ejecuta un paso de la máquina de Turing
 * @return true si se pudo ejecutar un paso, false si no hay transición aplicable
 */
bool TuringMachine::executeStep() {
  std::vector<char> current_symbols = readCurrentSymbols();
  
  const Transition* transition = findTransition(current_state_, current_symbols);
  
  if (transition == nullptr) {
    return false;
  }
  
  current_state_ = transition->getNextState();
  
  const std::vector<char>& write_symbols = transition->getWriteSymbols();
  const std::vector<Movement>& movements = transition->getMovements();
  
  for (size_t i = 0; i < num_tapes_; ++i) {
    tapes_[i].write(write_symbols[i]);
    
    switch (movements[i]) {
      case Movement::LEFT:
        tapes_[i].moveLeft();
        break;
      case Movement::RIGHT:
        tapes_[i].moveRight();
        break;
      case Movement::STAY:
        tapes_[i].moveStay();
        break;
    }
  }
  
  // Imprimir traza después de aplicar la transición
  // printTrace(transition);
  
  return true;
}

/**
 * @brief Inicializa las cintas con la cadena de entrada
 * @param input Cadena de entrada (se coloca en la primera cinta)
 */
void TuringMachine::initializeTapes(const std::string& input) {
  for (size_t i = 0; i < num_tapes_; ++i) {
    if (i == 0) {
      tapes_[i].reset(input);
    } else {
      tapes_[i].reset("");
    }
  }
}

/**
 * @brief Lee los símbolos actuales de todas las cintas
 * @return Vector con el símbolo actual de cada cinta
 */
std::vector<char> TuringMachine::readCurrentSymbols() const {
  std::vector<char> symbols;
  for (const auto& tape : tapes_) {
    symbols.push_back(tape.read());
  }
  return symbols;
}
