#ifndef __FRIENDLYARM_HARDWARE_GPIOSENSOR_H__
#define __FRIENDLYARM_HARDWARE_GPIOSENSOR_H__

#define ADD_SENSOR                  (0)
#define DEL_SENSOR                  (1)
#define START_ALL_SENSOR            (4)
#define STOP_ALL_SENSOR             (8)
#define IRQ_TYPE_EDGE_RISING        (0x1)
#define IRQ_TYPE_EDGE_FALLING       (0x2)
#define IRQ_TYPE_EDGE_BOTH          (IRQ_TYPE_EDGE_FALLING | IRQ_TYPE_EDGE_RISING)

struct sensor {
    int gpio;
    int intType;
};
#define ARRAY_SIZE(a)               (sizeof(a) / sizeof((a)[0]))
#define SENSOR_DEVICE               "/dev/sensor"

int sensorInit(struct sensor *dev, int num);
int sensorRead(int devFD, char *buf, int len);
void sensorDeinit(int devFD);

#endif
