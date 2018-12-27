#include <stdio.h>
#include <u8g2.h>
#include "gpio.h"
#include "i2c.h"
#include "spi.h"

#define OLED_SPI_PIN_RES            201
#define OLED_SPI_PIN_DC             199
#define OLED_SPI_PIN_CS             365

u8g2_t u8g2;

int i2c_device;
const char i2c_bus[] = "/dev/i2c-0";

int spi_device;
const char spi_bus[] = "/dev/spidev0.0";

uint8_t u8x8_arm_linux_gpio_and_delay(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr)
{
    switch(msg)
    {
        case U8X8_MSG_DELAY_NANO:            // delay arg_int * 1 nano second
            sleep_ns(arg_int);
            break;    
    
        case U8X8_MSG_DELAY_100NANO:        // delay arg_int * 100 nano seconds
            sleep_ns(arg_int * 100);
            break;
  
        case U8X8_MSG_DELAY_10MICRO:        // delay arg_int * 10 micro seconds
            sleep_us(arg_int * 10);
            break;
  
        case U8X8_MSG_DELAY_MILLI:            // delay arg_int * 1 milli second
            sleep_ms(arg_int);
            break;
  
        case U8X8_MSG_GPIO_AND_DELAY_INIT:  
            // Function which implements a delay, arg_int contains the amount of ms  
            
            // Export pin 
            exportGPIOPin(u8x8->pins[U8X8_PIN_RESET]);
            exportGPIOPin(u8x8->pins[U8X8_PIN_DC]);
            // exportGPIOPin(u8x8->pins[U8X8_PIN_CS]);
  
            // Set mode
            setGPIODirection(u8x8->pins[U8X8_PIN_RESET], GPIO_OUT);
            setGPIODirection(u8x8->pins[U8X8_PIN_DC], GPIO_OUT);
            // setGPIODirection(u8x8->pins[U8X8_PIN_CS], GPIO_OUT);
  
            // Set value
            setGPIOValue(u8x8->pins[U8X8_PIN_RESET], GPIO_HIGH);
            setGPIOValue(u8x8->pins[U8X8_PIN_DC], GPIO_HIGH);
            // setGPIOValue(u8x8->pins[U8X8_PIN_CS], GPIO_HIGH);
            break;
  
        case U8X8_MSG_GPIO_CS:
            // Function to define the logic level of the CS line  
            // if(arg_int)        setGPIOValue(u8x8->pins[U8X8_PIN_CS], GPIO_HIGH);
            // else        setGPIOValue(u8x8->pins[U8X8_PIN_CS], GPIO_LOW);
            break;
  
        case U8X8_MSG_GPIO_DC:
            //Function to define the logic level of the Data/ Command line  
            if(arg_int) setGPIOValue(u8x8->pins[U8X8_PIN_DC], GPIO_HIGH);
            else setGPIOValue(u8x8->pins[U8X8_PIN_DC], GPIO_LOW);
            break;  
  
        case U8X8_MSG_GPIO_RESET:
            //Function to define the logic level of the RESET line
            if (arg_int) setGPIOValue(u8x8->pins[U8X8_PIN_RESET], GPIO_HIGH);
            else setGPIOValue(u8x8->pins[U8X8_PIN_RESET], GPIO_LOW);
            break;
  
            default:
            return 0;
    }
    return 1;
}

