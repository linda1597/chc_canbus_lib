
#include "CAN_base.h"

#include "chc_protocol_hyena2.h"

// esp_err_t err;
// String err_s;
// twai_message_t rx_msg;
// twai_message_t message;
CAN_frame_t rx_msg;
CAN_frame_t tx_msg;
CHC_PROTOCOL_HYENA2 chcProtocol_hyena2;

CHC_PROTOCOL_HYENA2::CHC_PROTOCOL_HYENA2()
{
    ModuleID.moduleID = 0x4D4944FFFFFFFFFF;
    // 建構子
}
CHC_PROTOCOL_HYENA2::REQ_type CHC_PROTOCOL_HYENA2::rx()
{
    if (CAN_base_receive(&rx_msg, 70) == false) {
        return NONE;
    }
    CHC_PL_LOG("Received:");
    for (uint8_t i = 0; i < rx_msg.data_length_code; i++) {
        CHC_PL_LOG_S("%02X ", rx_msg.data[i]);
    }
    CHC_PL_LOG_S("\n");
    // ----------------- 以下為接收到的資料 -----------------
    switch (rx_msg.identifier) {

        // DIAG ID --------------------------------
// ----------------------------------------------------------------
#ifdef rx_DIAGtoHMI
    case CHC_PROTOCOL_HYENA2::DIAGtoHMI: // = 0x772,
        // return PROCESS_DONE;
        return GET_DIAG;
        break;
#endif
// ----------------------------------------------------------------
#ifdef rx_DIAGtoMCU
    case CHC_PROTOCOL::DIAGtoMCU: // = 0x774,
        // return PROCESS_DONE;
        return GET_DIAG;
        break;
#endif
// ----------------------------------------------------------------
#ifdef rx_DIAGtoRRU
    case CHC_PROTOCOL_HYENA2::DIAGtoRRU: // = 0x775,
        // return PROCESS_DONE;
        return GET_DIAG;
        break;
#endif
// ----------------------------------------------------------------
#ifdef rx_DIAGtoCWS
    case CHC_PROTOCOL::DIAGtoCWS: // = 0x776,
        // return PROCESS_DONE;
        return GET_DIAG;
        break;
#endif
// ----------------------------------------------------------------
#ifdef rx_DIAGtoNU
    case CHC_PROTOCOL::DIAGtoNU: // = 0x777,
        // return PROCESS_DONE;
        return GET_DIAG;
        break;
#endif
// HMI ID
// ----------------------------------------------------------------
#ifdef rx_HMItoDIAG
    case CHC_PROTOCOL::HMI_DIAG: // = 0x130,
                                 // all_dtc.HMI_DTC = rx_msg.data[0];
        sData.dtc.HMI = rx_msg.data[0];
        // return PROCESS_DONE;
        return GET_HMI;
        break;
#endif

// ----------------------------------------------------------------
#ifdef rx_HMI_1
    case CHC_PROTOCOL::HMI_ID1: // = 0x140,
        sData.hmi.hr_status = rx_msg.data[0];
        sData.hmi.hr_value = rx_msg.data[1];
        sData.hmi.sport_mode = rx_msg.data[2];
        return PROCESS_DONE;
        break;
#endif

// ----------------------------------------------------------------
#ifdef rx_HMI_2
    case CHC_PROTOCOL::HMI_ID2: // = 0x141,

        break;
#endif

// ----------------------------------------------------------------
#ifdef rx_NM_get_INFO
    case CHC_PROTOCOL_HYENA2::NM_get_info: // = 0x14B,

        // ----------------------------------------------------------------
#ifdef node_MCU
        rx_msg.data[0] = rx_msg.data[0] & 0b00000011;
        if (rx_msg.data[0] == 0b00000001) {
            MCU_version(
                protocol_Major,
                protocol_Minor,
                sw_Major,
                sw_Minor,
                hw_Major,
                hw_Minor);
            return REQ_MCU_INFO;
        }
#endif

        // ----------------------------------------------------------------
#ifdef node_RRU
        rx_msg.data[0] = rx_msg.data[0] & 0b00001100;
        if (rx_msg.data[0] == 0b00000100) {
            RRU_version(
                protocol_Major,
                protocol_Minor,
                sw_Major,
                sw_Minor,
                hw_Major,
                hw_Minor);
            return REQ_RRU_INFO;
        }
#endif

        // ----------------------------------------------------------------
#ifdef node_CWS
        rx_msg.data[0] = rx_msg.data[0] & 0b00110000;
        if (rx_msg.data[0] == 0b00010000) {
            CWS_version(
                protocol_Major,
                protocol_Minor,
                sw_Major,
                sw_Minor,
                hw_Major,
                hw_Minor);
            return REQ_CWS_INFO;
        }
#endif

        // ----------------------------------------------------------------
#ifdef node_NU
        rx_msg.data[0] = rx_msg.data[0] & 0b11000000;
        if (rx_msg.data[0] == 0b01000000) {
            NU_version(
                protocol_Major,
                protocol_Minor,
                sw_Major,
                sw_Minor,
                hw_Major,
                hw_Minor);
            return REQ_NU_INFO;
        }
#endif
        return NONE;
        break;
#endif
// ----------------------------------------------------------------
#ifdef rx_NM_set_CMD
    case CHC_PROTOCOL_HYENA2::NM_set_CMD: // = 0x14C,

        // ----------------------------------------------------------------
#ifdef node_MCU
        rx_msg.data[0] = rx_msg.data[0] & 0b00000011;
        if (rx_msg.data[0] == 0b00000001) {
            return SET_MCU_AWAKE;
        }
        return SET_MCU_SLEEP;
#endif
        // ----------------------------------------------------------------
#ifdef node_RRU
        rx_msg.data[0] = rx_msg.data[0] & 0b00001100;
        if (rx_msg.data[0] == 0b00000100) {
            return SET_RRU_AWAKE;
        }
        return SET_RRU_SLEEP;

#endif
        // ----------------------------------------------------------------
#ifdef node_CWS
        rx_msg.data[0] = rx_msg.data[0] & 0b00110000;
        if (rx_msg.data[0] == 0b00010000) {
            return SET_CWS_AWAKE;
        }
        return SET_CWS_SLEEP;
#endif
#ifdef node_NU
        rx_msg.data[0] = rx_msg.data[0] & 0b11000000;
        if (rx_msg.data[0] == 0b01000000) {
            return SET_NU_AWAKE;
        }
        return SET_NU_SLEEP;
#endif
        break;
#endif

// ----------------------------------------------------------------
#ifdef rx_HMItoRRU
    case CHC_PROTOCOL_HYENA2::HMItoRRU: // = 0x14D,
        sData.rru.set_detect_range = rx_msg.data[0] | (rx_msg.data[1] << 8);
        sData.rru.set_bling_hz = rx_msg.data[2];
        // return PROCESS_DONE;
        return GET_HMI;
        break;
#endif
// ----------------------------------------------------------------
#ifdef rx_HMItoCWS
    case CHC_PROTOCOL::HMItoCWS: // = 0x14E,
        sData.cws.set_detect_range = rx_msg.data[0] | (rx_msg.data[1] << 8);
        // return PROCESS_DONE;
        return GET_HMI;
        break;
#endif
// ----------------------------------------------------------------
#ifdef rx_HMI_V
    case CHC_PROTOCOL_HYENA2::HMI_V: // = 0x14F,
        for (uint8_t i = 0; i < 6; i++) {
            sData.ver.HMI[i] = rx_msg.data[i];
        }
        // return PROCESS_DONE;
        return GET_HMI;
        break;
#endif
#ifdef rx_update_rru_FW
    case CHC_PROTOCOL_HYENA2::RRU_FW_UPDATE_REQ:
        return RRU_UPDATE;
        break;
#endif
#ifdef rx_read_rru_modID
    case CHC_PROTOCOL_HYENA2::RRU_MODULE_ID_READ:
        RRU_ModIDBroadcast();
        return REQ_RRU_ID;
        break;
#endif
// MCU ID
// ----------------------------------------------------------------
#ifdef rx_MCUtoDIAG
    case CHC_PROTOCOL::MCU_DIAG: // = 0x150,
        sData.dtc.MCU = rx_msg.data[0];
        // return PROCESS_DONE;
        return GET_MCU;
        break;
#endif
// ----------------------------------------------------------------
#ifdef rx_MCU_1
    case CHC_PROTOCOL::MCU_ID1: // = 0x160,
        sData.mcu.assist = rx_msg.data[0];
        sData.mcu.torque = rx_msg.data[1] | (rx_msg.data[2] << 8);
        sData.mcu.cadence = rx_msg.data[3] | (rx_msg.data[4] << 8);
        sData.mcu.speed = rx_msg.data[5] | (rx_msg.data[6] << 8);
        sData.mcu.battery = rx_msg.data[7];

        // return PROCESS_DONE;
        return GET_MCU;
        break;
#endif
// ----------------------------------------------------------------
#ifdef rx_MCU_V
    case CHC_PROTOCOL_HYENA2::MCU_V: // = 0x16F,
        for (uint8_t i = 0; i < 6; i++) {
            sData.ver.MCU[i] = rx_msg.data[i];
        }
        // return PROCESS_DONE;
        return GET_MCU;
        break;
#endif
// RRU ID
// ----------------------------------------------------------------
#ifdef rx_RRUtoDIAG
    case CHC_PROTOCOL::RRU_DIAG: // = 0x190,
        sData.dtc.RRU = rx_msg.data[0];
        // return PROCESS_DONE;
        return GET_RRU;
        break;
#endif
// ----------------------------------------------------------------
#ifdef rx_RRU_1
    case CHC_PROTOCOL::RRU_ID1: // = 0x1A0,
        sData.rru.id = rx_msg.data[0];
        sData.rru.distance = rx_msg.data[1] | (rx_msg.data[2] << 8);
        sData.rru.speed = rx_msg.data[3] | (rx_msg.data[4] << 8);
        sData.rru.angle = rx_msg.data[5];

        // return PROCESS_DONE;
        return GET_RRU;
        break;
#endif
// ----------------------------------------------------------------
#ifdef rx_RRU_2
    case CHC_PROTOCOL::RRU_ID2: // = 0x1A1,
        sData.rru.status_alarm_L = rx_msg.data[0];
        sData.rru.status_alarm_R = rx_msg.data[1];
        sData.rru.status_light = rx_msg.data[2];

        // return PROCESS_DONE;
        return GET_RRU;
        break;
#endif
// ----------------------------------------------------------------
#ifdef rx_RRU_V
    case CHC_PROTOCOL_HYENA2::RRU_V: // = 0x1AF,
        for (uint8_t i = 0; i < 6; i++) {
            sData.ver.RRU[i] = rx_msg.data[i];
        }
        // return PROCESS_DONE;
        return GET_RRU;
        break;
#endif
// CWS ID
// ----------------------------------------------------------------
#ifdef rx_CWStoDIAG
    case CHC_PROTOCOL::CWS_DIAG: // = 0x1B0,
        sData.dtc.CWS = rx_msg.data[0];
        // return PROCESS_DONE;
        return GET_CWS;
        break;
#endif
// ----------------------------------------------------------------
#ifdef rx_CWS_1
    case CHC_PROTOCOL::CWS_ID1: // = 0x1C0,
        sData.cws.distance = rx_msg.data[0] | (rx_msg.data[1] << 8);
        sData.cws.angle = rx_msg.data[2];

        // return PROCESS_DONE;
        return GET_CWS;
        break;
#endif
// ----------------------------------------------------------------
#ifdef rx_CWS_V
    case CHC_PROTOCOL_HYENA2::CWS_V: // = 0x1CF,
        for (uint8_t i = 0; i < 6; i++) {
            sData.ver.CWS[i] = rx_msg.data[i];
        }
        // return PROCESS_DONE;
        return GET_CWS;
        break;
#endif
// NU ID
// ----------------------------------------------------------------
#ifdef rx_NUtoDIAG
    case CHC_PROTOCOL::NU_DIAG: // = 0x1D0,
        sData.dtc.NU = rx_msg.data[0];
        return GET_NU;
        // return PROCESS_DONE;
        break;
#endif
// ----------------------------------------------------------------
#ifdef rx_NU_1
    case CHC_PROTOCOL::NU_ID1: // = 0x1E0,
        sData.nu.longitude = rx_msg.data[0] | (rx_msg.data[1] << 8) | (rx_msg.data[2] << 16) | (rx_msg.data[3] << 24);
        sData.nu.latitude = rx_msg.data[4] | (rx_msg.data[5] << 8) | (rx_msg.data[6] << 16) | (rx_msg.data[7] << 24);

        return GET_NU;
        // return PROCESS_DONE;
        break;
#endif
// ----------------------------------------------------------------
#ifdef rx_NU_2
    case CHC_PROTOCOL::NU_ID2: // = 0x1E1,
        sData.nu.altitude = rx_msg.data[0] | (rx_msg.data[1] << 8);
        sData.nu.speed = rx_msg.data[2] | (rx_msg.data[3] << 8);

        return GET_NU;
        // return PROCESS_DONE;
        break;
#endif
// ----------------------------------------------------------------
#ifdef rx_NU_V
    case CHC_PROTOCOL_HYENA2::NU_V: // = 0x1EF
        for (uint8_t i = 0; i < 6; i++) {
            sData.ver.NU[i] = rx_msg.data[i];
        }
        return GET_NU;
        // return PROCESS_DONE;
        break;
#endif
        // ----------------------------------------------------------------
    default:
        return PROCESS_DONE;
        break;
    }
    return PROCESS_DONE;
}
#ifdef node_HMI
// 診斷碼
bool CHC_PROTOCOL::HMItoDIAG(uint8_t error)
{
    tx_msg.identifier = HMI_DIAG;
    tx_msg.extd = 0;
    tx_msg.rtr = 0;
    tx_msg.data_length_code = 1;
    tx_msg.data[0] = error;
    return CAN_base_transmit(&tx_msg);
}
/**
 * @brief 傳送心跳裝置連線狀態、心跳、運動模式
 * @param hr_status 心跳裝置連線狀態
 * @param hr_value 心跳
 * @param sport_mode 運動模式，0：休閒，1：運動，2：訓練
*/
bool CHC_PROTOCOL::HMI_period(
    uint8_t hr_status,
    uint8_t hr_value,
    uint8_t sport_mode)
{
    tx_msg.identifier = HMI_ID1;
    tx_msg.extd = 0;
    tx_msg.rtr = 0;
    tx_msg.data_length_code = 3;
    tx_msg.data[0] = hr_status;
    tx_msg.data[1] = hr_value;
    tx_msg.data[2] = sport_mode;
    return CAN_base_transmit(&tx_msg);
}

