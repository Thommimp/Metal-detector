#include <Arduino.h>

#include <avr/interrupt.h>

//modules
#include <uart.h>
#include <timer.h>
#include <adc.h>

#include "I2C.h"  //include library for i2c driver
#include "ssd1306.h" //include display driver


#define sampling_freq 8000
#define ADC_length 64


unsigned char adc_array[ADC_length] = {0};
volatile char adc_flag = 0;
volatile char buzzer = 1;

float mag = 0;
float phase = 0;

float ref_mag = 1;
float ref_phase = 0;

float acc_mag = 0;
float acc_phase = 0;

int test = 0;

char buffer[100] = {0};

enum state {
    Rising,
    Full,
    Ready,
};

enum state current_state = Ready;  // Declare and initialize the state variable



 ISR(TIMER1_COMPA_vect) {
  //static boolean rising_edge = 0;

    static uint8_t count = 0; // Keep track of counts for toggling

    
    ++count;
    count %= 4;
    
    if (count == 0) {
        PORTD |= (1 << PD2);  // Set PD2 high
        if (current_state == Ready) {
            current_state = Rising;
        }
      
    }

    if (count == 2) {
        PORTD &= ~(1 << PD2);  // Set PD2 low
    }

    if (current_state == Rising) {
        ADCSRA |= (1 << ADSC);

    }
       
       
}





//ADC conversion complete interrupt service routine
ISR(ADC_vect) {
	static int i = 0;
    adc_array[i] = ADCH;
    i++;
    
    if (i == ADC_length) {
        
      current_state = Full;
      i = 0;
    }

}



void initial() {
  uart0_Init(207);
  initTimer1();
  init_ADC();
  initTimer2();

  I2C_Init();  //initialize i2c interface to display
  InitializeDisplay(); //initialize  display
  

  sei();
  

}

void computeDFT() {
    float real = 0;
    float imag = 0;
    float angle = 0;
    int element = 16; //8kHz 64 elements, and we are looking for 2Khz gives us element 16
    

   

        for (unsigned int n = 0; n < ADC_length; n++) {
            // Compute the angle (2 * pi * k * n / N)
            angle = 2 * M_PI * element * n / ADC_length;

            // Compute real and imaginary components using the angle
            real += adc_array[n] * cos(angle);  // Sum the real part
            imag -= adc_array[n] * sin(angle);  // Sum the imaginary part
        }

  
        // Optional: You can also compute the magnitude of the DFT
         mag = (sqrt((real * real) + (imag * imag)))/ADC_length;
         phase = (atan2(imag, real) * 180)/M_PI;

}

void calculate_changes() {
  // Thresholds for 5% change
    float mag_threshold = ref_mag * 0.5;  // 5% of the reference magnitude
    float mag_diff = 0;

    // Variables to store the new duty cycle values based on magnitude changes
    int duty_cycle = 0;

    // Check if metal is detected by comparing the magnitude
    if (mag >= 0.5 && mag > ref_mag + mag_threshold) {
        putchUSART0((int)mag);
        // Magnitude has risen by more than 5%
        
        // Calculate the percentage difference between the current magnitude and reference magnitude
        mag_diff = (acc_mag - ref_mag) / ref_mag;

        // Convert the percentage difference to a duty cycle (0 to 255 range for analogWrite)
        duty_cycle = (int)(mag_diff * 255) * 100;  // Scale to the range of 0-255
        // Clamp the duty cycle to the range of 0 to 255 to avoid overflow
       if (duty_cycle > 255) {duty_cycle = 255;}
        
        

        analogWrite(PD3, 200);  // Set the duty cycle for the buzzer
        //putchUSART0(duty_cycle);
    } else {
        analogWrite(PD3, 0);
    }
    // When no metal is detected, stop the buzzer immediately
    
}


void send_til_display() {
float test = 0;
putchUSART0(ref_phase);
if(ref_phase > 0) {
    test = phase - ref_phase;
} else {
    test = phase - ref_phase;
}


char mag_buffer[100];
char phase_buffer[100];

acc_mag = acc_mag * 0.9 + 0.1 * mag;
acc_phase = acc_phase * 0.9 + 0.1 * test;









// Format the magnitude and phase into their respective buffers
sprintf(mag_buffer, "Magnitude: %3d", (int)acc_mag);
sprintf(phase_buffer, "Phase: %4d", (int)acc_phase);
// sprintf(ref_mag_buffer, "Ref mag: %d", (int)ref_mag);
// sprintf(ref_phase_buffer, "Ref phase: %d", (int)ref_phase);



// Send the magnitude and phase to the display
sendStrXY(mag_buffer, 0, 0); // Display magnitude at (0,0)
sendStrXY(phase_buffer, 1, 0); // Display phase at (1,0)




  

}



int main() {
 
  initial();

  pinMode(PD2, OUTPUT);
  pinMode(PD7, INPUT_PULLUP);
  pinMode(PD3, OUTPUT);


  uint8_t nulstil = 1;
  print_fonts();
  clear_display();


 

 
  

  while (1) {
   // putchUSART0('a');
   if (nulstil == 1){
   nulstil = digitalRead(PD7);
   }
   




   if (current_state = Full ) {

    computeDFT();

    current_state = Ready;
   }

   if (nulstil == 0) {
    ref_mag = acc_mag;
    ref_phase = phase;
    nulstil = 1;
    

   }
    
   

   calculate_changes();

   send_til_display();



    
    

  }
}