idf_component_register(SRCS "src/appwifi.c"
                    INCLUDE_DIRS "include"
                    REQUIRES esp_wifi nvs_flash)
