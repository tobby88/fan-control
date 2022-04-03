/*
 * main.c
 *
 * Created: 4/2/2022 2:59:22 PM
 *  Author: tobby
 */ 

// define your personal settings here:
#define F_CPU 4800000L // system frequency in Hz
#define F_PWM 25000    // pwm frequency in Hz
// throw an error if the pwm frequency is to low or the system frequency is to high for the 8-bit-timer
#if (F_CPU/F_PWM>255)
	#error F_CPU to big or F_PWM to small for 8-bit timer
#endif
#define PWM_STEPS_PERCENT {0,10,25,50,60,70,80,85,90,95,100} // define the steps in percent that should be choosable via button

// include necessary libraries
#include <stddef.h>
#include <xc.h>
#include <avr/eeprom.h>
#include <util/delay.h>

// definition for space in EEPROM to save the last step, chosen before reboot
uint8_t ee_pwm_step EEMEM = 0;

int main(void)
{
	DDRB = (1<<PB1); // set all pins as input besides pwm pin
	PORTB = ~(1<<PB1); // set all pins to high (pull up) besides pwm pin

	// calculate timer (hardware pwm) values
	uint8_t upper_limit = F_CPU/F_PWM-1;
	uint8_t pwm_steps_percent[] = PWM_STEPS_PERCENT;
	size_t pwm_steps_count = sizeof(pwm_steps_percent)/sizeof(pwm_steps_percent[0]);
	uint8_t pwm_steps[pwm_steps_count];
	for(uint8_t i=0; i<pwm_steps_count; i++)
		pwm_steps[i] = pwm_steps_percent[i]*upper_limit/100;
	uint8_t pwm = 0;
	
	TCCR0A = (1<<WGM01) | (1<<WGM00) | (1<<COM0B1); // hardware pwm on hardware-pwm-pin-b (PB1), timer in fast pwm mode (7), clear on compare match, set on top
	TCCR0B = (1<<WGM02) | (1<<CS00); // timer in fast pwm mode (7), no divider, run timer with system clock
	OCR0A = upper_limit; // set compare register A, so the timer will run at the frequency defined in F_PWM
	
	// read last chosen step from EEPROM
	uint8_t current_pwm_step=eeprom_read_byte(&ee_pwm_step);
    while(1)
    {
		// set pwm to the desired pulse width
		pwm=pwm_steps[current_pwm_step];
		OCR0B = pwm;
		
		// wait for PB3 going down (button pressed)
		while((PINB & (1<<PB3))) {}
		_delay_ms(50);
		// wait for PB3 going up again (button released)
		while(!(PINB & (1<<PB3))) {}
		_delay_ms(50);
		// button has been pressed => loops above end, so increment the step to choose the next pwm step from the list now:
		current_pwm_step++;
		
		// if at the end of the list: start over at the beginning
		if(current_pwm_step>=pwm_steps_count)
			current_pwm_step=0;
		
		// save value of the chosen step to EEPROM
		eeprom_write_byte(&ee_pwm_step, current_pwm_step);
    }
	return 0;
}
