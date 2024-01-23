#ifndef _CHC_PROTOCOL_HYENA2_H_
#define _CHC_PROTOCOL_HYENA2_H_
#include <Arduino.h>
#include <CAN_base.h>

// ----------------------------------------------------------------
#ifdef node_RRU
#define rx_NM_get_INFO
#define rx_NM_set_CMD
#define rx_update_rru_FW
#define rx_read_rru_params
#define rx_write_rru_params
#define rx_read_rru_modID
#define rx_tool_control
#endif
// ----------------------------------------------------------------
#ifdef node_NU
#define rx_elockInfo
#define rx_HMIModuleIDBroadcasting
#define rx_HMIErrorInfo
#define rx_derailleurModuleIDBroadcasting
#define rx_derailleurState
#define rx_controllerModuleIDBroadcasting
#define rx_controllerInfo01
#define rx_controllerInfo02
#define rx_controllerInfo03
#define rx_controllerErrorInfo
#define rx_battery1ModuleIDBroadcasting
#define rx_battery1Info01
#define rx_battery1Info02
#define rx_battery1Info06
#define rx_battery2ModuleIDBroadcasting
#define rx_battery2Info01
#define rx_battery2Info02
#define rx_battery2Info06
#define rx_dropperModuleIDBroadcasting
#define rx_dropperErrorInfo
#define rx_forkModuleIDBroadcasting
#define rx_forkInfo01
#define rx_forkErrorInfo
#define rx_radarModuleIDBroadcasting
#define rx_radarErrorInfo
#endif
// ----------------------------------------------------------------

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
    
    bool bfirstConsecutive;
    uint8_t operatingTime;
    uint8_t SME;
    uint16_t paramlength;
    uint32_t startAddress;    
    
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

    // typedef union{
    //     uint8_t register_status;
    // }U_REGISTER_STATUS;

    typedef union{
        uint16_t safety_version;
        uint8_t bytes[2];
    }U_SAFETY_VERSION;

    typedef union{
        uint16_t supplier_code;
        uint8_t bytes[2];
    }U_SUPPLIER_CODE;

    typedef union{
        uint16_t custom_code;
        uint8_t bytes[2];
    }U_CUSTOM_CODE;

    typedef union 
    {
        //String ble_name;
        uint8_t bytes[22];
    }U_APP_BLE_NAME;

    typedef union{
        uint8_t bytes[22];
    }U_BOOTLOADER_BLE_NAME;

    typedef union{
        uint16_t HW_K_version;
        uint8_t bytes[2];
    }U_HW_K_VERSION;

    typedef union{
        uint16_t K_version;
        uint8_t bytes[2];
    }U_K_VERSION;

    typedef union{
        uint16_t bootloader_v;
        uint8_t bytes[2];
    }U_BOOTLOADER_V;

    typedef union{
        uint8_t bytes[3];
    }U_PARAM_VERSION;

    typedef struct //__attribute((__packed__))
    {
        uint8_t supported_distance;
        uint16_t frequency1;
        uint16_t frequency2;
        uint32_t warning_distance;
    }S_RADAR_PARAMS;
    //S_RADAR_PARAMS s_radar_params;
    uint8_t app_flag;
    typedef struct //__attribute((__packed__))
    {
        U_MODID u_modid;
        U_BIKEID u_bikeid;
        U_SOLDDATE u_solddate;
        U_ManufacturedDate u_manu;
        U_FW_VERSION u_fw_v;
        U_HW_VERSION u_hw_v;
        U_PART_NM u_part_nm;
        U_FRAME_NM u_frame_nm;
        U_TEST_RESULT u_test_result;
        U_BIKE_VERSION u_bike_version;
        uint8_t register_status;
        U_SAFETY_VERSION u_safety_version;
        U_SUPPLIER_CODE u_supplier_code;
        uint8_t reversed1[82];
        U_CUSTOM_CODE u_custom_code;
        U_APP_BLE_NAME u_app_ble_name;
        U_BOOTLOADER_BLE_NAME u_bootloader_ble_name;
        uint8_t reversed2[8];
        U_HW_K_VERSION u_hw_k_version;
        U_K_VERSION u_k_version;
        U_BOOTLOADER_V u_bootloader_v;
        U_PARAM_VERSION u_param_version;
#ifdef node_RRU
        S_RADAR_PARAMS s_radar_params;
#endif
    }S_BIKE_PARAMETERS;
    S_BIKE_PARAMETERS s_bike_parameters;

