#ifndef U8G2LIB_H
#define U8G2LIB_H

#include <u8g2.h>
#include "gpio.h"
#include "i2c.h"
#include "spi.h"

static int i2c_device;
static const char i2c_bus[] = "/dev/i2c-1";

static int spi_device;
static const char spi_bus[] = "/dev/spidev0.0";

uint8_t u8x8_arm_linux_gpio_and_delay(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr);
uint8_t u8x8_byte_arm_linux_hw_i2c(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr);
uint8_t u8x8_byte_arm_linux_hw_spi(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr);

#endif