// 設定輔助力
bool CHC_PROTOCOL::MCU_setAssist(uint8_t u8Assist)
{
    tx_msg.identifier = HMI_ID2;
    tx_msg.extd = 0;
    tx_msg.rtr = 0;
    tx_msg.data_length_code = 1;
    tx_msg.data[0] = u8Assist;
    return CAN_base_transmit(&tx_msg);
}

// NM獲取其他部件資訊
bool CHC_PROTOCOL::NM_getInfo(
    bool getMCUInfo,
    bool getRRUInfo,
    bool getCWSInfo,
    bool getNUInfo)
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

    tx_msg.identifier = NM_get_info;
    tx_msg.extd = 0;
    tx_msg.rtr = 0;
    tx_msg.data_length_code = 1;
    tx_msg.data[0] = flag;
    return CAN_base_transmit(&tx_msg);
}

/* NM指令
   0:休眠
   1:開機 */
bool CHC_PROTOCOL::NM_CMD(
    bool setMCU,
    bool setRRU,
    bool setCWS,
    bool setNU)
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

    tx_msg.identifier = NM_set_CMD;
    tx_msg.extd = 0;
    tx_msg.rtr = 0;
    tx_msg.data_length_code = 1;
    tx_msg.data[0] = flag;
    return CAN_base_transmit(&tx_msg);
}

