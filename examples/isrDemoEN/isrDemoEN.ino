/*!
 * @file isrDemoEN.ino
 * @brief Set the command word type to English. Set the wake-up time. Add command words.
 * @n Delete command words. Retrieve the recognized command word ID.
 *
 * @copyright Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
 * @license The MIT License (MIT)
 * @author [qsjhyy](yihuan.huang@dfrobot.com)
 * @version V1.0
 * @date 2022-08-30
 * @url https://github.com/DFRobot/DFRobot_ISRModule_ESP32S3
 */
#include "DFRobot_ISRModule.h"

#define I2C_COMMUNICATION  //use I2C for communication, but use the serial port for communication if the line of codes were masked

#ifdef I2C_COMMUNICATION
DFRobot_ISRModule_I2C isr(&Wire, MODULE_I2C_ADDRESS);
#else
/* ---------------------------------------------------------------------------------------------------------------------
  *    board   |             MCU                | Leonardo/Mega2560/M0 |    UNO    | ESP8266 | ESP32 |  microbit  |   m0  |
  *     VCC    |            3.3V/5V             |        VCC           |    VCC    |   VCC   |  VCC  |     X      |  vcc  |
  *     GND    |              GND               |        GND           |    GND    |   GND   |  GND  |     X      |  gnd  |
  *     RX     |              TX                |     Serial1 TX1      |     5     |   5/D6  |  D2   |     X      |  tx1  |
  *     TX     |              RX                |     Serial1 RX1      |     4     |   4/D7  |  D3   |     X      |  rx1  |
  * ----------------------------------------------------------------------------------------------------------------------*/
/* Baud rate cannot be changed  */
#if defined(ARDUINO_AVR_UNO) || defined(ESP8266)
SoftwareSerial mySerial(/*rx*/ 4, /*tx*/ 5);
DFRobot_ISRModule_UART isr(&mySerial, UART_BAUDRATE);
#elif defined(ESP32)
DFRobot_ISRModule_UART isr(&Serial1, UART_BAUDRATE, /*rx*/ D2, /*tx*/ D3);
#else
DFRobot_ISRModule_UART isr(&Serial1, UART_BAUDRATE);
#endif
#endif


void setup()
{
  Serial.begin(115200);

  /**
   * subclass initialization function
   * type - Types of speech recognition models: Standby, Chinese, English
   * duration - Wake-up duration. range: 0, [6, 120], unit: second. note: 0 To always stay awake.
   * bool type, means returning initialization status
   */
  while (!isr.begin(/* type */ isr.eSpeechModelEnglish, /* duration */ 10)) {
    Serial.println("Failed to init chip, please check if the chip connection is fine. ");
    delay(1000);
  }
  Serial.println("begin OK!");

  Serial.println("add command word...\n");
  /*!
   * note:
   * The numbering of command words starts from 1 (0 is the wake-up word).
   * Command word numbers range from 1 to 254, and the byte size of each entry should be less than 120.
   * Mixing Chinese and English, including Arabic numerals and special characters, is not supported.
   * Command words with incorrect formats may result in model reset.
   * Please avoid adding command words with the same number, and you can re-add them after deletion.
   * Do not add command words during wake up.
   */
  isr.addCommandWord(1, "tell me a joke");
  isr.addCommandWord(2, "turn on the computer");
  isr.addCommandWord(3, "turn off the computer");

  // isr.delCommandWord(1);   // Example Delete the specified ID command word
  // isr.delCommandWord("turn on the computer");   // Deletes the specified name command word

  Serial.println("------------detect start------------\n");
}

void loop() {
  uint8_t id = isr.getKeywordID();
  switch (id) {
    case 0:
      Serial.println("Recognize the wake word!");
      break;
    case 1:
      Serial.println("Command word 1 is identified!");
      break;
    case 2:
      Serial.println("Command word 2 is identified!");
      break;
    case 3:
      Serial.println("Command word 3 is identified!");
      break;
    case 0xFF:
      // Serial.println("Keywords not recognized!");
      break;

    default:
      Serial.print(id);
      Serial.println(" Unknown command word!");
      break;
  }
  delay(150);
}