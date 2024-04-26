#include <avr/io.h>
#include <stdio.h>
#include <string.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <Arduino.h>
#include "setup.h"

#define SERIAL_8N1 ((1 << UCSZ00) | (1 << UCSZ01))
#define LED_PIN (1 << PD3)
#define F_CPU 16000000UL // Microcontroller's frequency in Hz
#define BAUD 9600 // Desired baud rate

uint16_t val = (ADCL | (ADCH << 8));


void initADC() {
    
    ADMUX |= (1 << REFS0);

    
    ADMUX |= (0 << MUX0);

    
    ADCSRA |= (1 << ADEN) | (1 << ADPS0) | (1 << ADPS1) | (1 << ADPS2);
}

void potentiometerRun() {
    uint16_t adc_value = readADC();

    float voltage = (adc_value * 5.0) / 1023.0;

    // Print ADC value and voltage via UART
    uart_print_uint16(adc_value);

    uartTransmit(' '); // Separate ADC value and voltage with a space

    uart_print_uint16((uint16_t)(voltage * 1000)); // Print voltage in millivolts

    uartTransmit('\n'); // Newline

    _delay_ms(1000); // Delay 1 second
}

void uartTransmit(uint8_t data) {
    // Wait for empty transmit buffer
    while (!(UCSR0A & (1 << UDRE0)));

    // Put data into buffer, sends the data
    UDR0 = data;
}

void uart_print_uint16(uint16_t value) {
    // digit of the value
    uartTransmit((value / 10000) + '0');

    uartTransmit(((value % 10000) / 1000) + '0');

    uartTransmit(((value % 1000) / 100) + '0');

    uartTransmit(((value % 100) / 10) + '0');
    
    uartTransmit((value % 10) + '0');
}

uint16_t readADC() {
    // Start ADC
    ADCSRA |= (1 << ADSC);

    // Wait for conversion to complete
    while (ADCSRA & (1 << ADSC));

    // Read ADC value
    return ADC;
}

void toggleLed() {
    PORTD ^= LED_PIN; // LED
}

void interruptSetup() {
    TCCR1A = 0;         
    TCCR1B = 0;         
    TCCR1B |= (1 << WGM12);
    TCCR1B |= (1 << CS10) | (1 << CS11);
    OCR1A = 31250;      // 200 ms
    TIMSK1 |= (1 << OCIE1A);
}

void uartInit() {
    //baud rate setting
    uint16_t baud_setting = F_CPU / 16 / BAUD - 1;

    //baud rate
    UBRR0H = (baud_setting >> 8);
    UBRR0L = baud_setting;

    // Enable tran/rec
    UCSR0B |= (1 << TXEN0) | (1 << RXEN0);

   
    UCSR0C |= (1 << UCSZ01) | (1 << UCSZ00);
}


ISR(TIMER1_COMPA_vect){
    toggleLed();
}