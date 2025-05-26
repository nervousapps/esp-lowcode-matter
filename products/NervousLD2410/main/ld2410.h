// // #pragma once
// #ifndef _ld2410_h
// #define _ld2410_h

// #include <string>
// #include <system.h>
// #include "simple_uart.h"

// // Config
// #define LD2410_BAUD_RATE 256000
// #define LD2410_BUFFER_SIZE 0x40
// #define LD2410_MAX_FRAME_LENGTH 40
// #define LD2410_LATEST_FIRMWARE "2.44"
// #define RX_GPIO_NUM (gpio_num_t) GPIO_NUM_4
// #define TX_GPIO_NUM (gpio_num_t) GPIO_NUM_5
// #define RTS_GPIO_NUM (gpio_num_t)(-1)
// #define CTS_GPIO_NUM (gpio_num_t)(-1)

// #define UART_PORT_NUM LP_UART_NUM_0
// // #define UART_PORT_NUM    UART_NUM_1

// int ld2410_init();
// void ld2410_read_data();
// std::string charToHexCustom(char c);

// void printBuf(const char *buf, uint8_t size);

// // void setNumbers(Number *maxMovingDistanceRange_, Number *maxStillDistanceRange_, Number *noneDuration_);
// // void sendCommand(char *commandStr, char *commandValue, int commandValueLen);
// int twoByteToInt(char firstByte, char secondByte);
// void handlePeriodicData(char *buffer, int len);
// // void handleACKData(char *buffer, int len);
// void readline(int readch, char *buffer, int len);
// // void setConfigMode(bool enable);
// // void queryParameters();
// // void LD2410_setup();
// // void ld2410_loop();
// // void setEngineeringMode(bool enable);
// // void setMaxDistancesAndNoneDuration(int maxMovingDistanceRange, int maxStillDistanceRange, int noneDuration);
// // void factoryReset();
// // void reboot();
// // void setBaudrate(int index);
// // void update();

// #endif