// 設定RRU警示距離、警示LED閃爍頻率
bool CHC_PROTOCOL::RRU_setParam(
    uint16_t distance,
    uint8_t Hz)
{
    tx_msg.identifier = HMItoRRU;
    tx_msg.extd = 0;
    tx_msg.rtr = 0;
    tx_msg.data_length_code = 3;
    tx_msg.data[0] = distance;
    tx_msg.data[1] = distance >> 8;
    tx_msg.data[2] = Hz;
    return CAN_base_transmit(&tx_msg);
}

// 設定CWS警示距離
bool CHC_PROTOCOL::CWS_setParam(
    uint16_t u16Distance,
    uint8_t u8Range)
{
    tx_msg.identifier = HMItoCWS;
    tx_msg.extd = 0;
    tx_msg.rtr = 0;
    tx_msg.data_length_code = 3;
    tx_msg.data[0] = u16Distance;
    tx_msg.data[1] = u16Distance >> 8;
    tx_msg.data[2] = u8Range;
    return CAN_base_transmit(&tx_msg);
}

// 傳送版本資訊
bool CHC_PROTOCOL::HMI_version(
    uint8_t protocol_major,
    uint8_t protocol_minor,
    uint8_t sw_major,
    uint8_t sw_minor,
    uint8_t hw_major,
    uint8_t hw_minor)
{
    tx_msg.identifier = HMI_V;
    tx_msg.extd = 0;
    tx_msg.rtr = 0;
    tx_msg.data_length_code = 6;
    tx_msg.data[0] = protocol_major;
    tx_msg.data[1] = protocol_minor;
    tx_msg.data[2] = sw_major;
    tx_msg.data[3] = sw_minor;
    tx_msg.data[4] = hw_major;
    tx_msg.data[5] = hw_minor;
    return CAN_base_transmit(&tx_msg);
}
#endif
#ifdef node_MCU
// 診斷碼
bool CHC_PROTOCOL::MCUtoDIAG(uint8_t error)
{
    tx_msg.identifier = MCU_DIAG;
    tx_msg.extd = 0;
    tx_msg.rtr = 0;
    tx_msg.data_length_code = 1;
    tx_msg.data[0] = error;
    return CAN_base_transmit(&tx_msg);
}

