#include "U8g2lib.h"

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
            // printf("CLK:%d, DATA:%d, CS:%d, RST:%d, DC:%d\n", u8x8->pins[U8X8_PIN_SPI_CLOCK], u8x8->pins[U8X8_PIN_SPI_DATA], u8x8->pins[U8X8_PIN_CS], u8x8->pins[U8X8_PIN_RESET], u8x8->pins[U8X8_PIN_DC]);

            if(u8x8->pins[U8X8_PIN_SPI_CLOCK] != U8X8_PIN_NONE)
            {
                exportGPIOPin(u8x8->pins[U8X8_PIN_SPI_CLOCK]);
                setGPIODirection(u8x8->pins[U8X8_PIN_SPI_CLOCK], GPIO_OUT);
                setGPIOValue(u8x8->pins[U8X8_PIN_SPI_CLOCK], GPIO_HIGH);
            }
            if(u8x8->pins[U8X8_PIN_SPI_DATA] != U8X8_PIN_NONE) 
            {
                exportGPIOPin(u8x8->pins[U8X8_PIN_SPI_DATA]);
                setGPIODirection(u8x8->pins[U8X8_PIN_SPI_DATA], GPIO_OUT);
                setGPIOValue(u8x8->pins[U8X8_PIN_SPI_DATA], GPIO_HIGH);
            }
            if(u8x8->pins[U8X8_PIN_CS] != U8X8_PIN_NONE) 
            {
                exportGPIOPin(u8x8->pins[U8X8_PIN_CS]);
                setGPIODirection(u8x8->pins[U8X8_PIN_CS], GPIO_OUT);
                setGPIOValue(u8x8->pins[U8X8_PIN_CS], GPIO_HIGH);
            }

            if(u8x8->pins[U8X8_PIN_RESET] != U8X8_PIN_NONE) 
            {
                exportGPIOPin(u8x8->pins[U8X8_PIN_RESET]);
                setGPIODirection(u8x8->pins[U8X8_PIN_RESET], GPIO_OUT);
                setGPIOValue(u8x8->pins[U8X8_PIN_RESET], GPIO_HIGH);
            }
            if(u8x8->pins[U8X8_PIN_DC] != U8X8_PIN_NONE) 
            {
                exportGPIOPin(u8x8->pins[U8X8_PIN_DC]);
                setGPIODirection(u8x8->pins[U8X8_PIN_DC], GPIO_OUT);
                setGPIOValue(u8x8->pins[U8X8_PIN_DC], GPIO_HIGH);
            }

            break;

        case U8X8_MSG_GPIO_SPI_CLOCK:  
            //Function to define the logic level of the clockline  
            setGPIOValue(u8x8->pins[U8X8_PIN_SPI_CLOCK], arg_int);  
            break;

        case U8X8_MSG_GPIO_SPI_DATA:
            //Function to define the logic level of the data line to the display  
            setGPIOValue(u8x8->pins[U8X8_PIN_SPI_DATA], arg_int);  
            break;

        case U8X8_MSG_GPIO_CS:
            // Function to define the logic level of the CS line  
            setGPIOValue(u8x8->pins[U8X8_PIN_CS], arg_int);
            break;

        case U8X8_MSG_GPIO_DC:
            //Function to define the logic level of the Data/ Command line  
            setGPIOValue(u8x8->pins[U8X8_PIN_DC], arg_int);
            break;  

        case U8X8_MSG_GPIO_RESET:
            //Function to define the logic level of the RESET line
            setGPIOValue(u8x8->pins[U8X8_PIN_RESET], arg_int);
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

uint8_t u8x8_byte_arm_linux_hw_spi(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr) 
{    
    uint8_t *data;
    uint8_t tx[2], rx[2];
    static uint8_t buf_idx, internal_spi_mode; 

    switch(msg) 
    {
        case U8X8_MSG_BYTE_SEND:
            data = (uint8_t *)arg_ptr;
            // printf("Buffering Data %d \n", arg_int);

            while( arg_int > 0) 
            {
                // printf("%.2X ", (uint8_t)*data);
                tx[0] = (uint8_t)*data;
                struct spi_ioc_transfer tr = {
                    .tx_buf = (unsigned long)tx,
                    .rx_buf = (unsigned long)rx,
                    .len = 1,
                    .delay_usecs = 0,
                    .speed_hz = 500000,
                    .bits_per_word = 8,
                };

                SPITransfer(spi_device, &tr);
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
            break;

        case U8X8_MSG_BYTE_END_TRANSFER:      
            buf_idx = 0;
            break;
        
        default:
            return 0;
    }  
    return 1;
}
