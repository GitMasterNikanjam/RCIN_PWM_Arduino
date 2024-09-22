# RCIN_PWM Library

RCIN_PWM - a small RC hobby receiver library for Arduino.  
Developed by: Mohammad Nikanjam

- This library used for 8 channel RC reciever PWM measurement.   
- This library use Hardware and Software interrupts of arduino boards.   
- using Hardware interrupts pins is for HIGH precision measurement value of RC channel pin.   
- using Software interrupts pins is for LOW precision measurement value of RC channel pin.   
- This library can used for Standard arduino board: UNO, NANO, MEGA2560, DUE

This Library use extra library **PinChangeInterrupt** Library version 1.2.9 or later.

---------------------------------------------------------------------------------

## Types of Interrupts  

  There are two types of interrupts:

  - **Hardware Interrupt:**     
    It happens when an external event occurs like an external interrupt pin changes its state from 
    LOW to HIGH or HIGH to LOW.
    These interrupt are interpreted by hardware and are very fast. These interrupts can be set to trigger on the event 
    of RISING or FALLING or LOW levels.

  - **Software Interrupt:**  
    It happens according to the instruction from the software. For example Timer interrupts are software interrupt.
    Arduinos can have more interrupt pins enabled by using pin change interrupts.
    The library for use software interrupts is PinChangeInterrupt Library 1.2.9. That it can install from Manage Libraries Tab in Arduino IDE Tab.

  ### Arduino Board External Interrupt pins:(Hardware interrupt pins)

  - UNO , NANO   
    2,3

  - Mega  
    2,3,18,19,20,21(pins 20 & 21 are not available to use for interrupts while they are used for I2C communication)

  - Nano 33 IoT  
    2, 3, 9, 10, 11, 13, A1, A5, A7

  - Due   
    all digital pins

-------------------------------------------------------------------------------------------------

## PinChangeInterrupt Library 1.2.9:

PinChangeInterrupt library with a resource friendly implementation (API and LowLevel).   
PinChangeInterrupts are different than normal Interrupts. See detail below.

For more information for PinChangeInterrupt library see this link:  
https://github.com/NicoHood/PinChangeInterrupt

### Supported pins for PinChangeInterrupt:

  - Arduino Uno/Nano/Mini: All pins are usable   

  - Arduino Mega: 10, 11, 12, 13, 50, 51, 52, 53, A8 (62), A9 (63), A10 (64),   
    A11 (65), A12 (66), A13 (67), A14 (68), A15 (69)   

  - Arduino Leonardo/Micro: 8, 9, 10, 11, 14 (MISO), 15 (SCK), 16 (MOSI)   

  - HoodLoader2: All (broken out 1-7) pins are usable   

  - Attiny24/44/84: All pins are usable   

  - Attiny25/45/85: All pins are usable   

  - Attiny13: All pins are usable   

  - Attiny441/841: All pins are usable   

  - Attiny261/461/861: All pins are usable   

  - Attiny2313/2313A/4313: PORTB is usable   

  - ATmega644/ATmega644P/ATmega1284P: All pins are usable  

  - ATmega162: PORTA and PORTC usable   

  - ATmega48/88/168/328/328PB: All pins are usable     

  ----------------------------------------------------------------------------------------------

## Singleton pattern

  To modify the RCIN_PWM_CLASS so that only one instance of this class can be created (implementing the Singleton pattern), we'll make the following changes:

  - Private Constructor: Make the constructor of the RCIN_PWM_CLASS private to prevent creating instances using the new keyword or normal instantiation.

  - Delete Copy Constructor and Assignment Operator: To prevent copying the Singleton instance.

  - Static Method for Access: Implement a static method (getInstance) to provide a global access point to the single instance of the class.

  - Static Private Instance: Use a private static pointer to hold the single instance of the class.