// 傳送扭力、踏頻、速度、電量
bool CHC_PROTOCOL::MCU_period(
    uint8_t support,
    uint16_t torque,
    uint16_t cadence,
    uint16_t speed,
    uint8_t battery)
{

    tx_msg.identifier = MCU_ID1;
    tx_msg.extd = 0;
    tx_msg.rtr = 0;
    tx_msg.data_length_code = 8;
    tx_msg.data[0] = support; // 輔助力段位
    tx_msg.data[1] = torque; // 扭力_L
    tx_msg.data[2] = torque >> 8; // 扭力_H
    tx_msg.data[3] = cadence; // 踏頻_L
    tx_msg.data[4] = cadence >> 8; // 踏頻_H
    tx_msg.data[5] = speed; // 速度_L
    tx_msg.data[6] = speed >> 8; // 速度_H
    tx_msg.data[7] = battery; // 電量
    return CAN_base_transmit(&tx_msg);
}

// 傳送版本資訊
bool CHC_PROTOCOL::MCU_version(
    uint8_t protocol_major,
    uint8_t protocol_minor,
    uint8_t sw_major,
    uint8_t sw_minor,
    uint8_t hw_major,
    uint8_t hw_minor)
{
    tx_msg.identifier = MCU_V;
    tx_msg.extd = 0;
    tx_msg.rtr = 0;
    tx_msg.data_length_code = 6;
    tx_msg.data[0] = protocol_major;
    tx_msg.data[1] = protocol_minor;
    tx_msg.data[2] = sw_major;
    tx_msg.data[3] = sw_minor;
    tx_msg.data[4] = hw_major;
    tx_msg.data[5] = hw_minor;
    return CAN_base_transmit(&tx_msg);
}
#endif
#ifdef node_RRU
// 診斷碼
bool CHC_PROTOCOL_HYENA2::RRUError(uint8_t ErrPage, uint8_t ErrCode)
{
    tx_msg.identifier = RRU_ERR;
    tx_msg.extd = 0;
    tx_msg.rtr = 0;
    tx_msg.data_length_code = 2;
    tx_msg.data[0] = ErrPage;
    tx_msg.data[1] = ErrCode;
    return CAN_base_transmit(&tx_msg);
}

