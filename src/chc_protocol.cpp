
#include "CAN.h"

#include "chc_protocol.h"

// esp_err_t err;
// String err_s;
// twai_message_t rx_msg;
// twai_message_t message;

chc_PROTOCOL::chc_PROTOCOL()
{
    // 建構子
}
// #if node_HMI == 1
#if node_HMI == 1
bool chc_PROTOCOL::rx_HMI()
{

    // err = twai_receive(&rx_msg, 500);
    // if (err == ESP_OK)
    //     ;
    // else
    //     return err;

    if (CAN_receive(rx_msg, 500) == false) {
        return false;
    }

    CHC_PL_LOG("Received:");
    for (uint8_t i = 0; i < rx_msg.data_length_code; i++) {
        CHC_PL_S("%02X ", rx_msg.data[i]);
    }
    CHC_PL_S("\n");

    switch (rx_msg.identifier) {
    case DIAGtoHMI:
        return REQ_ERC;
    case MCU_ID1:
        mcu_data.gear = rx_msg.data[0];
        mcu_data.torque = rx_msg.data[1] | (rx_msg.data[2] << 8);
        mcu_data.cadence = rx_msg.data[3] | (rx_msg.data[4] << 8);
        mcu_data.velocity = rx_msg.data[5] | (rx_msg.data[6] << 8);
        mcu_data.battery = rx_msg.data[7];
        return PROCESS_DONE;
    case RRU_ID1:
        rru_data.ID = rx_msg.data[0];
        rru_data.distance = rx_msg.data[1] | (rx_msg.data[2] << 8);
        rru_data.velocity = rx_msg.data[3] | (rx_msg.data[4] << 8);
        rru_data.angle = rx_msg.data[5];
        return PROCESS_DONE;
    case RRU_ID2:
        rru_data.alarm_status = rx_msg.data[0];
        rru_data.light_status = rx_msg.data[1];
        return PROCESS_DONE;
    case CWS_ID1:
        cws_data.distance = rx_msg.data[0] | (rx_msg.data[1] << 8);
        cws_data.angle = rx_msg.data[2];
        return PROCESS_DONE;
    case NU_ID1:
        nu_data.longitude = rx_msg.data[0] | (rx_msg.data[1] << 8) | (rx_msg.data[2] << 16) | (rx_msg.data[3] << 24);
        nu_data.latitude = rx_msg.data[4] | (rx_msg.data[5] << 8) | (rx_msg.data[6] << 16) | (rx_msg.data[7] << 24);
        return PROCESS_DONE;
    case NU_ID2:
        nu_data.altitude = rx_msg.data[0] | (rx_msg.data[1] << 8);
        nu_data.velocity = rx_msg.data[2] | (rx_msg.data[3] << 8);
        return PROCESS_DONE;
    case MCU_DIAG:
        all_dtc.MCU_DTC = rx_msg.data[0];
        return PROCESS_DONE;
    case RRU_DIAG:
        all_dtc.RRU_DTC = rx_msg.data[0];
        return PROCESS_DONE;
    case CWS_DIAG:
        all_dtc.CWS_DTC = rx_msg.data[0];
        return PROCESS_DONE;
    case NU_DIAG:
        all_dtc.NU_DTC = rx_msg.data[0];
        return PROCESS_DONE;
    case HMI_V:
        for (uint8_t i = 0; i < 6; i++) {
            Rx.HMI_V_data[i] = rx_msg.data[i];
        }
        return PROCESS_DONE;
    case MCU_V:
        for (uint8_t i = 0; i < 6; i++) {
            Rx.MCU_V_data[i] = rx_msg.data[i];
        }
        return PROCESS_DONE;
    case RRU_V:
        for (uint8_t i = 0; i < 6; i++) {
            Rx.RRU_V_data[i] = rx_msg.data[i];
        }
        return PROCESS_DONE;
    case CWS_V:
        for (uint8_t i = 0; i < 6; i++) {
            Rx.CWS_V_data[i] = rx_msg.data[i];
        }
        return PROCESS_DONE;
    case NU_V:
        for (uint8_t i = 0; i < 6; i++) {
            Rx.NU_V_data[i] = rx_msg.data[i];
        }
        return PROCESS_DONE;
    default:
        return NONE;
    }
}

