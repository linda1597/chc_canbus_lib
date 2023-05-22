#include "CAN.h"

#ifdef ESP32
#include "driver/gpio.h"
#include "driver/twai.h"

void CAN_init(int pinCanRx, int pinCanTx);
{
    twai_general_config_t g_config = TWAI_GENERAL_CONFIG_DEFAULT((num_), GPIO_NUM_45, TWAI_MODE_NORMAL); // TWAI_MODE_NORMAL, TWAI_MODE_NO_ACK or TWAI_MODE_LISTEN_ONLY
    twai_timing_config_t t_config = TWAI_TIMING_CONFIG_500KBITS();
    twai_filter_config_t f_config = TWAI_FILTER_CONFIG_ACCEPT_ALL();

    err = twai_driver_install(&g_config, &t_config, &f_config);

    if (err == ESP_OK) {
        CAN_LOG_I("Driver installed");
    } else {
        CAN_LOG_I("Failed to install driver");
        // return err;
        return false;
    }
    err = twai_start();
    if (err == ESP_OK) {
        CAN_LOG_I("Driver started");
    } else {
        CAN_LOG_I("Failed to start driver");
        // return err;
        return false;
    }
    //     return err;
    return true;
}
bool CAN_transmit(uint8_t* data)
{
    esp_err_t err = twai_transmit(data, 0);
    if (err == ESP_OK) {
        CAN_LOG_I("Message queued for transmission");
    } else {
        String err_s = esp_err_to_name(err);
        CAN_LOG_I("Failed to queue message for transmission: %s", err_s.c_str());
        return false;
    }
    return true;
}

bool CAN_receive(uint8_t* data, long timeout_ms)
{
    esp_err_t err = twai_receive(data, timeout_ms);
    if (err == ESP_OK) {
        CAN_LOG_I("Message received");
    } else if (err == ESP_ERR_TIMEOUT) {
        CAN_LOG_E("Receive timeout");
        return false;
    } else {
        String err_s = esp_err_to_name(err);
        CAN_LOG_E("Failed to receive message: %s", err_s.c_str());
        return false;
    }
    return true;
}
#else

void CAN_init(int pinCanRx, int pinCanTx)
{
}

bool CAN_transmit(uint8_t* data)
{
}

bool CAN_receive(uint8_t* data, long timeout_ms)
{
}

#endif