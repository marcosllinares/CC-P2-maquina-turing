#include <fstream>
#include <iostream>

#include "../include/PDA.hpp"
#include "../include/Parser.hpp"
#include "../include/TraceLogger.hpp"

void printPDAStatus(const PDA &pda, bool trace, std::string filename) {
  std::cout << "\nSuccessfully parsed PDA from file: " << filename << std::endl;

  // Mostrar información básica sobre el PDA analizado
  std::cout << "=== PDA Information ===" << std::endl;
  std::cout << "Acceptance Mode: " << (pda.acceptanceMode() == AcceptanceMode::FinalState ? "Final State" : "Empty Stack")
            << std::endl;

  std::cout << "States: " << pda.states().size() << std::endl;
  std::cout << "Initial State: " << pda.initialState().getValue() << std::endl;
  std::cout << "Initial Stack Symbol: " << pda.initialStackSymbol().getValue() << std::endl;

  std::cout << "Final States: ";
  for (const auto &state : pda.finalStates()) {
    std::cout << state.GetId() << " ";
  }
  std::cout << std::endl;

  std::cout << "Input Alphabet Size: " << pda.chainAlphabet().GetSize() << std::endl;
  std::cout << "Stack Alphabet Size: " << pda.stackAlphabet().GetSize() << std::endl;
  std::cout << "Transitions: " << pda.transitions().size() << std::endl;

  if (trace) {
    std::cout << "\n=== Transitions ===" << std::endl;
    for (const auto &transition : pda.transitions()) {
  std::cout << "(" << transition.getFromState().GetId() << ", " << transition.getInputSymbol().getValue() << ", "
        << transition.getStackPopSymbol().getValue() << ") -> (" << transition.getToState().GetId() << ", ";

      if (transition.getStackPushSymbols().empty()) {
        std::cout << ".";
      } else {
        for (const auto &symbol : transition.getStackPushSymbols()) {
          std::cout << symbol.getValue();
        }
      }
      std::cout << ")" << std::endl;
    }
  }
}

int main(int argc, char **argv) {
  std::cout << "PDA Simulator\n";

  // Usage: requiere definición de PDA y archivo de entradas
  if (argc < 3 || argc > 4) {
    std::cout << "Usage: ./bin/pda <definition_file> <inputs_file> [--trace]" << std::endl;
    return 1;
  }

  std::string filename = argv[1];
  std::string inputsFile = argv[2];
  bool trace = false;
  std::string mode = "APf"; // Modo por defecto y único implementado

  // Analizar Flag opcional para trazado
  if (argc == 4 && argv[3] == "--trace") {
    trace = true;
  }

  try {
    // Parsear el PDA y las entradas
    Parser parser;
    PDA pda = parser.parsePDAFromFile(filename);
    std::vector<std::string> inputs = parser.parseInputFromFile(inputsFile);
    //printPDAStatus(pda, trace, filename);

    pda.validatePDA();

    // Iterar sobre todas las cadenas de entrada y ejecutar la lógica existente por entrada
    for (const auto &input : inputs) {
      std::cout << "\n--- Processing input: " << input << " ---" << std::endl;
      // Preparar pila inicial
      std::stack<Symbol> initial_stack;
      initial_stack.push(pda.initialStackSymbol());

      // Estado inicial
      State init_state(pda.initialState().getValue());

      bool accepted = pda.accepts_recursive(input, 0, init_state, initial_stack);
      std::cout << "Result: " << (accepted ? "ACCEPTED" : "REJECTED") << std::endl;

      // Si trace es true, se podría inicializar TraceLogger y pasar a la simulación
    }
    return 0;

  } catch (const std::exception &e) {
    std::cerr << "Error: " << e.what() << std::endl;
    return 1;
  }
}
