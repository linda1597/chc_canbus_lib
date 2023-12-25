#ifndef _CHC_PROTOCOL_HYENA_H_
#define _CHC_PROTOCOL_HYENA_H_
#include <Arduino.h>
#include <CAN_base.h>

#define DEBUG_BIKE_HYENA

#ifndef DEBUG_BIKE_HYENA_LEVEL
#define DEBUG_BIKE_HYENA_LEVEL 4
#endif

#define DEBUG_BIKE_HYENA_SERIAL Serial

#if DEBUG_BIKE_HYENA_LEVEL >= 1

#define BIKE_HYENA_LOG_E(fmt, ...) DEBUG_BIKE_HYENA_SERIAL.printf("[E][%10lu][BKH]%25s:%-5d > " fmt "\n", millis(), __FILE__, __LINE__, ##__VA_ARGS__);
#else
#define BIKE_HYENA_LOG_E(fmt, ...) ;
#endif

#if DEBUG_BIKE_HYENA_LEVEL >= 2
#define BIKE_HYENA_LOG_W(fmt, ...) DEBUG_BIKE_HYENA_SERIAL.printf("[W][%10lu][BKH]%25s:%-5d > " fmt "\n", millis(), __FILE__, __LINE__, ##__VA_ARGS__);
#else
#define BIKE_HYENA_LOG_W(fmt, ...) ;
#endif

#if DEBUG_BIKE_HYENA_LEVEL >= 3
#define BIKE_HYENA_LOG_I(fmt, ...) DEBUG_BIKE_HYENA_SERIAL.printf("[I][%10lu][BKH]%25s:%-5d > " fmt "\n", millis(), __FILE__, __LINE__, ##__VA_ARGS__);
#else
#define BIKE_HYENA_LOG_I(fmt, ...) ;
#endif

#if DEBUG_BIKE_HYENA_LEVEL >= 4
#define BIKE_HYENA_LOG(fmt, ...) DEBUG_BIKE_HYENA_SERIAL.printf("[ ][%10lu][BKH]%25s:%-5d > " fmt, millis(), __FILE__, __LINE__, ##__VA_ARGS__);
#define BIKE_HYENA_LOG_S(fmt, ...) DEBUG_BIKE_HYENA_SERIAL.printf(fmt, ##__VA_ARGS__);
#define BIKE_HYENA_LOG_V(fmt, ...) DEBUG_BIKE_HYENA_SERIAL.printf("[V][%10lu][BKH]%25s:%-5d > " fmt "\n", millis(), __FILENAME__, __LINE__, ##__VA_ARGS__);
#else
#define BIKE_HYENA_LOG(fmt, ...)
#define BIKE_HYENA_LOG_S(fmt, ...)
#define BIKE_HYENA_LOG_V(fmt, ...) ;
#endif

#ifdef node_NU
#define rx_MCU
#endif
// * --------------------------------------------------
class CHC_PROTOCOL_HYENA {
public:
    CHC_PROTOCOL_HYENA();
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
    // ! CANBUS 控制器 ........................................................
    typedef struct {
        int8_t assist;
        uint16_t torque;
        uint16_t cadence;
        uint16_t speed;
        uint8_t battery;
        // ----------------------
        uint8_t batteryHealth;

        int8_t tempDriveFet;
        int32_t odo;
        int32_t current;
        int32_t voltage;
        uint8_t uniqueId[8];
        int8_t error[7];
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
        // uint16_t distance;
        // int8_t angle;
        uint16_t location;
        uint16_t size;
        uint16_t distance;
        int8_t type;

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

    // ! CANBUS 控制器 ........................................................

    // unsigned char* getUniqueId(void);
    // char getRpm(void);
    // int getTorque(void);

    // char getAssistLevel(void);
    // char getTempDriveFet(void);
    // int getSpeed(void);

    // long getOdo(void);

    // char getBatCapacity(void);
    // char getBatHealth(void);

    // long getCurrent(void);
    // long getVoltage(void);
    // char getError(unsigned char cIndex);

    // ! CANBUS 主程式 ...............................................................
    bool init(int rx_pin, int tx_pin, long baudrate = 250000);
    REQ_type rx(void);

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
private:
};

extern CHC_PROTOCOL_HYENA chcProtocol_hyena;

#endif