// 傳送偵測物體ID、距離、速度、角度、警戒狀態
bool CHC_PROTOCOL_HYENA2::RRU_Info(
    uint8_t id,
    uint16_t distance,
    uint16_t speed,
    uint8_t degree,
    uint8_t status)
{
    tx_msg.identifier = RRU_INFO;
    tx_msg.extd = 0;
    tx_msg.rtr = 0;
    tx_msg.data_length_code = 6;
    tx_msg.data[0] = id;
    tx_msg.data[1] = speed;
    tx_msg.data[2] = speed >> 8;
    tx_msg.data[3] = distance;
    tx_msg.data[4] = distance >> 8;    
    tx_msg.data[5] = degree;
    tx_msg.data[6] = status;
    return CAN_base_transmit(&tx_msg);
}

// 傳送版本資訊
bool CHC_PROTOCOL_HYENA2::RRU_version(
    uint8_t protocol_major,
    uint8_t protocol_minor,
    uint8_t sw_major,
    uint8_t sw_minor,
    uint8_t hw_major,
    uint8_t hw_minor)
{
    tx_msg.identifier = RRU_V;
    tx_msg.extd = 0;
    tx_msg.rtr = 0;
    tx_msg.data_length_code = 6;
    tx_msg.data[0] = protocol_major;
    tx_msg.data[1] = protocol_minor;
    tx_msg.data[2] = sw_major;
    tx_msg.data[3] = sw_minor;
    tx_msg.data[4] = hw_major;
    tx_msg.data[5] = hw_minor;
    return CAN_base_transmit(&tx_msg);
}

