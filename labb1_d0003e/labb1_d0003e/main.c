/*
 * labb1.cpp
 *
 * Created: 2023-01-18 12:58:25
 * Author : rallo, Simon
 */ 

#include <avr/io.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>

#define BlankValue   0x0000
#define ZeroValue   0x1551 //0001010101010001 0x1 0x5 0x5 0x1 = 0x1551
#define OneValue    0x2080 //0000100000000010 0x2 0x0 0x8 0x0 = 0x2080 
#define TwoValue    0x1e11 //0001000111100001 0x1 0xd 0x1 0x1 = 0x1d11
#define ThreeValue  0x1b11 //0001000110110001 0x1 0xb 0x1 0x1 = 0x1b11
#define FourValue   0x0b50 //0000010110110000 0x0 0xb 0x5 0x0 = 0x0b50
#define FiveValue   0x1b41 //0001010010110001 0x1 0xb 0x4 0x1 = 0x1b41
#define SixValue    0x1f41 //0001010011110001 0x1 0xf 0x4 0x1 = 0x1f41
#define SevenValue  0x0111 //0001000100010000 0x0 0x1 0x1 0x1 = 0x0111
#define EightValue  0x1f51 //0001010111110001 0x1 0xf 0x5 0x1 = 0x1f51
#define NineValue   0x0b51 //0001010110110000 0x0 0xb 0x5 0x1 = 0x0b51 
#define ValueArray (int[]){ZeroValue,OneValue,TwoValue,ThreeValue,FourValue,FiveValue,SixValue,SevenValue,EightValue,NineValue,BlankValue}




void LCD_Init(void) {
	CLKPR = (1 << CLKPCE); 
	CLKPR = 0x00;

    // Use external asynchronous clock source 
    // 1/3 Bias and 1/4 duty, 25 segments enabled 
    LCDCRB = (1<<LCDCS) | (1<<LCDMUX1)| (1<<LCDMUX0)| (1<<LCDPM2) | (1<<LCDPM1) | (1<<LCDPM0);
	// 
    // Using 16 as prescaler selection and 8 as LCD Clock Divide 
    LCDFRR = (1<<LCDCD2) | (1<<LCDCD1) | (1<<LCDCD0);
    // Set segment drive time to 300 us and output voltage to 3.35 V
    LCDCCR = (1<<LCDCC3) | (1<<LCDCC2) | (1<<LCDCC1)| (1<<LCDCC0);
    // Enable LCD, low power waveform, no frame interrupt, no blanking 
    LCDCRA = (1<<LCDEN) | (1<<LCDAB) |  (0<<LCDBL);
}


void LCD_update(unsigned char data1 ,char data2)
{
	/* LCD Blanking and Low power waveform are unchanged. */
	/* Update Display memory. */
	LCDDR0 = data1;
	LCDDR6 = data2;
}

