#ifndef __FRIENDLYARM_HARDWARE_PCF8591_H__
#define __FRIENDLYARM_HARDWARE_PCF8591_H__

#define  PCF8591_ADDRESS             (0x48)  //PCF8591 address

/*
 * The PCF8591 control byte
 *      7    6    5    4    3    2    1    0
 *   |  0 |AOEF|   AIP   |  0 |AINC|  AICH   |
 */
#define PCF8591_INIT_AD_CONTROL   (0x00)
#define PCF8591_INIT_DA_CONTROL   (0x40)
#define PCF8591_AIN_CHANNEL0      (0)  
#define PCF8591_AIN_CHANNEL1      (1)
#define PCF8591_AIN_CHANNEL2      (2)
#define PCF8591_AIN_CHANNEL3      (3)

/*
 * Channel selection
 * 0x00 = channel 0
 * 0x01 = channel 1
 * 0x02 = channel 2
 * 0x03 = channel 3
 */
#define PCF8591_CONTROL_AICH_MASK   0x00
#define REG_TO_SIGNED(reg)  (((reg) & 0x80) ? ((reg) - 256) : (reg))

int pcf8591Init();
int pcf8591SetCtrl(int devFD, int control);
int pcf8591Read(int devFD,int mode,int channel);
int pcf8591ReadChannels(int devFD,int mode,int channels[], int values[], int count);
int pcf8591Write(int devFD, int analog);
void pcf8591DeInit(int devFD);

#endif