bool CHC_PROTOCOL_HYENA2::RRU_FWupdateRP(
    uint8_t dlc,
    uint8_t *payload)
{
    tx_msg.identifier = RRU_FW_UPDATE_RP;
    tx_msg.extd = 0;
    tx_msg.rtr = 0;
    tx_msg.data_length_code = dlc;
    for(uint8_t i=0; i<dlc; i++){
        tx_msg.data[i] = *(payload+i);
    }
    return CAN_base_transmit(&tx_msg);    
}

bool CHC_PROTOCOL_HYENA2::RRU_ModIDBroadcast()
{
    tx_msg.identifier = RRU_MODULE_ID_BROADCAST;
    tx_msg.extd = 1;
    tx_msg.rtr = 0;
    tx_msg.data_length_code = 8;
    for(uint8_t i=0; i<8; i++){
        tx_msg.data[i] = ModuleID.bytes[i];
    }
    return CAN_base_transmit(&tx_msg);
}
#endif
#ifdef node_CWS
c
    // 診斷碼
    bool
    CHC_PROTOCOL::CWStoDIAG(uint8_t error)
{
    tx_msg.identifier = CWS_DIAG;
    tx_msg.extd = 0;
    tx_msg.rtr = 0;
    tx_msg.data_length_code = 1;
    tx_msg.data[0] = error;
    return CAN_base_transmit(&tx_msg);
}

// 坑洞資訊
bool CHC_PROTOCOL::CWS_period(
    uint16_t distance,
    uint8_t degree)
{
    tx_msg.identifier = CWS_ID1;
    tx_msg.extd = 0;
    tx_msg.rtr = 0;
    tx_msg.data_length_code = 3;
    tx_msg.data[0] = distance;
    tx_msg.data[1] = distance >> 8;
    tx_msg.data[2] = degree;
    return CAN_base_transmit(&tx_msg);
}

// 傳送版本資訊
bool CHC_PROTOCOL::CWS_version(
    uint8_t protocol_major,
    uint8_t protocol_minor,
    uint8_t sw_major,
    uint8_t sw_minor,
    uint8_t hw_major,
    uint8_t hw_minor)
{
    tx_msg.identifier = CWS_V;
    tx_msg.extd = 0;
    tx_msg.rtr = 0;
    tx_msg.data_length_code = 6;
    tx_msg.data[0] = protocol_major;
    tx_msg.data[1] = protocol_minor;
    tx_msg.data[2] = sw_major;
    tx_msg.data[3] = sw_minor;
    tx_msg.data[4] = hw_major;
    tx_msg.data[5] = hw_minor;
    return CAN_base_transmit(&tx_msg);
}
#endif
#ifdef node_NU

// 診斷碼
bool CHC_PROTOCOL::NUtoDIAG(uint8_t error)
{
    tx_msg.identifier = NU_DIAG;
    tx_msg.extd = 0;
    tx_msg.rtr = 0;
    tx_msg.data_length_code = 1;
    tx_msg.data[0] = error;
    return CAN_base_transmit(&tx_msg);
}

