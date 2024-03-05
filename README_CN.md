# DFRobot_ISRModule_ESP32S3
- [English Version](./README.md)

该模块采用由 乐鑫 公司设计的 ESP32S3 芯片，只需要在程序中设定好要识别的关键词语列表并下载进主控的MCU中，语音识别模块就可以对用户说出的关键词语进行识别，并根据程序进行相应的处理。本品不需要用户事先训练和录音就可以完成非特定人语音识别，识别准确率高达95%。

![正反面svg效果图](./resources/images/DFR0715.png) 


## 产品链接(https://www.dfrobot.com.cn/)

    SKU: DFR0715


## 目录

* [概述](#概述)
* [库安装](#库安装)
* [方法](#方法)
* [兼容性](#兼容性y)
* [历史](#历史)
* [创作者](#创作者)


## 概述

  * 设置命令字类型为中文。
  * 设置唤醒持续时长。
  * 添加命令字。
  * 删除命令字。
  * 检索已识别的命令字ID。


## 库安装
这里提供两种使用本库的方法：<br>
1.打开Arduino IDE,在状态栏中的Tools--->Manager Libraries 搜索"DFRobot_ISRModule_ESP32S3"并安装本库.<br>
2.首先下载库文件,将其粘贴到\Arduino\libraries目录中,然后打开examples文件夹并在该文件夹中运行演示.<br>


## 方法

```C++

  /**
   * @fn begin
   * @brief 初始化函数
   * @param type - 语音识别模型的类型: 待机，中文，英文
   * @param duration - 唤醒持续时间. 范围: 0, [6, 120], 单位: 秒. 注：设置0为始终唤醒
   * @return Bool类型，表示初始化结果
   * @retval true NO_ERROR
   */
  virtual bool begin(eSpeechModelType_t type = eSpeechModelChinese, uint8_t duration = 10);

  /**
   * @fn addCommandWord
   * @brief 添加关键词(词条, 编号)
   * @param num - 命令词编号
   * @param str - 命令词词条
   * @return 返回值类型为bool: 成功返回true，失败返回false
   */
  bool addCommandWord(uint8_t num, String str);

  /**
   * @fn delCMDByNUM
   * @brief 删除关键词（编号 or 词条）
   * @param num - 命令词编号
   * @param str - 命令词词条
   * @return None
   */
  void delCommandWord(uint8_t num);
  void delCommandWord(String str);

  /**
   * @fn getKeywordID
   * @brief 获取识别结果（ID）
   * @return 返回识别结果
   * @retval 0 : 识别到唤醒词
   * @retval 1~254 : 识别到对应编号的命令词
   * @retval 0xFF : 未识别到关键词
   */
  uint8_t getKeywordID(void);

```


## 兼容性

MCU                | Work Well    |   Work Wrong    | Untested    | Remarks
------------------ | :----------: | :-------------: | :---------: | :----:
Arduino Uno        |      √       |                 |             |
Arduino MEGA2560   |      √       |                 |             |
Arduino Leonardo   |      √       |                 |             |
FireBeetle-ESP8266 |      √       |                 |             |
FireBeetle-ESP32   |      √       |                 |             |
FireBeetle-M0      |      √       |                 |             |
Micro:bit          |      √       | nonsupport uart |             |

备注: esp8266 SDK: http://arduino.esp8266.com/stable/package_esp8266com_index.json


## 历史

- 2024/01/08 - V1.0.0 版本


## 创作者

Written by qsjhyy(yihuan.huang@dfrobot.com), 2024. (Welcome to our website)

