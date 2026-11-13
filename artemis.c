#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "artemis.h"
#include "validaciones.h"

#ifdef _WIN32
#include <windows.h>
#define DORMIR_MS(ms) Sleep(ms)
#define LIMPIAR_PANTALLA() system("cls")
#else
#include <unistd.h>
#define DORMIR_MS(ms) usleep((ms) * 1000)
#define LIMPIAR_PANTALLA() system("clear")
#endif

// Codigos de colores ANSI para consola
#define RESET   "\033[0m"
#define ROJO    "\033[1;31m"
#define VERDE   "\033[1;32m"
#define AMARILLO "\033[1;33m"
#define CIAN    "\033[1;36m"
#define BLANCO  "\033[1;37m"

void inicializar_parametros_base(ParametrosMision *params) {
    params->a1 = 20.0;
    params->t1 = 120.0;
    params->a2 = 12.5;
    params->altitud_meta = 160000.0;
}

void calcular_analisis_teorico(const ParametrosMision *params, ResultadoAnalitico *res) {
    res->v1 = params->a1 * params->t1;
    res->y1 = 0.5 * params->a1 * (params->t1 * params->t1);
    res->g_forces_fase1 = params->a1 / G_TERRESTRE;

    double delta_y2 = params->altitud_meta - res->y1;
    if (delta_y2 > 0) {
        res->v2_max = sqrt((res->v1 * res->v1) + (2.0 * params->a2 * delta_y2));
        res->t_fase2 = (res->v2_max - res->v1) / params->a2;
    } else {
        res->v2_max = res->v1;
        res->t_fase2 = 0.0;
    }
    res->t_total = params->t1 + res->t_fase2;

    double delta_h_inercia = (res->v2_max * res->v2_max) / (2.0 * G_TERRESTRE);
    res->h_max_falla = params->altitud_meta + delta_h_inercia;
}

void mostrar_reporte_analitico(const ParametrosMision *params, const ResultadoAnalitico *res) {
    LIMPIAR_PANTALLA();
    printf(CIAN "╔══════════════════════════════════════════════════════════════╗\n" RESET);
    printf(CIAN "║" BLANCO "          INFORME FISICO-ANALITICO: SLS ARTEMIS               " CIAN "║\n" RESET);
    printf(CIAN "╠══════════════════════════════════════════════════════════════╣\n" RESET);
    printf(CIAN "║" AMARILLO " 1. DESACOPLE SRB (t = %6.2f s):                             " CIAN "║\n" RESET, params->t1);
    printf(CIAN "║" RESET "    • Rapidez alcanzada (v1):    " VERDE "%10.2f m/s" RESET " (%8.2f km/h) " CIAN "║\n" RESET, res->v1, res->v1 * 3.6);
    printf(CIAN "║" RESET "    • Altitud alcanzada (y1):    " VERDE "%10.2f m  " RESET " (%8.2f km)   " CIAN "║\n" RESET, res->y1, res->y1 / 1000.0);
    printf(CIAN "║" RESET "    • Carga sobre tripulacion:   " AMARILLO "%10.2f g                  " CIAN "║\n" RESET, res->g_forces_fase1);
    printf(CIAN "╠══════════════════════════════════════════════════════════════╣\n" RESET);
    printf(CIAN "║" AMARILLO " 2. APAGADO DE MOTORES / MECO (h = %6.2f km):                " CIAN "║\n" RESET, params->altitud_meta / 1000.0);
    printf(CIAN "║" RESET "    • Rapidez final maxima (v2): " VERDE "%10.2f m/s" RESET " (%8.2f km/h) " CIAN "║\n" RESET, res->v2_max, res->v2_max * 3.6);
    printf(CIAN "║" RESET "    • Tiempo en Fase 2 (Core):   " BLANCO "%10.2f s                  " CIAN "║\n" RESET, res->t_fase2);
    printf(CIAN "║" RESET "    • Tiempo total de ascenso:   " BLANCO "%10.2f s  " RESET " (%8.2f min)  " CIAN "║\n" RESET, res->t_total, res->t_total / 60.0);
    printf(CIAN "╠══════════════════════════════════════════════════════════════╣\n" RESET);
    printf(CIAN "║" ROJO " 3. CONTINGENCIA (Falla motor / Caida Libre a 160 km):        " CIAN "║\n" RESET);
    printf(CIAN "║" RESET "    • Altitud maxima (inercia):  " ROJO "%10.2f m  " RESET " (%8.2f km)   " CIAN "║\n" RESET, res->h_max_falla, res->h_max_falla / 1000.0);
    printf(CIAN "║" RESET "    • Ascenso extra sin empuje:  " BLANCO "%10.2f km                 " CIAN "║\n" RESET, (res->h_max_falla - params->altitud_meta) / 1000.0);
    printf(CIAN "╚══════════════════════════════════════════════════════════════╝\n" RESET);
}