#ifdef node_NU
    typedef struct
    {
        uint8_t manufacturer;
        uint8_t model;
        uint8_t FWVersion;
        uint8_t HWVersion;
        uint8_t protocolVersion;
        uint8_t SN[3];
        uint16_t errorCode;
    }S_BASIC_INFO;

    typedef union
    {
        S_BASIC_INFO contents;
        uint8_t bytes[sizeof(S_BASIC_INFO)];
    }U_BASIC_INFO;
    // typedef struct
    // {
    //     S_BASIC_INFO s_radar_basic_info;
    //     uint16_t errorCode;
    // }S_RADAR_INFO;

    typedef struct
    {
        U_BASIC_INFO u_fork_basic_info;
        uint8_t suspensionLevel;
    }S_FORK_INFO;

    typedef struct
    {
        U_BASIC_INFO u_battery_basic_info;
        uint32_t batteryPercent; // %
        int8_t temperature1; // 
        int8_t temperature2; // 
        int8_t temperature3; // 
        int8_t temperature4; // 
        int8_t temperature5; // 
        int8_t temperature6; // 
        int8_t temperature7; // 
        int8_t temperature8; // 
        uint32_t voltage; // mV
        int32_t current; // mA
    }S_BATTERY_INFO;

    typedef struct
    {
        U_BASIC_INFO u_controller_basic_info;
        uint16_t bikeSpeed; //0.01 km/h
        uint16_t torque;
        uint8_t lightStatus;
        uint8_t assistLevel;
        uint32_t odo; //m
        uint8_t estimatedRange; //km
        uint16_t cadence; //0.025 RPM
    }S_CONTROLLER_INFO;
    
    typedef struct
    {
        U_BASIC_INFO u_derailleur_basic_info;
        uint8_t gearIndex;
    }S_DERAILLEUR_INFO;

    typedef struct 
    {
        U_BASIC_INFO u_elock_basic_info;
        uint8_t elockStatus;
    }S_ELOCK_INFO;

    typedef struct
    {
        U_BASIC_INFO u_radar_info;
        U_BASIC_INFO u_dropper_info;
        U_BASIC_INFO u_hmi_info;
        U_BASIC_INFO u_iot_info;
        S_FORK_INFO s_fork_info;
        S_BATTERY_INFO s_battery1_info;
        S_BATTERY_INFO s_battery2_info;
        S_CONTROLLER_INFO s_controller_info;
        S_DERAILLEUR_INFO s_derailleur_info;
        S_ELOCK_INFO s_elock_info;
    }S_BIKE_INFO;

    typedef union 
    {
        S_BIKE_INFO contents;
        uint8_t bytes[sizeof(S_BIKE_INFO)];
    }U_BIKE_INFO;   
    
#endif
    enum CAN_ID {
        // Console ID
        BikeControl00 = 0x300,
        ToolControl00 = 0x309,
        // elock ID
        eLockInfo = 0x633,
        // HMI ID
        HMIModuleIDBroadcasting = 0x14001,
        HMIErrorInfo = 0x329,
        // Controller ID
        ControllerModuleIDBroadcasting = 0x13000,
        ControllerInfo01 = 0x201,
        ControllerInfo02 = 0x202,
        ControllerInfo03 = 0x203,
        ControllerErrorInfo = 0x209,
        // Derailleur ID
        DerailleurModuleIDBroadcasting = 0x1F000,
        DerailleurState = 0x650,
        // Dropper ID
        DropperModuleIDBroadcasting = 0x1B000,
        DropperErrorInfo = 0x559,
        // Fork ID
        ForkModuleIDBroadcasting = 0x13500,
        ForkInfo01 = 0x751,
        ForkErrorInfo = 0x759,
        // Radar ID        
        RRU_FW_UPDATE_REQ = 0x190,
        RRU_FW_UPDATE_RP = 0x191,
        RRU_MODULE_ID_BROADCAST = 0x19000,
        RRU_MODULE_ID_READ = 0x19001,
        RadarInfo01 = 0x701,
        RadarErrorInfo = 0x709,
        RRU_PARAM_R_REQ = 0x70000,
        RRU_PARAM_R_RES = 0x70001,
        RRU_PARAM_W_REQ = 0x70002,
        RRU_PARAM_W_RES = 0x70003,
        // Battery1 ID
        Battery1ModuleIDBroadcasting = 0x11000,
        Battery1Info00 = 0x400,
        Battery1Info01 = 0x401,
        Battery1Info02 = 0x402,
        Battery1Info06 = 0x406,
        // Battery2 ID
        Battery2ModuleIDBroadcasting = 0x12000,
        Battery2Info00 = 0x450,
        Battery2Info01 = 0x451,
        Battery2Info02 = 0x452,
        Battery2Info06 = 0x456,
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
        ReadRadarParameters,
        REQ_ERC,
        REQ_MCU_INFO,
        REQ_RRU_INFO,
        REQ_CWS_INFO,
        REQ_NU_INFO,
        REQ_RRU_ID,
        RRU_UPDATE,
        SET_RRU_AWAKE,
        SET_RRU_SLEEP,
        SET_RRU_PARAM,
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
    bool RadarModIDBroadcast();
    bool RadarStartRead();
    bool rxReadRadarParamSingleFrame();
    bool rxReadRadarParamFlowControlFrame();
    bool txReadRadarParamConsecutiveFrame();
    typedef enum {
        JumptoBootloader = 0x01,
        JumptoApp,
        MCUInfoRead,
        FlashErase,
        FlashProgram,
        Writedata,
        FlashVerify,
        ParameterRead = 0x10,
        ParameterWrite,
        LogRead,
        LogClear,
        SetShipMode
    }OPCode; 

    typedef enum{
        SUCCESS,
        TimeOut,
        InvalidAddress,
        InvalidSize,
        InvalidParameter,
        CRCError,
        Unknown,
        Error
    } RPCode;

    typedef enum {
        Idle,
        Start,
        Concetutive,
    }RWStatus;
    //RWStatus rwStatus;
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