#ifndef MY_LD2410_H
#define MY_LD2410_H

#include <stdio.h>
#include <cstdint>
#include <string>

#include <system.h>
#include <esp_amp_platform.h>
#include <esp_log.h>

#include "simple_uart.h"
#include "queue.h"

/*

MyLD2410 library
An Arduino library for the LD2410 presence sensor, including HLK-LD2410B and HLK-LD2410C.
https://github.com/iavorvel/MyLD2410

*/

/**
 * @file MyLD2410.h
 */
// Config
#define LD2410_BAUD_RATE  256000
#define LD2410_BUFFER_SIZE 0x40
#define LD2410_LATEST_FIRMWARE "2.44"
#define LD2410_MAX_FRAME_LENGTH 40
#define RX_GPIO_NUM (gpio_num_t) GPIO_NUM_4
#define TX_GPIO_NUM (gpio_num_t) GPIO_NUM_5
#define RTS_GPIO_NUM (gpio_num_t)(-1)
#define CTS_GPIO_NUM (gpio_num_t)(-1)

/**
 * @brief The auxiliary light control status
 */
enum class LightControl
{
  NOT_SET = -1,
  NO_LIGHT_CONTROL,
  LIGHT_BELOW_THRESHOLD,
  LIGHT_ABOVE_THRESHOLD
};

/**
 * @brief The auxiliary output control status
 */
enum class OutputControl
{
  NOT_SET = -1,
  DEFAULT_LOW,
  DEFAULT_HIGH,
};

/**
 * @brief The status of the auto-thresholds routine
 */
enum class AutoStatus
{
  NOT_SET = -1,
  NOT_IN_PROGRESS,
  IN_PROGRESS,
  COMPLETED
};

class MyLD2410
{
public:
  enum Response
  {
    FAIL = 0,
    ACK,
    DATA
  };
  struct ValuesArray
  {
    uint8_t values[9];
    uint8_t N;

    void setN(uint8_t n)
    {
      N = (n <= 8) ? n : 8;
    }
    ValuesArray &operator=(const ValuesArray &other)
    {
      if (this != &other)
      {
        N = other.N;
        for (uint8_t i = 0; i <= N; i++)
          values[i] = other.values[i];
      }
      return *this;
    }
    template <typename uint8_tHandler>
    void forEach(uint8_tHandler func) const
    {
      for (uint8_t i = 0; i <= N; i++)
        func(values[i]);
    }
  };
  struct SensorData
  {
    uint8_t status;
    unsigned long timestamp;
    unsigned long mTargetDistance;
    uint8_t mTargetSignal;
    unsigned long sTargetDistance;
    uint8_t sTargetSignal;
    unsigned long distance;
    // Enhanced data
    ValuesArray mTargetSignals;
    ValuesArray sTargetSignals;
  };

private:
  SensorData sData;
  ValuesArray stationaryThresholds;
  ValuesArray movingThresholds;
  uint8_t maxRange = 0;
  uint8_t noOne_window = 0;
  uint8_t lightLevel = 0;
  uint8_t outLevel = 0;
  uint8_t lightThreshold = 0;
  LightControl lightControl = LightControl::NOT_SET;
  OutputControl outputControl = OutputControl::NOT_SET;
  AutoStatus autoStatus = AutoStatus::NOT_SET;
  unsigned long version = 0;
  unsigned long bufferSize = 0;
  uint8_t MAC[6];
  // std::string  MACstr = "None";
  // std::string  firmware = "None";
  uint8_t firmwareMajor = 0;
  uint8_t firmwareMinor = 0;
  int fineRes = -1;
  bool isEnhanced = false;
  bool isConfig = false;
  unsigned long timeout = 2000;
  unsigned long dataLifespan = 500;
  uint8_t inBuf[LD2410_BUFFER_SIZE];
  uint8_t inBufI = 0;
  uint8_t headBuf[4];
  uint8_t headBufI = 0;
  uart_port_t UART_PORT_NUM = UART_NUM_1;
  bool _debug = false;
  bool isDataValid();
  bool readFrame();
  bool sendCommand(const uint8_t *command);
  bool processAck();
  bool processData();

