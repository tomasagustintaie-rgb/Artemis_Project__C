# Simulación del lanzamiento SLS (Artemis)

Trabajo Final Integrador hecho en lenguaje C.
El programa resuelve y simula el lanzamiento del cohete Space Launch System (SLS), calculando velocidades, alturas y tiempos en cada etapa hasta llegar a los 160 km.

## 1. Integrantes y comisión

**Comisión:** K1.1

**Integrantes:**

- Tomas Agustin Taie - Legajo: 28934
- Franco Javier Sosa Gomez - Legajo: 28800
- Axel Antonio Echeverria - Legajo: 28799

## 2. Descripción del escenario y reglas del programa

Nos tocó la *Situación Problemática 1*, que divide el despegue del cohete en tres momentos clave usando fórmulas de Movimiento Rectilíneo Uniformemente Variado (MRUV).

### Datos del enunciado (valores por defecto)

| Parámetro | Símbolo | Valor |
|---|---|---|
| Velocidad inicial | v₀ | 0 m/s (el cohete arranca quieto) |
| Aceleración Fase 1 (con SRB) | a₁ | 20,0 m/s² |
| Tiempo de desacople de los SRB | t₁ | 120 s |
| Aceleración Fase 2 (solo motor central) | a₂ | 12,5 m/s² |
| Altitud de apagado de motores (MECO) | h | 160 km (160 000 m) |
| Gravedad terrestre | g | 9,8 m/s² |

### Etapas

- **Etapa 1 (Despegue con propulsores):** el cohete sube con aceleración a₁ durante t₁ segundos. Calculamos qué velocidad alcanza, a qué altura llega y cuántas fuerzas G soporta la tripulación.
- **Etapa 2 (Separación de los SRB):** a los 120 segundos se sueltan los propulsores laterales. El cohete sigue subiendo con una aceleración menor (a₂) hasta llegar a los 160 km de altura, donde apaga sus motores.
- **Etapa 3 (Falla hipotética de motores):** calculamos qué pasaría si los motores se apagan de golpe a los 160 km: cuánto seguiría subiendo por pura inercia antes de empezar a caer por la gravedad.

### Fórmulas utilizadas

```
Etapa 1:   v1 = a1 · t1
           y1 = ½ · a1 · t1²
           carga g = a1 / g

Etapa 2:   Δy2 = h − y1
           v2 = √( v1² + 2 · a2 · Δy2 )        (ecuación independiente del tiempo)
           t2 = (v2 − v1) / a2
           t_total = t1 + t2

Etapa 3:   Δh = v2² / (2 · g)                   (ascenso extra por inercia)
           h_max = h + Δh
```

### Resultados esperados con los valores por defecto

Valores aproximados, que se pueden comparar con la opción 2 del menú:

| Magnitud | Resultado |
|---|---|
| v₁ | 2400 m/s (8640 km/h) |
| y₁ | 144 km |
| Carga sobre la tripulación | ≈ 2,04 g |
| v₂ | ≈ 2481,93 m/s |
| t₂ (Fase 2) | ≈ 6,55 s |
| Tiempo total de ascenso | ≈ 126,55 s |
| Ascenso extra sin empuje | ≈ 314,29 km |
| Altitud máxima en caso de falla | ≈ 474,29 km |

### Reglas básicas de uso

- Se pueden usar los valores por defecto del enunciado o cargar datos nuevos desde el menú.
- Todos los valores físicos (aceleraciones, tiempos, alturas) deben ser números mayores a cero. Si se escribe una letra o un número negativo, el programa avisa del error y lo vuelve a pedir sin cerrarse.
- Las opciones del menú se eligen con un número entero entre 1 y 5.

## 3. Funcionamiento del programa

Al ejecutarlo se muestra el **Centro de Control** con cinco opciones:

1. **Ver parámetros actuales:** muestra a₁, t₁, a₂ y la altitud meta que se están usando.
2. **Análisis físico-analítico:** calcula con las fórmulas anteriores y muestra el informe completo de las tres etapas.
3. **Modificar condiciones iniciales:** permite cargar nuevos valores, validando que sean mayores a cero.
4. **Simulación gráfica en tiempo real:** anima el ascenso del cohete en la consola.
5. **Salir.**

### La simulación gráfica

- Avanza el tiempo en pasos de `dt = 1 s` y actualiza la velocidad y la altitud con el método de Euler (`v += a·dt`, `y += v·dt`).
- Usa a₁ mientras `t < t₁` (SRB acoplados) y a₂ después (SRB desacoplados).
- El cohete se dibuja con caracteres y sube por la pantalla según la fracción de altitud alcanzada.
- Se muestran en vivo el tiempo, la altitud, la velocidad, la aceleración, el estado de los propulsores y el progreso.
- Se dibuja un cuadro cada dos pasos y se pausa 60 ms entre cuadros. Para evitar el parpadeo no se borra la pantalla en cada cuadro: se vuelve el cursor al inicio y se sobrescribe (ver historial de cambios).
- Como la simulación es numérica, sus valores pueden diferir levemente de los del análisis teórico por el error acumulado del método de Euler.