// 診斷碼
bool chc_PROTOCOL::HMItoDIAG(uint8_t error)
{
    message.identifier = HMI_DIAG;
    message.extd = 0;
    message.rtr = 0;
    message.data_length_code = 1;
    message.data[0] = error;
    return CAN_transmit(message);
    // err = twai_transmit(&message, 0);
    // if (err == ESP_OK)
    //     ;
    // else {
    //     err_s = esp_err_to_name(err);
    //     CHC_PL_E("%s", err_s);
    // }
}

// 心跳裝置連線狀態、心跳數據、運動模式
bool chc_PROTOCOL::HMI_period(uint8_t status, uint8_t HR, uint8_t mode)
{
    message.identifier = HMI_ID1;
    message.extd = 0;
    message.rtr = 0;
    message.data_length_code = 3;
    message.data[0] = status; // 心跳裝置連線狀態
    message.data[1] = HR; // 心跳數據
    message.data[2] = mode; // 運動模式
    return CAN_transmit(message);

    // err = twai_transmit(&message, 0);
    // if (err == ESP_OK)
    //     ;
    // else {
    //     err_s = esp_err_to_name(err);
    //     CHC_PL_E("%s", err_s);
    // }
}

// 設定輔助力
bool chc_PROTOCOL::HMI_setSupport(uint8_t support)
{
    message.identifier = HMI_ID2;
    message.extd = 0;
    message.rtr = 0;
    message.data_length_code = 1;
    message.data[0] = support; // 輔助力段位 0~5
    return CAN_transmit(message);
    // err = twai_transmit(&message, 0);
    // if (err == ESP_OK)
    //     ;
    // else {
    //     err_s = esp_err_to_name(err);
    //     CHC_PL_E("%s", err_s);
    // }
}

// NM獲取其他部件資訊
bool chc_PROTOCOL::NM_getInfo(bool getMCUInfo, bool getRRUInfo, bool getCWSInfo, bool getNUInfo)
{
    uint8_t flag = 0b00000000;
    if (getMCUInfo == 1)
        flag = flag | 0b00000001;
    if (getRRUInfo == 1)
        flag = flag | 0b00000100;
    if (getCWSInfo == 1)
        flag = flag | 0b00010000;
    if (getNUInfo == 1)
        flag = flag | 0b01000000;
    message.identifier = NM_get_info;
    message.extd = 0;
    message.rtr = 0;
    message.data_length_code = 1;
    message.data[0] = flag;
    return CAN_transmit(message);

    // err = twai_transmit(&message, 0);
    // if (err == ESP_OK)
    //     ;
    // else {
    //     err_s = esp_err_to_name(err);
    //     CHC_PL_E("%s", err_s);
    // }
}

/* NM指令
   0:休眠
   1:開機 */
bool chc_PROTOCOL::NM_CMD(bool setMCU, bool setRRU, bool setCWS, bool setNU)
{
    uint8_t flag = 0b00000000;
    if (setMCU == 1)
        flag = flag | 0b00000001;
    if (setRRU == 1)
        flag = flag | 0b00000100;
    if (setCWS == 1)
        flag = flag | 0b00010000;
    if (setNU == 1)
        flag = flag | 0b01000000;
    message.identifier = NM_set_CMD;
    message.extd = 0;
    message.rtr = 0;
    message.data_length_code = 1;
    message.data[0] = flag;
    return CAN_transmit(&message);

    // err = twai_transmit(&message, 0);
    // if (err == ESP_OK)
    //     ;
    // else {
    //     err_s = esp_err_to_name(err);
    //     CHC_PL_E("%s", err_s);
    // }
}

