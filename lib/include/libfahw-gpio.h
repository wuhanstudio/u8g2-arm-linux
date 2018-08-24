#ifndef __FRIENDLYARM_HARDWARE_GPIO_H__
#define __FRIENDLYARM_HARDWARE_GPIO_H__

// Direction
#define GPIO_IN                 (1) 
#define GPIO_OUT                (2) 

// Value
#define GPIO_LOW                (0)
#define GPIO_HIGH               (1)

//nanopi
#define GPIO_PIN1 (161) // ETIN1 / GPIO_GPF1
#define GPIO_PIN2 (162) // EINT2 / GPIO_GPF2
#define GPIO_PIN3 (163) // EINT3 / GPIO_GPF3
#define GPIO_PIN4 (164) // EINT4 / GPIO_GPF4
#define GPIO_PIN5 (165) // EINT5 / GPIO_GPF5
#define GPIO_PIN6 (193) // EINT9 / GPIO_GPG1
#define GPIO_PIN7 (195) // EINT11 / GPIO_GPG3
#define GPIO_PIN8 (196) // EINT12 / GPIO_GPG4
#define GPIO_PIN9 (197) // EINT13 / GPIO_GPG5
#define GPIO_PIN10 (198) // EINT14 / GPIO_GPG6
#define GPIO_PIN11 (199) // EINT15 / GPIO_GPG7
#define GPIO_PIN12 (200) // EINT16 / GPIO_GPG8
#define GPIO_PIN13 (201) // EINT17 / GPIO_GPG9
#define GPIO_PIN14 (202) // EINT18 / GPIO_GPG10
#define GPIO_PIN15 (203) // EINT19 / GPIO_GPG11

#define PWM_PIN1 (32) // GPB0
#define PWM_PIN2 (33) // GPB1
#define PWM_PIN3 (34) // GPB2

int exportGPIOPin(int pin);
int unexportGPIOPin(int pin);

// GPIO_LOW or GPIO_HIGH
int setGPIOValue(int pin, int value);
int getGPIOValue(int pin);

// GPIO_IN or GPIO_OUT
int setGPIODirection(int pin, int direction);
int getGPIODirection(int pin);

#endif