  Queue q;
  int rx_read_len = 0;

public:
  /**
   * @brief Construct a new MyLD2410 object
   *
   * @param serial - a reference to a stream object (sensorSerial)
   * @param debug - a flag that controls whether debug data will be sent to Serial
   */
  MyLD2410(uart_port_t uart_port_num = UART_NUM_1, bool debug = false);

  // CONTROLS

  /**
   * @brief Call this function in setup() to ascertain whether the device is responding
   */
  bool begin();

  // /**
  //  * @brief Call this function to gracefully close the sensor. Useful for entering sleep mode.
  //  */
  void end();

  // /**
  //  * @brief Set the debug flag
  //  *
  //  */
  void debugOn();

  // /**
  //  * @brief Reset the debug flag
  //  *
  //  */
  void debugOff();

  /**
    @brief Call this function in the main loop
    @return MyLD2410::DATA = (evaluates to true) if the latest frame contained data
    @return MyLD2410::ACK  = (evaluates to true) if the latest frame contained a reply to a command
    @return MyLD2410::FAIL = (evaluates to false) if no useful info was processed
    */
  Response check();

  // // GETTERS

  // /**
  //  * @brief Check whether the device is in config mode
  //  * (accepts commands)
  //  */
  bool inConfigMode();

  // /**
  //  * @brief Check whether the device is in basic mode
  //  * (continuously sends basic presence data)
  //  */
  bool inBasicMode();

  // /**
  //  * @brief Check whether the device is in enhanced mode
  //  * (continuously sends enhanced presence data)
  //  */
  bool inEnhancedMode();

  // /**
  //  * @brief Get the status of the sensor:
  //  * 0 - No presence;
  //  * 1 - Moving only;
  //  * 2 - Stationary only;
  //  * 3 - Both moving and stationary;
  //  * 4 - Auto thresholds in progress;
  //  * 5 - Auto thresholds successful;
  //  * 6 - Auto thresholds failed;
  //  * 255 - The sensor status is invalid
  //  *
  //  * @return uint8_t
  //  */
  uint8_t getStatus();

  // /**
  //  * @brief Get the presence status as a c-string
  //  *
  //  * @return const char* :
  //  * "No target",
  //  * "Moving only",
  //  * "Stationary only",
  //  * "Both moving and stationary",
  //  * "Auto thresholds in progress",
  //  * "Auto thresholds successful",
  //  * "Auto thresholds failed".
  //  */
  const char *statusString();

  /**
   * @brief Check whether presence was detected in the latest frame
   */
  bool presenceDetected();

  // /**
  //  * @brief Check whether a stationary target was detected in the latest frame
  //  */
  bool stationaryTargetDetected();

  // /**
  //  * @brief Get the distance to the stationary target in [cm]
  //  *
  //  * @return unsigned long - distance in [cm]
  //  */
  unsigned long stationaryTargetDistance();

  // /**
  //  * @brief Get the signal from the stationary target
  //  *
  //  * @return uint8_t - signal value [0:100]
  //  */
  uint8_t stationaryTargetSignal();

  // /**
  //  * @brief Get the Stationary Signals object, if in enhanced mode
  //  *
  //  * @return const MyLD2410::ValuesArray& - the signals for each detection gate
  //  */
  const ValuesArray &getStationarySignals();

  // /**
  //  * @brief Check whether a moving target was detected in the latest frame
  //  */
  bool movingTargetDetected();

  // /**
  //  * @brief Get the distance to the moving target in [cm]
  //  *
  //  * @return unsigned long - distance in [cm]
  //  */
  unsigned long movingTargetDistance();

  // /**
  //  * @brief Get the signal from the moving target
  //  *
  //  * @return uint8_t - signal value [0:100]
  //  */
  uint8_t movingTargetSignal();

  // /**
  //  * @brief Get the Moving Signals object, if in enhanced mode
  //  *
  //  * @return const MyLD2410::ValuesArray& - the signals for each detection gate
  //  */
  const ValuesArray &getMovingSignals();

  // /**
  //  * @brief Get the detected distance
  //  *
  //  * @return unsigned long - distance in [cm]
  //  */
  unsigned long detectedDistance();

  // /**
  //  * @brief Get the Bluetooth MAC address as an array uint8_t[6]
  //  *
  //  * @return const uint8_t*
  //  */
  // const uint8_t *getMAC();

  // /**
  //  * @brief Get the Bluetooth MAC address as a std::string 
  //  *
  //  * @return std::string 
  //  */
  // std::string getMACstr();