// 設定RRU警示距離、警示LED閃爍頻率
bool chc_PROTOCOL::setRRU(uint16_t distance, uint8_t Hz)
{
    message.identifier = HMItoRRU;
    message.extd = 0;
    message.rtr = 0;
    message.data_length_code = 3;
    message.data[0] = distance;
    message.data[1] = distance >> 8;
    message.data[2] = Hz;
    return CAN_transmit(message);
    // err = twai_transmit(&message, 0);
    // if (err == ESP_OK)
    //     ;
    // else {
    //     err_s = esp_err_to_name(err);
    //     CHC_PL_E("%s", err_s);
    // }
}

// 設定CWS警示距離
bool chc_PROTOCOL::setCWS(uint16_t distance)
{
    message.identifier = HMItoCWS;
    message.extd = 0;
    message.rtr = 0;
    message.data_length_code = 2;
    message.data[0] = distance;
    message.data[1] = distance >> 8;
    return CAN_transmit(message);
    // err = twai_transmit(&message, 0);
    // if (err == ESP_OK)
    //     ;
    // else {
    //     err_s = esp_err_to_name(err);
    //     CHC_PL_E("%s", err_s);
    // }
}

// 傳送版本資訊
bool chc_PROTOCOL::HMI_version(uint8_t protocol_major, uint8_t protocol_minor, uint8_t sw_major, uint8_t sw_minor, uint8_t hw_major, uint8_t hw_minor)
{
    message.identifier = HMI_V;
    message.extd = 0;
    message.rtr = 0;
    message.data_length_code = 6;
    message.data[0] = protocol_major;
    message.data[1] = protocol_minor;
    message.data[2] = sw_major;
    message.data[3] = sw_minor;
    message.data[4] = hw_major;
    message.data[5] = hw_minor;
    return CAN_transmit(message);

    // err = twai_transmit(&message, 0);
    // if (err == ESP_OK)
    //     ;
    // else {
    //     err_s = esp_err_to_name(err);
    //     CHC_PL_E("%s", err_s);
    // }
}

#elif node_MCU == 1
// 接收段程式
bool chc_PROTOCOL::rx_MCU()
{
    // err = twai_receive(&rx_msg, 500);
    // if (err == ESP_OK)
    //     ;
    // else {
    //     return err;
    // }

    if (CAN_receive(rx_msg, 500) == false)
        return false;

    CHC_PL_LOG("Received:");
    for (uint8_t i = 0; i < rx_msg.data_length_code; i++) {
        CHC_PL_S("%02X ", rx_msg.data[i]);
    }
    CHC_PL_S("\n");

    switch (rx_msg.identifier) {
    case NM_get_info:
        rx_msg.data[0] = rx_msg.data[0] & 0b00000011;
        if (rx_msg.data[0] == 0b00000001) {
            MCU_version(protocol_Major, protocol_Minor, sw_Major, sw_Minor, hw_Major, hw_Minor);
            return PROCESS_DONE;
        } else {
            return NONE;
        }
    case NM_set_CMD:
        rx_msg.data[0] = rx_msg.data[0] & 0b00000011;
        if (rx_msg.data[0] == 0b00000001) {
            return SET_MCU_AWAKE;
        } else {
            return SET_MCU_SLEEP;
        }
    case DIAGtoMCU:
        return REQ_ERC;
    case HMI_V:
        for (uint8_t i = 0; i < 6; i++) {
            Rx.HMI_V_data[i] = rx_msg.data[i];
        }
        return PROCESS_DONE;
    case MCU_V:
        for (uint8_t i = 0; i < 6; i++) {
            Rx.MCU_V_data[i] = rx_msg.data[i];
        }
        return PROCESS_DONE;
    case RRU_V:
        for (uint8_t i = 0; i < 6; i++) {
            Rx.RRU_V_data[i] = rx_msg.data[i];
        }
        return PROCESS_DONE;
    case CWS_V:
        for (uint8_t i = 0; i < 6; i++) {
            Rx.CWS_V_data[i] = rx_msg.data[i];
        }
        return PROCESS_DONE;
    case NU_V:
        for (uint8_t i = 0; i < 6; i++) {
            Rx.NU_V_data[i] = rx_msg.data[i];
        }
        return PROCESS_DONE;
    default:
        return NONE;
    }
    return true;
}

