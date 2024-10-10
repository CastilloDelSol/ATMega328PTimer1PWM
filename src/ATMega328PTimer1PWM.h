#ifndef ATMEGA328P_TIMER1_PWM_H
#define ATMEGA328P_TIMER1_PWM_H

#include <Arduino.h>

/**
 * Class to manage Timer1 on an ATmega328P microcontroller for generating PWM signals.
 * 
 * This class configures Timer1 for Fast PWM mode and provides methods to set the PWM 
 * frequency, duty cycle, and resolution. It works with pins 9 and 10, which are tied 
 * to Timer1 on the ATmega328P.
 */
class ATMega328PTimer1PWM {
  public:
    ATMega328PTimer1PWM();

    /**
     * Initializes Timer1 for PWM generation with the given period.
     * 
     * @param periodMicros The desired PWM period in microseconds.
     * @return True if a valid prescaler and TOP value were found and initialized, otherwise false.
     */
	bool initialize(uint32_t periodMicros);

	/**
     * Sets the resolution for PWM output.
     * 
     * @param resolutionInBits The desired PWM resolution in bits (1-16). If the value exceeds the 
     *        timer's capability, it will be constrained to a valid range.
     * @return The actual resolution set in bits.
     */
    uint8_t analogWriteResolution(uint8_t resolutionInBits);

    /**
     * Sets the duty cycle for PWM output on the specified pin.
     * 
     * @param pin The pin number (9 or 10) for which the duty cycle is set.
     * @param dutyCycle The duty cycle value to set, between 0 and the resolution value.
     */
    void analogWrite(uint8_t pin, uint16_t dutyCycle);
    /**
     * Sets the duty cycle for pin 9 as a floating-point value.
     * 
     * @param value The duty cycle as a percentage (0.0 to 1.0), where 1.0 represents 100%.
     */
    void setDutyCyclePin9(float value);
	
	/**
     * Sets the duty cycle for pin 10 as a floating-point value.
     * 
     * @param value The duty cycle as a percentage (0.0 to 1.0), where 1.0 represents 100%.
     */
    void setDutyCyclePin10(float value);

	/**
     * Gets the current TOP value (ICR1) for Timer1.
     * 
     * @return The current TOP value.
     */
    uint16_t getTop() const;

  private:
    /**
     * Sets the appropriate prescaler for Timer1 based on the given value.
     * 
     * @param prescaler The prescaler value (1, 8, 64, 256, or 1024).
     */
    void setPrescaler(uint16_t prescaler);

	/**
     * Computes the base-2 logarithm of a given number.
     * 
     * @param x The value to compute the log2 of.
     * @return The base-2 logarithm of the input value.
     */
    uint8_t fastLog2(uint16_t x);
	
    uint32_t _top;
    uint16_t _resolution;
};

#endif // ATMEGA328P_TIMER1_PWM_H