// 經度、緯度
bool CHC_PROTOCOL::NU_period1(
    float longitude,
    float latitude)
{
    U_float2bytes trans;
    tx_msg.identifier = NU_ID1;
    tx_msg.extd = 0;
    tx_msg.rtr = 0;
    tx_msg.data_length_code = 8;
    trans.var = latitude;

    for (int i = 0; i < 4; i++) {
        tx_msg.data[i] = trans.array[3 - i];
    }
    trans.var = latitude;

    for (int i = 0; i < 4; i++) {
        tx_msg.data[i + 4] = trans.array[3 - i];
    }

    return CAN_base_transmit(&tx_msg);
}

// 海拔、速度、模組連網狀態
bool CHC_PROTOCOL::NU_period2(
    uint16_t altitude,
    uint16_t speed,
    uint8_t status)
{
    tx_msg.identifier = NU_ID2;
    tx_msg.extd = 0;
    tx_msg.rtr = 0;
    tx_msg.data_length_code = 5;
    tx_msg.data[0] = (uint8_t)altitude;
    tx_msg.data[1] = (uint8_t)altitude >> 8;
    tx_msg.data[2] = (uint8_t)speed;
    tx_msg.data[3] = (uint8_t)speed >> 8;
    tx_msg.data[4] = status;
    return CAN_base_transmit(&tx_msg);
}

bool CHC_PROTOCOL::NU_version(
    uint8_t protocol_major,
    uint8_t protocol_minor,
    uint8_t sw_major,
    uint8_t sw_minor,
    uint8_t hw_major,
    uint8_t hw_minor)
{
    tx_msg.identifier = NU_V;
    tx_msg.extd = 0;
    tx_msg.rtr = 0;
    tx_msg.data_length_code = 6;
    tx_msg.data[0] = protocol_major;
    tx_msg.data[1] = protocol_minor;
    tx_msg.data[2] = sw_major;
    tx_msg.data[3] = sw_minor;
    tx_msg.data[4] = hw_major;
    tx_msg.data[5] = hw_minor;
    return CAN_base_transmit(&tx_msg);
}
#endif

