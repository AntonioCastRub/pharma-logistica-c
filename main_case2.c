/* ================================================================
   La Farmacéutica – Sistema modular para Cadena de Suministro (Logística)
   Lenguaje: C (C11)
   Autor: [Antonio]
   Descripción:
   Programa modular que demuestra: funciones, paso por valor y referencia,
   variables globales/locales, y sentencias de control (if–else, switch,
   for, while, do–while). El flujo sigue el modelo:
      Sistema Principal -> Entrada -> Procesamiento -> Salida -> Reportes
   ================================================================ */

#include <stdio.h>
#include <string.h>
#include <ctype.h>

/* ========= Variables "globales" (para demostrar ámbito) ========= */
double kpi_fill_rate_before = 0.88;   /* nivel de servicio previo */
double kpi_fill_rate_after  = 0.00;   /* se calculará */
double kpi_leadtime_before  = 4.5;    /* días */
double kpi_leadtime_after   = 0.00;

/* ========= Prototipos ========= */
void modulo_entrada(int *pedido_id, int *unidades, double *costo_unit, char cliente[]);
double modulo_procesamiento(int unidades, double costo_unit, char transporte, double *costo_total, double *lead_time);
void modulo_salida(int pedido_id, const char cliente[], int unidades, double costo_unit, char transporte, double costo_total, double lead_time);
void modulo_reportes(double costo_total, double lead_time);
char clasificar_transporte(double costo_total);

/* ========= Funciones ========= */

void modulo_entrada(int *pedido_id, int *unidades, double *costo_unit, char cliente[]) {
    /* Entrada de datos con validación (do–while) y uso de scanf/printf */
    printf("=== MODULO DE ENTRADA ===\n");
    printf("Cliente (ej. HOSPITAL RIO): ");
    scanf(" %49[^\n]", cliente); /* leer línea (hasta 49 chars) */

    do {
        printf("ID de pedido (positivo): ");
        scanf("%d", pedido_id);
    } while (*pedido_id <= 0);

    do {
        printf("Unidades solicitadas (1..1000): ");
        scanf("%d", unidades);
    } while (*unidades < 1 || *unidades > 1000);

    do {
        printf("Costo unitario (0.1..9999.9): ");
        scanf("%lf", costo_unit);
    } while (*costo_unit < 0.1 || *costo_unit > 9999.9);

    /* Normalizamos el nombre del cliente (toupper -> función predefinida) */
    for (int i = 0; cliente[i] != '\0'; ++i) cliente[i] = (char)toupper((unsigned char)cliente[i]);
}

double modulo_procesamiento(int unidades, double costo_unit, char transporte, double *costo_total, double *lead_time) {
    /* Procesamiento con for/if/switch/while y paso por referencia */
    printf("\n=== MODULO DE PROCESAMIENTO ===\n");
    /* Simular picking, packing y despacho */
    int etapas = 3;
    double tiempo = 0.0; /* horas */
    for (int i = 1; i <= etapas; ++i) {
        printf("Etapa %d completada\n", i);
        tiempo += 0.5; /* 30 min por etapa */
    }

    /* Política de descuentos escalonados (if–else) */
    double descuento = 0.0;
    if (unidades >= 500)      descuento = 0.08;
    else if (unidades >= 200) descuento = 0.05;
    else if (unidades >= 100) descuento = 0.03;

    *costo_total = unidades * costo_unit * (1.0 - descuento);

    /* Selección de transporte (switch) impacta el lead time */
    switch (transporte) {
        case 'A': /* Aéreo */
            *lead_time = 1.5;
            break;
        case 'T': /* Terrestre */
            *lead_time = 3.0;
            break;
        case 'M': /* Marítimo */
            *lead_time = 7.0;
            break;
        default:
            *lead_time = 4.0; /* estándar */
            break;
    }

    /* Mientras el costo sea muy bajo, sumamos tarifa mínima (while) */
    while (*costo_total < 200.0) {
        *costo_total += 20.0; /* tarifa mínima logística */
    }

    /* Actualizamos KPIs "globales" */
    kpi_fill_rate_after = 0.95;           /* mejora por estandarización */
    kpi_leadtime_after  = (*lead_time);   /* lead time resultante */

    return *costo_total;
}

void modulo_salida(int pedido_id, const char cliente[], int unidades, double costo_unit, char transporte, double costo_total, double lead_time) {
    printf("\n=== MODULO DE SALIDA ===\n");
    printf("Pedido #%d para %s\n", pedido_id, cliente);
    printf("Unidades: %d | Costo unitario: %.2f\n", unidades, costo_unit);
    printf("Transporte: %c | Costo total: %.2f | Lead time: %.1f dias\n",
           transporte, costo_total, lead_time);
    char clase = clasificar_transporte(costo_total);
    printf("Clasificacion del embarque: %c\n", clase);
}

void modulo_reportes(double costo_total, double lead_time) {
    printf("\n=== MODULO DE REPORTES ===\n");
    double delta_fill = (kpi_fill_rate_after - kpi_fill_rate_before) * 100.0;
    double delta_lead = (kpi_leadtime_before - lead_time);
    printf("Mejora de nivel de servicio (fill rate): +%.1f puntos porcentuales\n", delta_fill);
    printf("Reduccion de lead time: %.1f dias\n", delta_lead);
    printf("Gasto logistico del pedido: %.2f\n", costo_total);
}

char clasificar_transporte(double costo_total) {
    /* switch ejerce mapeo simple por tramos (cast a int) */
    int tramo = (int)(costo_total / 200.0);
    switch (tramo) {
        case 0: case 1: return 'S'; /* Small */
        case 2: case 3: return 'M'; /* Medium */
        default:        return 'L'; /* Large */
    }
}

int main(void) {
    /* === Sistema Principal === */
    printf("=== SISTEMA PRINCIPAL: CADENA DE SUMINISTRO ===\n");

    /* Variables "locales" al main */
    int pedido_id, unidades;
    double costo_unit, costo_total, lead_time;
    char cliente[50];
    char transporte;

    modulo_entrada(&pedido_id, &unidades, &costo_unit, cliente);

    /* Elegimos transporte con lectura controlada */
    printf("Modo de transporte [A=aereo, T=terrestre, M=maritimo]: ");
    scanf(" %c", &transporte);
    transporte = (char)toupper((unsigned char)transporte);

    modulo_procesamiento(unidades, costo_unit, transporte, &costo_total, &lead_time);
    modulo_salida(pedido_id, cliente, unidades, costo_unit, transporte, costo_total, lead_time);
    modulo_reportes(costo_total, lead_time);

    printf("\n>>> Flujo completado: Entrada -> Procesamiento -> Salida -> Reportes\n");
    return 0;
}
