# Práctica 02 - Máquina de Turing
Práctica que implementa un simulador de Máquina de Turing determinista multicinta.

## Autor
Trabajo realizado por Marcos Llinares Montes (alu0100972443)

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
│   └── Inputs/
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

## Características de implementación

**Opciones seleccionadas:**
- Máquina de Turing con **escritura y movimientos simultáneos**
- Movimientos: **L** (izquierda), **R** (derecha), **S** (sin movimiento)
- Cinta **infinita en ambas direcciones**
- Soporte para **Máquina de Turing multicinta**

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

## Formato del archivo de definición

```
# Conjunto Q (estados)
q1 q2 q3 ...

# Conjunto Σ (alfabeto de entrada)
a b c ...

# Conjunto Γ (alfabeto de cinta)
a b c X Y ...

# Estado inicial
q0

# Símbolo blanco
·

# Conjunto F (estados finales)
qf1 qf2 ...

# Número de cintas (solo para MT multicinta)
2

# Función de transición δ(q, s) = (q', s', m)
q1 a q2 b R
q2 b q3 c L
...
```

## Problemas implementados

1. **Lenguaje L = { aⁿbᵐ | m > n, n > 0 }**
   - Archivo: `data/Definitions/problema1_MT.txt`

2. **Contador de símbolos 'a' y 'b'**
   - Convierte una cadena de a's y b's en su representación de conteo
   - Formato: `#a • #b` (en unario: n = 1ⁿ⁺¹)
   - Archivo: `data/Definitions/problema2_MT.txt`

