# ATMega328PTimer1PWM Library

## Overview

**ATMega328PTimer1PWM** is a lightweight Arduino library designed for the ATMega328P microcontroller (e.g., Arduino Uno, Nano), simplifying the use of Timer1 for generating PWM signals. The library allows for easy configuration of PWM frequency and duty cycles on pins 9 and 10, leveraging the high-resolution capabilities of Timer1.

This library is ideal for applications requiring precise PWM control, such as motor control, LED dimming, or signal generation, freeing the main loop from managing complex timer configurations.

## Features

- Configure Timer1 for Fast PWM mode (mode 14) with flexible frequency settings.
- Set duty cycles on PWM pins 9 and 10 independently.
- Supports custom PWM resolutions (up to 16-bit).
- Automatically handles prescaler configurations for optimal frequency.
- Suitable for Arduino Uno, Nano, and other boards using the ATMega328P.

## Installation

1. Download the repository as a `.zip` file.
2. In the Arduino IDE, go to **Sketch** > **Include Library** > **Add .ZIP Library...**.
3. Select the downloaded `.zip` file.
4. The library is now ready to use!

Alternatively, you can manually copy the `ATMega328PTimer1PWM` folder into your Arduino `libraries` folder.

## Usage

Below is a simple example that demonstrates setting up Timer1 for PWM control on pin 9 with a 1 kHz frequency and 50% duty cycle.

### Example: PWM on Pin 9

```cpp
#include <ATMega328PTimer1PWM.h>

ATMega328PTimer1PWM Timer1;

void setup() {
    Timer1.initialize(1000); // Set up Timer1 with a 1ms (1kHz) period
    Timer1.analogWrite(9, 512); // Set 50% duty cycle (default 10-bit resolution)
}

void loop() {
    // Nothing is required in the main loop for PWM control
}

### API Documentation

- `ATMega328PTimer1PWM::initialize(uint32_t periodMicros)`
    - Initializes Timer1 for PWM with the specified period in microseconds.
    - @param `periodMicros`: The desired PWM period in microseconds.
    - @return `bool`: Returns `true` if initialization succeeds, `false` otherwise.

- `ATMega328PTimer1PWM::analogWriteResolution(uint8_t resolutionInBits)`
    - Sets the resolution of the PWM output.
    - @param `resolutionInBits`: The desired resolution in bits (<=16).
    - @return `uint8_t`: Returns the actual resolution that was set.

- `ATMega328PTimer1PWM::analogWrite(uint8_t pin, uint16_t dutyCycle)`
    - Sets the PWM duty cycle on the specified pin.
    - @param `pin`: The PWM pin (9 or 10).
    - @param `dutyCycle`: The duty cycle (0 to resolution) for the selected pin.

- `ATMega328PTimer1PWM::setDutyCyclePin9(float value)`
    - Sets the duty cycle for pin 9 as a floating-point value between 0.0 and 1.0.
    - @param `value`: A floating-point value representing the duty cycle percentage.

- `ATMega328PTimer1PWM::setDutyCyclePin10(float value)`
    - Sets the duty cycle for pin 10 as a floating-point value between 0.0 and 1.0.
    - @param `value`: A floating-point value representing the duty cycle percentage.

- `ATMega328PTimer1PWM::getTop()`
    - Returns the TOP value (ICR1) used for PWM frequency calculation.
    - @return `uint16_t`: The TOP value for Timer1.

## License

This library is released under the MIT License. See the [LICENSE](LICENSE) file for more details.
