#ifndef SERVO_CONTROL_H
#define SERVO_CONTROL_H

// Inicializa o driver LEDC para controlar o servo motor
void servo_init(void);

void servo_set_angle(int angle);

#endif