#include <stdio.h>
#include <stdlib.h>
#include "validaciones.h"

void limpiar_buffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

int leer_entero_rango(const char *mensaje, int min, int max) {
    int valor;
    int leido;
    while (1) {
        printf("%s", mensaje);
        leido = scanf("%d", &valor);
        if (leido != 1) {
            printf("[ERROR] Debe ingresar un numero entero valido.\n");
            limpiar_buffer();
        } else if (valor < min || valor > max) {
            printf("[ERROR] El valor debe estar entre %d y %d.\n", min, max);
            limpiar_buffer();
        } else {
            limpiar_buffer();
            return valor;
        }
    }
}

double leer_double_positivo(const char *mensaje) {
    double valor;
    int leido;
    while (1) {
        printf("%s", mensaje);
        leido = scanf("%lf", &valor);
        if (leido != 1) {
            printf("[ERROR] Entrada invalida. Ingrese un valor numerico.\n");
            limpiar_buffer();
        } else if (valor <= 0.0) {
            printf("[ERROR] El valor debe ser mayor a 0.\n");
            limpiar_buffer();
        } else {
            limpiar_buffer();
            return valor;
        }
    }
}

void pausar_pantalla(void) {
    printf("\nPresione ENTER para continuar...");
    getchar();
}