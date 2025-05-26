// // #pragma once
// #ifndef _ld2410_c
// #define _ld2410_c

// #include <string>

// #include <esp_amp_platform.h>
// #include <esp_log.h>

// #include "ld2410.h"

// static const char *TAG = "ld2410";

// std::string charToHexCustom(char c)
// {
//     const char *hexDigits = "0123456789ABCDEF";
//     std::string hexStr;
//     hexStr += hexDigits[(c >> 4) & 0x0F];
//     hexStr += hexDigits[c & 0x0F];
//     return hexStr;
// }

// void printBuf(const char *buf, uint8_t size)
// {
//     printf("Received UART Data:\n");
//     for (uint8_t i = 0; i < size; i++)
//     {
//         // printf(uint8_t2hex(buf[i]));
//         printf("%s", charToHexCustom(buf[i]).c_str());
//     }
// }

// int ld2410_init()
// {
//     printf("%s: Initializing driver\n", TAG);

//     // /* uart driver config */
//     static uart_cfg_t cfg = {
//         .uart_pin_cfg = {
//             .tx_io_num = TX_GPIO_NUM,
//             .rx_io_num = RX_GPIO_NUM,
//             .rts_io_num = RTS_GPIO_NUM,
//             .cts_io_num = CTS_GPIO_NUM,
//         },
//         /* Default UART protocol config */
//         .uart_proto_cfg = {
//             .baud_rate = LD2410_BAUD_RATE,
//             .data_bits = UART_DATA_8_BITS,
//             .parity = UART_PARITY_DISABLE,
//             .stop_bits = UART_STOP_BITS_1,
//             .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
//             .rx_flow_ctrl_thresh = 0,
//         },
//     };

//     /* initialise the uart drvier */
//     esp_err_t err = simple_uart_init(UART_PORT_NUM, cfg);
//     if (err != ESP_OK)
//     {
//         printf("%s: Failed to initialise the UART driver\n", TAG);
//         return -1;
//     }

//     printf("%s: UART driver initialized successfully\n", TAG);

//     return 0;
// }

// void ld2410_read_data()
// {
//     // /* receviing data from the rx uart port */
//     char rx_buffer[LD2410_BUFFER_SIZE];
//     const int max_line_length = 80;
//     static char buffer[max_line_length];
//     // // uint8_t rx_buffer[LD2410_BUFFER_SIZE];
//     int rx_len = simple_uart_read_bytes(UART_PORT_NUM, rx_buffer, LD2410_BUFFER_SIZE, 10);

//     if (rx_len > 0)
//     {
//         /* printing the received data */
//         // printf("%s: Received UART Data:\n%s\n", TAG, rx_buffer);
//         printBuf(rx_buffer, LD2410_BUFFER_SIZE);
//         for(int i = 0; i<rx_len; i++) {
//             readline(rx_buffer[i], buffer, max_line_length);
//         }
//     }

//     // /* delay of 1 sec */
//     // esp_amp_platform_delay_ms(10);
// }

// long lastPeriodicMillis = system_get_time();

// // void setNumbers(Number *maxMovingDistanceRange_, Number *maxStillDistanceRange_, Number *noneDuration_)
// // {
// //     maxMovingDistanceRange = maxMovingDistanceR ange_;
// //     maxStillDistanceRange = maxStillDistanceRange_;
// //     noneDuration = noneDuration_;
// // }

// // void sendCommand(char *commandStr, char *commandValue, int commandValueLen)
// // {
// //     // lastCommandSuccess->publish_state(false);
// //     // frame start bytes
// //     write_byte(0xFD);
// //     write_byte(0xFC);
// //     write_byte(0xFB);
// //     write_byte(0xFA);
// //     // length bytes
// //     int len = 2;
// //     if (commandValue != nullptr)
// //         len += commandValueLen;
// //     write_byte(lowByte(len));
// //     write_byte(highByte(len));
// //     // command string bytes
// //     write_byte(commandStr[0]);
// //     write_byte(commandStr[1]);
// //     // command value bytes
// //     if (commandValue != nullptr)
// //     {
// //         for (int i = 0; i < commandValueLen; i++)
// //         {
// //             write_byte(commandValue[i]);
// //         }
// //     }
// //     // frame end bytes
// //     write_byte(0x04);
// //     write_byte(0x03);
// //     write_byte(0x02);
// //     write_byte(0x01);
// //     delay(50);
// // }

// int twoByteToInt(char firstByte, char secondByte)
// {
//     return (int16_t)(secondByte << 8) + firstByte;
// }

