#ifndef MQTT_APP_H
#define MQTT_APP_H

#include "mqtt_app.h"

void mqtt_app_start(void);
void mqtt_publish_status(const char* status_msg);

#endif