  // /**
  //  * @brief Get the Firmware as a std::string 
  //  *
  //  * @return std::string 
  //  */
  // std::string  getFirmware();

  // /**
  //  *  @brief Get the Firmware Major
  //  *
  //  *  @return uint8_t
  //  */
  // uint8_t getFirmwareMajor();

  // /**
  //  *  @brief Get the Firmware Minor
  //  *
  //  *  @return uint8_t
  //  */
  // uint8_t getFirmwareMinor();

  // /**
  //  * @brief Get the protocol version
  //  *
  //  * @return unsigned long
  //  */
  // unsigned long getVersion();

  // /**
  //  * @brief Get the SensorData object
  //  *
  //  * @return const SensorData&
  //  */
  // const SensorData &getSensorData();

  // /**
  //  * @brief Get the sensor resolution (gate-width) in [cm]
  //  *
  //  * @return uint8_t either 20 or 75 on success, 0 on failure
  //  */
  // uint8_t getResolution();

  // // parameters

  // /**
  //  * @brief Get the detection thresholds for moving targets
  //  *
  //  * @return const ValuesArray&
  //  */
  // const ValuesArray &getMovingThresholds();

  // /**
  //  * @brief Get the detection thresholds for stationary targets
  //  *
  //  * @return const ValuesArray&
  //  */
  // const ValuesArray &getStationaryThresholds();

  // /**
  //  * @brief Get the maximum detection gate
  //  *
  //  * @return uint8_t
  //  */
  // uint8_t getRange();

  // /**
  //  * @brief Get the maximum detection range in [cm]
  //  *
  //  * @return unsigned long
  //  */
  // unsigned long getRange_cm();

  // /**
  //  * @brief Get the time-lag of "no presence" in [s].
  //  * The sensor begins reporting "no presence"
  //  * only after no motion has been detected for that many seconds.
  //  *
  //  * @return uint8_t
  //  */
  // uint8_t getNoOneWindow();
  // // end parameters

  // // REQUESTS

  // /**
  //  * @brief Request config mode
  //  *
  //  * @param enable [true]/false
  //  * @return true on success
  //  */
  bool configMode(bool enable = true);

  // /**
  //  * @brief Request enhanced mode
  //  *
  //  * @param enable [true]/false
  //  * @return true on success
  //  */
  bool enhancedMode(bool enable = true);

  // /**
  //  * @brief Request the current auxiliary configuration
  //  *
  //  * @return true on success
  //  */
  // bool requestAuxConfig();

  // /**
  //  * @brief Begin the automatic threshold detection routine
  //  * (firmware >= 2.44)
  //  *
  //  * @param _timeout - allow for timeout [s] to leave the room 
  //  * @return true on success
  //  */ 
  // bool autoThresholds(uint8_t _timeout = 10);

  // /**
  //  * @brief Get the status of the automatic threshold detection routine
  //  * (firmware >= 2.44)
  //  *
  //  * @return AutoStatus
  //  */
  // AutoStatus getAutoStatus();

  // /**
  //  * @brief Request the Bluetooth MAC address
  //  *
  //  * @return true on success
  //  */
  // bool requestMAC();

  // /**
  //  * @brief Request the Firmware
  //  *
  //  * @return true on success
  //  */
  bool requestFirmware();

  // /**
  //  * @brief Request the resolution (gate-width)
  //  *
  //  * @return true on success
  //  */
  // bool requestResolution();

  // /**
  //  * @brief Set the resolution of the sensor
  //  *
  //  * @param fine true=20cm; [false]=75cm
  //  * @return true on success
  //  */
  // bool setResolution(bool fine = false);

  // /**
  //  * @brief Request the sensor parameters:
  //  * range, motion thresholds, stationary thresholds, no-one window
  //  *
  //  * @return true on success
  //  */
  // bool requestParameters();

  // /**
  //  * @brief Set the gate parameters for a particular gate, or for all gates at once
  //  *
  //  * @param gate the gate to configure;
  //  * pass a value greater than 8 (e.g 0xFF) to apply the same thresholds to all gates
  //  * @param movingThreshold [0 - 100]
  //  * @param stationaryThreshold [0 - 100]
  //  * @return true on success
  //  */
  // bool setGateParameters(uint8_t gate, uint8_t movingThreshold = 100, uint8_t stationaryThreshold = 100);

