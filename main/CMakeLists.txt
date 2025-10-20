idf_component_register(SRCS "motion_main.c" "wifi.c" "mqtt_client.c"
                       INCLUDE_DIRS "."
                       REQUIRES esp_wifi esp_event nvs_flash driver mqtt)
