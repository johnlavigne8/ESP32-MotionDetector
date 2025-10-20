#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "driver/gpio.h"
#include "wifi.h"
#include "mqtt_client.h"
#include "mqtt_client_wrapper.h"

#define PIR_GPIO GPIO_NUM_25
static QueueHandle_t pir_event_queue = NULL;

static void IRAM_ATTR pir_isr_handler(void* arg)
{
	uint32_t gpio_num = (uint32_t) arg;
    // Do the minimum here — just notify another task
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    xQueueSendFromISR(pir_event_queue, &gpio_num, &xHigherPriorityTaskWoken);
    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}

static void pir_task(void* arg)
{
    uint32_t io_num;
    while (1) {
        if (xQueueReceive(pir_event_queue, &io_num, portMAX_DELAY)) {
            char msg[64];
			snprintf(msg, sizeof(msg), "Motion detected on GPIO %lu", io_num);
			esp_mqtt_client_publish(mqtt_client, MQTT_TOPIC, msg, 0, 1, 0);
			ESP_LOGI("PIR_TASK", "Published: %s", msg);
        }
    }
}

static esp_err_t pir_sensor_init(void)
{
	//configs GPIO pin 25 as input with pullup + pulldown diabled and interrupts on positive edge
	gpio_config_t io = {	
		.pin_bit_mask = 1ULL << PIR_GPIO,
		.mode = GPIO_MODE_INPUT,
		.pull_up_en = GPIO_PULLUP_DISABLE,
		.pull_down_en = GPIO_PULLDOWN_DISABLE,
		.intr_type = GPIO_INTR_POSEDGE,
	};
	esp_err_t err = gpio_config(&io);
	if(err != ESP_OK)
	{
		return err;
	}
	err = gpio_install_isr_service(0);
	
	if (err != ESP_OK && err != ESP_ERR_INVALID_STATE) {
        return err;
    }

	err = gpio_isr_handler_add(PIR_GPIO, pir_isr_handler, (void*) PIR_GPIO);
	return err;
}

void app_main(void)
{
	esp_err_t err = wifi_init_sta();
    if (err != ESP_OK) {
        ESP_LOGE("MAIN", "Wi-Fi init failed: %s", esp_err_to_name(err));
        return;
    }
	
	err = mqtt_app_start();
	if (err != ESP_OK) {
        ESP_LOGE("MAIN", "MQTT init failed: %s", esp_err_to_name(err));
        return;
    }
	
	
    pir_event_queue = xQueueCreate(10, sizeof(uint32_t));
    if (!pir_event_queue) {
        ESP_LOGE("MAIN", "Failed to create PIR queue");
        return;
    }

    err = pir_sensor_init();
    if (err != ESP_OK) {
        ESP_LOGE("MAIN", "Failed to init PIR sensor: %s", esp_err_to_name(err));
        return;
    }

    xTaskCreatePinnedToCore(pir_task, "pir_task", 2048, NULL, 10, NULL, tskNO_AFFINITY);
}