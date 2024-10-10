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
    ATMega328PTimer1PWM() : _top(0), _resolution(1023) {}

    /**
     * Initializes Timer1 for PWM generation with the given period.
     * 
     * @param periodMicros The desired PWM period in microseconds.
     * @return True if a valid prescaler and TOP value were found and initialized, otherwise false.
     */
	bool initialize(uint32_t periodMicros)
    {
      noInterrupts();  // Disable interrupts during setup

      bool initialized = false;

      // Clear registers and initialize variables
      TCCR1A = 0;
      TCCR1B = 0;
      TCNT1 = 0;

      // Configure Timer1 for Fast PWM mode (mode 14, ICR1 as TOP)
      TCCR1A = (1 << COM1A1) | (1 << COM1B1) | (1 << WGM11);
      TCCR1B = (1 << WGM13) | (1 << WGM12);

      const uint16_t prescalerValues[] = {1, 8, 64, 256, 1024};
      const size_t numPrescalers = sizeof(prescalerValues) / sizeof(prescalerValues[0]);
      const uint32_t F_CPU_MHZ = F_CPU / 1000000UL;  // Convert CPU frequency to MHz

      // Find the appropriate prescaler and ICR1 value
      for (size_t i = 0; i < numPrescalers; i++)
      {
        _top = ((F_CPU_MHZ * periodMicros) / prescalerValues[i]) - 1;

        if (_top <= 65535)
        {
          ICR1 = _top; // Set Output Compare Register
          setPrescaler(prescalerValues[i]);
          initialized = true;

          break;  // Exit the loop once a valid configuration is found
        }
      }
	  
	  pinMode(9, OUTPUT);
      pinMode(10, OUTPUT);

      interrupts();  // Re-enable interrupts after setup

      return initialized;
    }

	/**
     * Sets the resolution for PWM output.
     * 
     * @param resolutionInBits The desired PWM resolution in bits (1-16). If the value exceeds the 
     *        timer's capability, it will be constrained to a valid range.
     * @return The actual resolution set in bits.
     */
    uint8_t analogWriteResolution(uint8_t resolutionInBits)
    {
      resolutionInBits = min(resolutionInBits, 16);

      uint8_t actualResulutionInBits = fastLog2(_top);

      if (resolutionInBits > actualResulutionInBits)
      {
        _resolution = (1 << actualResulutionInBits) - 1;
        return actualResulutionInBits;
      }
      else
      {
        _resolution = (1 << resolutionInBits) - 1;
        return resolutionInBits;
      }
    }

    /**
     * Sets the duty cycle for PWM output on the specified pin.
     * 
     * @param pin The pin number (9 or 10) for which the duty cycle is set.
     * @param dutyCycle The duty cycle value to set, between 0 and the resolution value.
     */
    void analogWrite(uint8_t pin, uint16_t dutyCycle)
    {
      if (dutyCycle > _resolution) {
        dutyCycle = _resolution;
      }

      uint16_t ocrValue = ((uint32_t)dutyCycle * _top) / _resolution;

      switch (pin)
      {
        case 9: OCR1A = ocrValue; break;
        case 10: OCR1B = ocrValue; break;
        default: /* do nothing */ break;
      }
    }

    /**
     * Sets the duty cycle for pin 9 as a floating-point value.
     * 
     * @param value The duty cycle as a percentage (0.0 to 1.0), where 1.0 represents 100%.
     */
    void setDutyCyclePin9(float value)
    {
      OCR1A = value * _top;
    }

	/**
     * Sets the duty cycle for pin 10 as a floating-point value.
     * 
     * @param value The duty cycle as a percentage (0.0 to 1.0), where 1.0 represents 100%.
     */
    void setDutyCyclePin10(float value)
    {
      OCR1B = value * _top;
    }

	/**
     * Gets the current TOP value (ICR1) for Timer1.
     * 
     * @return The current TOP value.
     */
    uint16_t getTop() const
    {
      return _top;
    }

  private:
    /**
     * Sets the appropriate prescaler for Timer1 based on the given value.
     * 
     * @param prescaler The prescaler value (1, 8, 64, 256, or 1024).
     */
    void setPrescaler(uint16_t prescaler)
    {
      switch (prescaler)
      {
        case 1:
          TCCR1B |= (1 << CS10);
          break;
        case 8:
          TCCR1B |= (1 << CS11);
          break;
        case 64:
          TCCR1B |= (1 << CS11) | (1 << CS10);
          break;
        case 256:
          TCCR1B |= (1 << CS12);
          break;
        case 1024:
          TCCR1B |= (1 << CS12) | (1 << CS10);
          break;
        default:
          TCCR1B = 0; // Timer stopped if no valid prescaler
      }
    }

	/**
     * Computes the base-2 logarithm of a given number.
     * 
     * @param x The value to compute the log2 of.
     * @return The base-2 logarithm of the input value.
     */
    uint8_t fastLog2(uint16_t x)
    {
      if (x == 0) {
        return 0;  // Prevent infinite loop for zero input
      }

      uint8_t result = 0;

      while (x >>= 1)  // Shift right until x becomes 0
      {
        result++;
      }

      return result;
    }

    uint32_t _top;
    uint16_t _resolution;
};

#endif // ATMEGA328P_TIMER1_PWM_H
