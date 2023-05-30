#ifndef _CHC_PROTOCOL_H_
#define _CHC_PROTOCOL_H_
#include <Arduino.h>
#include <CAN_base.h>

// #define node_HMI
// #define node_MCU
// #define node_RRU
// #define node_CWS
// #define node_NU

/*
#define rx_HMItoDIAG
#define rx_MCUtoDIAG
#define rx_RRUtoDIAG
#define rx_CWStoDIAG
#define rx_NUtoDIAG

#define rx_DIAGtoHMI
#define rx_DIAGtoMCU
#define rx_DIAGtoRRU
#define rx_DIAGtoCWS
#define rx_DIAGtoNU

#define rx_HMI_1
#define rx_HMI_2
#define rx_HMI_V
#define rx_NM_get_INFO
#define rx_NM_get_CMD
#define rx_HMItoRRU
#define rx_HMItoCWS

#define rx_MCU_1
#define rx_MCU_V

#define rx_RRU_1
#define rx_RRU_2
#define rx_RRU_V

#define rx_CWS_1
#define rx_CWS_V

#define rx_NU_1
#define rx_NU_2
#define rx_NU_V
*/

#ifdef node_HMI
#define rx_MCUtoDIAG
#define rx_MCU_1
#define rx_MCU_V

#define rx_RRUtoDIAG
#define rx_RRU_1
#define rx_RRU_2
#define rx_RRU_V

#define rx_CWStoDIAG
#define rx_CWS_1
#define rx_CWS_V

#define rx_NUtoDIAG
#define rx_NU_2
#define rx_NU_V
#define rx_DIAGtoHMI
#endif
// ----------------------------------------------------------------
#ifdef node_MCU
#define rx_HMI_2
#define rx_NM_get_INFO
#define rx_NM_set_CMD
#define rx_HMI_V

#define rx_RRU_V
#define rx_CWS_V
#define rx_NU_V
#define rx_DIAGtoMCU
#endif
// ----------------------------------------------------------------
#ifdef node_RRU
#define rx_NM_get_INFO
#define rx_NM_set_CMD
#define rx_HMItoRRU
#define rx_HMI_V

#define rx_MCU_V

#define rx_RRU_V

#define rx_CWS_V

#define rx_NU_V
#define rx_DIAGtoRRU
#endif
// ----------------------------------------------------------------
#ifdef node_CWS
#define NM_get_INFO
#define NM_set_CMD
#define rx_HMItoCWS
#define rx_HMI_V
#define rx_MCU_V
#define rx_RRU_V
#define rx_NU_V
#define rx_DIAGtoCWS
#endif
// ----------------------------------------------------------------
#ifdef node_NU
#define rx_HMI_1
#define rx_HMI_2
#define rx_NM_get_INFO
#define rx_NM_set_CMD
#define rx_HMI_V
#define rx_MCUtoDIAG
#define rx_MCU_1
#define rx_MCU_V
#define rx_RRUtoDIAG
#define rx_RRU_1
#define rx_RRU_2
#define rx_RRU_V
#define rx_CWStoDIAG
#define rx_CWS_1
#define rx_CWS_V
#define rx_NUtoDIAG
#define rx_DIAGtoNU
#endif
// ----------------------------------------------------------------
#ifdef node_DIAG
#define rx_HMItoDIAG
#define rx_HMI_V
#define rx_MCUtoDIAG
#define rx_MCU_V
#define rx_RRUtoDIAG
#define rx_RRU_V
#define rx_CWStoDIAG
#define rx_CWS_V
#define rx_NUtoDIAG
#define rx_NU_V
#endif

// -- verson ..

// #define vCompanyId 1 // 2

#define protocol_Major 1
#define protocol_Minor 0
#define sw_Major 1
#define sw_Minor 0
#define hw_Major 1
#define hw_Minor 0

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

class CHC_PROTOCOL {
public:
    CHC_PROTOCOL();
    typedef union {
        float var;
        uint8_t array[4];
    } U_float2bytes;

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

    typedef struct
    {
        uint8_t HMI[6];
        uint8_t MCU[6];
        uint8_t RRU[6];
        uint8_t CWS[6];
        uint8_t NU[6];
    } S_VERSION;

    typedef struct
    {
        uint8_t assist;
        uint16_t torque;
        uint16_t cadence;
        uint16_t speed;
        uint8_t battery;
    } S_MCU_DATA;

