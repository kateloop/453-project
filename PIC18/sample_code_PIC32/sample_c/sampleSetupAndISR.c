#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <avr/eeprom.h>

#define MAX_NUM 6 //Max number of ADCs
#define DEBOUNCE_SWITCH_PARAM 8 //Debounce Paramter for switches, must have 8 contiguous values over 40.96 ms
#define DEBOUNCE_ESTOP_PARAM 8 //Debounce Paramter for EStop, must have 8 contiguous values over 40.96 ms

#define ARRAY_LENGTH 256
uint8_t EEMEM LOOK_UP_TABLE[ARRAY_LENGTH] = {-128, -125, -122, -119, -116, -113, -110, -108, -105, -102, 
-100, -97, -95, -92, -90, -88, -85, -83, -81, -79, -76, -74, -72, -70, -68, -66, -64, -62, -61, -59, -57, -55, 
-54, -52, -50, -49, -47, -46, -44, -43, -41, -40, -38, -37, -36, -34, -33, -32, -31, -30, -28, -27, -26, -25, 
-24, -23, -22, -21, -20, -20, -19, -18, -17, -16, -16, -15, -14, -13, -13, -12, -11, -11, -10, -10, -9, -9, -8, 
-8, -7, -7, -6, -6, -5, -5, -5, -4, -4, -4, -3, -3, -3, -3, -2, -2, -2, -2, -2, -1, -1, -1, -1, -1, -1, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 3, 3, 3, 3, 4, 4, 4, 5, 5, 6, 6, 6, 7, 7, 8, 8, 9, 
9, 10, 10, 11, 12, 12, 13, 14, 14, 15, 16, 17, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 34, 
35, 36, 38, 39, 40, 42, 43, 45, 46, 48, 49, 51, 53, 54, 56, 58, 60, 62, 63, 65, 67, 69, 71, 73, 75, 78, 80, 82, 
84, 87, 89, 91, 94, 96, 99, 101, 104, 107, 109, 112, 115, 118, 121, 124, 127}; 

/*Bitmasks for switches and EStop; also used when storing the state of the switches in the 
switch_states variable.  Bitmasks off the pins we aren't interested in*/
#define BITMASK_SWITCH_A 0x01 //Bitmask for Language Switch
#define BITMASK_SWITCH_B 0x02 //Bitmask for Autonomy Switch
#define BITMASK_ESTOP 0x04 //Bitmask for EStop


#define SWITCH_PIN PINB //Pin that the switches and EStop are in  

#define INPUT_1 0 //ADC 1
#define INPUT_2 1 //ADC 2
#define INPUT_3 2 //ADC 3
#define INPUT_4 3 //ADC 4
#define INPUT_5 4 //ADC 5
#define INPUT_6 5 //ADC 6

volatile uint8_t adc_num; //Keeps track of which ADC is being used

volatile uint8_t current_value; //Used to keep track of the current ADC input value

volatile uint8_t switch_states; /*Status of Switch A stored in bit 0, Status of Switch B stored in bit 1,and status of EStop stored in bit 2.*/

volatile uint8_t ss_estop;
volatile uint8_t ss_A;
volatile uint8_t ss_B;

//Switch Counters store how long the switch has been at a contiguous value; high at 1 and low at 0
volatile uint8_t switch_cont_high_A = 0;
volatile uint8_t switch_cont_low_A = 0;
volatile uint8_t switch_cont_high_B = 0;
volatile uint8_t switch_cont_low_B = 0;
volatile uint8_t estop_cont_high = 0;
volatile uint8_t estop_cont_low = 0;


int main(void){
	//initialization
	DDRA = 0x00;
	PORTA = 0x00;
	DDRB &= 0xF8;
	PORTB &= 0xF8;
	DDRC = 0xFF;
	PORTC = 0x00;
	DDRD |= 0xB0;
	PORTD = 0x00;

	TCCR0B |= 0x04; //Prescaler of 1/256
	TIMSK0 |= 0x02; //enable compare A interrupt
	OCR0A = 200; //Interrupting on a frequency of 5.12 ms 
	
	//Configure ADC
	ADMUX = 0x60;
	ADCSRA = 0xCC; //Turns ADC on and enables ADC interrupts

	adc_num = 0;

	sei(); //enables interrupts

	while(1){
	}
	
}


