#include <fstream>
#include <iostream>
#include <vector>
#include "../include/FileParser.hpp"
#include "../include/TuringMachine.hpp"

/**
 * @brief Metodo que muestra el uso correcto del programa
 * @param program_name Nombre del ejecutable
 */
void Usage(const std::string& program_name) {
  std::cerr << "Uso: " << program_name << " <definition_file> <input_file>\n\n";
  std::cerr << "Argumentos:\n";
  std::cerr << "  definition_file  Archivo con la definición de la Máquina de Turing\n";
  std::cerr << "  input_file       Archivo con la cadena de entrada\n\n";
  std::cerr << "Ejemplo:\n";
  std::cerr << "  " << program_name << " data/Definitions/Ejemplo_MT.txt data/Inputs/input1.txt\n";
}

int main(int argc, char **argv) {
  if (argc != 3) {
    Usage(argv[0]);
    return 1;
  }

  std::string definition_file = argv[1];
  std::string input_file = argv[2];

  try {
    TuringMachine tm = FileParser::parseMachineDefinition(definition_file);
    std::vector<std::string> input_strings = FileParser::parseInputStrings(input_file);

    for (size_t i = 0; i < input_strings.size(); ++i) {
      const std::string& input = input_strings[i];

      std::cout << "---------------------------------------------------\n";
      std::cout << "Cadena de entrada #" << (i + 1) << ": \"" << input << "\"\n";
      std::cout << "---------------------------------------------------\n\n";

      bool finished = tm.run(input);
      
      if (!finished) {
        std::cout << "La máquina excedió el número máximo de pasos\n\n";
      }
      
      std::cout << "Resultado: " << (tm.isAccepted() ? "ACEPTADA" : "RECHAZADA") << "\n";
      std::cout << "Estado final: " << tm.getCurrentState() << "\n";
      std::cout << "Pasos ejecutados: " << tm.getStepCount() << "\n\n";
      
      std::vector<std::string> tapes_content = tm.getTapesContentWithHead();
      for (size_t j = 0; j < tapes_content.size(); ++j) {
        std::cout << "Cinta " << (j + 1) << ": " << tapes_content[j] << "\n";
      }
      
      std::cout << "\n";
      tm.reset();
    }

  } catch (const std::exception& e) {
    std::cerr << "Error: " << e.what() << std::endl;
    return 1;
  }

  return 0;
}
