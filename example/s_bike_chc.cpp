#include "codeHw.h"
#include "codeSw.h"

// #define node_HMI

#include <chc_protocol.h>

#include "s_bike_chc.h"
#include "s_bike_chc_params.h"

bool bBikeConnected = false;
long lBikeConnectedTime = 0;

bool bike_getConnected_chc(void)
{
    return bBikeConnected;
}
// ------------------------------------------------------------------------------------------------
bool bike_init_chc(void)
{
    bBikeConnected = false;
    lBikeConnectedTime = millis();

    if (!CAN_base_init(pinMcu_Canbus_Rx, pinMcu_Canbus_Tx)) {
        BIKE_LOG_E("CANbus init fail");
        return false;
    }
    BIKE_LOG_I("CANbus init");
    return true;
}

// ------------------------------------------------------------------------------------------------
bool bike_loop_chc(void)
{
    // S_BIKE_STATUS_CHC slBikeStatus_chc;
    static uint8_t u8lTransmitFailCount = 0;
    CHC_PROTOCOL::REQ_type req;
    req = chcProtocol.rx();
    // 判斷目前收到的位址有哪些
    switch (req) {
    case chcProtocol.NONE:
        break;
    case chcProtocol.GET_HMI:
        // sBikeData_chc.hmi_getUpdate = true;
        // slBikeStatus_chc.hmi_getUpdate = true;
        uParams_chc.contents.hmi.bUpdate = true;
        break;
    case chcProtocol.GET_MCU:
        // sBikeData_chc.mcu_getUpdate = true;
        // slBikeStatus_chc.mcu_getUpdate = true;
        uParams_chc.contents.mcu.bUpdate = true;
        break;
    case chcProtocol.GET_RRU:
        // sBikeData_chc.rru_getUpdate = true;
        // slBikeStatus_chc.rru_getUpdate = true;
        uParams_chc.contents.rru.bUpdate = true;
        break;
    case chcProtocol.GET_CWS:
        // sBikeData_chc.cws_getUpdate = true;
        // slBikeStatus_chc.cws_getUpdate = true;
        uParams_chc.contents.cws.bUpdate = true;
        break;
    case chcProtocol.GET_NU:
        // sBikeData_chc.nu_getUpdate = true;
        // slBikeStatus_chc.nu_getUpdate = true;
        uParams_chc.contents.nu.bUpdate = true;
        break;
    case chcProtocol.PROCESS_DONE:
        // 更新資料
        // BIKE_LOG_I("process done");
        return false;
        break;
    case chcProtocol.SET_NU_AWAKE:
        break;
    case chcProtocol.SET_NU_SLEEP:
        break;
    default:

        break;
    }
    // 判斷是否有收到資料
    /*
        if (sBikeData_chc.hmi_getUpdate == true
            || sBikeData_chc.mcu_getUpdate == true
            || sBikeData_chc.rru_getUpdate == true
            || sBikeData_chc.cws_getUpdate == true
            || sBikeData_chc.nu_getUpdate == true) {
     */
    /*
        if (slBikeStatus_chc.hmi_getUpdate == true
            || slBikeStatus_chc.mcu_getUpdate == true
            || slBikeStatus_chc.rru_getUpdate == true
            || slBikeStatus_chc.cws_getUpdate == true
            || slBikeStatus_chc.nu_getUpdate == true) {
     */
    if (uParams_chc.contents.hmi.bUpdate == true
        || uParams_chc.contents.mcu.bUpdate == true
        || uParams_chc.contents.rru.bUpdate == true
        || uParams_chc.contents.cws.bUpdate == true
        || uParams_chc.contents.nu.bUpdate == true) {
        bBikeConnected = true;

        {
            // ? receive message from MCU
            // ? 檢查裝置是否連線
            static long lMcuConnectTime = millis();
            // if (sBikeData_chc.mcu_getUpdate == true) {
            // if (slBikeStatus_chc.mcu_getUpdate == true) {
            if (uParams_chc.contents.mcu.bUpdate == true) {
                // sBikeData_chc.mcu_getUpdate = false;
                // slBikeStatus_chc.mcu_getUpdate = false;
                uParams_chc.contents.mcu.bUpdate = false;

                BIKE_LOG_I("get mcu data");
                // uParams_chc.contents.mcu.connected = true;
                uParams_chc.contents.mcu.bConnected = true;
                uParams_chc.contents.mcu.assist = chcProtocol.sData.mcu.assist;
                uParams_chc.contents.mcu.speed = chcProtocol.sData.mcu.speed;
                uParams_chc.contents.mcu.battery = chcProtocol.sData.mcu.battery;
                uParams_chc.contents.mcu.torque = chcProtocol.sData.mcu.torque;
                uParams_chc.contents.mcu.cadence = chcProtocol.sData.mcu.cadence;
            } else {
                if (millis() - lMcuConnectTime > 3000) {
                    lMcuConnectTime = millis();
                    // uParams_chc.contents.mcu.connected = false;
                    uParams_chc.contents.mcu.bConnected = false;
                }
            }
        }
        {
            static long lHmiConnectTime = millis();
            // ? 接受 HMI 資料
            // if (sBikeData_chc.hmi_getUpdate == true) {
            // if (slBikeStatus_chc.hmi_getUpdate == true) {
            if (uParams_chc.contents.hmi.bUpdate == true) {
                // sBikeData_chc.hmi_getUpdate = false;
                // slBikeStatus_chc.hmi_getUpdate = false;
                uParams_chc.contents.hmi.bUpdate = false;

                uParams_chc.contents.hmi.bConnected = true;
                BIKE_LOG_I("get hmi data");
                uParams_chc.contents.hmi.hr_connected = chcProtocol.sData.hmi.hr_status;
                uParams_chc.contents.hmi.hr_value = chcProtocol.sData.hmi.hr_value;
                BIKE_LOG_I("hr_value %d", uParams_chc.contents.hmi.hr_value);
                uParams_chc.contents.hmi.sport_level = chcProtocol.sData.hmi.sport_level;
                uParams_chc.contents.hmi.mode = chcProtocol.sData.hmi.mode;
                uParams_chc.contents.hmi.hr_warning = chcProtocol.sData.hmi.hr_warning;

                uParams_chc.contents.mcu.set_assist_value = chcProtocol.sData.hmi.assist;
                // uParams_chc.contents.rru.set_alarm_distance = chcProtocol.sData.hmi.assist;

                uParams_chc.contents.rru.set_alarm_distance = chcProtocol.sData.rru.set_detect_range;
                uParams_chc.contents.rru.set_alarm_light_hz = chcProtocol.sData.rru.set_bling_hz;
                lHmiConnectTime = millis();
            } else {
                if (millis() - lHmiConnectTime > 3000) {
                    lHmiConnectTime = millis();
                    uParams_chc.contents.hmi.bConnected = false;
                }
            }
        }
        {
            // ? 接受 IOT 資料
            // if (sBikeData_chc.nu_getUpdate == true) {
            // if (slBikeStatus_chc.nu_getUpdate == true) {
            if (uParams_chc.contents.nu.bUpdate == true) {
                // sBikeData_chc.nu_getUpdate = false;
                // slBikeStatus_chc.nu_getUpdate = false;
                uParams_chc.contents.nu.bUpdate = false;
                BIKE_LOG_I("get nu data");
                uParams_chc.contents.nu.longitude = chcProtocol.sData.nu.longitude;
                uParams_chc.contents.nu.latitude = chcProtocol.sData.nu.latitude;
                uParams_chc.contents.nu.altitude = chcProtocol.sData.nu.altitude;
                uParams_chc.contents.nu.speed = chcProtocol.sData.nu.speed;
                if ((chcProtocol.sData.nu.status & 0b00000001) != 0) {
                    uParams_chc.contents.nu.bModule_connected = true;
                } else {
                    uParams_chc.contents.nu.bModule_connected = false;
                }
                if ((chcProtocol.sData.nu.status & 0b00000010) != 0) {
                    uParams_chc.contents.nu.bNet_connected = true;
                } else {
                    uParams_chc.contents.nu.bNet_connected = false;
                }
                if ((chcProtocol.sData.nu.status & 0b00000100) != 0) {
                    uParams_chc.contents.nu.bMqtt_connected = true;
                } else {
                    uParams_chc.contents.nu.bMqtt_connected = false;
                }
                if ((chcProtocol.sData.nu.status & 0b00001000) != 0) {
                    uParams_chc.contents.nu.bGps_connected = true;
                } else {
                    uParams_chc.contents.nu.bGps_connected = false;
                }
            }
        }
        {
            static long lRruConnectTime = millis();
            // ? 接受 RRU 資料
            // if (sBikeData_chc.rru_getUpdate == true) {
            // if (slBikeStatus_chc.rru_getUpdate == true) {
            if (uParams_chc.contents.rru.bUpdate == true) {
                // sBikeData_chc.rru_getUpdate = false;
                // slBikeStatus_chc.rru_getUpdate = false;
                uParams_chc.contents.rru.bUpdate = false;
                BIKE_LOG_I("get rru data");
                // uParams_chc.contents.rru.connected = true;
                uParams_chc.contents.rru.bConnected = true;
                uParams_chc.contents.rru.alarm_left = chcProtocol.sData.rru.status_alarm_L;
                uParams_chc.contents.rru.alarm_right = chcProtocol.sData.rru.status_alarm_R;
            } else {
                if (millis() - lRruConnectTime > 3000) {
                    lRruConnectTime = millis();
                    // uParams_chc.contents.rru.connected = false;
                    uParams_chc.contents.rru.bConnected = false;
                }
            }
        }
        // ? 接受 CWS 資料
        // if (sBikeData_chc.cws_getUpdate == true) {
        // if (slBikeStatus_chc.cws_getUpdate == true) {
        if (uParams_chc.contents.cws.bUpdate == true) {
            // sBikeData_chc.cws_getUpdate = false;
            // slBikeStatus_chc.cws_getUpdate = false;
            uParams_chc.contents.cws.bUpdate = false;
            BIKE_LOG_I("get cws data");
        }
    } else {
        bBikeConnected = false;
    }
    // ? 偵測5次發送失敗時，重新初始化CANbus
    if (u8lTransmitFailCount >= 5) {
        BIKE_LOG_E("CANbus transmit fail");
        u8lTransmitFailCount = 0;
        bike_init_chc();
    }
#ifdef node_MCU
    {
        // ? 每隔100ms時間發送狀態
        // ? 發送 Assist / Speed / Battery / torque / cadence
        static long lfMCUUpdateTime = millis();
        if (millis() > (lfMCUUpdateTime + 100)) {
            lfMCUUpdateTime = millis();
            chcProtocol.sData.mcu.assist = uParams_chc.contents.mcu.assist;
            chcProtocol.sData.mcu.speed = (uint16_t)uParams_chc.contents.mcu.speed * 10;
            chcProtocol.sData.mcu.battery = uParams_chc.contents.mcu.battery;
            chcProtocol.sData.mcu.torque = (uint16_t)uParams_chc.contents.mcu.torque * 10;
            chcProtocol.sData.mcu.cadence = uParams_chc.contents.mcu.cadence;
            BIKE_LOG_I("send mcu data");
            if (!chcProtocol.MCU_period(
                    chcProtocol.sData.mcu.assist,
                    chcProtocol.sData.mcu.torque,
                    chcProtocol.sData.mcu.cadence,
                    chcProtocol.sData.mcu.speed,
                    chcProtocol.sData.mcu.battery)) {
                u8lTransmitFailCount++;
            }
            /*
                        chcProtocol.MCU_period(
                            uParams_chc.contents.mcu.assist,
                            uParams_chc.contents.mcu.torque,
                            uParams_chc.contents.mcu.cadence,
                            uParams_chc.contents.mcu.speed,
                            uParams_chc.contents.mcu.battery);
                 */
            if (uParams_chc.contents.mcu.bSet_power_status == 1) {
                chcProtocol.MCU_setPower(1);
            }
        }
    }

#endif
#ifdef node_NU
    {
        // 每1秒 發送GPS相關資訊 精度、緯度、海拔、速度
        static long lfGPSUpdateTime = 0;
        if (millis() - lfGPSUpdateTime > 1000) {
            lfGPSUpdateTime = millis();
            // send_GPS();
            BIKE_LOG_I("send .. ");
            chcProtocol.sData.nu.longitude = uParams_chc.contents.nu.longitude;
            chcProtocol.sData.nu.latitude = uParams_chc.contents.nu.latitude;
            chcProtocol.sData.nu.altitude = uParams_chc.contents.nu.altitude;
            chcProtocol.sData.nu.speed = uParams_chc.contents.nu.speed;
            chcProtocol.sData.nu.status = 0x00;
            if (uParams_chc.contents.nu.bModule_connected == true) {
                BIKE_LOG_I("module connected");
                chcProtocol.sData.nu.status |= 0b00000001;
            }
            if (uParams_chc.contents.nu.bNet_connected == true) {
                chcProtocol.sData.nu.status |= 0b00000010;
            }
            if (uParams_chc.contents.nu.bMqtt_connected == true) {
                chcProtocol.sData.nu.status |= 0b00000100;
            }
            if (uParams_chc.contents.nu.bGps_connected == true) {
                chcProtocol.sData.nu.status |= 0b00001000;
            }

            if (!chcProtocol.NU_period1(
                    chcProtocol.sData.nu.longitude,
                    chcProtocol.sData.nu.latitude)) {
                u8lTransmitFailCount++;
            }
            if (!chcProtocol.NU_period2(
                    chcProtocol.sData.nu.altitude,
                    chcProtocol.sData.nu.speed,
                    chcProtocol.sData.nu.status)) {
                u8lTransmitFailCount++;
            }
            /*
                        chcProtocol.NU_period1(
                            chcProtocol.sData.nu.longitude,
                            chcProtocol.sData.nu.latitude);
                        chcProtocol.NU_period2(
                            chcProtocol.sData.nu.altitude,
                            chcProtocol.sData.nu.speed,
                            chcProtocol.sData.nu.status);
             */
            /*
                        chcProtocol.NU_period1(
                            sBikeData_chc.nu_longitude,
                            sBikeData_chc.nu_latitude);

                        chcProtocol.NU_period2(
                            sBikeData_chc.nu_altitude,
                            sBikeData_chc.nu_speed,
                            sBikeData_chc.nu_status);
             */
        }
    }
#endif
#ifdef node_HMI
    {
        // ? 每1秒 發送心跳
        static long lfHMIUpdateTime = 0;
        if (millis() - lfHMIUpdateTime >= 1000) {
            lfHMIUpdateTime = millis(); //
            if (uParams_chc.contents.hmi.bHr_connected == true) {
                uParams_chc.contents.hmi.hr_connected = 1;
            } else {
                uParams_chc.contents.hmi.hr_connected = 0;
            }
            chcProtocol.sData.hmi.hr_status = uParams_chc.contents.hmi.hr_connected;
            chcProtocol.sData.hmi.hr_value = uParams_chc.contents.hmi.hr_value;
            chcProtocol.sData.hmi.sport_level = uParams_chc.contents.hmi.sport_level;
            chcProtocol.sData.hmi.mode = uParams_chc.contents.hmi.mode;
            chcProtocol.sData.hmi.hr_warning = uParams_chc.contents.hmi.hr_warning;
            if (!chcProtocol.HMI_period(
                    chcProtocol.sData.hmi.hr_status,
                    chcProtocol.sData.hmi.hr_value,
                    chcProtocol.sData.hmi.sport_level,
                    chcProtocol.sData.hmi.mode,
                    chcProtocol.sData.hmi.hr_warning)) {
                u8lTransmitFailCount++;
            }
        }

        // ? 確認是否更新輔助力度，未變更時每秒發送一次
        static long lfHMIUpdateTime_assist = 0;
        // if (sBikeData_chc.hmi_set_bike == true) {
        // if (slBikeStatus_chc.hmi_set_bike == true) {
        if (uParams_chc.contents.mcu.bSet_assist == true) {
            // BIKE_LOG_I("set assist %d", sBikeData_chc.hmi_set_bike_assist);
            // BIKE_LOG_I("set assist %d", slBikeStatus_chc.hmi_set_bike_assist);
            BIKE_LOG_I("set assist %d", uParams_chc.contents.mcu.set_assist_value);
            // if (!chcProtocol.MCU_setAssist(sBikeData_chc.hmi_set_bike_assist)) {
            // if (!chcProtocol.MCU_setAssist(slBikeStatus_chc.hmi_set_bike_assist)) {
            if (!chcProtocol.MCU_setAssist(uParams_chc.contents.mcu.set_assist_value)) {
                u8lTransmitFailCount++;
            }
            // sBikeData_chc.hmi_set_bike = false;
            // slBikeStatus_chc.hmi_set_bike = false;
            uParams_chc.contents.mcu.bSet_assist = false;
        } else {
            lfHMIUpdateTime_assist = millis();
        }

        // ? 確認是否更新設定後方雷達，未變更時每秒發送一次
        static long lfHMIUpdateTime_rru = 0;
        // if (sBikeData_chc.hmi_set_rru == true) {
        // if (slBikeStatus_chc.hmi_set_rru == true) {
        if (uParams_chc.contents.rru.bSet_alarm == true) {
            if (millis() - lfHMIUpdateTime_rru >= 1000)
                lfHMIUpdateTime_rru = millis();
            /*             if (!chcProtocol.RRU_setParam(
                                sBikeData_chc.hmi_set_rru_distance,
                                sBikeData_chc.hmi_set_rru_light_hz)) {
                            u8lTransmitFailCount++;
                        }
            */
            if (!chcProtocol.RRU_setParam(
                    uParams_chc.contents.rru.set_alarm_distance,
                    uParams_chc.contents.rru.set_alarm_light_hz)) {
                u8lTransmitFailCount++;
            }
        } else {
            lfHMIUpdateTime_rru = millis();
        }

        // ? 確認是否更新前方坑洞偵測，未變更時每秒發送一次
        static long lfHMIUpdateTime_cws = 0;
        // if (sBikeData_chc.hmi_set_cws == true) {
        // if (slBikeStatus_chc.hmi_set_cws == true) {
        if (uParams_chc.contents.cws.bSet_cws == true) {
            if (millis() - lfHMIUpdateTime_cws >= 1000)
                lfHMIUpdateTime_cws = millis();
            // if (!chcProtocol.CWS_setParam(
            // sBikeData_chc.hmi_set_cws_distance,
            // sBikeData_chc.hmi_set_cws_range)) {
            if (!chcProtocol.CWS_setParam(
                    uParams_chc.contents.cws.set_cws_distance,
                    uParams_chc.contents.cws.set_cws_range)) {
                u8lTransmitFailCount++;
            }
        } else {
            lfHMIUpdateTime_cws = millis();
        }
    }
#endif
    return true;
}