void dibujar_interfaz_vuelo(const EstadoTelemetria *estado, double altitud_meta) {
    LIMPIAR_PANTALLA();
    double fraccion = estado->y / altitud_meta;
    if (fraccion > 1.0) fraccion = 1.0;

    int altura_pantalla = 14;
    int pos_cohete = (int)(fraccion * (altura_pantalla - 1));

    printf(CIAN "╔══════════════════════════════════════════════════════════════════╗\n" RESET);
    printf(CIAN "║" BLANCO "             TELEMETRIA EN VIVO - MISION ARTEMIS SLS              " CIAN "║\n" RESET);
    printf(CIAN "╠═════════════════════════════════╦════════════════════════════════╣\n" RESET);

    for (int fila = altura_pantalla - 1; fila >= 0; fila--) {
        printf(CIAN "║ " RESET);
        
        // Columna izquierda: Animacion visual del cohete
        if (fila == pos_cohete + 1) {
            printf("             /\\                  ");
        } else if (fila == pos_cohete) {
            if (estado->srb_acoplados) {
                printf(AMARILLO "           /|  |\\                " RESET);
            } else {
                printf(CIAN "            |  |  " ROJO "(SRB JETTISON) " RESET);
            }
        } else if (fila == pos_cohete - 1) {
            if (estado->srb_acoplados) {
                printf(ROJO "           ||**||                " RESET);
            } else {
                printf(AMARILLO "             **                  " RESET);
            }
        } else if (fila == 0 && pos_cohete > 2) {
            printf(VERDE "      ====[PLATAFORMA]====       " RESET);
        } else {
            printf("        .          *             ");
        }

        printf(CIAN "║ " RESET);

        // Columna derecha: Datos de telemetria
        if (fila == 12) {
            printf("TIEMPO:    " BLANCO "T+ %06.1f s         " RESET, estado->t);
        } else if (fila == 10) {
            printf("ALTITUD:   " VERDE "%8.2f km         " RESET, estado->y / 1000.0);
        } else if (fila == 8) {
            printf("VELOCIDAD: " VERDE "%8.1f m/s        " RESET, estado->v);
        } else if (fila == 6) {
            printf("ACELERAC.: " AMARILLO "%8.2f m/s^2      " RESET, estado->a);
        } else if (fila == 4) {
            if (estado->srb_acoplados) {
                printf("PROPULS.:  " VERDE "SRB ACOPLADOS       " RESET);
            } else {
                printf("PROPULS.:  " ROJO "SRB DESACOPLADOS    " RESET);
            }
        } else if (fila == 2) {
            printf("PROGRESO:  " CIAN "%6.1f %%            " RESET, fraccion * 100.0);
        } else {
            printf("                               ");
        }
        printf(CIAN "║\n" RESET);
    }

    printf(CIAN "╚═════════════════════════════════╩════════════════════════════════╝\n" RESET);
}

void ejecutar_simulacion_vuelo(const ParametrosMision *params) {
    EstadoTelemetria estado = {0.0, 0.0, 0.0, params->a1, 1};
    double dt = 1.0;

    while (estado.y < params->altitud_meta) {
        if (estado.t < params->t1) {
            estado.a = params->a1;
            estado.srb_acoplados = 1;
        } else {
            estado.a = params->a2;
            estado.srb_acoplados = 0;
        }

        estado.v += estado.a * dt;
        estado.y += estado.v * dt;
        estado.t += dt;

        if ((int)estado.t % 2 == 0) {
            dibujar_interfaz_vuelo(&estado, params->altitud_meta);
            DORMIR_MS(60); // Controla la velocidad de la animacion
        }
    }

    dibujar_interfaz_vuelo(&estado, params->altitud_meta);
    printf(VERDE "\n [OK] APAGADO DE MOTORES PRINCIPAL (MECO) CONFIRMADO A LOS %.2f km\n" RESET, estado.y / 1000.0);
}