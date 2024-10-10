#include <ATMega328PTimer1PWM.h>

ATMega328PTimer1PWM Timer1;

void setup() {
    Timer1.initialize(1000); // Set up Timer1 with a 1ms (1kHz) period
    Timer1.analogWrite(9, 512); // Set 50% duty cycle (default 10-bit resolution)
}

void loop() {
    // Nothing is required in the main loop for PWM control
}
