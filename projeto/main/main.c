#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "driver/gpio.h"

#include "servo_control.h"
#include "wifi_config.h"
#include "mqtt_app.h"

static const char *TAG = "PET_FEEDER";

#define TEMPO_ESPERA_SEGUNDOS 1

#define BOTAO_PIN GPIO_NUM_14

void aciona_alimentador_task(void *pvParameter) {
    ESP_LOGI(TAG, "Abrindo a tampa (movendo o servo para 90 graus).");
    led_on();
    mqtt_publish_status("aberto");
    servo_set_angle(90);

    ESP_LOGI(TAG, "Tampa aberta. Aguardando %d segundos...", TEMPO_ESPERA_SEGUNDOS);
    vTaskDelay(pdMS_TO_TICKS(TEMPO_ESPERA_SEGUNDOS * 1000));

    ESP_LOGI(TAG, "Fechando a tampa (movendo o servo para 0 graus).");
    servo_set_angle(0);

    led_off();
    mqtt_publish_status("fechado");
    
    vTaskDelete(NULL);
}

void app_main(void) {
    servo_init();
    servo_set_angle(0);

    gpio_reset_pin(BOTAO_PIN);
    gpio_set_direction(BOTAO_PIN, GPIO_MODE_INPUT);
    gpio_set_pull_mode(BOTAO_PIN, GPIO_PULLUP_ONLY);

    ESP_LOGI(TAG, "Iniciando Wi-Fi...");
    wifi_init_sta();

    ESP_LOGI(TAG, "Iniciando cliente MQTT...");
    mqtt_app_start();

    ESP_LOGI(TAG, "Alimentador de pets pronto. Aguardando comandos...");

    while(1) {
        if (gpio_get_level(BOTAO_PIN) == 0) {
            ESP_LOGI(TAG, "Botao fisico pressionado!");
            
            vTaskDelay(pdMS_TO_TICKS(50));
            
            if (gpio_get_level(BOTAO_PIN) == 0) {
                xTaskCreate(&aciona_alimentador_task, "aciona_alimentador", 2048, NULL, 5, NULL);
                
                while (gpio_get_level(BOTAO_PIN) == 0) {
                    vTaskDelay(pdMS_TO_TICKS(50));
                }
            }
        }
        vTaskDelay(pdMS_TO_TICKS(50));
    }
}