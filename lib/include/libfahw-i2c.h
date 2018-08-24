#ifndef __FRIENDLYARM_HARDWARE_I2C_H__
#define __FRIENDLYARM_HARDWARE_I2C_H__

/* OldInterface: for EEPROM */
int openI2CDevice();
int setI2CSlave(int fd, int slave);
int setI2CTimeout(int fd, int timeout);
int setI2CRetries(int fd, int retries);
int I2CWriteByteTo(int fd, int pos, unsigned char byteData, int wait_ms);
int I2CReadByteFrom(int fd, int pos, int wait_ms);
int I2CWriteByte(int fd, unsigned char byteData, int wait_ms);
int I2CReadByte(int fd, int wait_ms);

#define I2C0_PATH "/dev/i2c-0"

#endif