// 診斷碼
bool chc_PROTOCOL::MCUtoDIAG(uint8_t error)
{
    message.identifier = MCU_DIAG;
    message.extd = 0;
    message.rtr = 0;
    message.data_length_code = 1;
    message.data[0] = error;
    return CAN_transmit(message);
    // err = twai_transmit(&message, 0);
    // if (err == ESP_OK)
    //     ;
    // else {
    //     err_s = esp_err_to_name(err);
    //     CHC_PL_E("%s", err_s);
    // }
}

// 傳送扭力、踏頻、速度、電量
bool chc_PROTOCOL::MCU_period(uint8_t support, uint16_t torque, uint16_t cadence, uint16_t speed, uint8_t battery)
{
    message.identifier = MCU_ID1;
    message.extd = 0;
    message.rtr = 0;
    message.data_length_code = 8;
    message.data[0] = support; // 輔助力段位
    message.data[1] = torque; // 扭力_L
    message.data[2] = torque >> 8; // 扭力_H
    message.data[3] = cadence; // 踏頻_L
    message.data[4] = cadence >> 8; // 踏頻_H
    message.data[5] = speed; // 速度_L
    message.data[6] = speed >> 8; // 速度_H
    message.data[7] = battery; // 電量
    return CAN_transmit(message);
    // err = twai_transmit(&message, 0);
    // if (err == ESP_OK)
    //     ;
    // else {
    //     err_s = esp_err_to_name(err);
    //     CHC_PL_E("%s", err_s);
    // }
}

// 傳送版本資訊
bool chc_PROTOCOL::MCU_version(uint8_t protocol_major, uint8_t protocol_minor, uint8_t sw_major, uint8_t sw_minor, uint8_t hw_major, uint8_t hw_minor)
{
    message.identifier = MCU_V;
    message.extd = 0;
    message.rtr = 0;
    message.data_length_code = 6;
    message.data[0] = protocol_major;
    message.data[1] = protocol_minor;
    message.data[2] = sw_major;
    message.data[3] = sw_minor;
    message.data[4] = hw_major;
    message.data[5] = hw_minor;
    return CAN_transmit(message);
    // err = twai_transmit(&message, 0);
    // if (err == ESP_OK)
    //     ;
    // else {
    //     err_s = esp_err_to_name(err);
    //     CHC_PL_E("%s", err_s);
    // }
}

