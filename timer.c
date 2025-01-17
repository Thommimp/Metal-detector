#include <avr/io.h>




void initTimer1() {
    // Set the prescaler to divide the clock by 64 (CS30 and CS31 bits set)
    TCCR1B |= (1 << CS10) | (1 << CS11);

    // Set the Timer/Counter Mode to CTC (Clear Timer on Compare Match) mode
    TCCR1B |= (1 << WGM12);


    // Set the compare match value
    OCR1A = 30; // 2 kHz frequency (adjust this as needed)
   // OCR1B = 10; // 8 kHz frequency (adjust this as needed)

    // Enable the Timer/Counter3 Output Compare A Match Interrupt
    TIMSK1 |= (1 << OCIE1A);
    //TIMSK1 |= (1 << OCIE1B);
   // TIMSK3 |= (1 << OCIE3B);
}

void initTimer2() {
 // Configure Timer2 for Fast PWM mode with 2kHz frequency
  // Set WGM22, WGM21, and WGM20 for Fast PWM mode
  TCCR2A = (1 << COM2A1) | (1 << WGM20);  // Non-inverted PWM on OC2A (pin 3)
  TCCR2B = (1 << WGM22) | (1 << CS11);    // Prescaler of 8 (CS11 bit), Fast PWM mode
  
  // Set the PWM frequency to 2kHz (top value for Timer2)
  // 16 MHz / (Prescaler * Frequency) = 16,000,000 / (8 * 2000) = 1000
  OCR2A = 1000;  // Set OCR2A to 1000 for 2kHz PWM signal
}


