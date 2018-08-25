#include "led-test.h"
#include <stdio.h>

void led_test(int ledPin)
{
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