#elif node_RRU == 1
// 接收段
bool chc_PROTOCOL::rx_RRU()
{

    // err = twai_receive(&rx_msg, 1000);
    // debug_printLog("Received:");
    // for (uint8_t i = 0; i < rx_msg.data_length_code; i++) {
    //     debug_print(rx_msg.data[i]);
    //     debug_print(" ");
    // }
    // debug_println();
    // if (err == ESP_OK)
    //     ;
    // else {
    //     return err;
    // }

    if (CAN_receive(rx_msg, 1000) == false)
        return NONE;

    CHC_PL_LOG("Received:");
    for (uint8_t i = 0; i < rx_msg.data_length_code; i++) {
        CHC_PL_S("%02X ", rx_msg.data[i]);
    }
    CHC_PL_S("\n");

    switch (rx_msg.identifier) {
    case NM_get_info:
        rx_msg.data[0] = rx_msg.data[0] & 0b00001100;
        if (rx_msg.data[0] == 0b00000100) {
            RRU_version(protocol_Major, protocol_Minor, sw_Major, sw_Minor, hw_Major, hw_Minor);
            return REQ_RRU_INFO;
        } else {
            return NONE;
        }
    case NM_set_CMD:
        rx_msg.data[0] = rx_msg.data[0] & 0b00001100;
        if (rx_msg.data[0] == 0b00000100) {
            return SET_RRU_AWAKE;
        } else {
            return SET_RRU_SLEEP;
        }
    case DIAGtoRRU:
        return REQ_ERC;
    case HMItoRRU:
        detect_range = rx_msg.data[0] | (rx_msg.data[1] << 8);
        bling_hz = rx_msg.data[2];
        return PROCESS_DONE;
    case HMI_V:
        for (uint8_t i = 0; i < 6; i++) {
            Rx.HMI_V_data[i] = rx_msg.data[i];
        }
        return PROCESS_DONE;
    case MCU_V:
        for (uint8_t i = 0; i < 6; i++) {
            Rx.MCU_V_data[i] = rx_msg.data[i];
        }
        return PROCESS_DONE;
    case RRU_V:
        for (uint8_t i = 0; i < 6; i++) {
            Rx.RRU_V_data[i] = rx_msg.data[i];
        }
        return PROCESS_DONE;
    case CWS_V:
        for (uint8_t i = 0; i < 6; i++) {
            Rx.CWS_V_data[i] = rx_msg.data[i];
        }
        return PROCESS_DONE;
    case NU_V:
        for (uint8_t i = 0; i < 6; i++) {
            Rx.NU_V_data[i] = rx_msg.data[i];
        }
        return PROCESS_DONE;
    default:
        return PROCESS_DONE;
    }
}

// 診斷碼
bool chc_PROTOCOL::RRUtoDIAG(uint8_t error)
{
    message.identifier = RRU_DIAG;
    message.extd = 0;
    message.rtr = 0;
    message.data_length_code = 1;
    message.data[0] = error;
    return CAN_transmit(message);
    // err = twai_transmit(&message, 0);
    // if (err == ESP_OK)
    //     ;
    // else {
    //     err_s = esp_err_to_name(err);
    //     CHC_PL_E("%s", err_s);
    // }
}

// 傳送偵測物體ID、距離、速度、角度
bool chc_PROTOCOL::RRU_E(uint8_t id, uint16_t distance, uint16_t speed, uint8_t degree)
{
    message.identifier = RRU_ID1;
    message.extd = 0;
    message.rtr = 0;
    message.data_length_code = 6;
    message.data[0] = id;
    message.data[1] = distance;
    message.data[2] = distance >> 8;
    message.data[3] = speed;
    message.data[4] = speed >> 8;
    message.data[5] = degree;
    return CAN_transmit(message);
    // err = twai_transmit(&message, 0);
    // if (err == ESP_OK)
    //     ;
    // else {
    //     err_s = esp_err_to_name(err);
    //     CHC_PL_E("%s", err_s);
    // }
}

/* 傳送警戒狀態、燈狀態
   警戒狀態: 0:無物體在警戒範圍內
            1:有物體在警戒範圍內
   燈狀態  0:關閉
           1:恆亮
           2:閃爍
*/
bool chc_PROTOCOL::RRU_period(uint8_t alarm_status, uint8_t light_status)
{
    message.identifier = RRU_ID2;
    message.extd = 0;
    message.rtr = 0;
    message.data_length_code = 2;
    message.data[0] = alarm_status;
    message.data[1] = light_status;
    return CAN_transmit(message);

    // err = twai_transmit(&message, 0);
    // if (err == ESP_OK)
    //     ;
    // else {
    //     err_s = esp_err_to_name(err);
    //     CHC_PL_E("%s", err_s);
    // }
}

