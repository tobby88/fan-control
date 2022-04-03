# fan-control
Microchip/Atmel ATTINY13-based pwm fan controller, used for Noctua 5V fan

ATTINY13 has to run at 4.8 MHz (set it via the fuses)
Adjustments can be done in the define-region in "5V-Luefter-PWM/main.c"
Compile with MPLAB XC8 compiler (free version is ok), included in Microchip Studio >=7.x.

Wiring of the ATTINY13:
VCC: 5V+, additional 100nF capacitor to ground, optional: capacitor with some µF to ground
GND: GND (5V-)
PB0: DNC (**d**o **n**ot **c**onnect), exept for MOSI during programming
PB1: PWM output to fan, MISO during programming
PB2: DNC, clock/SCK during programming
PB3: button, connect the other side of the button to ground
PB4: DNC
PB5: 10kOhm resistor to VCC, 100nF capacitor to GND, RESET during programming
