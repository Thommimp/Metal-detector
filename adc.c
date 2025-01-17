//initialiseringen af ADC'en
#include <avr/io.h>
void init_ADC(){
	ADCSRA |= (1<<ADEN)|(1<<ADIE);
	
    // Set ADC prescaler to 64 (16 MHz / 64 = 250 kHz ADC clock)
	ADCSRA |= (1 << ADPS2) | (1 << ADPS1);
	
	//using channel 1
	ADMUX=1; 

	 
	ADMUX|=(1<<ADLAR);
	DIDR0 = (1<<1); //channel 1
}