// 傳送版本資訊
bool chc_PROTOCOL::RRU_version(uint8_t protocol_major, uint8_t protocol_minor, uint8_t sw_major, uint8_t sw_minor, uint8_t hw_major, uint8_t hw_minor)
{
    message.identifier = RRU_V;
    message.extd = 0;
    message.rtr = 0;
    message.data_length_code = 6;
    message.data[0] = protocol_major;
    message.data[1] = protocol_minor;
    message.data[2] = sw_major;
    message.data[3] = sw_minor;
    message.data[4] = hw_major;
    message.data[5] = hw_minor;
    return CAN_transmit(message);
    // err = twai_transmit(&message, 0);
    // if (err == ESP_OK)
    //     ;
    // else {
    //     err_s = esp_err_to_name(err);
    //     CHC_PL_E("%s", err_s);
    // }
}
#elif node_CWS == 1
REQ_type chc_PROTOCOL::rx_CWS()
{
    // err = twai_receive(&rx_msg, 1000);
    // if (err == ESP_OK)
    //     ;
    // else {
    //     return err;
    // }
    if (CAN_receive(rx_msg, 1000) == false)
        return NONE;

    CHC_PL_LOG("Received:");
    for (uint8_t i = 0; i < rx_msg.data_length_code; i++) {
        CHC_PL_S("%02X ", rx_msg.data[i]);
    }
    CHC_PL_S("\n");

    switch (rx_msg.identifier) {
    case NM_get_info:
        rx_msg.data[0] = rx_msg.data[0] & 0b00110000;
        if (rx_msg.data[0] == 0b00010000) {
            CWS_version(protocol_Major, protocol_Minor, sw_Major, sw_Minor, hw_Major, hw_Minor);
            return REQ_CWS_INFO;
        } else {
            return NONE;
        }
    case NM_set_CMD:
        rx_msg.data[0] = rx_msg.data[0] & 0b00110000;
        if (rx_msg.data[0] == 0b00010000) {
            return SET_CWS_AWAKE;
        } else {
            return SET_CWS_SLEEP;
        }
    case DIAGtoCWS:
        return REQ_ERC;
    case HMItoCWS:
        detect_range = rx_msg.data[0] | (rx_msg.data[1] << 8);
        return PROCESS_DONE;
    case HMI_V:
        for (uint8_t i = 0; i < 6; i++) {
            Rx.HMI_V_data[i] = rx_msg.data[i];
        }
        return PROCESS_DONE;
    case MCU_V:
        for (uint8_t i = 0; i < 6; i++) {
            Rx.MCU_V_data[i] = rx_msg.data[i];
        }
        return PROCESS_DONE;
    case RRU_V:
        for (uint8_t i = 0; i < 6; i++) {
            Rx.RRU_V_data[i] = rx_msg.data[i];
        }
        return PROCESS_DONE;
    case CWS_V:
        for (uint8_t i = 0; i < 6; i++) {
            Rx.CWS_V_data[i] = rx_msg.data[i];
        }
        return PROCESS_DONE;
    case NU_V:
        for (uint8_t i = 0; i < 6; i++) {
            Rx.NU_V_data[i] = rx_msg.data[i];
        }
        return PROCESS_DONE;
    default:
        return NONE;
    }
}
// 診斷碼
bool chc_PROTOCOL::CWStoDIAG(uint8_t error)
{
    message.identifier = CWS_DIAG;
    message.extd = 0;
    message.rtr = 0;
    message.data_length_code = 1;
    message.data[0] = error;
    return CAN_transmit(&message);
    // err = twai_transmit(&message, 0);
    // if (err == ESP_OK)
    //     ;
    // else {
    //     err_s = esp_err_to_name(err);
    //     CHC_PL_E("%s", err_s);
    // }
}

// 坑洞資訊
bool chc_PROTOCOL::CWS_period(uint16_t distance, uint8_t degree)
{
    message.identifier = CWS_ID1;
    message.extd = 0;
    message.rtr = 0;
    message.data_length_code = 3;
    message.data[0] = distance;
    message.data[1] = distance >> 8;
    message.data[2] = degree;
    return CAN_transmit(&message);
    // err = twai_transmit(&message, 0);
    // if (err == ESP_OK)
    //     ;
    // else {
    //     err_s = esp_err_to_name(err);
    //     CHC_PL_E("%s", err_s);
    // }
}