uint8_t u8x8_byte_arm_linux_hw_i2c(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr)
{
    /* u8g2/u8x8 will never send more than 32 bytes between START_TRANSFER and END_TRANSFER */
    static uint8_t buffer[32];        
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
            i2c_device = openI2CDevice(i2c_bus);
            // printf("I2C File Descriptor: %d\n", fd);
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

uint8_t u8x8_byte_arm_linux_hw_spi(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr) {
    
    uint8_t i;
    uint8_t *data;

    uint8_t tx[256];
    uint8_t rx[256];

    static uint8_t buf_idx;
    static uint8_t internal_spi_mode; 
    static uint8_t buffer_tx[256];

    switch(msg) 
    {
        case U8X8_MSG_BYTE_SEND:
            data = (uint8_t *)arg_ptr;
            // printf("Buffering Data %d \n", arg_int);

            while( arg_int > 0) 
            {
                // printf("%.2X ", (uint8_t)*data);
                buffer_tx[buf_idx++] = (uint8_t)*data;
                data++;
                arg_int--;
            }  
            // printf("\n");
            break;

        case U8X8_MSG_BYTE_INIT:
        //u8x8_gpio_SetCS(u8x8, u8x8->display_info->chip_disable_level);
        /* SPI mode has to be mapped to the mode of the current controller, at least Uno, Due, 101 have different SPI_MODEx values */
        /*   0: clock active high, data out on falling edge, clock default value is zero, takover on rising edge */
        /*   1: clock active high, data out on rising edge, clock default value is zero, takover on falling edge */
        /*   2: clock active low, data out on rising edge */
        /*   3: clock active low, data out on falling edge */
            internal_spi_mode =  0;
            switch(u8x8->display_info->spi_mode) 
            {
                case 0: internal_spi_mode; break;
                case 1: internal_spi_mode |= SPI_CPHA; break;
                case 2: internal_spi_mode |= SPI_CPOL; break;
                case 3: internal_spi_mode |= SPI_CPOL; internal_spi_mode |= SPI_CPHA; break;
            }
            // printf("SPI Device Mode Set\n");

            spi_device = openSPIDevice(spi_bus, internal_spi_mode, 8, 500000);
            if (spi_device  < 0 )
            {
                printf("Failed to open SPI Device %s\n", spi_bus);
            }
            else 
            {
                printf("SPI Device Opened\n");
            }
            break;

        case U8X8_MSG_BYTE_SET_DC:
            u8x8_gpio_SetDC(u8x8, arg_int);
            break;

        case U8X8_MSG_BYTE_START_TRANSFER:
            u8x8->gpio_and_delay_cb(u8x8, U8X8_MSG_DELAY_NANO, u8x8->display_info->post_chip_enable_wait_ns, NULL);
            break;

        case U8X8_MSG_BYTE_END_TRANSFER:      
            u8x8->gpio_and_delay_cb(u8x8, U8X8_MSG_DELAY_NANO, u8x8->display_info->pre_chip_disable_wait_ns, NULL);
            memset( tx, 0, ARRAY_SIZE(tx)*sizeof(uint8_t) );
            memset( rx, 0, ARRAY_SIZE(rx)*sizeof(uint8_t) );
  
            // printf("SPI Data Sending %d\n", buf_idx);            
            for (i = 0; i < buf_idx; ++i)
            {
                // printf("%.2X ", buffer_tx[i]);
                tx[i] = buffer_tx[i];
            }
            // printf("\n");
          
            struct spi_ioc_transfer tr = {
                .tx_buf = (unsigned long)tx,
                .rx_buf = (unsigned long)rx,
                .len = buf_idx,
                .delay_usecs = 0,
                .speed_hz = 500000,
                .bits_per_word = 8,
            };
            buf_idx = 0;

            SPITransfer(spi_device, &tr);

            break;
        
        default:
            return 0;
    }  
    return 1;
}

int main(void)
{

    // u8g2_Setup_ssd1306_i2c_128x64_noname_f( &u8g2, U8G2_R0, u8x8_byte_arm_linux_hw_i2c, u8x8_arm_linux_gpio_and_delay);
    u8g2_Setup_ssd1306_128x64_noname_f(&u8g2, U8G2_R0, u8x8_byte_arm_linux_hw_spi, u8x8_arm_linux_gpio_and_delay);
    // u8x8_SetPin(u8g2_GetU8x8(&u8g2), U8X8_PIN_CS, OLED_SPI_PIN_CS);
    u8x8_SetPin(u8g2_GetU8x8(&u8g2), U8X8_PIN_DC, OLED_SPI_PIN_DC);
    u8x8_SetPin(u8g2_GetU8x8(&u8g2), U8X8_PIN_RESET, OLED_SPI_PIN_RES);
    
    u8g2_InitDisplay(&u8g2);
    u8g2_SetPowerSave(&u8g2, 0);
    
    /* full buffer example, setup procedure ends in _f */
    u8g2_ClearBuffer(&u8g2);
    u8g2_SetFont(&u8g2, u8g2_font_ncenB08_tr);
    u8g2_DrawStr(&u8g2, 1, 18, "U8g2 on C-SKY");
    u8g2_SendBuffer(&u8g2);

    u8g2_SetFont(&u8g2, u8g2_font_unifont_t_symbols);
    u8g2_DrawGlyph(&u8g2, 112, 56, 0x2603 );
    u8g2_SendBuffer(&u8g2);

    printf("Initialized ...\n");
    
    return 0;
}
