/*
 * labb1_d0003e.c
 *
 * Created: 2023-01-18 10:22:46
 * Author : Simon
 */ 

#include <avr/io.h>




void LCD_Init(void);
{
	/* Use 32 kHz crystal oscillator */
	/* 1/3 Bias and 1/3 duty, SEG21:SEG24 is used as port pins */
	LCDCRB = (1<<LCDCS) | (1<<LCDMUX1)| (1<<LCDPM2);
	/* Using 16 as prescaler selection and 7 as LCD Clock Divide */
	/* gives a frame rate of 49 Hz */
	LCDFRR = (1<<LCDCD2) | (1<<LCDCD1);
	/* Set segment drive time to 125 �s and output voltage to 3.3 V*/
	LCDCCR = (1<<LCDDC1) | (1<<LCDCC3) | (1<<LCDCC2) | (1<<LCDCC1);
	/* Enable LCD, default waveform and no interrupt enabled */
	LCDCRA = (1<<LCDEN);
}

void LCD_update(unsigned char data1 ,char data2);
{
	/* LCD Blanking and Low power waveform are unchanged. */
	/* Update Display memory. */
	LCDDR0 = data1;
	LCDDR6 = data2;
}

void LCD_disable(void);
{
	/* Wait until a new frame is started. */
	while ( !(LCDCRA & (1<<LCDIF)) )
	;
	/* Set LCD Blanking and clear interrupt flag */
	/* by writing a logical one to the flag. */
	LCDCRA = (1<<LCDEN)|(1<<LCDIF)|(1<<LCDBL);
	/* Wait until LCD Blanking is effective. */
	while ( !(LCDCRA & (1<<LCDIF)) )
	;
	/* Disable LCD */
	LCDCRA = (0<<LCDEN);
}

void writeChar(char ch, int pos) {
	if (pos < 0 || pos > 5) {
		return; // do nothing if pos is outside the supported range
	}
	if (ch < '0' || ch > '9') {
		ch = ' '; // display a blank for anything outside 0 - 9
	}
	
int main(void)
{
	CLKPR = 1000000000000000;
	// probably same as doing
	//CLKPR = (1 << CLKPCE); 
	//CLKPR = 0x00;
		
}