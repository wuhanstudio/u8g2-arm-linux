#ifndef __FRIENDLYARM_HARDWARE_MCP23017_H__
#define __FRIENDLYARM_HARDWARE_MCP23017_H__

#define     MCP23017_ADDRESS         (0x27)    

int mcpInit();
int mcpWriteData8(int devFD, unsigned char data);
int mcpWriteData4(int devFD, unsigned char data);
int mcpWriteCmd8(int devFD, unsigned char command);
int mcpWriteCmd4(int devFD, unsigned char command);
void mcpDeInit(int devFD);

int LCD1602KeyInit();
int LCD1602KeyClear(int devFD);
int LCD1602KeyDispChar(int devFD, unsigned char x, unsigned char y, unsigned char data);
int LCD1602KeyDispStr(int devFD, unsigned char x, unsigned char y, char *str);
int LCD1602KeyDispLines(int devFD, char* line1, char* line2);
int LCD1602GetKey(int devFD);
void LCD1602KeyDeInit(int devFD);

#define REG_IODIRA  (0x0)
#define REG_IODIRB  (0x1)
#define REG_IOCON   (0x0a)
#define REG_GPIOA   (0x12)
#define REG_GPIOB   (0x13)

#endif
