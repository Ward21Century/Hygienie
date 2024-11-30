idf_component_register(SRCS "src/Appmqtt.c"
INCLUDE_DIRS "include"
REQUIRES mqtt esp_driver_gpio json appdata log esp_netif)