    typedef struct
    {
        uint8_t id;
        uint16_t distance;
        uint16_t speed;
        int8_t angle;
        uint8_t status_alarm;
        uint8_t status_light;

        uint16_t set_detect_range;
        uint8_t set_bling_hz;
    } S_RRU_DATA;

    typedef struct
    {
        uint16_t distance;
        int8_t angle;

        uint16_t set_detect_range;
    } S_CWS_DATA;

    typedef struct
    {
        float longitude;
        float latitude;
        uint16_t altitude;
        uint16_t speed;
        uint8_t status;
    } S_NU_DATA;

    typedef struct {
        uint8_t hr_status;
        uint8_t hr_value;
        uint8_t sport_mode;
    } S_HMI_DATA;

    typedef struct {
        uint8_t HMI;
        uint8_t MCU;
        uint8_t RRU;
        uint8_t CWS;
        uint8_t NU;
    } S_ALL_DTC;

    typedef struct {
        S_VERSION ver;
        S_MCU_DATA mcu;
        S_RRU_DATA rru;
        S_CWS_DATA cws;
        S_NU_DATA nu;
        S_HMI_DATA hmi;
        S_ALL_DTC dtc;
    } S_DATA;

    S_DATA sData;

    typedef enum {
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
        GET_HMI,
        GET_MCU,
        GET_RRU,
        GET_CWS,
        GET_NU,
        GET_DIAG,
    } REQ_type;

    REQ_type rx();

#ifdef node_HMI
    bool HMItoDIAG(uint8_t error);

    bool HMI_period(
        uint8_t hr_status,
        uint8_t hr_value,
        uint8_t sport_mode);

    bool HMI_setSupport(uint8_t support);

    bool NM_getInfo(
        bool getMCUInfo,
        bool getRRUInfo,
        bool getCWSInfo,
        bool getNUInfo);

    bool NM_CMD(
        bool setMCU,
        bool setRRU,
        bool setCWS,
        bool setNU);

    bool setRRU(
        uint16_t distance,
        uint8_t Hz);

    bool setCWS(uint16_t distance);

    bool HMI_version(
        uint8_t protocol_major,
        uint8_t protocol_minor,
        uint8_t sw_major,
        uint8_t sw_minor,
        uint8_t hw_major,
        uint8_t hw_minor);
#endif

#ifdef node_MCU
    bool MCUtoDIAG(uint8_t error);
    bool MCU_period(
        uint8_t assist,
        uint16_t torque,
        uint16_t cadence,
        uint16_t speed,
        uint8_t battery);
    bool MCU_version(
        uint8_t protocol_major,
        uint8_t protocol_minor,
        uint8_t sw_major,
        uint8_t sw_minor,
        uint8_t hw_major,
        uint8_t hw_minor);
#endif

#ifdef node_RRU
    bool RRUtoDIAG(uint8_t error);
    bool RRU_E(
        uint8_t id,
        uint16_t distance,
        uint16_t speed,
        uint8_t degree);
    bool RRU_period(
        uint8_t alarm_status,
        uint8_t light_status);
    bool RRU_version(
        uint8_t protocol_major,
        uint8_t protocol_minor,
        uint8_t sw_major,
        uint8_t sw_minor,
        uint8_t hw_major,
        uint8_t hw_minor);
#endif

#ifdef node_CWS
    bool CWStoDIAG(uint8_t error);
    bool CWS_period(
        uint16_t distance,
        uint8_t degree);
    bool CWS_version(
        uint8_t protocol_major,
        uint8_t protocol_minor,
        uint8_t sw_major,
        uint8_t sw_minor,
        uint8_t hw_major,
        uint8_t hw_minor);
#endif

#ifdef node_NU

    bool NUtoDIAG(uint8_t error);
    bool NU_period1(
        float longitude,
        float latitude);
    bool NU_period2(
        uint16_t altitude,
        uint16_t speed,
        uint8_t status);
    bool NU_version(
        uint8_t protocol_major,
        uint8_t protocol_minor,
        uint8_t sw_major,
        uint8_t sw_minor,
        uint8_t hw_major,
        uint8_t hw_minor);
#endif
};

extern CAN_frame_t rx_msg;
extern CHC_PROTOCOL chcProtocol;

#endif