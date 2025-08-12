#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "servo_control.h" // arquivo de cabeçalho

// Tag para logs
static const char *TAG = "PET_FEEDER";

// Tempo de espera em segundos para o servo fechar a tampa.
#define TEMPO_ESPERA_SEGUNDOS 5

void app_main(void) {
    // Inicializa o controle do servo motor
    servo_init();

    ESP_LOGI(TAG, "Alimentador de pets inicializado. Aguardando comando...");
    
    // Configura o servo para a posição inicial (tampa fechada)
    servo_set_angle(0);

    while(1) {
        // Esta é a lógica de abrir e fechar a tampa
        ESP_LOGI(TAG, "Abrindo a tampa (movendo o servo para 180 graus).");
        // Abre a tampa (gira o servo 180 graus).
        servo_set_angle(180);

        // Espera o tempo determinado para a ração cair.
        ESP_LOGI(TAG, "Tampa aberta. Aguardando %d segundos...", TEMPO_ESPERA_SEGUNDOS);
        vTaskDelay(pdMS_TO_TICKS(TEMPO_ESPERA_SEGUNDOS * 1000));

        // Fecha a tampa (gira o servo de volta para 0 graus).
        ESP_LOGI(TAG, "Fechando a tampa (movendo o servo para 0 graus).");
        servo_set_angle(0);
        
        ESP_LOGI(TAG, "Ciclo concluido. Aguardando 10 segundos para o próximo ciclo...");
        vTaskDelay(pdMS_TO_TICKS(10000));
    }
}