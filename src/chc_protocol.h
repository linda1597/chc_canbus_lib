#ifndef _CHC_PROTOCOL_H_
#define _CHC_PROTOCOL_H_

#ifndef node_HMI
#define node_HMI 1
#endif

#define DEBUG_CHC_PL_LEVEL 4
#define DEBUG_CHC_PL_SERIAL Serial
#if DEBUG_CHC_PL_LEVEL >= 1

#define CHC_PL_LOG_E(fmt, ...) DEBUG_CHC_PL_SERIAL.printf("[E][%10lu][   ]%25s:%-5d > " fmt "\n", millis(), __FILE__, __LINE__, ##__VA_ARGS__);
#else
#define CHC_PL_LOG_E(fmt, ...) ;
#endif

#if DEBUG_CHC_PL_LEVEL >= 2
#define CHC_PL_LOG_W(fmt, ...) DEBUG_CHC_PL_SERIAL.printf("[W][%10lu][   ]%25s:%-5d > " fmt "\n", millis(), __FILE__, __LINE__, ##__VA_ARGS__);
#else
#define CHC_PL_LOG_W(fmt, ...) ;
#endif

#if DEBUG_CHC_PL_LEVEL >= 3
#define CHC_PL_LOG_I(fmt, ...) DEBUG_CHC_PL_SERIAL.printf("[I][%10lu][   ]%25s:%-5d > " fmt "\n", millis(), __FILE__, __LINE__, ##__VA_ARGS__);
#else
#define CHC_PL_LOG_I(fmt, ...) ;
#endif

#if DEBUG_CHC_PL_LEVEL >= 4
#define CHC_PL_LOG(fmt, ...) DEBUG_CHC_PL_SERIAL.printf("[ ][%10lu][   ]%25s:%-5d > " fmt, millis(), __FILE__, __LINE__, ##__VA_ARGS__);
#define CHC_PL_LOG_S(fmt, ...) DEBUG_CHC_PL_SERIAL.printf(fmt, ##__VA_ARGS__);
#define CHC_PL_LOG_V(fmt, ...) DEBUG_CHC_PL_SERIAL.printf("[V][%10lu][   ]%25s:%-5d > " fmt "\n", millis(), __FILE__, __LINE__, ##__VA_ARGS__);
#else
#define CHC_PL_LOG(fmt, ...)
#define CHC_PL_LOG_S(fmt, ...)
#define CHC_PL_LOG_V(fmt, ...) ;
#endif

class chc_PROTOCOL {
public:
    chc_PROTOCOL();
    bool CAN_init();
    typedef struct
    {
        uint8_t HMI_V_data[6];
        uint8_t MCU_V_data[6];
        uint8_t RRU_V_data[6];
        uint8_t CWS_V_data[6];
        uint8_t NU_V_data[6];
    } S_component_version;
    S_component_version Rx;

    typedef struct
    {
        uint8_t gear;
        uint16_t torque;
        uint16_t cadence;
        uint16_t velocity;
        uint8_t battery;
    } S_MCU_data;
    S_MCU_data mcu_data;

    typedef struct
    {
        uint8_t ID;
        uint16_t distance;
        uint16_t velocity;
        uint8_t angle;
        uint8_t alarm_status;
        uint8_t light_status;
    } S_RRU_data;
    S_RRU_data rru_data;

    typedef struct
    {
        uint16_t distance;
        uint8_t angle;
    } S_CWS_data;
    S_CWS_data cws_data;

