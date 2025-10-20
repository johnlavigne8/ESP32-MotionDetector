#include "wifi.h"
#include "esp_log.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "esp_netif.h"


static const char *TAG = "wifi";

static void wifi_event_handler(void *arg, esp_event_base_t event_base, int32_t event_id, void *event_data)
{
    if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START) 
    {
        ESP_LOGI(TAG, "Wi-Fi started, connecting...");
        esp_wifi_connect();
    } 
    else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_DISCONNECTED) 
    {
        ESP_LOGW(TAG, "Disconnected from Wi-Fi, retrying...");
        esp_wifi_connect(); // Auto-reconnect
    } 
    else if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP) 
    {
        ip_event_got_ip_t *event = (ip_event_got_ip_t *)event_data;
        ESP_LOGI(TAG, "Got IP: " IPSTR, IP2STR(&event->ip_info.ip));
    }
}

esp_err_t wifi_init_sta(void)
{
	esp_err_t err;
	
	err = nvs_flash_init();
	if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) 
	{
    	ESP_ERROR_CHECK(nvs_flash_erase());
    	err = nvs_flash_init();
	}
	if (err != ESP_OK) 
	{
		return err;
	}
	
	
	err = esp_netif_init();
	
	if(err != ESP_OK){
		return err;
	}
	
	err = esp_event_loop_create_default();
	if(err != ESP_OK && err != ESP_ERR_INVALID_STATE){
		return err;
	}
	esp_netif_create_default_wifi_sta();
	
	esp_event_handler_instance_register(WIFI_EVENT,
                                    ESP_EVENT_ANY_ID,
                                    &wifi_event_handler,
                                    NULL,
                                    NULL);
	esp_event_handler_instance_register(IP_EVENT,
                                    IP_EVENT_STA_GOT_IP,
                                    &wifi_event_handler,
                                    NULL,
                                    NULL);
	
	wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    err = esp_wifi_init(&cfg);
    if (err != ESP_OK) 
    {
		return err;
    }
    
    wifi_config_t wifi_config = {
    	.sta = {
        	.ssid = "Lavignes Wifi",
        	.password = "JohnMay2002",
        	.threshold.authmode = WIFI_AUTH_WPA2_PSK,
    	},
	};
	esp_wifi_set_mode(WIFI_MODE_STA);
	esp_wifi_set_config(WIFI_IF_STA, &wifi_config);
	esp_wifi_start();
	
	return ESP_OK;
}

