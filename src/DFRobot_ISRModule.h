/*!
 * @file DFRobot_ISRModule.h
 * @brief Define the basic structure of class DFRobot_ISRModule
 * @details Set the command word type to Chinese. Set the wake-up time. Add command words.
 * @n Delete command words. Retrieve the recognized command word ID.

 * @copyright	Copyright (c) 2021 DFRobot Co.Ltd (http://www.dfrobot.com)
 * @license The MIT License (MIT)
 * @author [qsjhyy](yihuan.huang@dfrobot.com)
 * @version V1.0
 * @date 2024-01-06
 * @url https://github.com/DFRobot/DFRobot_ISRModule_ESP32S3
 */
#ifndef _DFRobot_ISR_Module_H_
#define _DFRobot_ISR_Module_H_

#include "Arduino.h"
#include "Wire.h"
// #include "cstring.h"
// #include <cstring>

#if defined(ARDUINO_AVR_UNO) || defined(ESP8266)
#include "SoftwareSerial.h"
#else
#include "HardwareSerial.h"
#endif

// #define ENABLE_DBG ///< Enable this macro to see the detailed running process of the program
#ifdef ENABLE_DBG
#define DBG(...) {Serial.print("[");Serial.print(__FUNCTION__); Serial.print("(): "); Serial.print(__LINE__); Serial.print(" ] "); Serial.println(__VA_ARGS__);}
#else
#define DBG(...)
#endif


class DFRobot_ISRModule
{
public:
#define MODULE_I2C_ADDRESS   0x14   //< Sensor device address

#define UART_BAUDRATE        ((uint32_t)57600)
#define CMD_READ_REGBUF      0xBB
#define CMD_WRITE_REGBUF     0xCC
#define TIME_OUT             200    //< 200 ms time out

#define VOICE_ID_REG         0x00   // Recognized voice ID
#define WAKEUP_TIME_REG      0x01   // Wake-up duration (unit: s)
#define MODEL_TYPE_REG       0x02   // Language model type (cn or en)
#define ADD_CMD_REG          0x03   // Add keyword (number, entry)
#define DEL_CMD_BY_ID_REG    0x04   // Delete keyword (by number)
#define DEL_CMD_BY_STR_REG   0x05   // Delete keyword (by entry)

#define CMD_ERROR_REG        0x03   // Error register, 0 indicates no error

#define REG_ISR_PID          0xAAU
#define REG_ISR_VID          0xACU
#define REG_ISR_VERSION      0xAEU

#define MODULE_DFR0715_PID       0x42CB   ///< Sensor PID
#define MODULE_DFR0715_VID       0x3343   ///< Sensor VID
#define MODULE_DFR0715_VERSION   0x0100   ///< Sensor VERSION

  /**
   * @enum eSpeechModelType_t
   * @brief Types of speech recognition models
   */
  typedef enum {
    eSpeechModelSTDBY = 0x00,
    eSpeechModelChinese = 0x01,
    eSpeechModelEnglish = 0x02,
  }eSpeechModelType_t;

public:
  /**
   * @fn DFRobot_ISRModule
   * @brief Constructor
   * @return None
   */
  DFRobot_ISRModule();

  /**
   * @fn begin
   * @brief subclass initialization function
   * @param type - Types of speech recognition models: Standby, Chinese, English
   * @param duration - Wake-up duration. range: 0, [6, 120], unit: second. note: 0 To always stay awake.
   * @return bool type, means returning initialization status
   * @retval true NO_ERROR
   */
  virtual bool begin(eSpeechModelType_t type = eSpeechModelChinese, uint8_t duration = 10);

  /**
   * @fn addCommandWord
   * @brief Add keyword (entry, number)
   * @param num - Command word number
   * @param str - Command word entry
   * @return The value is bool: true is returned on success, false is returned on failure
   */
  bool addCommandWord(uint8_t num, String str);

  /**
   * @fn delCMDByNUM
   * @brief Delete keyword (number or entry)
   * @param num - Command word number
   * @param str - Command word entry
   * @return None
   */
  void delCommandWord(uint8_t num);
  void delCommandWord(String str);

  /**
   * @fn getKeywordID
   * @brief Get recognition result (ID)
   * @return Returns the recognition result
   * @retval 0: Wake-up word recognized
   * @retval 1~254: Recognized command word with corresponding number
   * @retval 0xFF: No keyword recognized
   */
  uint8_t getKeywordID(void);

private:

  /**
   * @fn setSpeechModelType
   * @brief Set the type of the speech recognition model
   * @param type - Type of the speech recognition model: Standby, Chinese, English
   * @return None
   */
  void setSpeechModelType(eSpeechModelType_t type);

  /**
   * @fn setWakeupTime
   * @brief Set the exit wake-up time
   * @param duration - Wake-up duration. Range: 0, 6~120, unit: second.
   * @note 6 seconds is the lower limit; except for zero, durations less than 6 seconds are set to 6 seconds; 0 means no exit.
   * @return None
   */
  void setWakeupTime(uint8_t duration);

  /**
   * @fn writeReg
   * @brief Write register function, designed as virtual function, implemented by derived class
   * @param reg  Register address 8bits
   * @param pBuf Storage and buffer for data to be written
   * @param size Length of data to be written
   * @return None
   */
  virtual void writeReg(uint8_t reg, void* pBuf, size_t size) = 0;

  /**
   * @fn readReg
   * @brief Read register function, designed as virtual function, implemented by derived class
   * @param reg  Register address 8bits
   * @param pBuf Storage and buffer for data to be read
   * @param size Length of data to be read
   * @return uint8_t type, indicates returning read register status
   * @retval 0 Reading succeeded
   * @retval 1 Reading failed
   */
  virtual uint8_t readReg(uint8_t reg, void* pBuf, size_t size) = 0;

};

class DFRobot_ISRModule_I2C:public DFRobot_ISRModule
{
public:
  DFRobot_ISRModule_I2C(TwoWire* pWire = &Wire, uint8_t addr = MODULE_I2C_ADDRESS);
  bool begin(eSpeechModelType_t type = eSpeechModelChinese, uint8_t duration = 10);
protected:
  virtual void writeReg(uint8_t reg, void* pBuf, size_t size);
  virtual uint8_t readReg(uint8_t reg, void* pBuf, size_t size);
private:
  TwoWire* _pWire;
  uint8_t _deviceAddr;
};

class DFRobot_ISRModule_UART:public DFRobot_ISRModule
{
public:
#if defined(ARDUINO_AVR_UNO) || defined(ESP8266)
  DFRobot_ISRModule_UART(SoftwareSerial* sSerial, uint32_t Baud = UART_BAUDRATE);
#else
  DFRobot_ISRModule_UART(HardwareSerial* hSerial, uint32_t Baud = UART_BAUDRATE, uint8_t rxpin = 0, uint8_t txpin = 0);
#endif

  bool begin(eSpeechModelType_t type = eSpeechModelChinese, uint8_t duration = 10);
protected:
  virtual void writeReg(uint8_t reg, void* pBuf, size_t size);
  virtual uint8_t readReg(uint8_t reg, void* pBuf, size_t size);
private:
#if defined(ARDUINO_AVR_UNO) || defined(ESP8266)
  SoftwareSerial* _serial;
#else
  HardwareSerial* _serial;
#endif
  uint32_t _baud;
  uint8_t _rxpin;
  uint8_t _txpin;
};

#endif/* _DFRobot_ISR_Module_H_ */
