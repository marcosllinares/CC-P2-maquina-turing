#pragma once

#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <stdexcept>
#include "TuringMachine.hpp"

/**
 * @brief Parser para archivos de definición y entrada de la Máquina de Turing
 * 
 * Lee archivos de configuración y valida la definición formal
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
class FileParser {
public:
  /**
   * @brief Parsea un archivo de definición de MT y retorna la máquina configurada
   * @param filename Ruta al archivo de definición
   * @return TuringMachine configurada según el archivo
   * @throws std::runtime_error si el archivo no existe o el formato es inválido
   */
  static TuringMachine parseMachineDefinition(const std::string& filename);

  /**
   * @brief Lee cadenas de entrada desde un archivo (una por línea)
   * @param filename Ruta al archivo de entrada
   * @return Vector de cadenas de entrada
   * @throws std::runtime_error si el archivo no existe
   */
  static std::vector<std::string> parseInputStrings(const std::string& filename);

private:
  /**
   * @brief Lee la siguiente línea no vacía y no comentario del archivo
   * @param file Stream del archivo
   * @return Línea leída (sin espacios al inicio/final)
   * @throws std::runtime_error si llega al final del archivo
   */
  static std::string readNextLine(std::ifstream& file);

  /**
   * @brief Divide una línea en tokens separados por espacios
   * @param line Línea a dividir
   * @return Vector de tokens
   */
  static std::vector<std::string> tokenize(const std::string& line);

  /**
   * @brief Elimina espacios en blanco al inicio y final de una cadena
   * @param str Cadena a procesar
   * @return Cadena sin espacios al inicio/final
   */
  static std::string trim(const std::string& str);

  /**
   * @brief Verifica si una línea es un comentario o está vacía
   * @param line Línea a verificar
   * @return true si es comentario o vacía, false en caso contrario
   */
  static bool isCommentOrEmpty(const std::string& line);
};
