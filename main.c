#include <stdio.h>
#include <u8g2.h>
#include "i2c.h"

u8g2_t u8g2;
int i2c_device;
char device[] = "/dev/i2c-1";

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
			i2c_device = openI2CDevice(device);
			// printf("I2C File Descriptor: %d\n", fd);
			break;
		case U8X8_MSG_BYTE_SET_DC:
			break;
		case U8X8_MSG_BYTE_START_TRANSFER:
			setI2CSlave(i2c_device, u8x8_GetI2CAddress(u8x8)>>1);
			buf_idx = 0;
			// printf("I2C Address: %02x\n", u8x8_GetI2CAddress(u8x8)>>1);
			break;
		case U8X8_MSG_BYTE_END_TRANSFER:
			I2CWriteBytes(i2c_device, buffer, buf_idx);
			break;
		default:
			return 0;
	}
	return 1;
}

int main(void)
{

	u8g2_Setup_ssd1306_i2c_128x64_noname_f( &u8g2, U8G2_R0, u8x8_byte_arm_linux_hw_i2c, u8x8_arm_linux_gpio_and_delay);
	
	u8g2_InitDisplay(&u8g2);
	u8g2_SetPowerSave(&u8g2, 0);
	
	/* full buffer example, setup procedure ends in _f */
	u8g2_ClearBuffer(&u8g2);
	u8g2_SetFont(&u8g2, u8g2_font_ncenB10_tr);
	u8g2_DrawStr(&u8g2, 1, 18, "U8g2 on NanoPi");
	u8g2_SendBuffer(&u8g2);

	u8g2_SetFont(&u8g2, u8g2_font_unifont_t_symbols);
	u8g2_DrawGlyph(&u8g2, 112, 56, 0x2603 );
	u8g2_SendBuffer(&u8g2);

	printf("Initialized ...\n");
	
	while  (1){

	}
}
