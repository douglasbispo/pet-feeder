#include "servo_control.h"
#include "driver/ledc.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

// Define o pino GPIO
#define SERVO_PIN 12

// Define as configurações do PWM para o servo
#define SERVO_LEDC_TIMER LEDC_TIMER_0
#define SERVO_LEDC_MODE LEDC_LOW_SPEED_MODE
#define SERVO_LEDC_CHANNEL LEDC_CHANNEL_0
#define SERVO_LEDC_DUTY_RESOLUTION LEDC_TIMER_16_BIT
#define SERVO_LEDC_FREQUENCY 50

void servo_init() {
    ledc_timer_config_t timer_conf = {
        .speed_mode = SERVO_LEDC_MODE,
        .duty_resolution = SERVO_LEDC_DUTY_RESOLUTION,
        .timer_num = SERVO_LEDC_TIMER,
        .freq_hz = SERVO_LEDC_FREQUENCY,
        .clk_cfg = LEDC_AUTO_CLK
    };
    ledc_timer_config(&timer_conf);

    ledc_channel_config_t channel_conf = {
        .speed_mode = SERVO_LEDC_MODE,
        .channel = SERVO_LEDC_CHANNEL,
        .timer_sel = SERVO_LEDC_TIMER,
        .intr_type = LEDC_INTR_DISABLE,
        .gpio_num = SERVO_PIN,
        .duty = 0,
        .hpoint = 0
    };
    ledc_channel_config(&channel_conf);
}

void servo_set_angle(int angle) {
    uint32_t duty = (angle * (5000) / 180) + 1634;

    ledc_set_duty(SERVO_LEDC_MODE, SERVO_LEDC_CHANNEL, duty);
    ledc_update_duty(SERVO_LEDC_MODE, SERVO_LEDC_CHANNEL);
}