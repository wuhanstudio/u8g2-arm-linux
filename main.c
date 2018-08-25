#define USE_MY_I2C
// #define USE_FA_I2C

#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <u8g2.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <libfahw-gpio.h>

#ifdef USE_MY_I2C
	#include <linux/i2c-dev.h>
#endif

#ifdef USE_FA_I2C
	#include <libfahw-i2c.h>
	#include <i2c-dev.h>
#endif

u8g2_t u8g2;

int fd;
int openI2CDevice()
{
	int i2c_fd;
	char filename[40];
	sprintf(filename,"/dev/i2c-0");
	if ((i2c_fd = open(filename,O_RDWR)) < 0) {
		printf("Failed to open the bus.");
		/* ERROR HANDLING; you can check errno to see what went wrong */
		return -1;
	}
	return i2c_fd;
}

#ifdef USE_MY_I2C
void setI2CSlave(int i2c_fd,int addr)			//Wire.begin();
{	
	if (ioctl(i2c_fd, I2C_SLAVE, addr) < 0) {
    	printf("Failed to acquire bus access and/or talk to slave.\n");
    	/* ERROR HANDLING; you can check errno to see what went wrong */
    	// exit(1);
	}
}
#endif

void I2CWriteBytes(int i2c_fd, uint8_t* data, uint8_t length)
{
	int i = 0;

	// for(i = 0; i < length; i++)
	// {
	// 	printf("%02x ", data[i]);
	// }
	// printf("\n");
	
	#ifdef USE_FA_I2C
	for(i = 0; i < length; i++)
	{	
		I2CWriteByte(fd, data[i], 2);
	}
	#endif

	#ifdef USE_MY_I2C
	if (write(i2c_fd, data, length) != length) {
    	/* ERROR HANDLING: i2c transaction failed */
		printf("Failed to write to the i2c bus.\n");
	}
	#endif
}

void sleep_ms(unsigned long milliseconds)
{
	struct timespec ts;
	ts.tv_sec = milliseconds / 1000;
	ts.tv_nsec = (milliseconds % 1000) * 1000000;
	nanosleep(&ts, NULL);
}

void sleep_us(unsigned long microseconds)
{
	struct timespec ts;
	ts.tv_sec = microseconds / 1000 / 1000;
	ts.tv_nsec = (microseconds % 1000000) * 1000;
	nanosleep(&ts, NULL);
}

void sleep_ns(unsigned long nanoseconds)
{
	struct timespec ts;
	ts.tv_sec = 0;
	ts.tv_nsec = nanoseconds;
	nanosleep(&ts, NULL);
}

uint8_t u8x8_arm_linux_gpio_and_delay(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr)
{
  switch(msg)
  {
	case U8X8_MSG_DELAY_NANO:			// delay arg_int * 1 nano second
		sleep_ns(arg_int);
		break;    
	case U8X8_MSG_DELAY_100NANO:		// delay arg_int * 100 nano seconds
		sleep_ns(arg_int * 100);
		break;
	case U8X8_MSG_DELAY_10MICRO:		// delay arg_int * 10 micro seconds
		sleep_us(arg_int * 10);
		break;
	case U8X8_MSG_DELAY_MILLI:			// delay arg_int * 1 milli second
		sleep_ms(arg_int);
		break;
	default:
		return 0;
  }
  return 1;
}

uint8_t u8x8_byte_arm_linux_hw_i2c(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr)
{
	uint8_t i = 0;
	static uint8_t buffer[32];		/* u8g2/u8x8 will never send more than 32 bytes between START_TRANSFER and END_TRANSFER */
  	static uint8_t buf_idx;
  	uint8_t *data;
	switch(msg)
	{
		case U8X8_MSG_BYTE_SEND:
		    data = (uint8_t *)arg_ptr;      
      		while( arg_int > 0 )
      		{
				buffer[buf_idx++] = *data;
				data++;
				arg_int--;
      		}
			break;
		case U8X8_MSG_BYTE_INIT:
			fd = openI2CDevice();
			// printf("I2C File Descriptor: %d\n", fd);
			break;
		case U8X8_MSG_BYTE_SET_DC:
			break;
		case U8X8_MSG_BYTE_START_TRANSFER:
			setI2CSlave(fd, u8x8_GetI2CAddress(u8x8)>>1);
			buf_idx = 0;
			// printf("I2C Address: %02x\n", u8x8_GetI2CAddress(u8x8)>>1);
			break;
		case U8X8_MSG_BYTE_END_TRANSFER:
			I2CWriteBytes(fd, buffer, buf_idx);
			break;
		default:
			return 0;
	}
	return 1;
}

#define LED_BLINK_TIMES     10

#define I2C_WRITE

void led_test()
{
	int ledPin = GPIO_PIN8; 
	int i = 0;
	int ret = -1;

	if ((ret = exportGPIOPin(ledPin)) == -1) {   
		printf("exportGPIOPin(%d) failed\n", ledPin);
	}
	if ((ret = setGPIODirection(ledPin, GPIO_OUT)) == -1) {
		printf("setGPIODirection(%d) failed\n", ledPin);
	}

	for (i = 0; i < LED_BLINK_TIMES; i++) {
		if (i % 2) {
			ret = setGPIOValue(ledPin, GPIO_HIGH);
		} else {
			ret = setGPIOValue(ledPin, GPIO_LOW);
		}
		if (ret == -1) {
			printf("setGPIOValue(%d) failed\n", ledPin);
		}
		printf("LED blinking times %d\n", i);
		sleep(1);
	}
	unexportGPIOPin(ledPin);	
}

void i2c_setup()
{
	fd = openI2CDevice();
	setI2CSlave(fd, 0x48);
}

void i2c_read_test(){
	printf("%d\n", I2CReadByteFrom(fd, 0x43, 50));
	sleep(1);
}

void i2c_write_test(){

	int i = 0;
	for(i=0;i<256;i+=5)
	{
		I2CWriteByteTo(fd, 0xFF, i, 50);
		sleep_ms(5);
	}
	for(i=255;i>0;i-=5)
	{
		I2CWriteByteTo(fd, 0xFF, i, 50);
		sleep_ms(5);
	}
}

int main(void)
{

	u8g2_Setup_ssd1306_i2c_128x64_noname_f( &u8g2, U8G2_R0, u8x8_byte_arm_linux_hw_i2c, u8x8_arm_linux_gpio_and_delay);
	
	u8g2_InitDisplay(&u8g2);
	u8g2_SetPowerSave(&u8g2, 0);
	
	/* full buffer example, setup procedure ends in _f */
	u8g2_ClearBuffer(&u8g2);
	u8g2_SetFont(&u8g2, u8g2_font_ncenB14_tr);
	u8g2_DrawStr(&u8g2, 1, 18, "U8g2 on AVR");
	u8g2_SendBuffer(&u8g2);

	printf("Initialized ...\n");
	// i2c_setup();
	while(1){
		// led_test();
		// i2c_read_test();
		// i2c_write_test();
	}
}
