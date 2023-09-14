#include "chc_protocol_hyena.h"
#include "CAN_base.h"

CAN_frame_t rx_frame;
CAN_frame_t tx_frame;
CHC_PROTOCOL_HYENA chcProtocol_hyena;

// long lCanbusTimeOut = 0;
// bool bCanbusTimeOutCheck = false;

// #define vCanbusTimeOut 1000
// CAN_device_t CAN_cfg; // CAN Config

// unsigned long previousMillis = 0; // will store last time a CAN Message was send
// const int interval = 1000; // interval at which send CAN Messages (milliseconds)
// const int rx_queue_size = 1024; //// 10; // Receive Queue siz
// ! 凱納 ID ..
typedef enum {
    CANID_INFO = 0x123,
    CANID_NO = 0x124,
    // CANID_IOT = 0x125,
} E_CANID;

typedef enum {
    OPC,
    PL1,
    PL2,
    PL3,
    PL4,
    PL5,
    PL6,
    PL7
} E_ITEM;

// ! CANBUS 控制器 ........................................................
// typedef struct {
//     uint8_t ucUniqueId[8];
//     int8_t cRpm;
//     int16_t iTorque;
//     int8_t cAssistLevel;
//     int8_t cTempDriveFet;
//     int16_t iSpeed;
//     int32_t lOdo;
//     int8_t cBatCapacity;
//     int8_t cBatHealth;
//     int32_t lCurrent;
//     int32_t lVoltage;
//     int8_t cError[7];
// } S_DATA;

// S_DATA sData;

// ---------------------------------------------
typedef struct {
    uint8_t OPC;
    uint8_t RPM;
    uint8_t TorqueL;
    uint8_t AssistLevel : 4;
    uint8_t TorqueH : 4;
    uint8_t TempDriveFET;
    uint8_t SpeedL;
    uint8_t SpeedH : 2;
    uint8_t Reserved : 6;
} S_FRAME_DRV;

typedef struct {
    uint8_t OPC;
    uint8_t ODOL;
    uint8_t ODOM;
    uint8_t ODOH;
} S_FRAME_ODO;

typedef struct {
    uint8_t OPC;
    uint8_t Capacity;
    uint8_t Health;
} S_FRAME_BAT;

typedef struct {
    uint8_t OPC;
    uint8_t CurLL;
    uint8_t CurLH;
    uint8_t CurHL;
    uint8_t CurHH;
} S_FRAME_CUR;

typedef struct {
    uint8_t OPC;
    uint8_t VolLL;
    uint8_t VolLH;
    uint8_t VolHL;
    uint8_t VolHH;
} S_FRAME_VOL;

typedef struct {
    uint8_t OPC;
    uint8_t Error[7];
} S_FRAME_ERR;
// * --------------------------------------------------
typedef struct {
    uint8_t ID[8];
} S_FRAME_ID;

// * --------------------------------------------------
typedef enum {
    OPC_DRV = 0xF0,
    OPC_ODO = 0xF1,
    OPC_BAT = 0xF2,
    OPC_CUR = 0xF3,
    OPC_VOL = 0xF4,
    OPC_ERR = 0xF5,
    OPC_CFG = 0xF6,
} E_FRAME_OPC_;
// ! ------------------------------------------------------
typedef union {
    uint8_t ucInfo[8];
    S_FRAME_ID sUniqueId;
    S_FRAME_DRV sDrv;
    S_FRAME_BAT sBat;
    S_FRAME_CUR sCur;
    S_FRAME_VOL sVol;
    S_FRAME_ERR sErr;
    S_FRAME_ODO sOdo;
} U_FRAME;
// ! ------------------------------------------------------
// ! CANBUS 存放暫存器 .........

// ! ....................................................................
// ! CANBUS 控制器 ........................................................
// * Info from bike..
// unsigned char* CHC_PROTOCOL_HYENA::getUniqueId(void)
// {
//     return sData.ucUniqueId;
// }

// // ? --------------------------------------------------------
// char CHC_PROTOCOL_HYENA::getRpm(void)
// {
//     return sData.cRpm;
// }

// // ? --------------------------------------------------------
// int CHC_PROTOCOL_HYENA::getTorque(void)
// {
//     return sData.iTorque;
// }

// // ? --------------------------------------------------------
// char CHC_PROTOCOL_HYENA::getAssistLevel(void)
// {
//     return sData.cAssistLevel;
// }

// // ? --------------------------------------------------------
// char CHC_PROTOCOL_HYENA::getTempDriveFet(void)
// {
//     return sData.cTempDriveFet;
// }

// // ? --------------------------------------------------------
// int CHC_PROTOCOL_HYENA::getSpeed(void)
// {
//     return sData.iSpeed;
// }

// // ? --------------------------------------------------------
// long CHC_PROTOCOL_HYENA::getOdo(void)
// {
//     return sData.lOdo;
// }

// // ? --------------------------------------------------------
// char CHC_PROTOCOL_HYENA::getBatCapacity(void)
// {
//     return sData.cBatCapacity;
// }

// // ? --------------------------------------------------------
// char CHC_PROTOCOL_HYENA::getBatHealth(void)
// {
//     return sData.cBatHealth;
// }

// // ? --------------------------------------------------------
// long CHC_PROTOCOL_HYENA::getCurrent(void)
// {
//     return sData.lCurrent;
// }