### Organización del código

Para no meter todo en un solo archivo gigante, dividimos el proyecto en módulos según lo que hace cada parte:

- `main.c`: tiene el menú principal y maneja las opciones que elige el usuario.
- `artemis.h` y `artemis.c`: acá están las estructuras (`struct`) donde guardamos los datos de la misión (`ParametrosMision`, `ResultadoAnalitico`, `EstadoTelemetria`), todas las cuentas de física y la animación del cohete subiendo por la consola.
- `validaciones.h` y `validaciones.c`: funciones aparte que revisan lo que escribe el usuario por teclado y limpian el buffer para que el `scanf` no se trabe si alguien ingresa texto por error.
- `Makefile`: automatiza la compilación del proyecto.

## 4. Cómo compilar y ejecutar

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

La bandera `-lm` es necesaria para enlazar la librería matemática (`sqrt`).

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

## 5. Historial de cambios

- **v1.1:** se elimina el parpadeo de la animación. En lugar de borrar toda la pantalla con `system("cls")` en cada cuadro, se usan códigos ANSI para volver el cursor al inicio y sobrescribir el cuadro anterior, y se oculta el cursor durante la simulación.
- **v1.0:** versión inicial con menú, análisis físico-analítico y simulación gráfica en consola.

El detalle completo está en los commits del repositorio.

## 6. Registro de uso de Inteligencia Artificial

Usamos Gemini y Claude como apoyo para resolver dudas técnicas de programación en C. A continuación se detallan las consultas que hicimos, agrupadas por etapa del trabajo, junto con un resumen de la respuesta recibida y lo que aplicamos en el código.

### 6.1 Diseño y arquitectura (modularización)

**Consulta 1:** *"Tengo que hacer un simulador de física en C sobre el cohete SLS (MRUV en dos etapas) dividido en múltiples archivos .c y .h. ¿Cómo me recomiendas estructurar los struct para separar los parámetros iniciales de los resultados analíticos y la telemetría?"*

- **Respuesta (resumen):** agrupar las variables por su rol en tres estructuras dentro de `artemis.h`: `ParametrosMision` (datos de entrada configurables), `ResultadoAnalitico` (valores calculados con las fórmulas exactas) y `EstadoTelemetria` (variables dinámicas de la simulación). Pasarlas por puntero evita copiar datos y permite que las funciones modifiquen los originales.
- **Qué aplicamos:** las tres estructuras de `artemis.h`, pasadas por puntero a las funciones de cálculo y de dibujo.

**Consulta 2:** *"¿Cómo funcionan exactamente las guardas de inclusión #ifndef y #define en los archivos de cabecera .h para evitar errores de compilación cuando main.c y artemis.c incluyen el mismo archivo?"*

- **Respuesta (resumen):** si varios `.c` incluyen el mismo `.h`, el compilador podría leer dos veces las definiciones y dar error de redefinición. Con `#ifndef ARTEMIS_H` / `#define ARTEMIS_H` el preprocesador solo procesa el archivo la primera vez.
- **Qué aplicamos:** las guardas en `artemis.h` y `validaciones.h`.

### 6.2 Modelado matemático y física

**Consulta 3:** *"En un problema de MRUV de dos fases donde en la segunda etapa solo tengo la aceleración a₂, la velocidad inicial v₁ y la distancia Δy, ¿qué ecuación cinemática conviene implementar en C para despejar la velocidad final sin tener el tiempo previo?"*

- **Respuesta (resumen):** usar la ecuación independiente del tiempo, `v₂² = v₁² + 2·a₂·Δy₂`, de donde `v₂ = √(v₁² + 2·a₂·Δy₂)`. Con v₂ se despeja el tiempo de la fase: `Δt₂ = (v₂ − v₁) / a₂`.
- **Qué aplicamos:** esas fórmulas en `calcular_analisis_teorico` (`artemis.c`).

**Consulta 4:** *"¿Por qué me da error de 'undefined reference to sqrt' cuando compilo mi código en C con GCC si ya puse #include <math.h> arriba?"*

- **Respuesta (resumen):** `#include <math.h>` solo da al compilador los prototipos. El código de la librería matemática está en un archivo aparte (`libm`), así que hay que agregar la bandera `-lm` al final del comando de compilación para que el enlazador la una al programa.
- **Qué aplicamos:** la bandera `-lm` en el comando de compilación y en el Makefile.

### 6.3 Robustez y validación de entradas

**Consulta 5:** *"Cuando uso scanf("%d", &opcion) en un menú con do-while y el usuario ingresa una letra por error, el programa entra en un bucle infinito. ¿Cómo puedo limpiar el buffer de entrada en C estándar sin usar fflush(stdin)?"*

