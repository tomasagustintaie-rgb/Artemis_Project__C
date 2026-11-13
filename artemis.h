#ifndef ARTEMIS_H
#define ARTEMIS_H

#define G_TERRESTRE 9.80

// Parametros configurables de la mision
typedef struct {
    double a1;           // Aceleracion Fase 1 (m/s^2)
    double t1;           // Tiempo desacople SRB (s)
    double a2;           // Aceleracion Fase 2 (m/s^2)
    double altitud_meta; // Altura apagado de motores (m)
} ParametrosMision;

// Resultados analiticos calculados
typedef struct {
    double v1;             // Rapidez desacople SRB (m/s)
    double y1;             // Altura desacople SRB (m)
    double v2_max;         // Rapidez apagado motores (m/s)
    double t_fase2;        // Duracion fase 2 (s)
    double t_total;        // Tiempo total de ascenso (s)
    double h_max_falla;    // Altitud maxima en caida libre (m)
    double g_forces_fase1; // Carga g sobre tripulacion
} ResultadoAnalitico;

// Telemetria en tiempo real
typedef struct {
    double t;
    double y;
    double v;
    double a;
    int srb_acoplados;
} EstadoTelemetria;

// Prototipos de funciones
void inicializar_parametros_base(ParametrosMision *params);
void calcular_analisis_teorico(const ParametrosMision *params, ResultadoAnalitico *res);
void mostrar_reporte_analitico(const ParametrosMision *params, const ResultadoAnalitico *res);
void ejecutar_simulacion_vuelo(const ParametrosMision *params);
void dibujar_interfaz_vuelo(const EstadoTelemetria *estado, double altitud_meta);

#endif