    typedef struct
    {
        float longitude;
        float latitude;
        uint16_t altitude;
        uint16_t velocity;
    } S_NU_data;
    S_NU_data nu_data;
    typedef struct {
        uint8_t HMI_DTC;
        uint8_t MCU_DTC;
        uint8_t RRU_DTC;
        uint8_t CWS_DTC;
        uint8_t NU_DTC;
    } S_ALL_DTC;
    S_ALL_DTC all_dtc;
#if node_HMI == 1
    int rx_HMI();
    void HMItoDIAG(uint8_t error);
    void HMI_period(uint8_t status, uint8_t HR, uint8_t mode);
    void HMI_setSupport(uint8_t support);
    void NM_getInfo(bool getMCUInfo, bool getRRUInfo, bool getCWSInfo, bool getNUInfo);
    void NM_CMD(bool setMCU, bool setRRU, bool setCWS, bool setNU);
    void setRRU(uint16_t distance, uint8_t Hz);
    void setCWS(uint16_t distance);
    void HMI_version(uint8_t protocol_major, uint8_t protocol_minor, uint8_t sw_major, uint8_t sw_minor, uint8_t hw_major, uint8_t hw_minor);
#elif node_MCU == 1
    int rx_MCU();
    void MCUtoDIAG(uint8_t error);
    void MCU_period(uint8_t support, uint16_t torque, uint16_t cadence, uint16_t speed, uint8_t battery);
    void MCU_version(uint8_t protocol_major, uint8_t protocol_minor, uint8_t sw_major, uint8_t sw_minor, uint8_t hw_major, uint8_t hw_minor);
#elif node_RRU == 1
    uint16_t detect_range;
    uint8_t bling_hz;
    void RRUtoDIAG(uint8_t error);
    int rx_RRU();
    void RRU_E(uint8_t id, uint16_t distance, uint16_t speed, uint8_t degree);
    void RRU_period(uint8_t alarm_status, uint8_t light_status);
    void RRU_version(uint8_t protocol_major, uint8_t protocol_minor, uint8_t sw_major, uint8_t sw_minor, uint8_t hw_major, uint8_t hw_minor);
#elif node_CWS == 1
    uint16_t detect_range;
    int rx_CWS();
    void CWStoDIAG(uint8_t error);
    void CWS_period(uint16_t distance, uint8_t degree);
    void CWS_version(uint8_t protocol_major, uint8_t protocol_minor, uint8_t sw_major, uint8_t sw_minor, uint8_t hw_major, uint8_t hw_minor);
#elif node_NU == 1
    uint16_t detect_range;
    typedef struct {
        uint8_t device_status;
        uint8_t heart_rate;
        uint8_t sport_mode;
    } S_HMI_data;
    S_HMI_data hmi_data;

    typedef union {
        float var;
        uint8_t array[4];
    } U_float2bytes;

    int rx_NU();
    void NUtoDIAG(uint8_t error);
    void NU_period1(float longitude, float latitude);
    void NU_period2(uint16_t altitude, uint16_t velocity, uint8_t status);
    void NU_version(uint8_t protocol_major, uint8_t protocol_minor, uint8_t sw_major, uint8_t sw_minor, uint8_t hw_major, uint8_t hw_minor);
#endif
};

enum REQ_type {
    NONE = 2,
    REQ_ERC,
    REQ_MCU_INFO,
    REQ_RRU_INFO,
    REQ_CWS_INFO,
    REQ_NU_INFO,
    SET_MCU_AWAKE,
    SET_MCU_SLEEP,
    SET_RRU_AWAKE,
    SET_RRU_SLEEP,
    SET_RRU_PARAM,
    SET_CWS_AWAKE,
    SET_CWS_SLEEP,
    SET_CWS_PARAM,
    SET_NU_AWAKE,
    SET_NU_SLEEP,
    PROCESS_DONE,
};

enum CAN_ID {
    // DIAG ID
    DIAGtoHMI = 0x772,
    DIAGtoMCU = 0x774,
    DIAGtoRRU = 0x775,
    DIAGtoCWS = 0x776,
    DIAGtoNU = 0x777,
    // HMI ID
    HMI_DIAG = 0x130,
    HMI_ID1 = 0x140,
    HMI_ID2 = 0x141,
    NM_get_info = 0x14B,
    NM_set_CMD = 0x14C,
    HMItoRRU = 0x14D,
    HMItoCWS = 0x14E,
    HMI_V = 0x14F,
    // MCU ID
    MCU_DIAG = 0x150,
    MCU_ID1 = 0x160,
    MCU_V = 0x16F,
    // RRU ID
    RRU_DIAG = 0x190,
    RRU_ID1 = 0x1A0,
    RRU_ID2 = 0x1A1,
    RRU_V = 0x1AF,
    // CWS ID
    CWS_DIAG = 0x1B0,
    CWS_ID1 = 0x1C0,
    CWS_V = 0x1CF,
    // NU ID
    NU_DIAG = 0x1D0,
    NU_ID1 = 0x1E0,
    NU_ID2 = 0x1E1,
    NU_V = 0x1EF
};

#endif