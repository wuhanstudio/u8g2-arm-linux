#include "gpio.h"

int writeValueToFile(char* fileName, char* buff) 
{
    int ret;
    FILE *fp = fopen(fileName, "w");
    if (fp == NULL) {
        ret = -1;
    } else {
        ret = fwrite(buff, strlen(buff), 1, fp);
        fclose(fp);
    }
    return ret;
}


int writeIntValueToFile(char* fileName, int value) 
{
    char buff[50];
    sprintf(buff, "%d", value);
    return writeValueToFile(fileName, buff);
}

int readValueFromFile(char* fileName, char* buff, int len) 
{
    int ret = -1;
    FILE *fp = fopen(fileName,"r");
    if (fp == NULL) {
        return -1;
    } else {
        if (fread(buff, sizeof(char), len, fp)>0) {
            ret = 0;
        }
    }
    fclose(fp);
    return ret;
}


int readIntValueFromFile(char* fileName) 
{
    char buff[255];
    memset(buff, 0, sizeof(buff));
    int ret = readValueFromFile(fileName, buff, sizeof(buff)-1);
    if (ret == 0) {
        return atoi(buff);
    }
    return ret;
}


int gpioToPin(const char* gpio)
{
    int i = 0;
    char buff[4];
    int index = 0;
    int gpio_index = -1;

    for(i=0; i<(unsigned)strlen(gpio); i++){
        if( gpio[i] >= '0' && gpio[i] <= '9'){
            buff[index] = gpio[i];
            index++;
            if(gpio_index == -1){
                gpio_index = (gpio[i-1] - 'A') * 32;
            }
        }
    }
    gpio_index += atoi(buff);

    return gpio_index;
}

int exportGPIOPin(int pin) 
{
    int ret = writeIntValueToFile("/sys/class/gpio/export", pin);
    if (ret > 0)
        return 0;
    else 
        return -1;
}

int unexportGPIOPin(int pin) 
{
        
    return writeIntValueToFile("/sys/class/gpio/unexport", pin);
}

int getGPIOValue(int pin) 
{
    
    GPIO_FILENAME_DEFINE(pin, "value")
    
    return readIntValueFromFile(fileName);
}

int setGPIOValue(int pin, int value) 
{
    GPIO_FILENAME_DEFINE(pin, "value")
    return writeIntValueToFile(fileName, value);
}

int setGPIODirection(int pin, int direction) 
{
    
    char directionStr[10];
    GPIO_FILENAME_DEFINE(pin, "direction")
    
    if (direction == GPIO_IN) {
        strcpy(directionStr, "in");
    } else if (direction == GPIO_OUT) {
        strcpy(directionStr, "out");
    } else {
        return -1;
    }
    return writeValueToFile(fileName, directionStr);
}

int getGPIODirection(int pin) 
{
    
    char buff[255] = {0};
    int direction;
    int ret;
    GPIO_FILENAME_DEFINE(pin, "direction")
    
    ret = readValueFromFile(fileName, buff, sizeof(buff)-1);
    if (ret >= 0) {
        if (strncasecmp(buff, "out", 3)==0) {
            direction = GPIO_OUT;
        } else if (strncasecmp(buff, "in", 2)==0) {
            direction = GPIO_IN;
        } else {
            return -1;
        }
        return direction;
    }
    return ret;
}