// // ? --------------------------------------------------------
// long CHC_PROTOCOL_HYENA::getVoltage(void)
// {
//     return sData.lVoltage;
// }

// // ? --------------------------------------------------------
// char CHC_PROTOCOL_HYENA::getError(unsigned char cIndex)
// {
//     return sData.cError[cIndex];
// }

// ! ....................................................................
// ! CANBUS 主程式 ...............................................................
CHC_PROTOCOL_HYENA::CHC_PROTOCOL_HYENA()
{
    // 建構子
}
// ? 初始化 ---
bool CHC_PROTOCOL_HYENA::init(int rx_pin, int tx_pin, long baudrate)
{
    if (!CAN_base_init(rx_pin, tx_pin, baudrate)) {
        BIKE_HYENA_LOG_E("CANbus init fail");
        return false;
    }
    BIKE_HYENA_LOG_I("CANbus init");
    return true;
}
// ? 接收程式 --
CHC_PROTOCOL_HYENA::REQ_type CHC_PROTOCOL_HYENA::rx(void)
{
    U_FRAME* ulFrame;
    if (CAN_base_receive(&rx_frame, 500) == false) {
        return NONE;
    }
    ulFrame = (U_FRAME*)rx_frame.data;

    switch (rx_frame.identifier) {
    case CANID_INFO: {
        switch (ulFrame->ucInfo[OPC]) {
        // * ----------------------------------
        // * 讀取控制器：內容接收 .......
        case OPC_DRV: {
            sData.mcu.cadence = (int8_t)ulFrame->sDrv.RPM;
            sData.mcu.torque = ((int16_t)ulFrame->sDrv.TorqueH << 8) | (int16_t)ulFrame->sDrv.TorqueL;
            sData.mcu.assist = (int8_t)ulFrame->sDrv.AssistLevel;
            sData.mcu.tempDriveFet = (int8_t)ulFrame->sDrv.TempDriveFET;
            sData.mcu.speed = ((int16_t)ulFrame->sDrv.SpeedH << 8) | (int16_t)ulFrame->sDrv.SpeedL;

            BIKE_HYENA_LOG_I("RPM: %d\tTorque:%d,Assist:%d,Temp:%d,Speed:%d ",
                sData.mcu.cadence,
                sData.mcu.torque,
                sData.mcu.assist,
                sData.mcu.tempDriveFet,
                sData.mcu.speed);
        } break;
        // * ----------------------------------
        // * 讀取控制器：里程數
        case OPC_ODO: {
            sData.mcu.odo = ((int32_t)ulFrame->sOdo.ODOH << 16)
                | ((int32_t)ulFrame->sOdo.ODOM << 8)
                | (int32_t)ulFrame->sOdo.ODOL;

            BIKE_HYENA_LOG_I("ODO: %d", sData.mcu.odo);
        } break;
        // * ----------------------------------
        // * 讀取控制器：電池狀態
        case OPC_BAT: {
            sData.mcu.battery = ulFrame->sBat.Capacity;
            sData.mcu.batteryHealth = ulFrame->sBat.Health;

            BIKE_HYENA_LOG_I("BAT Capacity: %d\tHealth:%d",
                sData.mcu.battery,
                sData.mcu.batteryHealth);
        } break;
        // * ----------------------------------
        // * 讀取控制器：電流狀態
        case OPC_CUR: {
            sData.mcu.current = ((int32_t)ulFrame->sCur.CurHH << 24)
                | ((int32_t)ulFrame->sCur.CurHL << 16)
                | ((int32_t)ulFrame->sCur.CurLH << 8)
                | ((int32_t)ulFrame->sCur.CurLL);

            BIKE_HYENA_LOG_I("Current: %d", sData.mcu.current);
        } break;
        // * ----------------------------------
        // * 讀取控制器：電壓狀態
        case OPC_VOL: {
            sData.mcu.voltage = ((int32_t)ulFrame->sVol.VolHH << 24)
                | ((int32_t)ulFrame->sVol.VolHL << 16)
                | ((int32_t)ulFrame->sVol.VolLH << 8)
                | ((int32_t)ulFrame->sVol.VolLL);

            BIKE_HYENA_LOG_I("Voltage: %d", sData.mcu.voltage);
        } break;
        // * ----------------------------------
        // * 讀取控制器：
        case OPC_CFG: {
        } break;
        // * ----------------------------------
        // * 讀取控制器：錯誤資訊
        case OPC_ERR: {
            BIKE_HYENA_LOG("Error:");
            for (int i = 0; i < 7; i++) {
                sData.mcu.error[i] = ulFrame->sErr.Error[i];
                BIKE_HYENA_LOG_S("%d ", sData.mcu.error[i]);
            }
            BIKE_HYENA_LOG_S("\n");
        } break;
        // * ----------------------------------
        // *
        default: {
        } break;
        }
    } break;
    // * ----------------------------------
    // * 讀取控制器：ID 讀取
    case CANID_NO: {
        BIKE_HYENA_LOG("Bike ID = ");
        for (int i = 0; i < 8; i++) {
            sData.mcu.uniqueId[i] = ulFrame->sUniqueId.ID[i];
            BIKE_HYENA_LOG_S("0x%02X ", sData.mcu.uniqueId[i]);
        }
        BIKE_HYENA_LOG_S("\n");
    } break;

    // * ----------------------------------
    // *
    default: {
    } break;
    }
}
