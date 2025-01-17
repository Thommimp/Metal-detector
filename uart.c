#include <avr/io.h>

void uart0_Init(unsigned int ubrr) {
    // Enable receive and transmit
    UCSR0B = (1 << RXEN0) | (1 << TXEN0);
    
    // Set frame format: 8 data bits, no parity, 1 stop bit
    UCSR0C = (1 << UCSZ00) | (1 << UCSZ01);
    
    // Set baud rate (16 MHz clock, 9600 baud rate)
    UBRR0H = (unsigned char)(ubrr >> 8);
    UBRR0L = (unsigned char)ubrr;
    
    // Enable double speed mode if necessary
    UCSR0A = (1 << U2X0);    // Full duplex mode
}

void putchUSART0(char tx) {
    // Wait for empty transmit buffer
    while (!(UCSR0A & (1 << UDRE0)));
    // Put data into the buffer, sends the data
    UDR0 = tx;
}

void putsUSART0(char *s) {
    while (*s) {
        putchUSART0(*s);
        s++;
    }
}