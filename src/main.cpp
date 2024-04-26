#include <avr/io.h>
#include <stdio.h>
#include <string.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <setup.h>

int main() {
    interruptSetup();
    
    uartInit();
    
    initADC();
    
    sei();

    while (1) {
        // ADC/UART
        potentiometerRun();
        _delay_ms(1000);
    }

    return 0;
}
