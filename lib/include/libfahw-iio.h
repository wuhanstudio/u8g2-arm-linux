#ifndef __FRIENDLYARM_HARDWARE_IIO_H__
#define __FRIENDLYARM_HARDWARE_IIO_H__

// DHT11
#define DHT11_DEV               "/dev/dht11"        
#define	DHT_TEMP 		        (1)
#define	DHT_HUMIDITY	        (2)
#define DHT11_PATH              "/sys/devices/platform/dht11/iio:device0/"   
#define SET_DHT11_PIN           (0)
#define UNSET_DHT11_PIN         (1)
#define GET_DHT11_PIN           (4)
int dht11Read(int type, int *data);
int dht11Init(int pin);
void dht11DeInit(int devFD);


// HCSR04
#define HCSR04_PATH             "/sys/class/hcsr04/value"  
#define HCSR04_MAX_DISTANCE     (200000)
#define HCSR04_PER_METRE        (58)
struct HCSR04_resource {
    int Pin;
};

int Hcsr04Init(int echoPin);
int Hcsr04Read(int *data);
void Hcsr04DeInit();
#endif
