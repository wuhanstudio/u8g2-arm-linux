#ifndef __FRIENDLYARM_HARDWARE_H__
#define __FRIENDLYARM_HARDWARE_H__

#include "libfahw-filectl.h"
#include "libfahw-gpio.h"
#include "libfahw-i2c.h"
#include "libfahw-iio.h"
#include "libfahw-pcf8591.h"
#include "libfahw-pcf8574.h"
#include "libfahw-adxl34x.h"
#include "libfahw-GPIOSensor.h"
#include "libfahw-hmc5883.h"
#include "libfahw-w1.h"
#include "libfahw-pwm.h"
#include "libfahw-mcp23017.h"
#include "libfahw-bmp180.h"

int getLastError(char* dest, int maxlen);

/* Serial Port */
int openSerialPort(
        const char *strDevName, long baud,
        int dataBits, int stopBits);
int openSerialPortEx(
        const char *strDevName,long baud,
        int dataBits, int stopBits, const char *strParityBit, const char *strFlowCtrl);

/* LED */
#define LED1 1
#define LED2 2
#define LED3 3
#define LED4 4
#define LED_ON   1
#define LED_OFF  0
// ledId: 1,2
int setLedState( int ledID, int ledState );
int getLedState(int ledID);

// ledId: 0~3
int setLedStateForTiny4412(int ledID, int ledState);

/* ADC */
int readADC();
int readADCWithChannels(int channels[], int values[], int count);
int readADCWithChannel(int channel);

/* return 6410 or 210 or 4412 */
int getBoardType();
int Test();

#endif
