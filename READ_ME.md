# Simulación del lanzamiento SLS (Artemis)

Trabajo Final Integrador hecho en lenguaje C.
El programa resuelve y simula el lanzamiento del cohete Space Launch System (SLS), calculando velocidades, alturas y tiempos en cada etapa hasta llegar a los 160 km.

## Integrantes y comisión

**Comisión:** K1.1

**Integrantes:**

- Tomas Agustin Taie - Legajo: 28934
- Franco Javier Sosa Gomez - Legajo: 28800
- Axel Antonio Echeverria - Legajo: 28799

## Descripción del problema y reglas del programa

Nos tocó la *Situación Problemática 1*, que divide el despegue del cohete en tres momentos clave usando fórmulas de Movimiento Rectilíneo Uniformemente Variado (MRUV):

- **Etapa 1 (Despegue con propulsores):** El cohete arranca quieto (v_0 = 0) y sube con una aceleración de 20,0 m/s² durante 120 segundos. Calculamos qué velocidad alcanza, a qué altura llega y cuántas fuerzas G soporta la tripulación.
- **Etapa 2 (Separación de los SRB):** A los 120 segundos se sueltan los propulsores laterales. El cohete sigue subiendo con una aceleración menor (12,5 m/s²) hasta llegar a los 160 km de altura, donde apaga sus motores.
- **Etapa 3 (Falla hipotética de motores):** Calculamos qué pasaría si los motores se apagan de golpe a los 160 km: cuánto seguiría subiendo por pura inercia antes de empezar a caer por la gravedad (9,8 m/s²).

**Reglas básicas de uso:**

- Se pueden usar los valores por defecto del enunciado o cargar datos nuevos desde el menú.
- Todos los valores físicos (aceleraciones, tiempos, alturas) deben ser números mayores a cero. Si se escribe una letra o un número negativo, el programa avisa del error y lo vuelve a pedir sin cerrarse.

## Cómo está armado el código

Para no meter todo en un solo archivo gigante, dividimos el proyecto en módulos según lo que hace cada parte:

- `main.c`: tiene el menú principal y maneja las opciones que elige el usuario.
- `artemis.h` y `artemis.c`: acá están las estructuras (`struct`) donde guardamos los datos de la misión, todas las cuentas de física y la animación del cohete subiendo por la consola.
- `validaciones.h` y `validaciones.c`: son funciones aparte que revisan lo que escribe el usuario por teclado y limpian el buffer para que el `scanf` no se trabe si alguien ingresa texto por error.
- `Makefile`: automatiza la compilación del proyecto.

## Cómo compilar y ejecutar

### Opción 1: con Makefile

```bash
make
```

Para borrar el ejecutable generado:

```bash
make clean
```

(En Windows con MinGW el comando puede ser `mingw32-make`.)

### Opción 2: con GCC directamente

Abrir la consola en la carpeta del proyecto y escribir:

```bash
gcc -Wall -Wextra -std=c99 main.c artemis.c validaciones.c -o artemis_sim -lm
```

### Ejecutar

En Windows:

```cmd
.\artemis_sim.exe
```

En Linux / macOS:

```bash
./artemis_sim
```

> **Nota:** en Windows conviene ejecutarlo desde CMD, PowerShell o Windows Terminal para que se vean bien los colores y los bordes de la interfaz.

## Historial de cambios

- **v1.1:** se elimina el parpadeo de la animación. En lugar de borrar toda la pantalla con `system("cls")` en cada cuadro, se usan códigos ANSI para volver el cursor al inicio y sobrescribir el cuadro anterior, y se oculta el cursor durante la simulación.
- **v1.0:** versión inicial con menú, análisis físico-analítico y simulación gráfica en consola.

El detalle completo está en los commits del repositorio.

## Uso de IA

Usamos Gemini y Claude como apoyo puntual para resolver dudas técnicas de programación en C mientras armábamos el trabajo:

- **División en archivos:** le consultamos cómo conectar varios archivos `.c` y `.h` entre sí, cómo organizar los `struct` sin repetir variables y cómo usar las guardas `#ifndef` / `#define` para que no tire error de compilación al incluir las cabeceras en varios archivos.
- **Problemas con el teclado (`scanf`):** como el menú entraba en bucle infinito cuando probábamos meter letras en vez de números, le pedimos ayuda para armar una función estándar que limpie el buffer de entrada usando `getchar()` en lugar de `fflush(stdin)`.
- **Errores de compilación y consola:** nos ayudó a entender por qué faltaba la bandera `-lm` al usar `sqrt()` y cómo habilitar los caracteres UTF-8 y colores ANSI en la terminal de Windows mediante `<windows.h>` para que la animación del cohete se vea prolija.
- **Parpadeo de la animación:** consultamos cómo evitar el parpadeo al actualizar la pantalla y aplicamos la solución con códigos ANSI de movimiento de cursor en `artemis.c`.