// void handlePeriodicData(char *buffer, int len)
// {
//     ESP_LOGI("LD2410", "handlePeriodicData : %d", len);
//     if (len < 12)
//         return; // 4 frame start bytes + 2 length bytes + 1 data end byte + 1 crc byte + 4 frame end bytes
//     if (buffer[0] != 0xF4 || buffer[1] != 0xF3 || buffer[2] != 0xF2 || buffer[3] != 0xF1)
//         return; // check 4 frame start bytes
//     if (buffer[7] != 0xAA || buffer[len - 6] != 0x55 || buffer[len - 5] != 0x00)
//         return; // data head=0xAA, data end=0x55, crc=0x00
//     /*
//       Data Type: 6th byte
//       0x01: Engineering mode
//       0x02: Normal mode
//     */
//     char dataType = buffer[5];
//     /*
//       Target states: 9th byte
//       0x00 = No target
//       0x01 = Moving targets
//       0x02 = Still targets
//       0x03 = Moving+Still targets
//     */
//     char stateByte = buffer[8];
//     if(stateByte != 0x00) {
//         ESP_LOGI("LD2410", "stateByte : %d", stateByte);
//     }
//     // hasTarget->publish_state(stateByte != 0x00);
//     /*
//       Reduce data update rate to prevent home assistant database size glow fast
//     */
//     long currentMillis = system_get_time();
//     if (currentMillis - lastPeriodicMillis < 1000)
//         return;
//     lastPeriodicMillis = currentMillis;

//     // hasMovingTarget->publish_state(CHECK_BIT(stateByte, 0));
//     // hasStillTarget->publish_state(CHECK_BIT(stateByte, 1));

//     // /*
//     //   Moving target distance: 10~11th bytes
//     //   Moving target energy: 12th byte
//     //   Still target distance: 13~14th bytes
//     //   Still target energy: 15th byte
//     //   Detect distance: 16~17th bytes
//     // */
//     int newMovingTargetDistance = twoByteToInt(buffer[9], buffer[10]);
//     ESP_LOGI("LD2410", "newMovingTargetDistance : %d", newMovingTargetDistance);
//     // if (movingTargetDistance->get_state() != newMovingTargetDistance)
//     //     movingTargetDistance->publish_state(newMovingTargetDistance);
//     int newMovingTargetEnergy = buffer[11];
//     ESP_LOGI("LD2410", "newMovingTargetEnergy : %d", newMovingTargetEnergy);
//     // if (movingTargetEnergy->get_state() != newMovingTargetEnergy)
//     //     movingTargetEnergy->publish_state(newMovingTargetEnergy);
//     int newStillTargetDistance = twoByteToInt(buffer[12], buffer[13]);
//     ESP_LOGI("LD2410", "newStillTargetDistance : %d", newStillTargetDistance);
//     // if (stillTargetDistance->get_state() != newStillTargetDistance)
//     //     stillTargetDistance->publish_state(newStillTargetDistance);
//     int newStillTargetEnergy = buffer[14];
//     ESP_LOGI("LD2410", "newStillTargetEnergy : %d", newStillTargetEnergy);
//     // if (stillTargetEnergy->get_state() != newStillTargetEnergy)
//     //     stillTargetEnergy->publish_state(buffer[14]);
//     int newDetectDistance = twoByteToInt(buffer[15], buffer[16]);
//     ESP_LOGI("LD2410", "newDetectDistance : %d", newDetectDistance);
//     // if (detectDistance->get_state() != newDetectDistance)
//     //     detectDistance->publish_state(newDetectDistance);
//     // if (dataType == 0x01)
//     // { // engineering mode
//     //   // todo: support engineering mode data
//     // }
// }

// // void handleACKData(char *buffer, int len)
// // {
// //     if (len < 10)
// //         return;
// //     if (buffer[0] != 0xFD || buffer[1] != 0xFC || buffer[2] != 0xFB || buffer[3] != 0xFA)
// //         return; // check 4 frame start bytes
// //     if (buffer[7] != 0x01)
// //         return;
// //     if (twoByteToInt(buffer[8], buffer[9]) != 0x00)
// //     {
// //         lastCommandSuccess->publish_state(false);
// //         return;
// //     }
// //     lastCommandSuccess->publish_state(true);
// //     switch (buffer[6])
// //     {
// //     case 0x61: // Query parameters response
// //     {
// //         if (buffer[10] != 0xAA)
// //             return; // value head=0xAA
// //         /*
// //           Moving distance range: 13th byte
// //           Still distance range: 14th byte
// //         */
// //         maxMovingDistanceRange->publish_state(buffer[12]);
// //         maxStillDistanceRange->publish_state(buffer[13]);
// //         /*
// //           Moving Sensitivities: 15~23th bytes
// //           Still Sensitivities: 24~32th bytes
// //         */
// //         for (int i = 0; i < 9; i++)
// //         {
// //             movingSensitivities[i] = buffer[14 + i];
// //         }
// //         for (int i = 0; i < 9; i++)
// //         {
// //             stillSensitivities[i] = buffer[23 + i];
// //         }
// //         /*
// //           None Duration: 33~34th bytes
// //         */
// //         noneDuration->publish_state(twoByteToInt(buffer[32], buffer[33]));
// //     }
// //     break;
// //     default:
// //         break;
// //     }
// // }

