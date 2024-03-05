/*!
 * @file DFRobot_ISRModule.cpp
 * @brief Implemention of DFRobot_ISRModule class
 * @copyright	Copyright (c) 2021 DFRobot Co.Ltd (http://www.dfrobot.com)
 * @license The MIT License (MIT)
 * @author [qsjhyy](yihuan.huang@dfrobot.com)
 * @version V1.0
 * @date 2024-01-06
 * @url https://github.com/DFRobot/DFRobot_ISRModule_ESP32S3
 */
#include "DFRobot_ISRModule.h"

DFRobot_ISRModule::DFRobot_ISRModule()
{
}

bool DFRobot_ISRModule::begin(eSpeechModelType_t type, uint8_t duration)
{
  uint8_t data[2] = { 0 };
  data[0] = duration;
  data[1] = type;
  writeReg(WAKEUP_TIME_REG, data, 2);
  delay(1000);   // Wait for the initialization to complete
  return true;
}

void DFRobot_ISRModule::setSpeechModelType(eSpeechModelType_t type)
{
  uint8_t mode = type;
  writeReg(MODEL_TYPE_REG, &mode, 1);
}

void DFRobot_ISRModule::setWakeupTime(uint8_t duration)
{
  writeReg(WAKEUP_TIME_REG, &duration, 1);
}

bool DFRobot_ISRModule::addCommandWord(uint8_t num, String str)
{
  int length = str.length();
  char data[30] = { (char)num, (char)length };   // Only 32 bytes can be sent at a time, and the receive limit is 28 stable
  DBG(length);

  for (int i = 0; i < length; i += 23) {
    String chunk = str.substring(i, min(i + 23, length));
    // strcat(data, chunk.c_str());
    strlcat(data, chunk.c_str(), sizeof(data));
    DBG(chunk);
    DBG(strlen(data));
    // DBG(data);
    writeReg(ADD_CMD_REG, data, strlen(data));
    data[2] = 0;
  }

  uint8_t ret = 0;
  readReg(CMD_ERROR_REG, &ret, 1);
  if (0 != ret) {   // Try adding it again
    DBG("addCommandWord error");
    DBG(ret);
    for (int i = 0; i < length; i += 23) {
      String chunk = str.substring(i, min(i + 23, length));
      // strcat(data, chunk.c_str());
      strlcat(data, chunk.c_str(), sizeof(data));
      DBG(chunk);
      DBG(strlen(data));
      // DBG(data);
      writeReg(ADD_CMD_REG, data, strlen(data));
      data[2] = 0;
    }
    readReg(CMD_ERROR_REG, &ret, 1);
  }

  if (0 == ret) {
    return true;
  } else {
    return false;
  }
}

void DFRobot_ISRModule::delCommandWord(uint8_t num)
{
  writeReg(DEL_CMD_BY_ID_REG, &num, 1);
}

void DFRobot_ISRModule::delCommandWord(String str)
{
  int length = str.length();
  char data[30] = { (char)length };   // Only 32 bytes can be sent at a time
  for (int i = 0; i < length; i += 23) {
    String chunk = str.substring(i, min(i + 23, length));
    strlcat(data, chunk.c_str(), sizeof(data));
    writeReg(DEL_CMD_BY_STR_REG, data, strlen(data));
    data[1] = 0;
  }

  uint8_t ret = 0;
  readReg(CMD_ERROR_REG, &ret, 1);
  if (0 != ret) {   // Try deleting it again
    DBG("delCommandWord error");
    DBG(ret);
    for (int i = 0; i < length; i += 23) {
      String chunk = str.substring(i, min(i + 23, length));
      strlcat(data, chunk.c_str(), sizeof(data));
      writeReg(DEL_CMD_BY_STR_REG, data, strlen(data));
      data[1] = 0;
    }
  }
}

uint8_t DFRobot_ISRModule::getKeywordID(void)
{
  uint8_t id = 0xFF;
  readReg(VOICE_ID_REG, &id, 1);
  // DBG(id);
  return id;
}

DFRobot_ISRModule_I2C::DFRobot_ISRModule_I2C(TwoWire* pWire, uint8_t addr)
{
  _pWire = pWire;
  _deviceAddr = addr;
}

bool DFRobot_ISRModule_I2C::begin(eSpeechModelType_t type, uint8_t duration)
{
  _pWire->begin();
  _pWire->beginTransmission(_deviceAddr);
  if (_pWire->endTransmission() != 0) {
    DBG("I2C init error!!!!");
    return false;
  }
  return DFRobot_ISRModule::begin(type, duration);
}