static const unsigned int crc32_table[] =
    {
        0x00000000, 0x04c11db7, 0x09823b6e, 0x0d4326d9,
        0x130476dc, 0x17c56b6b, 0x1a864db2, 0x1e475005,
        0x2608edb8, 0x22c9f00f, 0x2f8ad6d6, 0x2b4bcb61,
        0x350c9b64, 0x31cd86d3, 0x3c8ea00a, 0x384fbdbd,
        0x4c11db70, 0x48d0c6c7, 0x4593e01e, 0x4152fda9,
        0x5f15adac, 0x5bd4b01b, 0x569796c2, 0x52568b75,
        0x6a1936c8, 0x6ed82b7f, 0x639b0da6, 0x675a1011,
        0x791d4014, 0x7ddc5da3, 0x709f7b7a, 0x745e66cd,
        0x9823b6e0, 0x9ce2ab57, 0x91a18d8e, 0x95609039,
        0x8b27c03c, 0x8fe6dd8b, 0x82a5fb52, 0x8664e6e5,
        0xbe2b5b58, 0xbaea46ef, 0xb7a96036, 0xb3687d81,
        0xad2f2d84, 0xa9ee3033, 0xa4ad16ea, 0xa06c0b5d,
        0xd4326d90, 0xd0f37027, 0xddb056fe, 0xd9714b49,
        0xc7361b4c, 0xc3f706fb, 0xceb42022, 0xca753d95,
        0xf23a8028, 0xf6fb9d9f, 0xfbb8bb46, 0xff79a6f1,
        0xe13ef6f4, 0xe5ffeb43, 0xe8bccd9a, 0xec7dd02d,
        0x34867077, 0x30476dc0, 0x3d044b19, 0x39c556ae,
        0x278206ab, 0x23431b1c, 0x2e003dc5, 0x2ac12072,
        0x128e9dcf, 0x164f8078, 0x1b0ca6a1, 0x1fcdbb16,
        0x018aeb13, 0x054bf6a4, 0x0808d07d, 0x0cc9cdca,
        0x7897ab07, 0x7c56b6b0, 0x71159069, 0x75d48dde,
        0x6b93dddb, 0x6f52c06c, 0x6211e6b5, 0x66d0fb02,
        0x5e9f46bf, 0x5a5e5b08, 0x571d7dd1, 0x53dc6066,
        0x4d9b3063, 0x495a2dd4, 0x44190b0d, 0x40d816ba,
        0xaca5c697, 0xa864db20, 0xa527fdf9, 0xa1e6e04e,
        0xbfa1b04b, 0xbb60adfc, 0xb6238b25, 0xb2e29692,
        0x8aad2b2f, 0x8e6c3698, 0x832f1041, 0x87ee0df6,
        0x99a95df3, 0x9d684044, 0x902b669d, 0x94ea7b2a,
        0xe0b41de7, 0xe4750050, 0xe9362689, 0xedf73b3e,
        0xf3b06b3b, 0xf771768c, 0xfa325055, 0xfef34de2,
        0xc6bcf05f, 0xc27dede8, 0xcf3ecb31, 0xcbffd686,
        0xd5b88683, 0xd1799b34, 0xdc3abded, 0xd8fba05a,
        0x690ce0ee, 0x6dcdfd59, 0x608edb80, 0x644fc637,
        0x7a089632, 0x7ec98b85, 0x738aad5c, 0x774bb0eb,
        0x4f040d56, 0x4bc510e1, 0x46863638, 0x42472b8f,
        0x5c007b8a, 0x58c1663d, 0x558240e4, 0x51435d53,
        0x251d3b9e, 0x21dc2629, 0x2c9f00f0, 0x285e1d47,
        0x36194d42, 0x32d850f5, 0x3f9b762c, 0x3b5a6b9b,
        0x0315d626, 0x07d4cb91, 0x0a97ed48, 0x0e56f0ff,
        0x1011a0fa, 0x14d0bd4d, 0x19939b94, 0x1d528623,
        0xf12f560e, 0xf5ee4bb9, 0xf8ad6d60, 0xfc6c70d7,
        0xe22b20d2, 0xe6ea3d65, 0xeba91bbc, 0xef68060b,
        0xd727bbb6, 0xd3e6a601, 0xdea580d8, 0xda649d6f,
        0xc423cd6a, 0xc0e2d0dd, 0xcda1f604, 0xc960ebb3,
        0xbd3e8d7e, 0xb9ff90c9, 0xb4bcb610, 0xb07daba7,
        0xae3afba2, 0xaafbe615, 0xa7b8c0cc, 0xa379dd7b,
        0x9b3660c6, 0x9ff77d71, 0x92b45ba8, 0x9675461f,
        0x8832161a, 0x8cf30bad, 0x81b02d74, 0x857130c3,
        0x5d8a9099, 0x594b8d2e, 0x5408abf7, 0x50c9b640,
        0x4e8ee645, 0x4a4ffbf2, 0x470cdd2b, 0x43cdc09c,
        0x7b827d21, 0x7f436096, 0x7200464f, 0x76c15bf8,
        0x68860bfd, 0x6c47164a, 0x61043093, 0x65c52d24,
        0x119b4be9, 0x155a565e, 0x18197087, 0x1cd86d30,
        0x029f3d35, 0x065e2082, 0x0b1d065b, 0x0fdc1bec,
        0x3793a651, 0x3352bbe6, 0x3e119d3f, 0x3ad08088,
        0x2497d08d, 0x2056cd3a, 0x2d15ebe3, 0x29d4f654,
        0xc5a92679, 0xc1683bce, 0xcc2b1d17, 0xc8ea00a0,
        0xd6ad50a5, 0xd26c4d12, 0xdf2f6bcb, 0xdbee767c,
        0xe3a1cbc1, 0xe760d676, 0xea23f0af, 0xeee2ed18,
        0xf0a5bd1d, 0xf464a0aa, 0xf9278673, 0xfde69bc4,
        0x89b8fd09, 0x8d79e0be, 0x803ac667, 0x84fbdbd0,
        0x9abc8bd5, 0x9e7d9662, 0x933eb0bb, 0x97ffad0c,
        0xafb010b1, 0xab710d06, 0xa6322bdf, 0xa2f33668,
        0xbcb4666d, 0xb8757bda, 0xb5365d03, 0xb1f740b4
        };

unsigned int CHC_PROTOCOL_HYENA2::cal_crc32(const unsigned char *buf, int len, unsigned int init)
{
    unsigned int crc = init;
    while (len--)
    {
        crc = (crc << 8) ^ crc32_table[((crc >> 24) ^ *buf) & 255];
        buf++;
    }
    return crc;
}