// void readline(int readch, char *buffer, int len)
// {
//     static int pos = 0;

//     for(int i = 0; i < len; i++) {
//         ESP_LOGI("LD2410", "buffer[%d] : %x", i, buffer[i]);
//     }

//     if (readch >= 0)
//     {
//         if (pos < len - 1)
//         {
//             buffer[pos++] = readch;
//             buffer[pos] = 0;
//         }
//         else
//         {
//             pos = 0;
//         }
//         if (pos >= 4)
//         {
//             ESP_LOGI("LD2410", "buffer[pos - 4] == 0xF8 : %d", buffer[pos - 4] == 0xF8);
//             ESP_LOGI("LD2410", "buffer[pos - 3] == 0xF7 : %d", buffer[pos - 3] == 0xF7);
//             ESP_LOGI("LD2410", "buffer[pos - 2] == 0xF6 : %d", buffer[pos - 2] == 0xF6);
//             ESP_LOGI("LD2410", "buffer[pos - 1] == 0xF5 : %d", buffer[pos - 1] == 0xF5);
//             if (buffer[pos - 4] == 0xF8 && buffer[pos - 3] == 0xF7 && buffer[pos - 2] == 0xF6 && buffer[pos - 1] == 0xF5)
//             {
//                 handlePeriodicData(buffer, pos);
//                 pos = 0; // Reset position index ready for next time
//             }
//             else if (buffer[pos - 4] == 0x04 && buffer[pos - 3] == 0x03 && buffer[pos - 2] == 0x02 && buffer[pos - 1] == 0x01)
//             {
//                 // handleACKData(buffer, pos);
//                 pos = 0; // Reset position index ready for next time
//             }
//             ESP_LOGI("LD2410", "pos : %d", pos);
//         }
//     }
//     return;
// }

// // void setConfigMode(bool enable)
// // {
// //     char cmd[2] = {enable ? 0xFF : 0xFE, 0x00};
// //     char value[2] = {0x01, 0x00};
// //     sendCommand(cmd, enable ? value : nullptr, 2);
// // }

// // void queryParameters()
// // {
// //     char cmd_query[2] = {0x61, 0x00};
// //     sendCommand(cmd_query, nullptr, 0);
// // }

// // void setup() override
// // {
// //     set_update_interval(15000);
// // }

// // void loop() override
// // {
// //     const int max_line_length = 80;
// //     static char buffer[max_line_length];
// //     while (available())
// //     {
// //         readline(read(), buffer, max_line_length);
// //     }
// // }

// // void setEngineeringMode(bool enable)
// // {
// //     char cmd[2] = {enable ? 0x62 : 0x63, 0x00};
// //     sendCommand(cmd, nullptr, 0);
// // }

// // void setMaxDistancesAndNoneDuration(int maxMovingDistanceRange, int maxStillDistanceRange, int noneDuration)
// // {
// //     char cmd[2] = {0x60, 0x00};
// //     char value[18] = {0x00, 0x00, lowByte(maxMovingDistanceRange), highByte(maxMovingDistanceRange), 0x00, 0x00, 0x01, 0x00, lowByte(maxStillDistanceRange), highByte(maxStillDistanceRange), 0x00, 0x00, 0x02, 0x00, lowByte(noneDuration), highByte(noneDuration), 0x00, 0x00};
// //     sendCommand(cmd, value, 18);
// //     queryParameters();
// // }

// // void factoryReset()
// // {
// //     char cmd[2] = {0xA2, 0x00};
// //     sendCommand(cmd, nullptr, 0);
// // }

// // void reboot()
// // {
// //     char cmd[2] = {0xA3, 0x00};
// //     sendCommand(cmd, nullptr, 0);
// //     // not need to exit config mode because the ld2410 will reboot automatically
// // }

// // void setBaudrate(int index)
// // {
// //     char cmd[2] = {0xA1, 0x00};
// //     char value[2] = {index, 0x00};
// //     sendCommand(cmd, value, 2);
// // }

// // void update()
// // {
// // }

// #endif