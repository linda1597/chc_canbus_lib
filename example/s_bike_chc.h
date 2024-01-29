#ifndef _S_BIKE_CHC_H_
#define _S_BIKE_CHC_H_

#define DEBUG_BIKE
#ifndef DEBUG_BIKE_LEVEL
#define DEBUG_BIKE_LEVEL 4
#endif

#define DEBUG_BIKE_SERIAL Serial

#if DEBUG_BIKE_LEVEL >= 1

#define BIKE_LOG_E(fmt, ...) DEBUG_BIKE_SERIAL.printf("[E][%10lu][   ]%25s:%-5d > " fmt "\n", millis(), __FILE__, __LINE__, ##__VA_ARGS__);
#else
#define BIKE_LOG_E(fmt, ...) ;
#endif

#if DEBUG_BIKE_LEVEL >= 2
#define BIKE_LOG_W(fmt, ...) DEBUG_BIKE_SERIAL.printf("[W][%10lu][   ]%25s:%-5d > " fmt "\n", millis(), __FILE__, __LINE__, ##__VA_ARGS__);
#else
#define BIKE_LOG_W(fmt, ...) ;
#endif

#if DEBUG_BIKE_LEVEL >= 3
#define BIKE_LOG_I(fmt, ...) DEBUG_BIKE_SERIAL.printf("[I][%10lu][   ]%25s:%-5d > " fmt "\n", millis(), __FILE__, __LINE__, ##__VA_ARGS__);
#else
#define BIKE_LOG_I(fmt, ...) ;
#endif

#if DEBUG_BIKE_LEVEL >= 4
#define BIKE_LOG(fmt, ...) DEBUG_BIKE_SERIAL.printf("[ ][%10lu][   ]%25s:%-5d > " fmt, millis(), __FILE__, __LINE__, ##__VA_ARGS__);
#define BIKE_LOG_S(fmt, ...) DEBUG_BIKE_SERIAL.printf(fmt, ##__VA_ARGS__);
#define BIKE_LOG_V(fmt, ...) DEBUG_BIKE_SERIAL.printf("[V][%10lu][   ]%25s:%-5d > " fmt "\n", millis(), __FILE__, __LINE__, ##__VA_ARGS__);
#else
#define BIKE_LOG(fmt, ...) ;
#define BIKE_LOG_S(fmt, ...) ;
#define BIKE_LOG_V(fmt, ...) ;
#endif
#ifdef node_NU
typedef struct
{
    uint8_t hmi_getUpdate : 1;
    uint8_t mcu_getUpdate : 1;
    uint8_t rru_getUpdate : 1;
    uint8_t cws_getUpdate : 1;

    uint8_t nu_getUpdate : 1;
    uint8_t reserve : 3;

    float nu_longitude;
    float nu_latitude;
    uint16_t nu_altitude;
    uint16_t nu_speed;
    uint8_t nu_status;

} S_BIKE_DATA_CHC;
extern S_BIKE_DATA_CHC sBikeDataIn;
#endif

#ifdef node_HMI
/* typedef struct
{
    // S_BIKE_UPDATE update;
    uint8_t hmi_getUpdate : 1;
    uint8_t mcu_getUpdate : 1;
    uint8_t rru_getUpdate : 1;
    uint8_t cws_getUpdate : 1;

    uint8_t nu_getUpdate : 1;
    uint8_t reserve : 3;

    uint8_t hmi_hr_status;
    uint8_t hmi_hr_value;
    uint8_t hmi_sport_level;

    uint8_t hmi_set_bike;
    uint8_t hmi_set_bike_assist;

    uint8_t hmi_set_rru;
    uint8_t hmi_set_rru_distance;
    uint8_t hmi_set_rru_light_hz;

    uint8_t hmi_set_cws;
    uint8_t hmi_set_cws_distance;
    uint8_t hmi_set_cws_range;
} S_BIKE_DATA_CHC;

extern  S_BIKE_DATA_CHC sBikeData_chc;
*/
#endif

#ifdef node_MCU
typedef struct {
    uint8_t hmi_getUpdate : 1;
    uint8_t mcu_getUpdate : 1;
    uint8_t rru_getUpdate : 1;
    uint8_t cws_getUpdate : 1;

    uint8_t nu_getUpdate : 1;
    uint8_t reserve : 3;

} S_BIKE_DATA_CHC;
#endif
/*
// ? 讀取控制器資訊
extern bool bike_getData_mcu(
    uint8_t* assist,
    float* torque,
    float* cadence,
    float* speed,
    float* battery);

// ? 讀取後方雷達資訊
extern bool bike_getData_rru(
    uint8_t* id,
    uint16_t* distance,
    float* speed,
    uint8_t* angle,
    uint8_t* status_alarm_l,
    uint8_t* status_alarm_r,
    uint8_t* status_light);

// ? 讀取HMI心跳資訊
extern bool bike_getData_hmi(
    uint8_t* hr_status,
    uint8_t* hr_value,
    uint8_t* sport_mode);

// ? 讀取前方坑洞資訊
extern bool bike_getData_cws(
    uint16_t* location,
    uint16_t* size,
    uint16_t* distance,
    int8_t* type);

// ? 讀取網路資訊
extern bool bike_getData_nu(
    float* longitude,
    float* latitude,
    uint16_t* altitude,
    uint16_t* speed,
    uint8_t* status) ;
*/
// extern int8_t bike_getConnect_chc(void);
extern bool bike_getConnect_chc(void);

extern bool bike_init_chc(void);
extern bool bike_loop_chc(void);
#endif