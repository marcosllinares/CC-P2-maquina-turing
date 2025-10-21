# Práctica 02 - Máquina de Turing
Práctica que implementa un 

## Autor
Trabajo realizado por Marcos Llinares Montes (alu0100972443)

## Estructura del proyecto
- `include/` : cabeceras (.hpp)
- `src/`     : implementación (.cpp)
- `data/`    : archivos de ejemplo (definiciones de )
- `build/`, `bin/` : artefactos de compilación (creados por `make`)

```
.
├── Makefile
├── README.md
├── data
├── include
└── src
    └── main.cpp
```

## Compilación
El proyecto se compila mediante `make` (Makefile incluido). Basta ejecutar desde la raíz del proyecto:

```sh
make
```

El binario resultante es `bin\...`

## Ejecución
Uso básico:

```sh
./bin/pda <definition_file> <inputs_file> [--trace]
```

- `definition_file`: fichero con la definición del PDA (ej. `data/APf/APf-1.txt`).
- `inputs_file`: fichero con cadenas de entrada, una por línea (ej. `data/Inputs/Input1.txt`).
- `--trace` (opcional): muestra información de trazas / transiciones.

Ejemplo:

```sh
./bin/pda data/APf/APf-1.txt data/Inputs/Input1.txt --trace
```

