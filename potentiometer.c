/*
 * potentiometer.c
 *
 *  Created on: Sep 28, 2018
 *      Author: A_Elziady
 */


 
#include "adc.h"
#include "lcd.h"


void PWM_Timer0_Init(){

	TCNT0 = 0; //initial timer value

	OCR0  = 128;

	DDRB  = DDRB | (1<<PB3); //set OC0 as output pin --> pin where the PWM signal is generated from MC.

	/* Configure timer control register
	 * 1. Fast PWM mode FOC0=0
	 * 2. Fast PWM Mode WGM01=1 & WGM00=1
	 * 3. Clear OC0 when match occurs (non inverted mode) COM00=0 & COM01=1
	 * 4. clock = F_CPU/8 CS00=0 CS01=1 CS02=0
	 */
	TCCR0 = (1<<WGM00) | (1<<WGM01) | (1<<COM01) | (1<<CS01);
}

int main(void)
{

	uint16 res_value;
	uint16 duty_value;
	DDRB = DDRB & (~(1<<PB4)); // configure pin 0 of PORTA as input pin
	DDRB = DDRB | (1<<PB0);    // configure pin 3 of PORTC as output pin
	DDRB = DDRB | (1<<PB1);    // configure pin 4 of PORTC as output pin
	PORTB = PORTB & (~(1<<PB0));
	PORTB = PORTB | (1<<PB1);

	PWM_Timer0_Init();
	LCD_init(); /* initialize LCD driver */
	ADC_init(); /* initialize ADC driver */
	LCD_clearScreen(); /* clear LCD at the beginning */
	/* display this string "ADC Value = " only once at LCD */
	LCD_displayString("ADC Value = ");
	while(1)
	{
		LCD_goToRowColumn(0,12); /* display the number every time at this position */
		res_value = ADC_readChannel(0); /* read channel zero where the potentiometer is connect */
		LCD_intgerToString(res_value); /* display the ADC value on LCD screen */
		duty_value = res_value/4;
		OCR0  = duty_value;

		if(PINB & (1<<PB4))
		{
			PORTB = PORTB ^(1<<PB0);
			PORTB = PORTB ^(1<<PB1);
			//PORTB = PORTB & (~(1<<PB0));
			//PORTB = PORTB | (1<<PB1);
		}

	}
}