void DFRobot_ISRModule_I2C::writeReg(uint8_t reg, void* pBuf, size_t size)
{
  if (pBuf == NULL) {
    DBG("pBuf ERROR!! : null pointer");
  }
  uint8_t* _pBuf = (uint8_t*)pBuf;
  _pWire->beginTransmission(_deviceAddr);
  _pWire->write(CMD_WRITE_REGBUF);   // Custom packets are valid for this module
  _pWire->write(reg);
  _pWire->write(size);   // Custom packets are valid for this module
  // for (size_t i = 0; i < size; i++) {
  //   _pWire->write(_pBuf[i]);
  // }
  _pWire->write(_pBuf, size);
  _pWire->endTransmission();
  delay(500);
}

uint8_t DFRobot_ISRModule_I2C::readReg(uint8_t reg, void* pBuf, size_t size)
{
  if (pBuf == NULL) {
    DBG("pBuf ERROR!! : null pointer");
  }
  uint8_t* _pBuf = (uint8_t*)pBuf;

  _pWire->beginTransmission(_deviceAddr);
  _pWire->write(CMD_READ_REGBUF);   // Custom packets are valid for this module
  _pWire->write(reg);
  _pWire->write(size);   // Custom packets are valid for this module
  if (_pWire->endTransmission() != 0) {
    DBG("endTransmission ERROR!!");
    return 1;
  }
  delay(50);
  _pWire->requestFrom((uint8_t)_deviceAddr, (uint8_t)(size+1));
  _pWire->read();   // Invalid first may be error data bytes only i2c
  size_t i = 0;
  while (_pWire->available()) {
    _pBuf[i++] = _pWire->read();
  }
  return 0;
}

#if defined(ARDUINO_AVR_UNO) || defined(ESP8266)
DFRobot_ISRModule_UART::DFRobot_ISRModule_UART(SoftwareSerial* sSerial, uint32_t Baud)
{
  _serial = sSerial;
  _baud = Baud;
  // _serial->begin(_baud);
}
#else
DFRobot_ISRModule_UART::DFRobot_ISRModule_UART(HardwareSerial* hSerial, uint32_t Baud, uint8_t txpin, uint8_t rxpin)
{
  _serial = hSerial;
  _baud = Baud;
  _txpin = txpin;
  _rxpin = rxpin;
}
#endif

bool DFRobot_ISRModule_UART::begin(eSpeechModelType_t type, uint8_t duration)
{
#ifdef ESP32
  _serial->begin(_baud, SERIAL_8N1, _txpin, _rxpin);
  // #elif defined(ARDUINO_AVR_UNO) || defined(ESP8266)
    // nothing use software
#else
  _serial->begin(_baud);  // M0 cannot create a begin in a construct
#endif

  uint8_t data[2] = { 0 };
  readReg(REG_ISR_PID, data, 2);
  if (MODULE_DFR0715_PID != ((data[0] << 8) | data[1])) {
    DBG(((data[0] << 8) | data[1]), HEX);
    return false;
  }
  return DFRobot_ISRModule::begin(type, duration);
}

void DFRobot_ISRModule_UART::writeReg(uint8_t reg, void* pBuf, size_t size)
{
  if (pBuf == NULL) {
    DBG("pBuf ERROR!! : null pointer");
  }
  uint8_t* _pBuf = (uint8_t*)pBuf;

  _serial->write(CMD_WRITE_REGBUF);
  _serial->write(reg);
  _serial->write(size);
  for (size_t i = 0; i < size; i++) {
    _serial->write(_pBuf[i]);
  }
  delay(500);
}

uint8_t DFRobot_ISRModule_UART::readReg(uint8_t reg, void* pBuf, size_t size)
{
  if (pBuf == NULL) {
    DBG("pBuf ERROR!! : null pointer");
  }
  uint8_t* _pBuf = (uint8_t*)pBuf;

  _serial->write(CMD_READ_REGBUF);   // read type
  _serial->write(reg);   // read reg
  _serial->write(size);   // read len
  delay(50);
  uint32_t nowtime = millis();
  size_t i = 0;
  while (millis() - nowtime < TIME_OUT) {
    while (_serial->available()) {
      _pBuf[i++] = _serial->read();
    }
    if (i == size) break;
  }
  return 0;
}
