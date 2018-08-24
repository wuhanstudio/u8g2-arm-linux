#ifndef __FRIENDLYARM_HARDWARE_PCF8574_H__
#define __FRIENDLYARM_HARDWARE_PCF8574_H__

#define     PCF8574_ADDRESS         (0x27)    

int pcf8574Init();
int pcf8574WriteData8(int devFD, unsigned char data);
int pcf8574WriteData4(int devFD, unsigned char data);
int pcf8574WriteCmd8(int devFD, unsigned char command);
int pcf8574WriteCmd4(int devFD, unsigned char command);
void pcf8574DeInit(int devFD);

int LCD1602Init();
int LCD1602Clear(int devFD);
int LCD1602DispChar(int devFD, unsigned char x, unsigned char y, unsigned char data);
int LCD1602DispStr(int devFD, unsigned char x, unsigned char y, char *str);
int LCD1602DispLines(int devFD, char* line1, char* line2);
void LCD1602DeInit(int devFD);

#endif
