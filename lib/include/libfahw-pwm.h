#ifndef __FRIENDLYARM_HARDWARE_PWM_H__
#define __FRIENDLYARM_HARDWARE_PWM_H__

#define PWM_IOCTL_SET_FREQ      (0x1)
#define PWM_IOCTL_STOP          (0x0)
#define PWM_IOCTL_CONFIG        (0x4)
#define PWM_IOCTL_RELEASE       (0x8)

int PWMPlay(int pin, int freq, int duty);
int PWMStop(int pin);

#endif
