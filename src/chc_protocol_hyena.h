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

#define BIKE_HYENA_LOG_E(fmt, ...) DEBUG_BIKE_HYENA_SERIAL.printf("[E][%10lu][   ]%25s:%-5d > " fmt "\n", millis(), __FILE__, __LINE__, ##__VA_ARGS__);
#else
#define BIKE_HYENA_LOG_E(fmt, ...) ;
#endif

#if DEBUG_BIKE_HYENA_LEVEL >= 2
#define BIKE_HYENA_LOG_W(fmt, ...) DEBUG_BIKE_HYENA_SERIAL.printf("[W][%10lu][   ]%25s:%-5d > " fmt "\n", millis(), __FILE__, __LINE__, ##__VA_ARGS__);
#else
#define BIKE_HYENA_LOG_W(fmt, ...) ;
#endif

#if DEBUG_BIKE_HYENA_LEVEL >= 3
#define BIKE_HYENA_LOG_I(fmt, ...) DEBUG_BIKE_HYENA_SERIAL.printf("[I][%10lu][   ]%25s:%-5d > " fmt "\n", millis(), __FILE__, __LINE__, ##__VA_ARGS__);
#else
#define BIKE_HYENA_LOG_I(fmt, ...) ;
#endif

#if DEBUG_BIKE_HYENA_LEVEL >= 4
#define BIKE_HYENA_LOG(fmt, ...) DEBUG_BIKE_HYENA_SERIAL.printf("[ ][%10lu][   ]%25s:%-5d > " fmt, millis(), __FILE__, __LINE__, ##__VA_ARGS__);
#define BIKE_HYENA_LOG_S(fmt, ...) DEBUG_BIKE_HYENA_SERIAL.printf(fmt, ##__VA_ARGS__);
#define BIKE_HYENA_LOG_V(fmt, ...) DEBUG_BIKE_HYENA_SERIAL.printf("[V][%10lu][   ]%25s:%-5d > " fmt "\n", millis(), __FILE__, __LINE__, ##__VA_ARGS__);
#else
#define BIKE_HYENA_LOG(fmt, ...)
#define BIKE_HYENA_LOG_S(fmt, ...)
#define BIKE_HYENA_LOG_V(fmt, ...) ;
#endif

// * --------------------------------------------------
class CHC_PROTOCOL_HYENA {
public:
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

    typedef struct {
        S_MCU_DATA mcu;
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

    CHC_PROTOCOL_HYENA();

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

private:
};

extern CHC_PROTOCOL_HYENA chcProtocol_hyena;

#endif