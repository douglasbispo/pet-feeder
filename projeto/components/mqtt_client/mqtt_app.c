#include <stdio.h>
#include <string.h>
#include "esp_log.h"
#include "esp_event.h"
#include "mqtt_client.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define MQTT_BROKER_URL "mqtt://broker.hivemq.com"
#define MQTT_TOPIC_COMANDO "pet_feeder/comando"
#define MQTT_TOPIC_STATUS "pet_feeder/status"
#define MQTT_COMANDO_ACIONAR "acionar"

static const char *TAG = "MQTT_CLIENT";

extern void aciona_alimentador_task(void *pvParameter);

static esp_mqtt_client_handle_t client_handle;

void mqtt_publish_status(const char* status_msg) {
    if (client_handle) {
        esp_mqtt_client_publish(client_handle, MQTT_TOPIC_STATUS, status_msg, 0, 1, 0);
        ESP_LOGI(TAG, "Status publicado: %s", status_msg);
    }
}

static void mqtt_event_handler(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data) {
    esp_mqtt_event_handle_t event = event_data;
    esp_mqtt_client_handle_t client = event ? event->client : NULL;

    if (!event) {
        ESP_LOGW(TAG, "Event data é NULL");
        return;
    }

    switch (event->event_id) {
        case MQTT_EVENT_CONNECTED:
            ESP_LOGI(TAG, "Conectado ao broker MQTT.");
            if (client) {
                esp_mqtt_client_subscribe(client, MQTT_TOPIC_COMANDO, 0);
            }

            mqtt_publish_status("online");
            break;

        case MQTT_EVENT_DISCONNECTED:
            ESP_LOGW(TAG, "Desconectado do broker MQTT.");
            break;

        case MQTT_EVENT_DATA: {
            ESP_LOGI(TAG, "Mensagem recebida no tópico: %.*s", event->topic_len, event->topic);
            ESP_LOGI(TAG, "Mensagem: %.*s", event->data_len, event->data);

            char payload[event->data_len + 1];
            memcpy(payload, event->data, event->data_len);
            payload[event->data_len] = '\0';

            for (int i = (int)strlen(payload) - 1; i >= 0; i--) {
                if (payload[i] == '\n' || payload[i] == '\r' || payload[i] == ' ')
                    payload[i] = '\0';
                else
                    break;
            }

            if (strcmp(payload, MQTT_COMANDO_ACIONAR) == 0) {
                ESP_LOGI(TAG, "Comando de acionamento recebido via MQTT.");
                xTaskCreate(&aciona_alimentador_task, "aciona_alimentador", 2048, NULL, 5, NULL);
            } else {
                ESP_LOGW(TAG, "Comando desconhecido: %s", payload);
            }
            break;
        }

        case MQTT_EVENT_ERROR:
            ESP_LOGE(TAG, "Erro de MQTT.");
            break;

        default:
            ESP_LOGI(TAG, "Outro evento: %d", (int)event->event_id);
            break;
    }
}

void mqtt_app_start(void) {
    const esp_mqtt_client_config_t mqtt_cfg = {
        .broker.address.uri = MQTT_BROKER_URL,
    };

    esp_mqtt_client_handle_t client = esp_mqtt_client_init(&mqtt_cfg);
    esp_mqtt_client_register_event(client, ESP_EVENT_ANY_ID, mqtt_event_handler, NULL);
    esp_mqtt_client_start(client);
}
