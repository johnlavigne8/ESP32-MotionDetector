#include "mqtt_client_wrapper.h"
#include "esp_log.h"

#define MQTT_BROKER_URI "mqtt://192.168.200.3"
const char *MQTT_TOPIC = "home/motion";
esp_mqtt_client_handle_t mqtt_client = NULL;

static const char *TAG = "MQTT";

static void mqtt_event_handler_cb(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data)
{
    esp_mqtt_event_handle_t event = event_data;

    switch (event->event_id) {
        case MQTT_EVENT_CONNECTED:
            ESP_LOGI(TAG, "MQTT Connected");

            // Home Assistant MQTT Discovery payload
            const char* discovery_topic = "homeassistant/binary_sensor/pir_motion/config";
            const char* discovery_payload =
                "{"
                "\"name\": \"PIR Motion Sensor\","
                "\"state_topic\": \"home/motion\","
                "\"payload_on\": \"Motion detected on GPIO 25\","
                "\"payload_off\": \"idle\","
                "\"device_class\": \"motion\","
                "\"expire_after\": 10"
                "}";

            // Publish retained discovery config
            esp_mqtt_client_publish(event->client, discovery_topic, discovery_payload, 0, 1, true);
            ESP_LOGI(TAG, "Published discovery config");
            break;

        case MQTT_EVENT_DISCONNECTED:
            ESP_LOGW(TAG, "MQTT Disconnected");
            break;

        default:
            break;
    }
}
esp_err_t mqtt_app_start(void)
{
    esp_mqtt_client_config_t mqtt_cfg = {
    	.broker = {
        	.address = {
            	.uri = MQTT_BROKER_URI
        	}
    	}
	};
    mqtt_client = esp_mqtt_client_init(&mqtt_cfg);
    if (mqtt_client == NULL) {
        return ESP_FAIL;
    }

    esp_mqtt_client_register_event(mqtt_client, ESP_EVENT_ANY_ID, mqtt_event_handler_cb, NULL);
    return esp_mqtt_client_start(mqtt_client);
}

