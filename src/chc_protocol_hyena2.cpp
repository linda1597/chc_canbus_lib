
#include "CAN_base.h"

#include "chc_protocol_hyena2.h"

// esp_err_t err;
// String err_s;
// twai_message_t rx_msg;
// twai_message_t message;
CAN_frame_t rx_msg;
CAN_frame_t tx_msg;
CHC_PROTOCOL_HYENA2 chcProtocol_hyena2;
#ifdef node_NU
// CHC_PROTOCOL_HYENA2::U_BIKE_INFO u_bike_info;
// CHC_PROTOCOL_HYENA2::S_DEVICE_CONNECTED sDeviceConnected[CHC_PROTOCOL_HYENA2::_ED_COUNT];
#endif
CHC_PROTOCOL_HYENA2::CHC_PROTOCOL_HYENA2()
{
    s_bike_parameters.u_modid.ID = 0x397FB1100F02A10A;
    bfirstConsecutive = true;

    // 建構子
}
CHC_PROTOCOL_HYENA2::REQ_type CHC_PROTOCOL_HYENA2::rx()
{
    // ----------------- 以下為檢查連線狀態 -----------------
    for (int i = 0; i < CHC_PROTOCOL_HYENA2::_EC_COUNT; i++) {
        if ((millis() - sDeviceConnected[i].lLastTime) > 3000) {
            u_bike[i].components.sBasic.u8Connected = 0x00;
            sDeviceConnected[i].lLastTime = millis();
        }
    }
    // ----------------- 以下為接收資料 -----------------
    if (CAN_base_receive(&rx_msg, 70) == false) {
        return NONE;
    }
    CHC_PL_LOG("RX:");
#ifdef CAN_lib_2
    CHC_PL_LOG_S("ID: 0x%08X, DLC: %d, Data: ", rx_msg.MsgID, rx_msg.FIR.B.DLC);
    for (uint8_t i = 0; i < rx_msg.FIR.B.DLC; i++) {
        CHC_PL_LOG_S("%02X ", rx_msg.data.u8[i]);
    }
#else
    for (uint8_t i = 0; i < rx_msg.data_length_code; i++) {
        CHC_PL_LOG_S("%02X ", rx_msg.data[i]);
    }
#endif
    CHC_PL_LOG_S("\n");
    // ----------------- 以下為檢查連線狀態 -----------------
    switch (rx_msg.MsgID) {
    case CHC_PROTOCOL_HYENA2::ToolControl00: // 0x200
        break;
        //
    case CHC_PROTOCOL_HYENA2::eLockInfo: // 0x633

        u_bike[EC_E_LOCK].components.sBasic.u8Connected = 0x01;
        sDeviceConnected[EC_E_LOCK].lLastTime = millis();
        break;
        //
    case CHC_PROTOCOL_HYENA2::BikeControl00: // 0x300
    case CHC_PROTOCOL_HYENA2::HMIModuleIDBroadcasting: // 0x14001
    case CHC_PROTOCOL_HYENA2::HMIModuleIDBroadcastingRequest: // 0x14001
    case CHC_PROTOCOL_HYENA2::HMIErrorInfo: // 0x329
        u_bike[EC_HMI].components.sBasic.u8Connected = 0x01;
        sDeviceConnected[EC_HMI].lLastTime = millis();
        break;
        //
    case CHC_PROTOCOL_HYENA2::ControllerModuleIDBroadcasting: // 0x13000
    case CHC_PROTOCOL_HYENA2::ControllerModuleIDBroadcastingRequest: // 0x13001
    case CHC_PROTOCOL_HYENA2::ControllerInfo01: // 0x201
    case CHC_PROTOCOL_HYENA2::ControllerInfo02: // 0x202
    case CHC_PROTOCOL_HYENA2::ControllerInfo03: // 0x203
    case CHC_PROTOCOL_HYENA2::ControllerErrorInfo: // 0x209
        u_bike[EC_CONTROLLER].components.sBasic.u8Connected = 0x01;
        sDeviceConnected[EC_CONTROLLER].lLastTime = millis();
        break;

        //
    case CHC_PROTOCOL_HYENA2::DerailleurModuleIDBroadcasting: // 0x1F000
    case CHC_PROTOCOL_HYENA2::DerailleurModuleIDBroadcastingRequest: // 0x1F000
    case CHC_PROTOCOL_HYENA2::DerailleurState: // 0x650
        u_bike[EC_DERAILLEUR].components.sBasic.u8Connected = 0x01;
        sDeviceConnected[EC_DERAILLEUR].lLastTime = millis();
        break;
        //
    case CHC_PROTOCOL_HYENA2::DropperModuleIDBroadcasting: // 0x1B000
    case CHC_PROTOCOL_HYENA2::DropperModuleIDBroadcastingRequest: // 0x1B000
    case CHC_PROTOCOL_HYENA2::DropperErrorInfo: // 0x559
        u_bike[EC_DROPPER].components.sBasic.u8Connected = 0x01;
        sDeviceConnected[EC_DROPPER].lLastTime = millis();
        break;
        //
    case CHC_PROTOCOL_HYENA2::ForkModuleIDBroadcasting: // 0x13500
    case CHC_PROTOCOL_HYENA2::ForkModuleIDBroadcastingRequest: // 0x13500
    case CHC_PROTOCOL_HYENA2::ForkInfo01: // 0x751
    case CHC_PROTOCOL_HYENA2::ForkErrorInfo: // 0x759
        u_bike[EC_FORK].components.sBasic.u8Connected = 0x01;
        sDeviceConnected[EC_FORK].lLastTime = millis();
        break;
        //
    case CHC_PROTOCOL_HYENA2::RadarUpdateFW: // = 0x190,
    case CHC_PROTOCOL_HYENA2::RadarUpdateFWResponse: //  = 0x191,
    case CHC_PROTOCOL_HYENA2::RadarModuleIDBroadcast: //  = 0x19000,
    case CHC_PROTOCOL_HYENA2::RadarModuleIDBroadcastRequest: //  = 0x19001,
    case CHC_PROTOCOL_HYENA2::RadarInfo01: //  = 0x701,
    case CHC_PROTOCOL_HYENA2::RadarErrorInfo: //  = 0x709,
    case CHC_PROTOCOL_HYENA2::RadarParameterRead: //  = 0x70000,
    case CHC_PROTOCOL_HYENA2::RadarParameterReadResponse: //  = 0x70001,
    case CHC_PROTOCOL_HYENA2::RadarParameterWrite: //  = 0x70002,
    case CHC_PROTOCOL_HYENA2::RadarParameterWriteResponse: //  = 0x70003        break;
        u_bike[EC_RADAR].components.sBasic.u8Connected = 0x01;
        sDeviceConnected[EC_RADAR].lLastTime = millis();
        break;
        //
    case CHC_PROTOCOL_HYENA2::Battery1ModuleIDBroadcasting: // 0x11000
    case CHC_PROTOCOL_HYENA2::Battery1ModuleIDBroadcastingRequest: // 0x11000
    case CHC_PROTOCOL_HYENA2::Battery1Info01: // 0x401
    case CHC_PROTOCOL_HYENA2::Battery1Info02: // 0x402
    case CHC_PROTOCOL_HYENA2::Battery1Info06: // 0x406
        u_bike[EC_BATTERY].components.sBasic.u8Connected = 0x01;
        sDeviceConnected[EC_BATTERY].lLastTime = millis();
        break;
        //
    case CHC_PROTOCOL_HYENA2::Battery2ModuleIDBroadcasting: // 0x11000
    case CHC_PROTOCOL_HYENA2::Battery2ModuleIDBroadcastingRequest: // 0x11000
    case CHC_PROTOCOL_HYENA2::Battery2Info01: // 0x451
    case CHC_PROTOCOL_HYENA2::Battery2Info02: // 0x452
    case CHC_PROTOCOL_HYENA2::Battery2Info06: // 0x456
        u_bike[EC_BATTERY2].components.sBasic.u8Connected = 0x01;
        sDeviceConnected[EC_BATTERY2].lLastTime = millis();
        break;

    default:
        break;
    }
    // ----------------- 以下為接收到的資料 -----------------
#ifdef CAN_lib_2
    switch (rx_msg.MsgID) {
#else
    switch (rx_msg.identifier) {
#endif
        // HMI ID --------------------------------
#ifdef rx_HMIModuleIDBroadcasting
    case CHC_PROTOCOL_HYENA2::HMIModuleIDBroadcasting: // 0x14000
        for (uint8_t i = 0; i < 8; i++) {
#ifdef CAN_lib_2
            //     u_bike_info.components.sHmi.bytes[i] = rx_msg.data.u8[i];
            u_bike[EC_HMI].components.sHmi.uInfo.bytes[i] = rx_msg.data.u8[i];
#else
            //     u_bike_info.contents.u_hmi_info.bytes[i] = rx_msg.data[i];
            u_bike[EC_HMI].components.sHmi.uInfo.bytes[i] = rx_msg.data[i];
#endif
        }
        break;
#endif
#ifdef rx_HMIErrorInfo
    case CHC_PROTOCOL_HYENA2::HMIErrorInfo: // 0x329
#ifdef CAN_lib_2
                                            // u_bike_info.components.sHmi.contents.errorCode = rx_msg.data.u8[0] | rx_msg.data.u8[1] << 8;
        u_bike[EC_HMI].components.sHmi.u16ErrorCode = (uint16_t)rx_msg.data.u8[0] | rx_msg.data.u8[1] << 8;
#else
                                            // u_bike_info.contents.u_hmi_info.contents.errorCode = rx_msg.data[0] | rx_msg.data[1] << 8;
        u_bike[EC_HMI].components.sHmi.u16ErrorCode = (uint16_t)rx_msg.data[0] | rx_msg.data[1] << 8;
#endif
        break;
#endif
// ----------------------------------------------------------------
// Derailleur ID --------------------------------
#ifdef rx_derailleurModuleIDBroadcasting
    case CHC_PROTOCOL_HYENA2::DerailleurModuleIDBroadcasting: // 0x1F000
        for (uint8_t i = 0; i < 8; i++) {
#ifdef CAN_lib_2
            u_bike[EC_DERAILLEUR].components.sDerailleur.uInfo.bytes[i] = rx_msg.data.u8[i];
#else
            //     u_bike_info.contents.s_derailleur_info.u_derailleur_basic_info.bytes[i] = rx_msg.data[i];
            u_bike[EC_DERAILLEUR].components.sDerailleur.uInfo.bytes[i] = rx_msg.data[i];
#endif
        }
        break;
#endif
#ifdef rx_derailleurState
    case CHC_PROTOCOL_HYENA2::DerailleurState: // 0x650
#ifdef CAN_lib_2
        u_bike[EC_DERAILLEUR].components.sDerailleur.gearIndex = rx_msg.data.u8[0];
        // u_bike_info.components.sDerailleur.uInfo.contents.errorCode = rx_msg.data.u8[1];
        u_bike[EC_DERAILLEUR].components.sDerailleur.u16ErrorCode = (uint16_t)rx_msg.data.u8[0] | rx_msg.data.u8[1] << 8;
#else
                                               // u_bike_info.contents.s_derailleur_info.gearIndex = rx_msg.data[0];
        // u_bike_info.contents.s_derailleur_info.u_derailleur_basic_info.contents.errorCode = rx_msg.data[1];

        u_bike[EC_DERAILLEUR].components.sDerailleur.gearIndex = rx_msg.data[0];
        u_bike[EC_DERAILLEUR].components.sDerailleur.u16ErrorCode = (uint16_t)rx_msg.data[0] | rx_msg.data[1] << 8;
#endif
        break;
#endif
// ----------------------------------------------------------------
// Fork ID ---------------------------------------------------
#ifdef rx_forkModuleIDBroadcasting
    case CHC_PROTOCOL_HYENA2::ForkModuleIDBroadcasting: // 0x13500
        for (uint8_t i = 0; i < 8; i++) {
#ifdef CAN_lib_2
            u_bike[EC_FORK].components.sFork.uInfo.bytes[i] = rx_msg.data.u8[i];
#else
            //     u_bike_info.contents.s_fork_info.u_fork_basic_info.bytes[i] = rx_msg.data[i];
            u_bike[EC_FORK].components.sFork.uInfo.bytes[i] = rx_msg.data[i];
#endif
        }
        break;
#endif
#ifdef rx_forkInfo01
    case CHC_PROTOCOL_HYENA2::ForkInfo01: // 0x751
#ifdef CAN_lib_2
        u_bike[EC_FORK].components.sFork.suspensionLevel = rx_msg.data.u8[2];
#else
                                          // u_bike_info.contents.s_fork_info.suspensionLevel = rx_msg.data[0];
        u_bike[EC_FORK].components.sFork.suspensionLevel = rx_msg.data[2];
#endif
        break;
#endif
#ifdef rx_forkErrorInfo
    case CHC_PROTOCOL_HYENA2::ForkErrorInfo: // 0x759
#ifdef CAN_lib_2
                                             // u_bike_info.components.sFork.uInfo.contents.errorCode = rx_msg.data.u8[0] | rx_msg.data.u8[1] << 8;
        u_bike[EC_FORK].components.sFork.u16ErrorCode = (uint16_t)rx_msg.data.u8[0] | rx_msg.data.u8[1] << 8;
#else
                                             // u_bike_info.contents.s_fork_info.u_fork_basic_info.contents.errorCode = rx_msg.data[0] | rx_msg.data[1] << 8;
        u_bike[EC_FORK].components.sFork.u16ErrorCode = (uint16_t)rx_msg.data[0] | rx_msg.data[1] << 8;
#endif
        break;
#endif
// ----------------------------------------------------------------
// Controller ID --------------------------------
#ifdef rx_controllerModuleIDBroadcasting
    case CHC_PROTOCOL_HYENA2::ControllerModuleIDBroadcasting: // 0x13000
        for (uint8_t i = 0; i < 8; i++) {

#ifdef CAN_lib_2
            u_bike[EC_CONTROLLER].components.sController.uInfo.bytes[i] = rx_msg.data.u8[i];
#else
            //     u_bike_info.contents.s_controller_info.u_controller_basic_info.bytes[i] = rx_msg.data[i];
            u_bike[EC_CONTROLLER].components.sController.uInfo.bytes[i] = rx_msg.data[i];
#endif
        }
        break;
#endif
#ifdef rx_controllerInfo01
    case CHC_PROTOCOL_HYENA2::ControllerInfo01: // 0x201
#ifdef CAN_lib_2
        u_bike[EC_CONTROLLER].components.sController.bikeSpeed = (uint16_t)rx_msg.data.u8[0] | rx_msg.data.u8[1] << 8;
        u_bike[EC_CONTROLLER].components.sController.torque = (uint16_t)rx_msg.data.u8[2] | rx_msg.data.u8[3] << 8;
        u_bike[EC_CONTROLLER].components.sController.lightStatus = (rx_msg.data.u8[4] >> 1) & 0x01;
        u_bike[EC_CONTROLLER].components.sController.assistLevel = (rx_msg.data.u8[4] >> 7) & 0x01;
#else
        u_bike[EC_CONTROLLER].components.sController.bikeSpeed = (uint16_t)rx_msg.data[0] | rx_msg.data[1] << 8;
        u_bike[EC_CONTROLLER].components.sController.torque = (uint16_t)rx_msg.data[2] | rx_msg.data[3] << 8;
        u_bike[EC_CONTROLLER].components.sController.lightStatus = (rx_msg.data[4] >> 1) & 0x01;
        u_bike[EC_CONTROLLER].components.sController.assistLevel = (rx_msg.data[4] >> 7) & 0x01;
/*
        u_bike_info.contents.s_controller_info.bikeSpeed = rx_msg.data[0] | rx_msg.data[1] << 8;
        u_bike_info.contents.s_controller_info.torque = rx_msg.data[2] | rx_msg.data[3] << 8;
        u_bike_info.contents.s_controller_info.lightStatus = (rx_msg.data[4] >> 1) & 0x01;
        u_bike_info.contents.s_controller_info.assistLevel = (rx_msg.data[4] >> 7) & 0x01;
 */
#endif
        break;
#endif
#ifdef rx_controllerInfo02
    case CHC_PROTOCOL_HYENA2::ControllerInfo02: // 0x202
#ifdef CAN_lib_2
        u_bike[EC_CONTROLLER].components.sController.odo = (uint32_t)rx_msg.data.u8[4] | rx_msg.data.u8[5] << 8 | rx_msg.data.u8[6] << 16 | rx_msg.data.u8[7] << 24;
#else
                                                // u_bike_info.contents.s_controller_info.odo = rx_msg.data[4] | rx_msg.data[5] << 8 | rx_msg.data[6] << 16 | rx_msg.data[7] << 24;
        u_bike[EC_CONTROLLER].components.sController.odo = (uint32_t)rx_msg.data[4] | rx_msg.data[5] << 8 | rx_msg.data[6] << 16 | rx_msg.data[7] << 24;
#endif
        break;
#endif
#ifdef rx_controllerInfo03
    case CHC_PROTOCOL_HYENA2::ControllerInfo03: // 0x203
#ifdef CAN_lib_2
        u_bike[EC_CONTROLLER].components.sController.cadence = (uint16_t)rx_msg.data.u8[0] | rx_msg.data.u8[1] << 8;
        u_bike[EC_CONTROLLER].components.sController.estimatedRange = (uint16_t)rx_msg.data.u8[6] | (rx_msg.data.u8[7] & 0x0F) << 8;
#else
                                                // u_bike_info.contents.s_controller_info.cadence = rx_msg.data[0] | rx_msg.data[1] << 8;
        // u_bike_info.contents.s_controller_info.estimatedRange = rx_msg.data[6] | (rx_msg.data[7] & 0x0F) << 8;
        u_bike[EC_CONTROLLER].components.sController.cadence = (uint16_t)rx_msg.data[0] | rx_msg.data[1] << 8;
        u_bike[EC_CONTROLLER].components.sController.estimatedRange = (uint16_t)rx_msg.data[6] | (rx_msg.data[7] & 0x0F) << 8;

#endif
        break;
#endif
#ifdef rx_controllerErrorInfo
    case CHC_PROTOCOL_HYENA2::ControllerErrorInfo: // 0x209
#ifdef CAN_lib_2
                                                   // u_bike_info.components.sController.uInfo.contents.errorCode = rx_msg.data.u8[0] | rx_msg.data.u8[1] << 8;
        u_bike[EC_CONTROLLER].components.sController.u16ErrorCode = (uint16_t)rx_msg.data.u8[0] | rx_msg.data.u8[1] << 8;
#else
                                                   // u_bike_info.contents.s_controller_info.u_controller_basic_info.contents.errorCode = rx_msg.data[0] | rx_msg.data[1] << 8;
        u_bike[EC_CONTROLLER].components.sController.u16ErrorCode = (uint16_t)rx_msg.data[0] | rx_msg.data[1] << 8;
#endif
        break;
#endif
// ----------------------------------------------------------------
// eLock ID --------------------------------
#ifdef rx_elockInfo
    case CHC_PROTOCOL_HYENA2::eLockInfo: // 0x633
#ifdef CAN_lib_2
                                         // u_bike_info.components.sELock.elockStatus = rx_msg.data.u8[2];
        u_bike[EC_E_LOCK].components.sELock.u8Status = rx_msg.data.u8[2];
#else
                                         // u_bike_info.contents.s_elock_info.elockStatus = rx_msg.data[2];
        u_bike[EC_E_LOCK].components.sELock.u8Status = rx_msg.data[2];
#endif
        break;
#endif
// ----------------------------------------------------------------
// Dropper ID --------------------------------
#ifdef rx_dropperModuleIDBroadcasting
    case CHC_PROTOCOL_HYENA2::DropperModuleIDBroadcasting: // 0x1B000
        for (uint8_t i = 0; i < 8; i++) {
#ifdef CAN_lib_2
            //     u_bike_info.components.sDropper.bytes[i] = rx_msg.data.u8[i];
            u_bike[EC_DROPPER].components.sDropper.uInfo.bytes[i] = rx_msg.data.u8[i];
#else
            //     u_bike_info.contents.u_dropper_info.bytes[i] = rx_msg.data[i];
            u_bike[EC_DROPPER].components.sDropper.uInfo.bytes[i] = rx_msg.data[i];
#endif
        }
        break;
#endif
#ifdef rx_dropperErrorInfo
    case CHC_PROTOCOL_HYENA2::DropperErrorInfo: // 0x559
#ifdef CAN_lib_2
                                                // u_bike_info.components.sDropper.contents.errorCode = rx_msg.data.u8[0] | rx_msg.data.u8[1] << 8;
        u_bike[EC_DROPPER].components.sDropper.u16ErrorCode = (uint16_t)rx_msg.data.u8[0] | rx_msg.data.u8[1] << 8;
#else
                                                // u_bike_info.contents.u_dropper_info.contents.errorCode = rx_msg.data[0] | rx_msg.data[1] << 8;
        u_bike[EC_DROPPER].components.sDropper.u16ErrorCode = (uint16_t)rx_msg.data[0] | rx_msg.data[1] << 8;
#endif
        break;
#endif
// ----------------------------------------------------------------
// Battery ID --------------------------------
#ifdef rx_battery1ModuleIDBroadcasting
    case CHC_PROTOCOL_HYENA2::Battery1ModuleIDBroadcasting: // 0x11000
        for (uint8_t i = 0; i < 8; i++) {
#ifdef CAN_lib_2
            u_bike[EC_BATTERY].components.sBattery1.uInfo.bytes[i] = rx_msg.data.u8[i];
#else
            //     u_bike_info.contents.s_battery1_info.u_battery_basic_info.bytes[i] = rx_msg.data[i];
            u_bike[EC_BATTERY].components.sBattery1.uInfo.bytes[i] = rx_msg.data[i];
#endif
        }
        break;
#endif
#ifdef rx_battery1Info01
    case CHC_PROTOCOL_HYENA2::Battery1Info01: // 0x401
#ifdef CAN_lib_2
        u_bike[EC_BATTERY].components.sBattery1.u32Voltage = (uint32_t)rx_msg.data.u8[0] | (uint32_t)rx_msg.data.u8[1] << 8 | (uint32_t)rx_msg.data.u8[2] << 16 | (uint32_t)rx_msg.data.u8[3] << 24;
        u_bike[EC_BATTERY].components.sBattery1.i32Current = (int32_t)rx_msg.data.u8[4] | (int32_t)rx_msg.data.u8[5] << 8 | (int32_t)rx_msg.data.u8[6] << 16 | (int32_t)rx_msg.data.u8[7] << 24;
#else
        u_bike[EC_BATTERY].components.sBattery1.voltage = (uint32_t)rx_msg.data[0] | (uint32_t)rx_msg.data[1] << 8 | (uint32_t)rx_msg.data[2] << 16 | (uint32_t)rx_msg.data[3] << 24;
        u_bike[EC_BATTERY].components.sBattery1.current = (int32_t)rx_msg.data[4] | (int32_t)rx_msg.data[5] << 8 | (int32_t)rx_msg.data[6] << 16 | (int32_t)rx_msg.data[7] << 24;

        // u_bike_info.contents.s_battery1_info.voltage = (uint32_t)rx_msg.data[0] | (uint32_t)rx_msg.data[1] << 8 | (uint32_t)rx_msg.data[2] << 16 | (uint32_t)rx_msg.data[3] << 24;
        // u_bike_info.contents.s_battery1_info.current = (int32_t)rx_msg.data[4] | (int32_t)rx_msg.data[5] << 8 | (int32_t)rx_msg.data[6] << 16 | (int32_t)rx_msg.data[7] << 24;
#endif
        break;
#endif
#ifdef rx_battery1Info02
    case CHC_PROTOCOL_HYENA2::Battery1Info02: // 0x402
#ifdef CAN_lib_2
        u_bike[EC_BATTERY].components.sBattery1.batteryPercent = (uint32_t)rx_msg.data.u8[0];
#else
                                              // u_bike_info.contents.s_battery1_info.batteryPercent = (uint32_t)rx_msg.data[0];
        u_bike[EC_BATTERY].components.sBattery1.batteryPercent = (uint32_t)rx_msg.data[0];
#endif
        break;
#endif
#ifdef rx_battery1Info06
    case CHC_PROTOCOL_HYENA2::Battery1Info06: // 0x406
#ifdef CAN_lib_2
        u_bike[EC_BATTERY].components.sBattery1.temperature1 = (int8_t)rx_msg.data.u8[0];
        u_bike[EC_BATTERY].components.sBattery1.temperature2 = (int8_t)rx_msg.data.u8[1];
        u_bike[EC_BATTERY].components.sBattery1.temperature3 = (int8_t)rx_msg.data.u8[2];
        u_bike[EC_BATTERY].components.sBattery1.temperature4 = (int8_t)rx_msg.data.u8[3];
        u_bike[EC_BATTERY].components.sBattery1.temperature5 = (int8_t)rx_msg.data.u8[4];
        u_bike[EC_BATTERY].components.sBattery1.temperature6 = (int8_t)rx_msg.data.u8[5];
        u_bike[EC_BATTERY].components.sBattery1.temperature7 = (int8_t)rx_msg.data.u8[6];
        u_bike[EC_BATTERY].components.sBattery1.temperature8 = (int8_t)rx_msg.data.u8[7];
#else
        u_bike[EC_BATTERY].components.sBattery1.temperature1 = (int8_t)rx_msg.data[0];
        u_bike[EC_BATTERY].components.sBattery1.temperature2 = (int8_t)rx_msg.data[1];
        u_bike[EC_BATTERY].components.sBattery1.temperature3 = (int8_t)rx_msg.data[2];
        u_bike[EC_BATTERY].components.sBattery1.temperature4 = (int8_t)rx_msg.data[3];
        u_bike[EC_BATTERY].components.sBattery1.temperature5 = (int8_t)rx_msg.data[4];
        u_bike[EC_BATTERY].components.sBattery1.temperature6 = (int8_t)rx_msg.data[5];
        u_bike[EC_BATTERY].components.sBattery1.temperature7 = (int8_t)rx_msg.data[6];
        u_bike[EC_BATTERY].components.sBattery1.temperature8 = (int8_t)rx_msg.data[7];
/*
        u_bike_info.contents.s_battery1_info.temperature1 = (int8_t)rx_msg.data[0];
        u_bike_info.contents.s_battery1_info.temperature2 = (int8_t)rx_msg.data[1];
        u_bike_info.contents.s_battery1_info.temperature3 = (int8_t)rx_msg.data[2];
        u_bike_info.contents.s_battery1_info.temperature4 = (int8_t)rx_msg.data[3];
        u_bike_info.contents.s_battery1_info.temperature5 = (int8_t)rx_msg.data[4];
        u_bike_info.contents.s_battery1_info.temperature6 = (int8_t)rx_msg.data[5];
        u_bike_info.contents.s_battery1_info.temperature7 = (int8_t)rx_msg.data[6];
        u_bike_info.contents.s_battery1_info.temperature8 = (int8_t)rx_msg.data[7];
 */
#endif
        break;
#endif
#ifdef rx_battery2ModuleIDBroadcasting
    case CHC_PROTOCOL_HYENA2::Battery2ModuleIDBroadcasting: // 0x11000
        for (uint8_t i = 0; i < 8; i++) {
#ifdef CAN_lib_2
            //     u_bike_info.components.s_battery2_info.uInfo.bytes[i] = rx_msg.data.u8[i];
            u_bike[EC_BATTERY2].components.sBattery2.uInfo.bytes[i] = rx_msg.data.u8[i];
#else
            //     u_bike_info.contents.s_battery2_info.u_battery_basic_info.bytes[i] = rx_msg.data[i];
            u_bike[EC_BATTERY2].components.sBattery2.uInfo.bytes[i] = rx_msg.data[i];
#endif
        }
        break;
#endif
#ifdef rx_battery2Info01
    case CHC_PROTOCOL_HYENA2::Battery2Info01: // 0x451
#ifdef CAN_lib_2
                                              // u_bike_info.components.s_battery2_info.voltage = (uint32_t)rx_msg.data.u8[0] | (uint32_t)rx_msg.data.u8[1] << 8 | (uint32_t)rx_msg.data.u8[2] << 16 | (uint32_t)rx_msg.data.u8[3] << 24;
        u_bike[EC_BATTERY2].components.sBattery2.u32Voltage = (uint32_t)rx_msg.data.u8[0] | (uint32_t)rx_msg.data.u8[1] << 8 | (uint32_t)rx_msg.data.u8[2] << 16 | (uint32_t)rx_msg.data.u8[3] << 24;

        // u_bike_info.components.s_battery2_info.current = (int32_t)rx_msg.data.u8[4] | (int32_t)rx_msg.data.u8[5] << 8 | (int32_t)rx_msg.data.u8[6] << 16 | (int32_t)rx_msg.data.u8[7] << 24;
        u_bike[EC_BATTERY2].components.sBattery2.i32Current = (int32_t)rx_msg.data.u8[4] | (int32_t)rx_msg.data.u8[5] << 8 | (int32_t)rx_msg.data.u8[6] << 16 | (int32_t)rx_msg.data.u8[7] << 24;
#else
        u_bike.components.sBattery2.voltage = (uint32_t)rx_msg.data[0] | (uint32_t)rx_msg.data[1] << 8 | (uint32_t)rx_msg.data[2] << 16 | (uint32_t)rx_msg.data[3] << 24;
        u_bike.components.sBattery2.current = (int32_t)rx_msg.data8[4] | (int32_t)rx_msg.data[5] << 8 | (int32_t)rx_msg.data[6] << 16 | (int32_t)rx_msg.data[7] << 24;
        // u_bike_info.contents.s_battery2_info.voltage = (uint32_t)rx_msg.data[0] | (uint32_t)rx_msg.data[1] << 8 | (uint32_t)rx_msg.data[2] << 16 | (uint32_t)rx_msg.data[3] << 24;
        // u_bike_info.contents.s_battery2_info.current = (int32_t)rx_msg.data[4] | (int32_t)rx_msg.data[5] << 8 | (int32_t)rx_msg.data[6] << 16 | (int32_t)rx_msg.data[7] << 24;
#endif
        break;
#endif
#ifdef rx_battery2Info02
    case CHC_PROTOCOL_HYENA2::Battery2Info02: // 0x452
#ifdef CAN_lib_2
                                              // u_bike_info.components.s_battery2_info.batteryPercent = (uint32_t)rx_msg.data.u8[0];
        u_bike[EC_BATTERY2].components.sBattery2.batteryPercent = (uint32_t)rx_msg.data.u8[0];
#else
                                              // u_bike_info.contents.s_battery2_info.batteryPercent = (uint32_t)rx_msg.data[0];
        u_bike[EC_BATTERY2].components.sBattery2.batteryPercent = (uint32_t)rx_msg.data[0];
#endif
        break;
#endif
#ifdef rx_battery2Info06
    case CHC_PROTOCOL_HYENA2::Battery2Info06: // 0x456

#ifdef CAN_lib_2
        u_bike[EC_BATTERY2].components.sBattery2.temperature1 = (int8_t)rx_msg.data.u8[0];
        u_bike[EC_BATTERY2].components.sBattery2.temperature2 = (int8_t)rx_msg.data.u8[1];
        u_bike[EC_BATTERY2].components.sBattery2.temperature3 = (int8_t)rx_msg.data.u8[2];
        u_bike[EC_BATTERY2].components.sBattery2.temperature4 = (int8_t)rx_msg.data.u8[3];
        u_bike[EC_BATTERY2].components.sBattery2.temperature5 = (int8_t)rx_msg.data.u8[4];
        u_bike[EC_BATTERY2].components.sBattery2.temperature6 = (int8_t)rx_msg.data.u8[5];
        u_bike[EC_BATTERY2].components.sBattery2.temperature7 = (int8_t)rx_msg.data.u8[6];
        u_bike[EC_BATTERY2].components.sBattery2.temperature8 = (int8_t)rx_msg.data.u8[7];
/*
        u_bike_info.components.s_battery2_info.temperature1 = (int8_t)rx_msg.data.u8[0];
        u_bike_info.components.s_battery2_info.temperature2 = (int8_t)rx_msg.data.u8[1];
        u_bike_info.components.s_battery2_info.temperature3 = (int8_t)rx_msg.data.u8[2];
        u_bike_info.components.s_battery2_info.temperature4 = (int8_t)rx_msg.data.u8[3];
        u_bike_info.components.s_battery2_info.temperature5 = (int8_t)rx_msg.data.u8[4];
        u_bike_info.components.s_battery2_info.temperature6 = (int8_t)rx_msg.data.u8[5];
        u_bike_info.components.s_battery2_info.temperature7 = (int8_t)rx_msg.data.u8[6];
        u_bike_info.components.s_battery2_info.temperature8 = (int8_t)rx_msg.data.u8[7];
 */
#else
        u_bike[EC_BATTERY2].components.sBattery2.temperature1 = (int8_t)rx_msg.data[0];
        u_bike[EC_BATTERY2].components.sBattery2.temperature2 = (int8_t)rx_msg.data[1];
        u_bike[EC_BATTERY2].components.sBattery2.temperature3 = (int8_t)rx_msg.data[2];
        u_bike[EC_BATTERY2].components.sBattery2.temperature4 = (int8_t)rx_msg.data[3];
        u_bike[EC_BATTERY2].components.sBattery2.temperature5 = (int8_t)rx_msg.data[4];
        u_bike[EC_BATTERY2].components.sBattery2.temperature6 = (int8_t)rx_msg.data[5];
        u_bike[EC_BATTERY2].components.sBattery2.temperature7 = (int8_t)rx_msg.data[6];
        u_bike[EC_BATTERY2].components.sBattery2.temperature8 = (int8_t)rx_msg.data[7];
/*
        u_bike_info.contents.s_battery2_info.temperature1 = (int8_t)rx_msg.data[0];
        u_bike_info.contents.s_battery2_info.temperature2 = (int8_t)rx_msg.data[1];
        u_bike_info.contents.s_battery2_info.temperature3 = (int8_t)rx_msg.data[2];
        u_bike_info.contents.s_battery2_info.temperature4 = (int8_t)rx_msg.data[3];
        u_bike_info.contents.s_battery2_info.temperature5 = (int8_t)rx_msg.data[4];
        u_bike_info.contents.s_battery2_info.temperature6 = (int8_t)rx_msg.data[5];
        u_bike_info.contents.s_battery2_info.temperature7 = (int8_t)rx_msg.data[6];
        u_bike_info.contents.s_battery2_info.temperature8 = (int8_t)rx_msg.data[7];
 */
#endif
        break;
#endif
        // ----------------------------------------------------------------

#ifdef rx_update_rru_FW
    case CHC_PROTOCOL_HYENA2::RadarUpdateFW:
        return RRU_UPDATE;
        break;
#endif
#ifdef rx_read_rru_modID
    case CHC_PROTOCOL_HYENA2::RequestRadarModuleIDBroadcast:
        RadarModIDBroadcast();
        return REQ_RRU_ID;
        break;
#endif
#ifdef rx_tool_control
    case CHC_PROTOCOL_HYENA2::ToolControl00:
#ifdef CAN_lib_2
        this->operatingTime = rx_msg.data.u8[0];
        this->SME = rx_msg.data.u8[1];
#else
        this->operatingTime = rx_msg.data[0];
        this->SME = rx_msg.data[1];
#endif
        return GET_TOOL_CTRL;
        break;
#endif
#ifdef rx_read_rru_params
    case CHC_PROTOCOL_HYENA2::ReadRadarParameter:

        break;
#endif
#ifdef rx_write_rru_params
    case CHC_PROTOCOL_HYENA2::WriteRadarParameter:
        break;
#endif
// RRU ID
// ----------------------------------------------------------------
#ifdef rx_radarErrorInfo
    case CHC_PROTOCOL_HYENA2::RadarErrorInfo: // = 0x709,
// sData.dtc.RRU = rx_msg.data[0];
#ifdef CAN_lib_2
                                              // u_bike_info.components.sRadar.contents.errorCode = rx_msg.data.u8[0] | (rx_msg.data.u8[1] << 8);
        u_bike[EC_RADAR].components.sRadar.u16ErrorCode = (uint16_t)rx_msg.data.u8[0] | (rx_msg.data.u8[1] << 8);
#else
                                              // u_bike_info.contents.u_radar_info.contents.errorCode = rx_msg.data[0] | (rx_msg.data[1] << 8);
        u_bike[EC_RADAR].components.sRadar.u16ErrorCode = (uint16_t)rx_msg.data[0] | (rx_msg.data[1] << 8);
#endif
        // return PROCESS_DONE;
        // return GET_RRU;
        break;
#endif

#ifdef rx_radarModuleIDBroadcasting
    case CHC_PROTOCOL_HYENA2::RadarModuleIDBroadcast:
        for (uint8_t i = 0; i < 8; i++) {
#ifdef CAN_lib_2
            u_bike[EC_RADAR].components.sRadar.uInfo.bytes[i] = rx_msg.data.u8[i];
#else
            u_bike[EC_RADAR].components.sRadar.uInfo.bytes[0] = rx_msg.data[0];
#endif
        }
        break;
#endif
        // ----------------------------------------------------------------
#ifdef rx_chargerModuleIDBroadcasting
    case CHC_PROTOCOL_HYENA2::ChargerModuleIDBroadcasting: // 0x12000
        for (uint8_t i = 0; i < 8; i++) {
#ifdef CAN_lib_2
            u_bike[EC_CHARGER].components.sCharger.uInfo.bytes[i] = rx_msg.data.u8[i];
#else
            u_bike[EC_CHARGER].components.sCharger.uInfo.bytes[i] = rx_msg.data[i];
#endif
        }
        break;
#endif

#ifdef rx_chargerErrorInfo
    case CHC_PROTOCOL_HYENA2::ChargerErrorInfo: // 0x809
#ifdef CAN_lib_2
        u_bike[EC_CHARGER].components.sCharger.u16ErrorCode = (uint16_t)rx_msg.data.u8[0] | (rx_msg.data.u8[1] << 8);
#else
        u_bike[EC_CHARGER].components.sCharger.u16ErrorCode = (uint16_t)rx_msg.data[0] | (rx_msg.data[1] << 8);
#endif
        break;
#endif

    default:
        return PROCESS_DONE;
        break;
    }
    return PROCESS_DONE;
}

#ifdef node_RRU
// 診斷碼
bool CHC_PROTOCOL_HYENA2::radarErrorInfo(uint8_t ErrPage, uint8_t ErrCode)
{
    tx_msg.identifier = RadarErrorInfo;
    tx_msg.extd = 0;
    tx_msg.rtr = 0;
    tx_msg.data_length_code = 2;
    tx_msg.data[0] = ErrPage;
    tx_msg.data[1] = ErrCode;
    return CAN_base_transmit(&tx_msg);
}

// 傳送偵測物體ID、距離、速度、角度、警戒狀態
bool CHC_PROTOCOL_HYENA2::radarInfo01(
    uint8_t id,
    uint16_t speed,
    uint16_t distance,
    uint8_t degree,
    uint8_t status)
{
    tx_msg.identifier = RadarInfo01;
    tx_msg.extd = 0;
    tx_msg.rtr = 0;
    tx_msg.data_length_code = 7;
    tx_msg.data[0] = id;
    tx_msg.data[1] = speed;
    tx_msg.data[2] = speed >> 8;
    tx_msg.data[3] = distance;
    tx_msg.data[4] = distance >> 8;
    tx_msg.data[5] = degree;
    tx_msg.data[6] = status;
    return CAN_base_transmit(&tx_msg);
}
/*ISO TP部分施工中*/
bool CHC_PROTOCOL_HYENA2::rxReadRadarParamSingleFrame()
{
    if (rx_msg.data[0] == 0x06 && rx_msg.data[1] == ParameterRead) {
        startAddress = (uint32_t)rx_msg.data[2] | (uint32_t)rx_msg.data[3] << 8 | (uint32_t)rx_msg.data[4] << 16;
        paramlength = (uint16_t)rx_msg.data[5] | (uint16_t)rx_msg.data[6] << 8;
        return true;
    } else {
        return false;
    }
}

bool CHC_PROTOCOL_HYENA2::rxReadRadarParamFlowControlFrame()
{
    if (rx_msg.data[0] == 0x30 && rx_msg.data[1] == 0x00 && rx_msg.data[2] == 0x00) {
        return true;
    } else {
        return false;
    }
}

bool CHC_PROTOCOL_HYENA2::txReadRadarParamConsecutiveFrame()
{
    tx_msg.identifier = ReadRadarParameterResponse;
    tx_msg.extd = 1;
    tx_msg.rtr = 0;
    if (bfirstConsecutive == true) {
        if (paramlength > 2) {
            paramlength = paramlength - 2;
        }
        bfirstConsecutive = false;
    }
}

bool CHC_PROTOCOL_HYENA2::RadarStartRead()
{
    tx_msg.identifier = ReadRadarParameterResponse;
    tx_msg.extd = 1;
    tx_msg.rtr = 0;
    tx_msg.data_length_code = 8;
    tx_msg.data[0] = 0x10;
    tx_msg.data[1] = 11 + paramlength;
    tx_msg.data[2] = 0x10;
    tx_msg.data[3] = 0x00;
    tx_msg.data[4] = (uint8_t)startAddress;
    tx_msg.data[5] = uint8_t(startAddress >> 8);
    tx_msg.data[6] = uint8_t(startAddress >> 16);
    tx_msg.data[7] = (uint8_t)paramlength;
    return CAN_base_transmit(&tx_msg);
}

bool CHC_PROTOCOL_HYENA2::RRU_FWupdateRP(
    uint8_t dlc,
    uint8_t* payload)
{
    tx_msg.identifier = RadarUpdateFWResponse;
    tx_msg.extd = 0;
    tx_msg.rtr = 0;
    tx_msg.data_length_code = dlc;
    for (uint8_t i = 0; i < dlc; i++) {
        tx_msg.data[i] = *(payload + i);
    }
    return CAN_base_transmit(&tx_msg);
}

bool CHC_PROTOCOL_HYENA2::RadarModIDBroadcast()
{
    tx_msg.identifier = RadarModuleIDBroadcast;
    tx_msg.extd = 1;
    tx_msg.rtr = 0;
    tx_msg.data_length_code = 8;
    for (uint8_t i = 0; i < 8; i++) {
        tx_msg.data[i] = s_bike_parameters.u_modid.bytes[i];
    }
    return CAN_base_transmit(&tx_msg);
}
#endif

static const unsigned int crc32_table[] = {
    0x00000000, 0x04c11db7, 0x09823b6e, 0x0d4326d9,
    0x130476dc, 0x17c56b6b, 0x1a864db2, 0x1e475005,
    0x2608edb8, 0x22c9f00f, 0x2f8ad6d6, 0x2b4bcb61,
    0x350c9b64, 0x31cd86d3, 0x3c8ea00a, 0x384fbdbd,
    0x4c11db70, 0x48d0c6c7, 0x4593e01e, 0x4152fda9,
    0x5f15adac, 0x5bd4b01b, 0x569796c2, 0x52568b75,
    0x6a1936c8, 0x6ed82b7f, 0x639b0da6, 0x675a1011,
    0x791d4014, 0x7ddc5da3, 0x709f7b7a, 0x745e66cd,
    0x9823b6e0, 0x9ce2ab57, 0x91a18d8e, 0x95609039,
    0x8b27c03c, 0x8fe6dd8b, 0x82a5fb52, 0x8664e6e5,
    0xbe2b5b58, 0xbaea46ef, 0xb7a96036, 0xb3687d81,
    0xad2f2d84, 0xa9ee3033, 0xa4ad16ea, 0xa06c0b5d,
    0xd4326d90, 0xd0f37027, 0xddb056fe, 0xd9714b49,
    0xc7361b4c, 0xc3f706fb, 0xceb42022, 0xca753d95,
    0xf23a8028, 0xf6fb9d9f, 0xfbb8bb46, 0xff79a6f1,
    0xe13ef6f4, 0xe5ffeb43, 0xe8bccd9a, 0xec7dd02d,
    0x34867077, 0x30476dc0, 0x3d044b19, 0x39c556ae,
    0x278206ab, 0x23431b1c, 0x2e003dc5, 0x2ac12072,
    0x128e9dcf, 0x164f8078, 0x1b0ca6a1, 0x1fcdbb16,
    0x018aeb13, 0x054bf6a4, 0x0808d07d, 0x0cc9cdca,
    0x7897ab07, 0x7c56b6b0, 0x71159069, 0x75d48dde,
    0x6b93dddb, 0x6f52c06c, 0x6211e6b5, 0x66d0fb02,
    0x5e9f46bf, 0x5a5e5b08, 0x571d7dd1, 0x53dc6066,
    0x4d9b3063, 0x495a2dd4, 0x44190b0d, 0x40d816ba,
    0xaca5c697, 0xa864db20, 0xa527fdf9, 0xa1e6e04e,
    0xbfa1b04b, 0xbb60adfc, 0xb6238b25, 0xb2e29692,
    0x8aad2b2f, 0x8e6c3698, 0x832f1041, 0x87ee0df6,
    0x99a95df3, 0x9d684044, 0x902b669d, 0x94ea7b2a,
    0xe0b41de7, 0xe4750050, 0xe9362689, 0xedf73b3e,
    0xf3b06b3b, 0xf771768c, 0xfa325055, 0xfef34de2,
    0xc6bcf05f, 0xc27dede8, 0xcf3ecb31, 0xcbffd686,
    0xd5b88683, 0xd1799b34, 0xdc3abded, 0xd8fba05a,
    0x690ce0ee, 0x6dcdfd59, 0x608edb80, 0x644fc637,
    0x7a089632, 0x7ec98b85, 0x738aad5c, 0x774bb0eb,
    0x4f040d56, 0x4bc510e1, 0x46863638, 0x42472b8f,
    0x5c007b8a, 0x58c1663d, 0x558240e4, 0x51435d53,
    0x251d3b9e, 0x21dc2629, 0x2c9f00f0, 0x285e1d47,
    0x36194d42, 0x32d850f5, 0x3f9b762c, 0x3b5a6b9b,
    0x0315d626, 0x07d4cb91, 0x0a97ed48, 0x0e56f0ff,
    0x1011a0fa, 0x14d0bd4d, 0x19939b94, 0x1d528623,
    0xf12f560e, 0xf5ee4bb9, 0xf8ad6d60, 0xfc6c70d7,
    0xe22b20d2, 0xe6ea3d65, 0xeba91bbc, 0xef68060b,
    0xd727bbb6, 0xd3e6a601, 0xdea580d8, 0xda649d6f,
    0xc423cd6a, 0xc0e2d0dd, 0xcda1f604, 0xc960ebb3,
    0xbd3e8d7e, 0xb9ff90c9, 0xb4bcb610, 0xb07daba7,
    0xae3afba2, 0xaafbe615, 0xa7b8c0cc, 0xa379dd7b,
    0x9b3660c6, 0x9ff77d71, 0x92b45ba8, 0x9675461f,
    0x8832161a, 0x8cf30bad, 0x81b02d74, 0x857130c3,
    0x5d8a9099, 0x594b8d2e, 0x5408abf7, 0x50c9b640,
    0x4e8ee645, 0x4a4ffbf2, 0x470cdd2b, 0x43cdc09c,
    0x7b827d21, 0x7f436096, 0x7200464f, 0x76c15bf8,
    0x68860bfd, 0x6c47164a, 0x61043093, 0x65c52d24,
    0x119b4be9, 0x155a565e, 0x18197087, 0x1cd86d30,
    0x029f3d35, 0x065e2082, 0x0b1d065b, 0x0fdc1bec,
    0x3793a651, 0x3352bbe6, 0x3e119d3f, 0x3ad08088,
    0x2497d08d, 0x2056cd3a, 0x2d15ebe3, 0x29d4f654,
    0xc5a92679, 0xc1683bce, 0xcc2b1d17, 0xc8ea00a0,
    0xd6ad50a5, 0xd26c4d12, 0xdf2f6bcb, 0xdbee767c,
    0xe3a1cbc1, 0xe760d676, 0xea23f0af, 0xeee2ed18,
    0xf0a5bd1d, 0xf464a0aa, 0xf9278673, 0xfde69bc4,
    0x89b8fd09, 0x8d79e0be, 0x803ac667, 0x84fbdbd0,
    0x9abc8bd5, 0x9e7d9662, 0x933eb0bb, 0x97ffad0c,
    0xafb010b1, 0xab710d06, 0xa6322bdf, 0xa2f33668,
    0xbcb4666d, 0xb8757bda, 0xb5365d03, 0xb1f740b4
};

unsigned int CHC_PROTOCOL_HYENA2::cal_crc32(const unsigned char* buf, int len, unsigned int init)
{
    unsigned int crc = init;
    while (len--) {
        crc = (crc << 8) ^ crc32_table[((crc >> 24) ^ *buf) & 255];
        buf++;
    }
    return crc;
}