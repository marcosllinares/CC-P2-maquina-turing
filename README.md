# Práctica 02 - Máquina de Turing
Práctica que implementa un simulador de Máquina de Turing determinista multicinta.

Enlace al repositorio: [https://github.com/marcosllinares/CC-P2-maquina-turing](https://github.com/marcosllinares/CC-P2-maquina-turing)

Autor: Trabajo realizado por Marcos Llinares Montes (alu0100972443)

**Características de implementación** (Opciones seleccionadas)
- Máquina de Turing con **escritura y movimientos simultáneos**
- Movimientos: **L** (izquierda), **R** (derecha), **S** (sin movimiento)
- Cinta **infinita en ambas direcciones**
- Soporte para **Máquina de Turing multicinta**

**Explicación codificación Máquina de Turing multicinta:**
```
q0 q1 q2          # Q
a b               # Σ
a b X Y           # Γ
q0                # s
·                 # b
q2                # F
2                 # num_cintas 
q0 a b q1 X Y R L # transiciones
q1 a . q2 Y X R L
...
```

Explicación transición: `q0 a b q1 X Y R L`
- Estado actual: `q0`
- Símbolos leídos: `a` (cinta 1), `b` (cinta 2)
- Estado siguiente: `q1`
- Símbolos escritos: `X` (cinta 1), `Y` (cinta 2)
- Movimientos: `R` (cinta 1), `L` (cinta 2)

## Estructura del proyecto
- `include/` : cabeceras (.hpp)
- `src/`     : implementación (.cpp)
- `data/`    : archivos de ejemplo (definiciones de MT y entradas)
- `build/`, `bin/` : artefactos de compilación (creados por `make`)

```
.
├── Makefile
├── README.md
├── data/
│   ├── Definitions/
│   │   ├── Ejemplo_MT.txt
│   │   └── Ejemplo2_MT.txt
│   │   └── Problema1_MT.txt
│   │   └── Problema2_MT.txt
│   └── Inputs/
│   │   ├── input1.txt
│   │   └── input2.txt
│   │   ├── input_Problema1.txt
│   │   ├── input_Problema2.txt
├── include/
│   ├── Alphabet.hpp
│   ├── FileParser.hpp
│   ├── Tape.hpp
│   ├── Transition.hpp
│   └── TuringMachine.hpp
└── src/
    ├── Alphabet.cpp
    ├── FileParser.cpp
    ├── main.cpp
    ├── Tape.cpp
    ├── Transition.cpp
    └── TuringMachine.cpp
```

## Compilación
El proyecto se compila mediante `make` (Makefile incluido). Basta ejecutar desde la raíz del proyecto:

```sh
make
```

El binario resultante es `bin/MT`

## Ejecución
Uso básico:

```sh
./bin/MT <definition_file> <input_file>
```

- `definition_file`: fichero con la definición de la MT (ej. `data/Definitions/Ejemplo_MT.txt`)
- `input_file`: fichero con la cadena de entrada (ej. `data/Inputs/input1.txt`)

Ejemplo:

```sh
./bin/MT data/Definitions/Ejemplo_MT.txt data/Inputs/input1.txt
```

## Problemas implementados

1. **Lenguaje L = { aⁿbᵐ | m > n, n > 0 }**
   - Archivos: 
      - Definición: `data/Definitions/problema1_MT.txt`
      - Input: `data/Inputs/input_Problema1.txt`

2. **Contador de símbolos 'a' y 'b'**
   - Convierte una cadena de a's y b's en su representación de conteo
   - Formato: `#a • #b` (en unario: n = 1ⁿ⁺¹)
   - Archivos: 
      - Definición: `data/Definitions/problema2_MT.txt`
      - Input: `data/Inputs/input_Problema2.txt`

