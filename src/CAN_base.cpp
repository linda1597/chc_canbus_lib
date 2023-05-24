

#include "CAN_base.h"

#ifdef CAN_lib_1
#include "driver/gpio.h"
#include "driver/twai.h"
#endif

#ifdef CAN_lib_2
#include "CAN.h"
#endif

bool CAN_base_init(int pinCanRx, int pinCanTx)
{
#ifdef CAN_lib_1
    twai_general_config_t g_config = TWAI_GENERAL_CONFIG_DEFAULT(
        (gpio_num_t)pinCanRx,
        (gpio_num_t)pinCanTx,
        TWAI_MODE_NORMAL); // TWAI_MODE_NORMAL, TWAI_MODE_NO_ACK or TWAI_MODE_LISTEN_ONLY

    twai_timing_config_t t_config = TWAI_TIMING_CONFIG_500KBITS();
    twai_filter_config_t f_config = TWAI_FILTER_CONFIG_ACCEPT_ALL();

    esp_err_t err = twai_driver_install(&g_config, &t_config, &f_config);
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

#endif
#ifdef CAN_lib_2
    CAN.setPins(pinCanRx, pinCanTx);
    while (!CAN.begin(vCANbus_baudrate)) {
        CAN_LOG_E("CAN BUS Shield init fail");
        vTaskDelay(pdMS_TO_TICKS(1000));
        return false;
    }
    CAN_LOG_I("CAN BUS Shield init ok!");
    return true;
#endif
}

bool CAN_base_transmit(CAN_frame_t* CANFrame)
{
#ifdef CAN_lib_1
    esp_err_t err = twai_transmit((twai_message_t*)CANFrame, 0);
    if (err == ESP_OK) {
        CAN_LOG_I("Message queued for transmission");
    } else {
        String err_s = esp_err_to_name(err);
        CAN_LOG_I("Failed to queue message for transmission: %s", err_s.c_str());
        return false;
    }
    return true;
#endif
#ifdef CAN_lib_2
    CAN.beginPacket(
        CANFrame->identifier,
        CANFrame->data_length_code,
        CANFrame->rtr);

    CAN.write(CANFrame->data, CANFrame->data_length_code);

    if (!CAN.endPacket()) {
        CAN_LOG_E("send CAN Message . failed");
        CAN.end();
        vTaskDelay(pdMS_TO_TICKS(500));
        if (!CAN.begin(vCANbus_baudrate)) {
            CAN_LOG_I("CAN init failed ");
        }
        return false;
    }
    return true;
#endif
}

bool CAN_base_receive(CAN_frame_t* CANFrame, long timeout_ms)
{
#ifdef CAN_lib_1
    esp_err_t err = twai_receive((twai_message_t*)CANFrame, timeout_ms);
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
#endif
#ifdef CAN_lib_2
    int packetSize = CAN.parsePacket();
    if (packetSize) {
        CANFrame->identifier = CAN.packetId();
        CANFrame->extd = CAN.packetExtended();
        CANFrame->data_length_code = CAN.packetDlc();
        CANFrame->rtr = CAN.packetRtr();
        for (int i = 0; i < CANFrame->data_length_code; i++) {
            CANFrame->data[i] = CAN.read();
        }
        CAN_LOG_I("Received %d bytes", packetSize);
        return true;
    }
    return false;
#endif
}