- **Respuesta (resumen):** `fflush(stdin)` tiene comportamiento indefinido en el estándar de C. Lo portable es una función que consuma los caracteres sobrantes hasta el salto de línea: `while ((c = getchar()) != '\n' && c != EOF);`.
- **Qué aplicamos:** la función `limpiar_buffer()` de `validaciones.c`.

**Consulta 6:** *"¿Cómo diseño una función genérica en C que valide que un número double ingresado por teclado sea estrictamente positivo y vuelva a pedir el dato si el usuario ingresa texto o cero?"*

- **Respuesta (resumen):** `scanf("%lf", &valor)` devuelve la cantidad de datos leídos; si no es 1, la entrada no era un número. Dentro de un `while (1)` se verifica primero el tipo de dato y luego que `valor > 0`, y solo si ambas pasan se limpia el buffer y se devuelve el valor.
- **Qué aplicamos:** `leer_double_positivo()` y, con la misma lógica, `leer_entero_rango()` en `validaciones.c`.

### 6.4 Interfaz en consola y portabilidad

**Consulta 7:** *"¿Cómo puedo habilitar los colores ANSI y los caracteres UTF-8 (bordes dobles en ASCII) en la consola de Windows usando C puro para que no salgan símbolos raros al imprimir los cuadros?"*

- **Respuesta (resumen):** la consola de Windows usa por defecto una codificación antigua que rompe los bordes y tiene desactivados los códigos de color ANSI. Con `<windows.h>` se activa UTF-8 con `SetConsoleOutputCP(65001)` y se habilita `ENABLE_VIRTUAL_TERMINAL_PROCESSING` con `SetConsoleMode()`.
- **Qué aplicamos:** la función `configurar_consola_windows()` de `main.c`, encerrada en `#ifdef _WIN32`.

**Consulta 8:** *"Quiero hacer una animación sencilla en consola de un cohete subiendo y actualizando datos en pantalla. ¿Cómo hago en C para pausar unos milisegundos entre fotogramas de forma compatible tanto con Windows (Sleep) como con Linux (usleep)?"*

- **Respuesta (resumen):** usar compilación condicional con `#ifdef _WIN32`: en Windows se incluye `<windows.h>` y se define `DORMIR_MS(ms)` como `Sleep(ms)`; en Linux/macOS se incluye `<unistd.h>` y se define como `usleep((ms) * 1000)`.
- **Qué aplicamos:** las macros `DORMIR_MS` y `LIMPIAR_PANTALLA` al inicio de `artemis.c`.

### 6.5 Interfaz gráfica: explicación de la animación y mejora del parpadeo

**Consulta 9:** pedimos que nos explicaran cómo funciona la parte gráfica del código: los colores con códigos ANSI, cómo se actualizan en tiempo real el tiempo, la velocidad y la altitud, y cómo se hace subir el cohete dibujado con texto.

- **Respuesta (resumen):** los colores son secuencias de escape (`\033[...m`) que el terminal interpreta como instrucciones, y siempre se cierran con `RESET`. La animación se logra repitiendo un ciclo de calcular, dibujar y esperar. La altitud se convierte a una fila de pantalla con una regla de tres (`fracción = y / altitud_meta`), y los números se mantienen alineados con formatos de ancho fijo como `%8.2f`.
- **Qué aplicamos:** usamos la explicación para entender y ajustar `dibujar_interfaz_vuelo` y `ejecutar_simulacion_vuelo`.

**Consulta 10:** como la pantalla parpadeaba al actualizarse, preguntamos cómo evitarlo y pedimos una versión de `artemis.c` con la mejora.

- **Respuesta (resumen):** el parpadeo viene de `system("cls")`, que borra todo y lanza un proceso externo en cada cuadro. La alternativa es limpiar la pantalla una sola vez antes del bucle y, en cada cuadro, mover el cursor al inicio con `\033[H` y sobrescribir. También se oculta el cursor durante la animación (`\033[?25l`) y se lo restaura al final (`\033[?25h`).
- **Qué aplicamos:** reemplazamos `artemis.c` por la versión con la mejora, la compilamos, comprobamos que la animación ya no parpadea y registramos el cambio en un commit aparte (ver historial de cambios).

### 6.6 Otras consultas

**Consulta 11:** consultamos cómo armar el `Makefile` y cómo organizar y subir el proyecto a GitHub (`.gitignore`, commits y manejo del ejecutable).

- **Qué aplicamos:** el `Makefile` y el `.gitignore` del repositorio, y la organización de los commits.

### Criterio de uso

La IA se usó como apoyo para entender conceptos y resolver errores puntuales. Las respuestas se probaron compilando y ejecutando el programa, y el análisis físico (fórmulas, parámetros y resultados) se verificó contra el enunciado del trabajo.
