idf_component_register(
    SRCS "main.c"
    INCLUDE_DIRS "."
    REQUIRES servo_control wifi_config mqtt_client
)