void LCD_disable(void)
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

	int i = 0;
	if (pos < 0 || pos > 5) {
		return; // do nothing if pos is outside the supported range
	}
	if ((int)ch < (0+48) || (int)ch > (9+48)) {
		i=10;
	}else if(ch=='0'){
		i = 0;
	}else if(ch=='1'){
		i = 1;
	}else if(ch=='2'){
		i = 2;
	}else if(ch=='3'){
		i = 3;
	}else if(ch=='4'){
		i = 4;
	}else if(ch=='5'){
		i = 5;
	}else if(ch=='6'){
		i = 6;
	}else if(ch=='7'){
		i = 7;
	}else if(ch=='8'){
		i = 8;
	}else if(ch=='9'){
		i = 9;
	}
	if(pos==0){
		LCDDR0 =  (LCDDR0&0xF0)  |        (0xF&ValueArray[i])<<0;
		LCDDR5 =  (LCDDR5&0xF0)  |  (((0xF<<4)&ValueArray[i])>>4);
		LCDDR10 = (LCDDR10&0xF0) |  (((0xF<<8)&ValueArray[i])>>8);
		LCDDR15 = (LCDDR11&0xF0) | (((0xF<<12)&ValueArray[i])>>12);
		
	} else if(pos==1){
		LCDDR0 =     (LCDDR0&0x0F)  |       ((0xF&ValueArray[i])<<4);
		LCDDR5 =     (LCDDR5&0x0F)  |  (((0xF<<4)&ValueArray[i])>>0);
		LCDDR10 =    (LCDDR10&0x0F) |  (((0xF<<8)&ValueArray[i])>>4);
		LCDDR15 =    (LCDDR15&0x0F) | (((0xF<<12)&ValueArray[i])>>8);
	} else if(pos==2){
		LCDDR1 =  (LCDDR1&0xF0)  |        (0xF&ValueArray[i])<<0;
		LCDDR6 =  (LCDDR6&0xF0)  |  (((0xF<<4)&ValueArray[i])>>4);
		LCDDR11 = (LCDDR11&0xF0) |  (((0xF<<8)&ValueArray[i])>>8);
		LCDDR16 = (LCDDR16&0xF0) | (((0xF<<12)&ValueArray[i])>>12);
		
	} else if(pos==3){
		LCDDR1 =     (LCDDR1&0x0F)  |       ((0xF&ValueArray[i])<<4);
		LCDDR6 =     (LCDDR6&0x0F)  |  (((0xF<<4)&ValueArray[i])>>0);
		LCDDR11 =    (LCDDR11&0x0F) |  (((0xF<<8)&ValueArray[i])>>4);
		LCDDR16 =    (LCDDR16&0x0F) | (((0xF<<12)&ValueArray[i])>>8);
		
	} else if(pos==4){
		LCDDR2 =  (LCDDR2&0xF0)  |        (0xF&ValueArray[i])<<0;
		LCDDR7 =  (LCDDR7&0xF0)  |  (((0xF<<4)&ValueArray[i])>>4);
		LCDDR12 = (LCDDR12&0xF0) |  (((0xF<<8)&ValueArray[i])>>8);
		LCDDR17 = (LCDDR17&0xF0) | (((0xF<<12)&ValueArray[i])>>12);
		
	} else if(pos==5){
		LCDDR2 =     (LCDDR2&0x0F)  |       ((0xF&ValueArray[i])<<4);
		LCDDR7 =     (LCDDR7&0x0F)  |  (((0xF<<4)&ValueArray[i])>>0);
		LCDDR12 =    (LCDDR12&0x0F) |  (((0xF<<8)&ValueArray[i])>>4);
		LCDDR17 =    (LCDDR17&0x0F) | (((0xF<<12)&ValueArray[i])>>8);
		
	}
	

}


void writeLong(long i) {
	int32_t iDigits;
	if(i==0){iDigits=1;}

	else{iDigits = floor(log10( i )) + 1;}

	char Number[6];
	
	
	for(int j=0; j<iDigits; j++){
		 volatile int32_t i_copy = i;

		for(int k=iDigits-j-1; k>0; k--){
			i_copy = i_copy/10;

		} i_copy= i_copy%10;


		Number[j] = (char) (i_copy+48);
		//printf("print %c \n",Number[j]);
	}
	

	for(int j = 0; j<6; j++){
		
		writeChar(Number[j],j);
		//printf("pos: %d",j);
		//printf(" Value: %c \n",Number[j]);
	}
	
}


bool is_prime(long i) {
	for(int j=2; j<i;j++){
		if(i%j==0){
			return false;
			
		}
	}
	return true;
}


void primes(void){
	for(long n = 2; n<1000; n++){
		if (is_prime(n)){
			writeLong(n);
		}
		
	}
}

void blink(void){
	TCCR1B = (1<<CS12);
	CLKPR = (CLKPR) | (1 << CLKPS0); 
	for(int i = 0; i<10000; i++){
		if(TCNT1>=0x88B8){
			LCDDR3 = 1;
		} else{
			LCDDR3 = 0;
			LCDDR1 = 1;
		}
	}
	
	
}

int main(void)
{	
	LCD_Init();
	/*writeChar('1',0);
	writeChar('2',1);
	writeChar('3',2);
	writeChar('4',3);
	writeChar('5',4);
	writeChar('6',5);
	writeChar('7',6);*/
	//primes();
	//writeLong(2345798);
	blink();
	
	
	
	

}
	
