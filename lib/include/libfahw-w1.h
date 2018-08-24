#ifndef __FRIENDLYARM_HARDWARE_W1_H__
#define __FRIENDLYARM_HARDWARE_W1_H__

#define SET_W1GPIO_PIN                  (0)
#define UNSET_W1GPIO_PIN                (1)
#define GET_W1GPIO_PIN                  (4)
#define DS18B20_SYS_PATH                "/sys/devices/w1_bus_master1/"
#define DS18B20_DEV                     "/dev/w1GPIO" 

int ds18b20Init(int pin);
int ds18b20Read(char * temperature);
void ds18b20DeInit(int devFD);

#endif