  // /**
  //  * @brief Set the parameters for all gates at once, as well as the no-one window
  //  *
  //  * @param moving_thresholds as a ValueArray
  //  * @param stationary_thresholds as a ValueArray
  //  * @param noOneWindow
  //  * @return true on success
  //  */
  // bool setGateParameters(const ValuesArray &moving_thresholds, const ValuesArray &stationary_thresholds, uint8_t noOneWindow = 5);

  // /**
  //  * @brief Set the detection range for moving targets, stationary targets, as well as the no-one window
  //  *
  //  * @param movingGate
  //  * @param stationaryGate
  //  * @param noOneWindow
  //  * @return true on success
  //  */
  // bool setMaxGate(uint8_t movingGate, uint8_t stationaryGate, uint8_t noOneWindow = 5);

  // /**
  //  * @brief Set the no-one window parameter
  //  *
  //  * @param noOneWindow in [s]
  //  * @return true on success
  //  */
  // bool setNoOneWindow(uint8_t noOneWindow);

  // /**
  //  * @brief Set the maximum moving gate
  //  *
  //  * @param movingGate
  //  * @return true on success
  //  */
  // bool setMaxMovingGate(uint8_t movingGate);

  // /**
  //  * @brief Set the maximum stationary gate
  //  *
  //  * @param stationaryGate
  //  * @return true on success
  //  */
  // bool setMaxStationaryGate(uint8_t stationaryGate);

  // /**
  //  * @brief Request reset to factory default parameters
  //  *
  //  * @return true on success
  //  */
  // bool requestReset();

  // /**
  //  * @brief Request reboot
  //  *
  //  * @return true on success
  //  */
  // bool requestReboot();

  // /**
  //  * @brief Turn Bluetooth ON
  //  *
  //  * @return true on success
  //  */
  // bool requestBTon();

  // /**
  //  * @brief Turn Bluetooth OFF
  //  *
  //  * @return true on success
  //  */
  // bool requestBToff();

  // /**
  //  * @brief Set a new BT password.
  //  *
  //  * The BT password must be 6 characters long. If the string is shorter, it will be padded with spaces '\20'. If it is longer, only the first 6 characters will be used.
  //  *
  //  * @param passwd c-string
  //  * @return true on success
  //  */
  // bool setBTpassword(const char *passwd);

  // /**
  //  * @brief Set a new BT password.
  //  *
  //  * The BT password must be 6 characters long. If the string is shorter, it will be padded with spaces '\20'. If it is longer, only the first 6 characters will be used.
  //  *
  //  * @param passwd Arduino std::string 
  //  * @return true on success
  //  */
  // bool setBTpassword(const std::string  &passwd);

  // /**
  //  * @brief Reset the BT password
  //  *
  //  * @return true on success
  //  */
  // bool resetBTpassword();

  // /**
  //  * @brief Reset the serial baud rate. The sensor reboots at the new rate on success
  //  *
  //  * @return true on success
  //  */
  // bool setBaud(uint8_t baud);

  // /**
  //  * @brief Get the Light Level
  //  *
  //  * @return uint8_t
  //  */
  // uint8_t getLightLevel();

  // /**
  //  * @brief Get the Light Control parameter
  //  *
  //  * @return LightControl enum
  //  */
  // LightControl getLightControl();

  // /**
  //  * @brief Set the Auxiliary Control parameters
  //  *
  //  * @param light_control
  //  * @param light_threshold
  //  * @param output_control
  //  *
  //  * @return true on success
  //  */
  // bool setAuxControl(LightControl light_control, uint8_t light_threshold, OutputControl output_control);

  // /**
  //  * @brief Reset the Auxiliary Control parameters to their default values
  //  *
  //  * @return true on success
  //  */
  // bool resetAuxControl();

  // /**
  //  * @brief Get the Light Threshold
  //  *
  //  * @return uint8_t
  //  */
  // uint8_t getLightThreshold();

  // /**
  //  * @brief Get the Output Control parameter
  //  *
  //  * @return OutputControl enum
  //  */
  // OutputControl getOutputControl();

  // /**
  //  * @brief Get the Light Level
  //  *
  //  * @return uint8_t
  //  */
  // uint8_t getOutLevel();
};

#endif // MY_LD2410_H