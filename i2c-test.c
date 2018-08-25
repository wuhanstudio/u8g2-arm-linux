#include "i2c-test.h"
#include <libfahw-gpio.h>

void i2c_setup()
{
	i2c_test_device = openI2CDevice();
	setI2CSlave(i2c_test_device, 0x48);
}

void i2c_read_test(){
	printf("%d\n", I2CReadByteFrom(i2c_test_device, 0x43, 50));
	sleep(1);
}

void i2c_write_test(){

	int i = 0;
	for(i=0;i<256;i+=5)
	{
		I2CWriteByteTo(i2c_test_device, 0xFF, i, 50);
		sleep_ms(5);
	}
	for(i=255;i>0;i-=5)
	{
		I2CWriteByteTo(i2c_test_device, 0xFF, i, 50);
		sleep_ms(5);
	}
}