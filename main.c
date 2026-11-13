#include <stdio.h>
#include <stdlib.h>
#include "artemis.h"
#include "validaciones.h"

#ifdef _WIN32
#include <windows.h>
#define LIMPIAR() system("cls")
#else
#define LIMPIAR() system("clear")
#endif

#define RESET    "\033[0m"
#define CIAN     "\033[1;36m"
#define BLANCO   "\033[1;37m"
#define AMARILLO "\033[1;33m"
#define VERDE    "\033[1;32m"

void configurar_consola_windows(void) {
#ifdef _WIN32
    // Habilita caracteres UTF-8 (bordes dobles) y colores ANSI en Windows
    SetConsoleOutputCP(65001);
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD dwMode = 0;
    GetConsoleMode(hOut, &dwMode);
    dwMode |= 0x0004; // ENABLE_VIRTUAL_TERMINAL_PROCESSING
    SetConsoleMode(hOut, dwMode);
#endif
}

void mostrar_menu(void) {
    LIMPIAR();
    printf(CIAN "╔══════════════════════════════════════════════════════╗\n" RESET);
    printf(CIAN "║" BLANCO "        CENTRO DE CONTROL - MISION ARTEMIS SLS        " CIAN "║\n" RESET);
    printf(CIAN "╠══════════════════════════════════════════════════════╣\n" RESET);
    printf(CIAN "║" RESET "  1. Ver parametros actuales de la mision             " CIAN "║\n" RESET);
    printf(CIAN "║" RESET "  2. Ejecutar analisis fisico-analitico               " CIAN "║\n" RESET);
    printf(CIAN "║" RESET "  3. Modificar condiciones iniciales de vuelo         " CIAN "║\n" RESET);
    printf(CIAN "║" VERDE "  4. Iniciar simulacion grafica en tiempo real        " CIAN "║\n" RESET);
    printf(CIAN "║" AMARILLO "  5. Salir del sistema                                " CIAN "║\n" RESET);
    printf(CIAN "╚══════════════════════════════════════════════════════╝\n" RESET);
}

int main(void) {
    ParametrosMision params;
    ResultadoAnalitico res;
    int opcion;

    configurar_consola_windows();
    inicializar_parametros_base(&params);

    do {
        mostrar_menu();
        opcion = leer_entero_rango(" Seleccione una opcion [1-5]: ", 1, 5);

        switch (opcion) {
            case 1:
                LIMPIAR();
                printf(CIAN "\n--- PARAMETROS DE CONFIGURACION ---\n" RESET);
                printf(" Aceleracion Fase 1 (a1):      %.2f m/s^2\n", params.a1);
                printf(" Tiempo desacople SRB (t1):    %.2f s\n", params.t1);
                printf(" Aceleracion Fase 2 (a2):      %.2f m/s^2\n", params.a2);
                printf(" Altitud de apagado meta:      %.2f km\n", params.altitud_meta / 1000.0);
                pausar_pantalla();
                break;

            case 2:
                calcular_analisis_teorico(&params, &res);
                mostrar_reporte_analitico(&params, &res);
                pausar_pantalla();
                break;

            case 3:
                LIMPIAR();
                printf(AMARILLO "\n--- MODIFICAR PARAMETROS DE MISION ---\n" RESET);
                params.a1 = leer_double_positivo(" Ingrese a1 (m/s^2, default 20.0): ");
                params.t1 = leer_double_positivo(" Ingrese t1 desacople (s, default 120.0): ");
                params.a2 = leer_double_positivo(" Ingrese a2 (m/s^2, default 12.5): ");
                params.altitud_meta = leer_double_positivo(" Ingrese altitud meta (m, default 160000.0): ");
                printf(VERDE "\n [OK] Parametros actualizados con exito.\n" RESET);
                pausar_pantalla();
                break;

            case 4:
                ejecutar_simulacion_vuelo(&params);
                pausar_pantalla();
                break;

            case 5:
                printf(CIAN "\n Cerrando telemetria Artemis. Exitos en la defensa!\n" RESET);
                break;
        }
    } while (opcion != 5);

    return 0;
}