#ifndef _S_PARAMS_CHC_CHC_H_
#define _S_PARAMS_CHC_CHC_H_
#include <stdint.h>
typedef struct {
    uint8_t bUpdate : 1;
    uint8_t bConnected : 1;
    uint8_t bSet_assist : 1;
    uint8_t bSet_auto_mode : 1;

    uint8_t bSet_power_status : 1;
    uint8_t reverse : 3;

    float speed;
    float torque;
    uint16_t cadence;
    uint16_t error_code;
    uint8_t battery;
    uint8_t assist;

    uint8_t set_assist_value;
    // uint8_t set_auto_mode;
    uint8_t mode;

} S_MCU_CHC;

typedef struct {
    uint8_t bUpdate : 1;
    uint8_t bConnected : 1;
    uint8_t bSet_alarm : 1;
    uint8_t reverse : 5;

    uint8_t alarm_left;
    uint8_t alarm_right;
    uint8_t alarm_light;

    uint8_t set_alarm_distance;
    uint8_t set_alarm_light_hz;

} S_RRU_CHC;

typedef struct {
    uint8_t bUpdate : 1;
    uint8_t bConnected : 1;
    uint8_t bHr_connected : 1;
    uint8_t bSet_sport_level : 1;
    uint8_t reverse : 4;

    uint8_t hr_connected;
    uint8_t hr_value;
    uint8_t sport_level;
    uint8_t mode;
    uint8_t hr_warning;

    uint8_t set_sport_level;

} S_HMI_CHC;

typedef struct {
    uint8_t bUpdate : 1;
    uint8_t bConnected : 1;
    uint8_t bNet_connected : 1;
    uint8_t bGps_connected : 1;

    uint8_t bMqtt_connected : 1;
    uint8_t bModule_connected : 1;
    uint8_t reverse : 2;

    float longitude;
    float latitude;
    uint16_t altitude;
    uint16_t speed;
} S_NU_CHC;
typedef struct {
    uint8_t bUpdate : 1;
    uint8_t bConnected : 1;
    uint8_t bSet_cws : 1;
    uint8_t reverse : 5;

    uint8_t cws_connected;
    uint8_t cws_distance;
    uint8_t cws_range;

    uint8_t set_cws_distance;
    uint8_t set_cws_range;
} S_CWS_CHC;
typedef struct {
    S_MCU_CHC mcu;
    S_RRU_CHC rru;
    S_HMI_CHC hmi;
    S_NU_CHC nu;
    S_CWS_CHC cws;
} S_PARAMS_CONTENTS_CHC;

typedef union {
    S_PARAMS_CONTENTS_CHC contents;
    uint8_t bytes[sizeof(S_PARAMS_CONTENTS_CHC)];
} U_PARAMS_CHC;
extern U_PARAMS_CHC uParams_chc;

#endif