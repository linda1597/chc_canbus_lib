#ifndef _CHC_PROTOCOL_HYENA2_H_
#define _CHC_PROTOCOL_HYENA2_H_
#include <Arduino.h>
#include <CAN_base.h>

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
#define rx_update_rru_FW
#define rx_read_rru_params
#define rx_write_rru_params
#define rx_read_rru_modID
#define rx_tool_control

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

class CHC_PROTOCOL_HYENA2 {
public:
    CHC_PROTOCOL_HYENA2();
    unsigned int cal_crc32(const unsigned char *buf, int len, unsigned int init);
    
    uint8_t operatingTime;
    uint8_t SME;
    typedef union {
        float var;
        uint8_t array[4];
    } U_float2bytes;

    typedef union{
        uint64_t ID;
        uint8_t bytes[8];
    }U_MODID;    
    //U_PARAMETERS u_parameters;

    typedef union {
        uint64_t ID;
        uint8_t bytes[8];
    }U_BIKEID;

    typedef union{
        uint32_t Date;
        uint8_t bytes[4];
    }U_SOLDDATE;

    typedef union{
        uint32_t Date;
        uint8_t bytes[4];
    }U_ManufacturedDate;

    typedef union{
        uint16_t Version;
        uint8_t bytes[2];
    }U_FW_VERSION;    

    typedef union{
        uint16_t Version;
        uint8_t bytes[2];
    }U_HW_VERSION;

    typedef union{
        //String part_nm;
        uint8_t bytes[24];
    }U_PART_NM;

    typedef union{
        //String frame_nm;
        uint8_t bytes[32];
    }U_FRAME_NM;

    typedef union{
        uint64_t test_result;
        uint8_t bytes[8];
    }U_TEST_RESULT;

    typedef union{
        uint16_t bike_version;
        uint8_t bytes[2];
    }U_BIKE_VERSION;

    typedef union{
        uint8_t register_status;
    }U_REGISTER_STATUS;

    typedef union{
        uint16_t safety_version;
        uint8_t bytes[2];
    }U_SAFETY_VERSION;

    

    typedef struct __attribute((__packed__))
    {
        U_MODID u_modid;
        U_BIKEID u_bikeid;
        U_SOLDDATE u_solddate;
        U_ManufacturedDate u_manu;
        U_FW_VERSION u_fw_v;
        U_HW_VERSION u_hw_v;
        U_PART_NM u_part_nm;
        
    }S_PARAMETERS;
    S_PARAMETERS s_parameters;
    
    enum CAN_ID {
        //Tool Control
        TOOL_CONTROL = 0x309,
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
        RRU_FW_UPDATE_REQ = 0x190,
        RRU_FW_UPDATE_RP = 0x191,
        RRU_MODULE_ID_BROADCAST = 0x19000,
        RRU_MODULE_ID_READ = 0x19001,
        RRU_INFO = 0x701,
        RRU_ERR = 0x709,
        RRU_PARAM_R_REQ = 0x70000,
        RRU_PARAM_R_RES = 0x70001,
        RRU_PARAM_W_REQ = 0x70002,
        RRU_PARAM_W_RES = 0x70003,

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
        uint8_t status_alarm_L;
        uint8_t status_alarm_R;
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
        REQ_RRU_ID,
        RRU_UPDATE,
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
        GET_TOOL_CTRL
    } REQ_type;

    REQ_type rx();

#ifdef node_HMI
    bool HMItoDIAG(uint8_t error);

    bool HMI_period(
        uint8_t hr_status,
        uint8_t hr_value,
        uint8_t sport_mode);

    bool MCU_setAssist(uint8_t u8Assist);

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

    bool RRU_setParam(
        uint16_t distance,
        uint8_t Hz);

    bool CWS_setParam(
        uint16_t u16Distance,
        uint8_t u8Range);

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
    bool RRUError(uint8_t ErrPage,uint8_t ErrCode);
    bool RRU_Info(
        uint8_t id,
        uint16_t speed,
        uint16_t distance,
        uint8_t degree,
        uint8_t status);
    bool RRU_version(
        uint8_t protocol_major,
        uint8_t protocol_minor,
        uint8_t sw_major,
        uint8_t sw_minor,
        uint8_t hw_major,
        uint8_t hw_minor);

    bool RRU_FWupdateRP(uint8_t dlc,uint8_t *payload);
    bool RRU_ModIDBroadcast();
    enum OPCode{
        JumptoBootloader = 0x01,
        JumptoApp,
        MCUInfoRead,
        FlashErase,
        FlashProgram,
        Writedata,
        FlashVerify,
        ParameterRead,
        ParameterWrite,
        LogRead,
        LogClear,
        SetShipMode
    }; 

    enum RPCode{
        SUCCESS,
        TimeOut,
        InvalidAddress,
        InvalidSize,
        InvalidParameter,
        CRCError,
        Unknown,
        Error
    };
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
extern CHC_PROTOCOL_HYENA2 chcProtocol_hyena2;

#endif