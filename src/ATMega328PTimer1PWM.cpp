#include "ATMega328PTimer1PWM.h"

ATMega328PTimer1PWM::ATMega328PTimer1PWM() : _top(0), _resolution(1023) {}

bool ATMega328PTimer1PWM::initialize(uint32_t periodMicros)
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

uint8_t ATMega328PTimer1PWM::analogWriteResolution(uint8_t resolutionInBits)
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

void ATMega328PTimer1PWM::analogWrite(uint8_t pin, uint16_t dutyCycle)
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

void ATMega328PTimer1PWM::setDutyCyclePin9(float value)
{
  OCR1A = value * _top;
}

void ATMega328PTimer1PWM::setDutyCyclePin10(float value)
{
  OCR1B = value * _top;
}

uint16_t ATMega328PTimer1PWM::getTop() const
{
  return _top;
}

void ATMega328PTimer1PWM::setPrescaler(uint16_t prescaler)
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

uint8_t ATMega328PTimer1PWM::fastLog2(uint16_t x)
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
