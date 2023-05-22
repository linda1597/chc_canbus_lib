#ifndef _CAN_H_
#define _CAN_H_

#define DEBUG_CAN_LEVEL 4
#define DEBUG_CAN_SERIAL Serial

#if DEBUG_CAN_LEVEL >= 1

#define CAN_LOG_E(fmt, ...) DEBUG_CAN_SERIAL.printf("[E][%10lu][   ]%25s:%-5d > " fmt "\n", millis(), __FILE__, __LINE__, ##__VA_ARGS__);
#else
#define CAN_LOG_E(fmt, ...) ;
#endif

#if DEBUG_CAN_LEVEL >= 2
#define CAN_LOG_W(fmt, ...) DEBUG_CAN_SERIAL.printf("[W][%10lu][   ]%25s:%-5d > " fmt "\n", millis(), __FILE__, __LINE__, ##__VA_ARGS__);
#else
#define CAN_LOG_W(fmt, ...) ;
#endif

#if DEBUG_CAN_LEVEL >= 3
#define CAN_LOG_I(fmt, ...) DEBUG_CAN_SERIAL.printf("[I][%10lu][   ]%25s:%-5d > " fmt "\n", millis(), __FILE__, __LINE__, ##__VA_ARGS__);
#else
#define CAN_LOG_I(fmt, ...) ;
#endif

#if DEBUG_CAN_LEVEL >= 4
#define CAN_LOG(fmt, ...) DEBUG_CAN_SERIAL.printf("[ ][%10lu][   ]%25s:%-5d > " fmt, millis(), __FILE__, __LINE__, ##__VA_ARGS__);
#define CAN_LOG_S(fmt, ...) DEBUG_CAN_SERIAL.printf(fmt, ##__VA_ARGS__);
#define CAN_LOG_V(fmt, ...) DEBUG_CAN_SERIAL.printf("[V][%10lu][   ]%25s:%-5d > " fmt "\n", millis(), __FILE__, __LINE__, ##__VA_ARGS__);
#else
#define CAN_LOG(fmt, ...)
#define CAN_LOG_S(fmt, ...)
#define CAN_LOG_V(fmt, ...) ;

#endif

#ifdef ESP32

extern void CAN_init(int pinCanRx, int pinCanTx);
extern bool CAN_transmit(uint8_t* data);
extern bool CAN_receive(uint8_t* data, long timeout_ms);
#else
extern void CAN_init(int pinCanRx, int pinCanTx);
extern bool CAN_transmit(uint8_t* data);
extern bool CAN_receive(uint8_t* data, long timeout_ms);
#endif

#endif