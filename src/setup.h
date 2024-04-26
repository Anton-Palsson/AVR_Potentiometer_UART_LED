#ifndef SETUP_H
#define SETUP_H

void initADC();
uint16_t readADC();
void uartInit();
void uartTransmit(uint8_t data);
void uart_print_uint16(uint16_t value);
void potentiometerRun();
void interruptSetup();
void toggleLed();

#endif