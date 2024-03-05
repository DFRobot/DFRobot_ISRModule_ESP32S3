# DFRobot_ISRModule_ESP32S3
- [中文版](./README_CN.md)

The module uses the ESP32S3 chip designed by Le Xin company. It only needs to set the list of key words to be recognized in the program and download it into the MCU of the main control. The voice recognition module can identify the key words spoken by the user and process them accordingly according to the program. This product does not require the user to train and record in advance to complete non-specific person speech recognition, recognition accuracy of up to 95%.

![effect picture](./resources/images/DFR0715.png)


## Product Link(https://www.dfrobot.com/)

    SKU: DFR0715


## Table of Contents

* [Summary](#Summary)
* [Installation](#Installation)
* [Methods](#Methods)
* [Compatibility](#Compatibility)
* [History](#History)
* [Credits](#Credits)


## Summary

  * Set the command word type to Chinese.
  * Set the wake-up time.
  * Add command words.
  * Delete command words.
  * Retrieve the recognized command word ID.


## Installation
There are two methods for using this library:<br>
1. Open Arduino IDE, search for "DFRobot_ISRModule_ESP32S3" on the status bar in Tools ->Manager Libraries and install the library.<br>
2. Download the library file before use, paste it into \Arduino\libraries directory, then open the examples folder and run the demo in the folder.<br>


## Methods

```C++

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

```


## Compatibility

MCU                | Work Well    |   Work Wrong    | Untested    | Remarks
------------------ | :----------: | :-------------: | :---------: | :----:
Arduino Uno        |      √       |                 |             |
Arduino MEGA2560   |      √       |                 |             |
Arduino Leonardo   |      √       |                 |             |
FireBeetle-ESP8266 |      √       |                 |             |
FireBeetle-ESP32   |      √       |                 |             |
FireBeetle-M0      |      √       |                 |             |
Micro:bit          |      √       | nonsupport uart |             |

note: esp8266 SDK: http://arduino.esp8266.com/stable/package_esp8266com_index.json


## History

- 2024/01/08 - Version 1.0.0 released.


## Credits

Written by qsjhyy(yihuan.huang@dfrobot.com), 2024. (Welcome to our website)