void ADC_data_store(int ADCnum, int ADCvalue){


	if(ADCnum == 0){
		//Left Joystick, Y Direction
	}else if(ADCnum == 1){
		//Left Joystick, X Direction ******
		int value = LOOK_UP_TABLE[ADCvalue];
		value = value >> 2;

	}else if(ADCnum == 2){
		//Right Joystick, Y Direction ******
		int value = LOOK_UP_TABLE[ADCvalue];
		value = value >> 2;
	}else if(ADCnum == 3){
		//Right Joystick, X Direction
	}else if(ADCnum == 4){
		//Signal Strength (RSSI)
		//PORTA[7..0], Bitmask OR
		if(ADCvalue > 230){
			PORTD |= 0x80;
		}else{
			PORTD &= 0x7F;
		}
		if(ADCvalue > 204){
			PORTD |= 0x40;
		}else{
			PORTD &= 0xBF;
		}
		if(ADCvalue > 178){
			PORTC |= 0x80;
		}else{
			PORTC &= 0x7F;
		}
		if(ADCvalue > 152){
			PORTC |= 0x400;
		}else{
			PORTC &= 0xBF;
		}
		if(ADCvalue > 126){
			PORTC |= 0x20;
		}else{
			PORTC &= 0xDF;
		}
		if(ADCvalue > 100){
			PORTC |= 0x10;
		}else{
			PORTC &= 0xEF;
		}
		if(ADCvalue > 74){
			PORTC |= 0x08;
		}else{
			PORTC &= 0xF7;
		}
		if(ADCvalue > 48){
			PORTC |= 0x04;
		}else{
			PORTC &= 0xFB;
		}
		if(ADCvalue > 22){
			PORTC |= 0x02;
		}else{
			PORTC &= 0xFD;
		}
		if(ADCvalue > 0){
			PORTC |= 0x01;
		}else{
			PORTC &= 0xFE;
		}
	}else if(ADCnum == 5){
		//Battery Monitor
		if(ADCvalue < 49){
			PORTB |= 0x08;
		}else{
			PORTB &= 0xF7;
		}
	}

}

//ISR that processes ADC data from the five inputs in a sequential fashion
ISR(ADC_vect){
	
	cli();

	ADMUX = 0x60;
	
	//Chooses which ADC to process data from 
	if(adc_num == 0){
		ADMUX |= INPUT_1;
	}else if(adc_num == 1){
		ADMUX |= INPUT_2;
	}else if(adc_num == 2){
		ADMUX |= INPUT_3;
	}else if(adc_num == 3){
		ADMUX |= INPUT_4;
	}else if(adc_num == 4){
		ADMUX |= INPUT_5;
	}else if(adc_num == 5){
		ADMUX |= INPUT_6;
	}

	current_value = ADCH; //Stores the value in the ADC

	ADC_data_store(adc_num, current_value);
	adc_num++; //Increments the ADC being read from

	//Verifies adc_num does not exceed the maximum value
	if (adc_num >= (MAX_NUM)){
		adc_num = 0x00;
	}

	//Verifies adc_num does not exceed the maximum value
	if(adc_num >= MAX_NUM){
		adc_num = 0x00;
	}

	ADCSRA |= (1<<ADSC); //Starts conversion

	sei();
}


//ISR for switches and EStop
ISR(TIMER0_COMPA_vect){
	
	cli();

	//bitmask off switch_states to verify that it has been pressed again
	ss_estop = (switch_states & BITMASK_ESTOP);
	ss_A = (switch_states & BITMASK_SWITCH_A);
	ss_B = (switch_states & BITMASK_SWITCH_B);


		/*Counts continuous highs/lows for switches and EStop.  Once there are 8 contiguous values, 
	the switch is either on or off, depending on the values.*/
	if(BITMASK_SWITCH_A & SWITCH_PIN){
		switch_cont_high_A++;
		switch_cont_low_A = 0;
	}else{
		switch_cont_high_A = 0;
		switch_cont_low_A++;
	}
	if(BITMASK_SWITCH_B & SWITCH_PIN){
		switch_cont_high_B++;
		switch_cont_low_B = 0;
	}else{
		switch_cont_high_B = 0;
		switch_cont_low_B++;
	}
	if(BITMASK_ESTOP & SWITCH_PIN){
		estop_cont_high++;
		estop_cont_low = 0;
	}else{
		estop_cont_high = 0;
		estop_cont_low++;
	}
	
	/*Sets states of switches in the switch_states variable; if a switch has had a continous value for the past 8 cycles
	the state of that switch will change*/ 

	if(switch_cont_high_A == DEBOUNCE_SWITCH_PARAM){
		switch_states &= ~BITMASK_SWITCH_A;
	}
	if((switch_cont_low_A == DEBOUNCE_SWITCH_PARAM) && (ss_A != BITMASK_SWITCH_A)){
		switch_states |= BITMASK_SWITCH_A;
	}
	if(switch_cont_high_B == DEBOUNCE_SWITCH_PARAM){
		switch_states &= ~BITMASK_SWITCH_B;
	}
	if((switch_cont_low_B == DEBOUNCE_SWITCH_PARAM) && (ss_B != BITMASK_SWITCH_B)){
		switch_states |= BITMASK_SWITCH_B;
	}
	if(estop_cont_high == DEBOUNCE_ESTOP_PARAM){
		switch_states &= ~BITMASK_ESTOP;
	}
	if((estop_cont_low == DEBOUNCE_ESTOP_PARAM) && (ss_estop != BITMASK_ESTOP)){
		switch_states |= BITMASK_ESTOP;
	}
	
	sei();
}




