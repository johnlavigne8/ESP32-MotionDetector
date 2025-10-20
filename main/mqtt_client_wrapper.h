#pragma once

#include "esp_err.h"
#include "mqtt_client.h"

extern esp_mqtt_client_handle_t mqtt_client;  // Declared in C file
extern const char *MQTT_TOPIC;

esp_err_t mqtt_app_start(void);