// 傳送版本資訊
bool chc_PROTOCOL::CWS_version(uint8_t protocol_major, uint8_t protocol_minor, uint8_t sw_major, uint8_t sw_minor, uint8_t hw_major, uint8_t hw_minor)
{
    message.identifier = CWS_V;
    message.extd = 0;
    message.rtr = 0;
    message.data_length_code = 6;
    message.data[0] = protocol_major;
    message.data[1] = protocol_minor;
    message.data[2] = sw_major;
    message.data[3] = sw_minor;
    message.data[4] = hw_major;
    message.data[5] = hw_minor;
    return CAN_transmit(&message);
    // err = twai_transmit(&message, 0);
    // if (err == ESP_OK)
    //     ;
    // else {
    //     err_s = esp_err_to_name(err);
    //     CHC_PL_E("%s", err_s);
    // }
}
#elif node_NU == 1
// 接收段
REQ_type chc_PROTOCOL::rx_NU()
{
    // err = twai_receive(&rx_msg, 1000);
    // if (err == ESP_OK)
    //     ;
    // else {
    //     return err;
    // }
    if (CAN_receive(&rx_msg, 1000) == false)
        return NONE;

    CHC_PL_LOG("Received:");
    for (uint8_t i = 0; i < rx_msg.data_length_code; i++) {
        CHC_PL_S("%02X ", rx_msg.data[i]);
    }
    CHC_PL_S("\n");

    switch (rx_msg.identifier) {
    case NM_get_info:
        rx_msg.data[0] = rx_msg.data[0] & 0b11000000;
        if (rx_msg.data[0] == 0b01000000) {
            NU_version(protocol_Major, protocol_Minor, sw_Major, sw_Minor, hw_Major, hw_Minor);
            return REQ_NU_INFO;
        } else {
            return NONE;
        }
    case NM_set_CMD:
        rx_msg.data[0] = rx_msg.data[0] & 0b11000000;
        if (rx_msg.data[0] == 0b01000000) {
            return SET_NU_AWAKE;
        } else {
            return SET_NU_SLEEP;
        }
    case DIAGtoNU:
        return REQ_ERC;
    case HMI_ID1:
        hmi_data.device_status = rx_msg.data[0];
        hmi_data.heart_rate = rx_msg.data[1];
        hmi_data.sport_mode = rx_msg.data[2];
        return PROCESS_DONE;
    case HMI_DIAG:
        all_dtc.HMI_DTC = rx_msg.data[0];
        return PROCESS_DONE;
    case MCU_DIAG:
        all_dtc.MCU_DTC = rx_msg.data[0];
        return PROCESS_DONE;
    case RRU_DIAG:
        all_dtc.RRU_DTC = rx_msg.data[0];
        return PROCESS_DONE;
    case CWS_DIAG:
        all_dtc.CWS_DTC = rx_msg.data[0];
        return PROCESS_DONE;
    case NU_DIAG:
        all_dtc.NU_DTC = rx_msg.data[0];
        return PROCESS_DONE;
    case HMItoCWS:
        detect_range = rx_msg.data[0] | (rx_msg.data[1] << 8);
        return PROCESS_DONE;
    case HMI_V:
        for (uint8_t i = 0; i < 6; i++) {
            Rx.HMI_V_data[i] = rx_msg.data[i];
        }
        return PROCESS_DONE;
    case MCU_V:
        for (uint8_t i = 0; i < 6; i++) {
            Rx.MCU_V_data[i] = rx_msg.data[i];
        }
        return PROCESS_DONE;
    case RRU_V:
        for (uint8_t i = 0; i < 6; i++) {
            Rx.RRU_V_data[i] = rx_msg.data[i];
        }
        return PROCESS_DONE;
    case CWS_V:
        for (uint8_t i = 0; i < 6; i++) {
            Rx.CWS_V_data[i] = rx_msg.data[i];
        }
        return PROCESS_DONE;
    case NU_V:
        for (uint8_t i = 0; i < 6; i++) {
            Rx.NU_V_data[i] = rx_msg.data[i];
        }
        return PROCESS_DONE;
    default:
        return PROCESS_DONE;
    }
}

