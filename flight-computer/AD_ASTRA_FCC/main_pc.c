#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "App/estimador.h"
#include "App/flight_sm.h"

int main(void)
{
    // Intentar abrir el archivo de datos de vuelo
    FILE *fp = fopen("vuelo_normal.csv", "r");

    if (!fp)
    {
        fp = fopen("imu_simulada.csv", "r");
    }

    if (!fp)
    {
        printf("[ERROR] No se encontro el archivo CSV de datos ('vuelo_normal.csv' o 'imu_simulada.csv').\n");
        return 1;
    }

    printf("=========================================================\n");
    printf(" SIMULACION ALGORITMO DE VUELO EN PC (PROYECTO AD ASTRA) \n");
    printf("=========================================================\n\n");

    // Inicializar modulos del algoritmo
    Estimador_Init(0.0f);
    FlightSM_Init();

    char line_buffer[256];

    // Saltar la primera linea de cabecera del CSV
    if (!fgets(line_buffer, sizeof(line_buffer), fp))
    {
        fclose(fp);
        return 1;
    }

    EstadoVuelo_t estado_anterior = ESTADO_RAMPA;

    while (fgets(line_buffer, sizeof(line_buffer), fp))
    {
        float tiempo_s = 0.0f, ax = 0.0f, ay = 0.0f, az = 0.0f;
        float gx = 0.0f, gy = 0.0f, gz = 0.0f, presion = 0.0f, altitud = 0.0f;

        // Lectura de 9 columnas estándar (Tiempo, Ax, Ay, Az, Gx, Gy, Gz, Presion, Altitud)
        int parsed = sscanf(line_buffer, "%f,%f,%f,%f,%f,%f,%f,%f,%f",
                            &tiempo_s, &ax, &ay, &az, &gx, &gy, &gz, &presion, &altitud);

        if (parsed < 9)
        {
            // Formato alternativo simplificado (Tiempo, Az, Altitud)
            parsed = sscanf(line_buffer, "%f,%f,%f", &tiempo_s, &az, &altitud);

            if (parsed < 3)
                continue;
        }

        uint32_t tiempo_ms = (uint32_t)(tiempo_s * 1000.0f);
        float dt = 0.01f; // Frecuencia de muestreo (100 Hz = 0.01 s)

        // 1. Filtrar y estimar altitud / velocidad vertical
        Estimador_Update(altitud, az, dt);

        // 2. Actualizar maquina de estados de vuelo
        FlightSM_Update(tiempo_ms, dt);

        EstadoVuelo_t estado_actual = FlightSM_GetEstado();

        // 3. Imprimir eventos y cambios de estado por pantalla
        if (estado_actual != estado_anterior)
        {
            switch (estado_actual)
            {
                case ESTADO_PROPULSADO:
                    printf("[t=%6.2fs] EVENTO: DESPEGUE DETECTADO | Aceleracion Z: %.2f m/s2\n",
                           tiempo_s, az);
                    break;

                case ESTADO_COASTING:
                    printf("[t=%6.2fs] EVENTO: FIN DE PROPULSION (Coasting) | Altitud: %.2f m\n",
                           tiempo_s, Estimador_GetAltitude());
                    break;

                case ESTADO_APOGEO_DROGUE:
                    printf("\n[t=%6.2fs] EVENTO: APOGEO DETECTADO a %.2f m!\n",
                           tiempo_s, Estimador_GetAltitude());
                    printf(" >>> [DISPARO PYRO 1 (DROGUE)] ACTIVADO <<<\n\n");
                    break;

                case ESTADO_DESCENSO_MAIN:
                    printf("[t=%6.2fs] EVENTO: APERTURA PARACAIDAS PRINCIPAL a %.2f m\n",
                           tiempo_s, Estimador_GetAltitude());
                    printf(" >>> [DISPARO PYRO 2 (MAIN)] ACTIVADO <<<\n\n");
                    break;

                case ESTADO_ATERRIZADO:
                    printf("[t=%6.2fs] EVENTO: COHETE ATERRIZADO CON EXITO.\n",
                           tiempo_s);
                    break;

                default:
                    break;
            }

            estado_anterior = estado_actual;
        }
    }

    fclose(fp);

    printf("\n=========================================================\n");
    printf(" SIMULACION FINALIZADA COMPLETAMENTE\n");
    printf("=========================================================\n");

    return 0;
}