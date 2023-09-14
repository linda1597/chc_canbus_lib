
#include "CAN_base.h"

#include "chc_protocol.h"

// esp_err_t err;
// String err_s;
// twai_message_t rx_frame;
// twai_message_t message;
CAN_frame_t rx_frame;
CAN_frame_t tx_frame;
CHC_PROTOCOL chcProtocol;

CHC_PROTOCOL::CHC_PROTOCOL()
{
    // 建構子
}
bool CHC_PROTOCOL::init(int rx_pin, int tx_pin, long baudrate)
{
    // 初始化
    if (!CAN_base_init(rx_pin, tx_pin, baudrate)) {
        CHC_PL_LOG_E("CANbus init fail");
        return false;
    }
    CHC_PL_LOG_I("CANbus init");
    return true;
}
CHC_PROTOCOL::REQ_type CHC_PROTOCOL::rx()
{
    if (CAN_base_receive(&rx_frame, 500) == false) {
        return NONE;
    }
    // CHC_PL_LOG("Received:");
    // for (uint8_t i = 0; i < rx_frame.data_length_code; i++) {
    //     CHC_PL_LOG_S("%02X ", rx_frame.data[i]);
    // }
    // CHC_PL_LOG_S("\n");
    // ----------------- 以下為接收到的資料 -----------------
    switch (rx_frame.identifier) {
        // DIAG ID --------------------------------
// ----------------------------------------------------------------
#ifdef rx_DIAGtoHMI
    case CHC_PROTOCOL::DIAGtoHMI: // = 0x772,
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
    case CHC_PROTOCOL::DIAGtoRRU: // = 0x775,
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
                                 // all_dtc.HMI_DTC = rx_frame.data[0];
        sData.dtc.HMI = rx_frame.data[0];
        // return PROCESS_DONE;
        return GET_HMI;
        break;
#endif

// ----------------------------------------------------------------
#ifdef rx_HMI_1
    case CHC_PROTOCOL::HMI_ID1: // = 0x140,
        sData.hmi.hr_status = rx_frame.data[0];
        sData.hmi.hr_value = rx_frame.data[1];
        sData.hmi.sport_mode = rx_frame.data[2];
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
    case CHC_PROTOCOL::NM_get_info: // = 0x14B,

        // ----------------------------------------------------------------
#ifdef node_MCU
        rx_frame.data[0] = rx_frame.data[0] & 0b00000011;
        if (rx_frame.data[0] == 0b00000001) {
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
        rx_frame.data[0] = rx_frame.data[0] & 0b00001100;
        if (rx_frame.data[0] == 0b00000100) {
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
        rx_frame.data[0] = rx_frame.data[0] & 0b00110000;
        if (rx_frame.data[0] == 0b00010000) {
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
        rx_frame.data[0] = rx_frame.data[0] & 0b11000000;
        if (rx_frame.data[0] == 0b01000000) {
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
    case CHC_PROTOCOL::NM_set_CMD: // = 0x14C,

        // ----------------------------------------------------------------
#ifdef node_MCU
        rx_frame.data[0] = rx_frame.data[0] & 0b00000011;
        if (rx_frame.data[0] == 0b00000001) {
            return SET_MCU_AWAKE;
        }
        return SET_MCU_SLEEP;
#endif
        // ----------------------------------------------------------------
#ifdef node_RRU
        rx_frame.data[0] = rx_frame.data[0] & 0b00001100;
        if (rx_frame.data[0] == 0b00000100) {
            return SET_RRU_AWAKE;
        }
        return SET_RRU_SLEEP;

#endif
        // ----------------------------------------------------------------
#ifdef node_CWS
        rx_frame.data[0] = rx_frame.data[0] & 0b00110000;
        if (rx_frame.data[0] == 0b00010000) {
            return SET_CWS_AWAKE;
        }
        return SET_CWS_SLEEP;
#endif
#ifdef node_NU
        rx_frame.data[0] = rx_frame.data[0] & 0b11000000;
        if (rx_frame.data[0] == 0b01000000) {
            return SET_NU_AWAKE;
        }
        return SET_NU_SLEEP;
#endif
        break;
#endif

// ----------------------------------------------------------------
#ifdef rx_HMItoRRU
    case CHC_PROTOCOL::HMItoRRU: // = 0x14D,
        sData.rru.set_detect_range = rx_frame.data[0] | (rx_frame.data[1] << 8);
        sData.rru.set_bling_hz = rx_frame.data[2];
        // return PROCESS_DONE;
        return GET_HMI;
        break;
#endif
// ----------------------------------------------------------------
#ifdef rx_HMItoCWS
    case CHC_PROTOCOL::HMItoCWS: // = 0x14E,
        sData.cws.set_detect_range = rx_frame.data[0] | (rx_frame.data[1] << 8);
        // return PROCESS_DONE;
        return GET_HMI;
        break;
#endif
// ----------------------------------------------------------------
#ifdef rx_HMI_V
    case CHC_PROTOCOL::HMI_V: // = 0x14F,
        for (uint8_t i = 0; i < 6; i++) {
            sData.ver.HMI[i] = rx_frame.data[i];
        }
        // return PROCESS_DONE;
        return GET_HMI;
        break;
#endif
// MCU ID
// ----------------------------------------------------------------
#ifdef rx_MCUtoDIAG
    case CHC_PROTOCOL::MCU_DIAG: // = 0x150,
        sData.dtc.MCU = rx_frame.data[0];
        // return PROCESS_DONE;
        return GET_MCU;
        break;
#endif
// ----------------------------------------------------------------
#ifdef rx_MCU_1
    case CHC_PROTOCOL::MCU_ID1: // = 0x160,
        sData.mcu.assist = rx_frame.data[0];
        sData.mcu.torque = rx_frame.data[1] | (rx_frame.data[2] << 8);
        sData.mcu.cadence = rx_frame.data[3] | (rx_frame.data[4] << 8);
        sData.mcu.speed = rx_frame.data[5] | (rx_frame.data[6] << 8);
        sData.mcu.battery = rx_frame.data[7];

        // return PROCESS_DONE;
        return GET_MCU;
        break;
#endif
// ----------------------------------------------------------------
#ifdef rx_MCU_V
    case CHC_PROTOCOL::MCU_V: // = 0x16F,
        for (uint8_t i = 0; i < 6; i++) {
            sData.ver.MCU[i] = rx_frame.data[i];
        }
        // return PROCESS_DONE;
        return GET_MCU;
        break;
#endif
// RRU ID
// ----------------------------------------------------------------
#ifdef rx_RRUtoDIAG
    case CHC_PROTOCOL::RRU_DIAG: // = 0x190,
        sData.dtc.RRU = rx_frame.data[0];
        // return PROCESS_DONE;
        return GET_RRU;
        break;
#endif
// ----------------------------------------------------------------
#ifdef rx_RRU_1
    case CHC_PROTOCOL::RRU_ID1: // = 0x1A0,
        // rru_data.ID = rx_frame.data[0];
        // rru_data.distance = rx_frame.data[1] | (rx_frame.data[2] << 8);
        // rru_data.speed = rx_frame.data[3] | (rx_frame.data[4] << 8);
        // rru_data.angle = rx_frame.data[5];
        sData.rru.id = rx_frame.data[0];
        sData.rru.distance = rx_frame.data[1] | (rx_frame.data[2] << 8);
        sData.rru.speed = rx_frame.data[3] | (rx_frame.data[4] << 8);
        sData.rru.angle = rx_frame.data[5];

        // return PROCESS_DONE;
        return GET_RRU;
        break;
#endif
// ----------------------------------------------------------------
#ifdef rx_RRU_2
    case CHC_PROTOCOL::RRU_ID2: // = 0x1A1,
        sData.rru.status_alarm_L = rx_frame.data[0];
        sData.rru.status_alarm_R = rx_frame.data[1];
        sData.rru.status_light = rx_frame.data[2];

        // return PROCESS_DONE;
        return GET_RRU;
        break;
#endif
// ----------------------------------------------------------------
#ifdef rx_RRU_V
    case CHC_PROTOCOL::RRU_V: // = 0x1AF,
        for (uint8_t i = 0; i < 6; i++) {
            sData.ver.RRU[i] = rx_frame.data[i];
        }
        // return PROCESS_DONE;
        return GET_RRU;
        break;
#endif
// CWS ID
// ----------------------------------------------------------------
#ifdef rx_CWStoDIAG
    case CHC_PROTOCOL::CWS_DIAG: // = 0x1B0,
        sData.dtc.CWS = rx_frame.data[0];
        // return PROCESS_DONE;
        return GET_CWS;
        break;
#endif
// ----------------------------------------------------------------
#ifdef rx_CWS_1
    case CHC_PROTOCOL::CWS_ID1: // = 0x1C0,
        // sData.cws.distance = rx_frame.data[0] | (rx_frame.data[1] << 8);
        // sData.cws.angle = rx_frame.data[2];
        sData.cws.location = rx_frame.data[0] | ((uint16_t)rx_frame.data[1] << 8);
        sData.cws.size = rx_frame.data[2] | ((uint16_t)rx_frame.data[3] << 8);
        sData.cws.distance = rx_frame.data[4] | ((uint16_t)rx_frame.data[5] << 8);
        sData.cws.type = rx_frame.data[6];

        // return PROCESS_DONE;
        return GET_CWS;
        break;
#endif
// ----------------------------------------------------------------
#ifdef rx_CWS_V
    case CHC_PROTOCOL::CWS_V: // = 0x1CF,
        for (uint8_t i = 0; i < 6; i++) {
            sData.ver.CWS[i] = rx_frame.data[i];
        }
        // return PROCESS_DONE;
        return GET_CWS;
        break;
#endif
// NU ID
// ----------------------------------------------------------------
#ifdef rx_NUtoDIAG
    case CHC_PROTOCOL::NU_DIAG: // = 0x1D0,
        sData.dtc.NU = rx_frame.data[0];
        return GET_NU;
        // return PROCESS_DONE;
        break;
#endif
// ----------------------------------------------------------------
#ifdef rx_NU_1
    case CHC_PROTOCOL::NU_ID1: // = 0x1E0,
        sData.nu.longitude = rx_frame.data[0] | (rx_frame.data[1] << 8) | (rx_frame.data[2] << 16) | (rx_frame.data[3] << 24);
        sData.nu.latitude = rx_frame.data[4] | (rx_frame.data[5] << 8) | (rx_frame.data[6] << 16) | (rx_frame.data[7] << 24);

        return GET_NU;
        // return PROCESS_DONE;
        break;
#endif
// ----------------------------------------------------------------
#ifdef rx_NU_2
    case CHC_PROTOCOL::NU_ID2: // = 0x1E1,
        sData.nu.altitude = rx_frame.data[0] | (rx_frame.data[1] << 8);
        sData.nu.speed = rx_frame.data[2] | (rx_frame.data[3] << 8);

        return GET_NU;
        // return PROCESS_DONE;
        break;
#endif
// ----------------------------------------------------------------
#ifdef rx_NU_V
    case CHC_PROTOCOL::NU_V: // = 0x1EF
        for (uint8_t i = 0; i < 6; i++) {
            sData.ver.NU[i] = rx_frame.data[i];
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
    tx_frame.identifier = HMI_DIAG;
    tx_frame.extd = 0;
    tx_frame.rtr = 0;
    tx_frame.data_length_code = 1;
    tx_frame.data[0] = error;
    return CAN_base_transmit(&tx_frame);
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
    tx_frame.identifier = HMI_ID1;
    tx_frame.extd = 0;
    tx_frame.rtr = 0;
    tx_frame.data_length_code = 3;
    tx_frame.data[0] = hr_status;
    tx_frame.data[1] = hr_value;
    tx_frame.data[2] = sport_mode;
    return CAN_base_transmit(&tx_frame);
}

// 設定輔助力
bool CHC_PROTOCOL::MCU_setAssist(uint8_t u8Assist)
{
    tx_frame.identifier = HMI_ID2;
    tx_frame.extd = 0;
    tx_frame.rtr = 0;
    tx_frame.data_length_code = 1;
    tx_frame.data[0] = u8Assist;
    return CAN_base_transmit(&tx_frame);
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

    tx_frame.identifier = NM_get_info;
    tx_frame.extd = 0;
    tx_frame.rtr = 0;
    tx_frame.data_length_code = 1;
    tx_frame.data[0] = flag;
    return CAN_base_transmit(&tx_frame);
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

    tx_frame.identifier = NM_set_CMD;
    tx_frame.extd = 0;
    tx_frame.rtr = 0;
    tx_frame.data_length_code = 1;
    tx_frame.data[0] = flag;
    return CAN_base_transmit(&tx_frame);
}

// 設定RRU警示距離、警示LED閃爍頻率
bool CHC_PROTOCOL::RRU_setParam(
    uint16_t distance,
    uint8_t Hz)
{
    tx_frame.identifier = HMItoRRU;
    tx_frame.extd = 0;
    tx_frame.rtr = 0;
    tx_frame.data_length_code = 3;
    tx_frame.data[0] = distance;
    tx_frame.data[1] = distance >> 8;
    tx_frame.data[2] = Hz;
    return CAN_base_transmit(&tx_frame);
}

// 設定CWS警示距離
bool CHC_PROTOCOL::CWS_setParam(
    uint16_t u16Distance,
    uint8_t u8Range)
{
    tx_frame.identifier = HMItoCWS;
    tx_frame.extd = 0;
    tx_frame.rtr = 0;
    tx_frame.data_length_code = 3;
    tx_frame.data[0] = u16Distance;
    tx_frame.data[1] = u16Distance >> 8;
    tx_frame.data[2] = u8Range;
    return CAN_base_transmit(&tx_frame);
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
    tx_frame.identifier = HMI_V;
    tx_frame.extd = 0;
    tx_frame.rtr = 0;
    tx_frame.data_length_code = 6;
    tx_frame.data[0] = protocol_major;
    tx_frame.data[1] = protocol_minor;
    tx_frame.data[2] = sw_major;
    tx_frame.data[3] = sw_minor;
    tx_frame.data[4] = hw_major;
    tx_frame.data[5] = hw_minor;
    return CAN_base_transmit(&tx_frame);
}
#endif
#ifdef node_MCU
// 診斷碼
bool CHC_PROTOCOL::MCUtoDIAG(uint8_t error)
{
    tx_frame.identifier = MCU_DIAG;
    tx_frame.extd = 0;
    tx_frame.rtr = 0;
    tx_frame.data_length_code = 1;
    tx_frame.data[0] = error;
    return CAN_base_transmit(&tx_frame);
}

// 傳送扭力、踏頻、速度、電量
bool CHC_PROTOCOL::MCU_period(
    uint8_t support,
    uint16_t torque,
    uint16_t cadence,
    uint16_t speed,
    uint8_t battery)
{

    tx_frame.identifier = MCU_ID1;
    tx_frame.extd = 0;
    tx_frame.rtr = 0;
    tx_frame.data_length_code = 8;
    tx_frame.data[0] = support; // 輔助力段位
    tx_frame.data[1] = torque; // 扭力_L
    tx_frame.data[2] = torque >> 8; // 扭力_H
    tx_frame.data[3] = cadence; // 踏頻_L
    tx_frame.data[4] = cadence >> 8; // 踏頻_H
    tx_frame.data[5] = speed; // 速度_L
    tx_frame.data[6] = speed >> 8; // 速度_H
    tx_frame.data[7] = battery; // 電量
    return CAN_base_transmit(&tx_frame);
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
    tx_frame.identifier = MCU_V;
    tx_frame.extd = 0;
    tx_frame.rtr = 0;
    tx_frame.data_length_code = 6;
    tx_frame.data[0] = protocol_major;
    tx_frame.data[1] = protocol_minor;
    tx_frame.data[2] = sw_major;
    tx_frame.data[3] = sw_minor;
    tx_frame.data[4] = hw_major;
    tx_frame.data[5] = hw_minor;
    return CAN_base_transmit(&tx_frame);
}
#endif
#ifdef node_RRU
// 診斷碼
bool CHC_PROTOCOL::RRUtoDIAG(uint8_t error)
{
    tx_frame.identifier = RRU_DIAG;
    tx_frame.extd = 0;
    tx_frame.rtr = 0;
    tx_frame.data_length_code = 1;
    tx_frame.data[0] = error;
    return CAN_base_transmit(&tx_frame);
}

// 傳送偵測物體ID、距離、速度、角度
bool CHC_PROTOCOL::RRU_E(
    uint8_t id,
    uint16_t distance,
    uint16_t speed,
    uint8_t degree)
{
    tx_frame.identifier = RRU_ID1;
    tx_frame.extd = 0;
    tx_frame.rtr = 0;
    tx_frame.data_length_code = 6;
    tx_frame.data[0] = id;
    tx_frame.data[1] = distance;
    tx_frame.data[2] = distance >> 8;
    tx_frame.data[3] = speed;
    tx_frame.data[4] = speed >> 8;
    tx_frame.data[5] = degree;
    return CAN_base_transmit(&tx_frame);
}

/* 傳送警戒狀態、燈狀態
   警戒狀態: 0:無物體在警戒範圍內
            1:有物體在警戒範圍內
   燈狀態  0:關閉
           1:恆亮
           2:閃爍
*/
bool CHC_PROTOCOL::RRU_period(
    uint8_t status_alarm_L,
    uint8_t status_alarm_R,
    uint8_t status_light)
{
    tx_frame.identifier = RRU_ID2;
    tx_frame.extd = 0;
    tx_frame.rtr = 0;
    tx_frame.data_length_code = 3;
    tx_frame.data[0] = status_alarm_L;
    tx_frame.data[1] = status_alarm_R;
    tx_frame.data[2] = status_light;
    return CAN_base_transmit(&tx_frame);
}

// 傳送版本資訊
bool CHC_PROTOCOL::RRU_version(
    uint8_t protocol_major,
    uint8_t protocol_minor,
    uint8_t sw_major,
    uint8_t sw_minor,
    uint8_t hw_major,
    uint8_t hw_minor)
{
    tx_frame.identifier = RRU_V;
    tx_frame.extd = 0;
    tx_frame.rtr = 0;
    tx_frame.data_length_code = 6;
    tx_frame.data[0] = protocol_major;
    tx_frame.data[1] = protocol_minor;
    tx_frame.data[2] = sw_major;
    tx_frame.data[3] = sw_minor;
    tx_frame.data[4] = hw_major;
    tx_frame.data[5] = hw_minor;
    return CAN_base_transmit(&tx_frame);
}
#endif
#ifdef node_CWS
// 診斷碼
bool CHC_PROTOCOL::CWStoDIAG(uint8_t error)
{
    tx_frame.identifier = CWS_DIAG;
    tx_frame.extd = 0;
    tx_frame.rtr = 0;
    tx_frame.data_length_code = 1;
    tx_frame.data[0] = error;
    return CAN_base_transmit(&tx_frame);
}

// 坑洞資訊
bool CHC_PROTOCOL::CWS_period(
    uint16_t distance,
    uint8_t degree)
{
    tx_frame.identifier = CWS_ID1;
    tx_frame.extd = 0;
    tx_frame.rtr = 0;
    tx_frame.data_length_code = 3;
    tx_frame.data[0] = distance;
    tx_frame.data[1] = distance >> 8;
    tx_frame.data[2] = degree;
    return CAN_base_transmit(&tx_frame);
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
    tx_frame.identifier = CWS_V;
    tx_frame.extd = 0;
    tx_frame.rtr = 0;
    tx_frame.data_length_code = 6;
    tx_frame.data[0] = protocol_major;
    tx_frame.data[1] = protocol_minor;
    tx_frame.data[2] = sw_major;
    tx_frame.data[3] = sw_minor;
    tx_frame.data[4] = hw_major;
    tx_frame.data[5] = hw_minor;
    return CAN_base_transmit(&tx_frame);
}
#endif
#ifdef node_NU

// 診斷碼
bool CHC_PROTOCOL::NUtoDIAG(uint8_t error)
{
    tx_frame.identifier = NU_DIAG;
    tx_frame.extd = 0;
    tx_frame.rtr = 0;
    tx_frame.data_length_code = 1;
    tx_frame.data[0] = error;
    return CAN_base_transmit(&tx_frame);
}

// 經度、緯度
bool CHC_PROTOCOL::NU_period1(
    float longitude,
    float latitude)
{
    U_float2bytes trans;
    tx_frame.identifier = NU_ID1;
    tx_frame.extd = 0;
    tx_frame.rtr = 0;
    tx_frame.data_length_code = 8;
    trans.var = latitude;

    for (int i = 0; i < 4; i++) {
        tx_frame.data[i] = trans.array[3 - i];
    }
    trans.var = latitude;

    for (int i = 0; i < 4; i++) {
        tx_frame.data[i + 4] = trans.array[3 - i];
    }

    return CAN_base_transmit(&tx_frame);
}

// 海拔、速度、模組連網狀態
bool CHC_PROTOCOL::NU_period2(
    uint16_t altitude,
    uint16_t speed,
    uint8_t status)
{
    tx_frame.identifier = NU_ID2;
    tx_frame.extd = 0;
    tx_frame.rtr = 0;
    tx_frame.data_length_code = 5;
    tx_frame.data[0] = (uint8_t)altitude;
    tx_frame.data[1] = (uint8_t)altitude >> 8;
    tx_frame.data[2] = (uint8_t)speed;
    tx_frame.data[3] = (uint8_t)speed >> 8;
    tx_frame.data[4] = status;
    return CAN_base_transmit(&tx_frame);
}

bool CHC_PROTOCOL::NU_version(
    uint8_t protocol_major,
    uint8_t protocol_minor,
    uint8_t sw_major,
    uint8_t sw_minor,
    uint8_t hw_major,
    uint8_t hw_minor)
{
    tx_frame.identifier = NU_V;
    tx_frame.extd = 0;
    tx_frame.rtr = 0;
    tx_frame.data_length_code = 6;
    tx_frame.data[0] = protocol_major;
    tx_frame.data[1] = protocol_minor;
    tx_frame.data[2] = sw_major;
    tx_frame.data[3] = sw_minor;
    tx_frame.data[4] = hw_major;
    tx_frame.data[5] = hw_minor;
    return CAN_base_transmit(&tx_frame);
}
#endif