// 診斷碼
bool chc_PROTOCOL::NUtoDIAG(uint8_t error)
{
    message.identifier = HMI_DIAG;
    message.extd = 0;
    message.rtr = 0;
    message.data_length_code = 1;
    message.data[0] = error;
    return CAN_transmit(&message);

    // err = twai_transmit(&message, 0);
    // if (err == ESP_OK)
    //     ;
    // else {
    //     err_s = esp_err_to_name(err);
    //     CHC_PL_E("%s", err_s);
    // }
}

// 經度、緯度
bool chc_PROTOCOL::NU_period1(float longitude, float latitude)
{
    U_float2bytes trans;
    uint8_t i;
    message.identifier = NU_ID1;
    message.extd = 0;
    message.rtr = 0;
    message.data_length_code = 8;
    trans.var = longitude;
    for (i = 0; i < 4; i++) {
        message.data[i] = trans.array[3 - i];
    }
    trans.var = latitude;
    for (i = 0; i < 4; i++) {
        message.data[i + 4] = trans.array[3 - i];
    }
    return CAN_transmit(&message);
    // err = twai_transmit(&message, 0);
    // if (err == ESP_OK)
    //     ;
    // else {
    //     err_s = esp_err_to_name(err);
    //     CHC_PL_E("%s", err_s);
    // }
}

// 海拔、速度、模組連網狀態
bool chc_PROTOCOL::NU_period2(uint16_t altitude, uint16_t velocity, uint8_t status)
{
    message.identifier = NU_ID2;
    message.extd = 0;
    message.rtr = 0;
    message.data_length_code = 5;
    message.data[0] = altitude;
    message.data[1] = altitude >> 8;
    message.data[2] = velocity;
    message.data[3] = velocity >> 8;
    message.data[4] = status;
    return CAN_transmit(&message);
    // err = twai_transmit(&message, 0);
    // if (err == ESP_OK)
    //     ;
    // else {
    //     err_s = esp_err_to_name(err);
    //     CHC_PL_E("%s", err_s);
    // }
}

bool chc_PROTOCOL::NU_version(uint8_t protocol_major, uint8_t protocol_minor, uint8_t sw_major, uint8_t sw_minor, uint8_t hw_major, uint8_t hw_minor)
{
    message.identifier = NU_V;
    message.extd = 0;
    message.rtr = 0;
    message.data_length_code = 6;
    message.data[0] = protocol_major;
    message.data[1] = protocol_minor;
    message.data[2] = sw_major;
    message.data[3] = sw_minor;
    message.data[4] = hw_major;
    message.data[5] = hw_minor;
    return CAN_transmit(&message);

    // err = twai_transmit(&message, 0);
    // if (err == ESP_OK)
    //     ;
    // else {
    //     err_s = esp_err_to_name(err);
    //     CHC_PL_E("%s", err_s);
    // }
}
#endif

// esp_err_t chc_PROTOCOL::CAN_init()
// { // INPUT (high impedance) = slope control mode, OUTPUT = see next line
//     twai_general_config_t g_config = TWAI_GENERAL_CONFIG_DEFAULT(GPIO_NUM_48, GPIO_NUM_45, TWAI_MODE_NORMAL); // TWAI_MODE_NORMAL, TWAI_MODE_NO_ACK or TWAI_MODE_LISTEN_ONLY
//     twai_timing_config_t t_config = TWAI_TIMING_CONFIG_500KBITS();
//     twai_filter_config_t f_config = TWAI_FILTER_CONFIG_ACCEPT_ALL();
//     err = twai_driver_install(&g_config, &t_config, &f_config);
//     if (err == ESP_OK) {
//         debug_println("Driver installed");
//     } else {
//         debug_print("Failed to install driver");
//         return err;
//     }
//     err = twai_start();
//     if (err == ESP_OK) {
//         debug_println("Driver started");
//     } else {
//         debug_println("Failed to start driver");
//         return err;
//     }
//     return err;
// }