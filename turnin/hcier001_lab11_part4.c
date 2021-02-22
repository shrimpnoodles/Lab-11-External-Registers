/*	Author: lab
 *  Partner(s) Name: none
 *	Lab Section:
 *	Assignment: Lab #  Exercise #
 *	Exercise Description: [optional - include for your own benefit]
 *	DEMO LINK: https://drive.google.com/file/d/1yRD4ex5YYAYkr1Jk9lN30Ji0A-OUKiTq/view?usp=sharing
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

void transmit_data(unsigned short data) {

    int i;

    for (i = 0; i < 16 ; ++i) {

   	 // Sets SRCLR to 1 allowing data to be set

   	 // Also clears SRCLK in preparation of sending data

   	 PORTC = 0x08;

   	 // set SER = next bit of data to be sent.

   	 PORTC |= ((data >> i) & 0x01);

   	 // set SRCLK = 1. Rising edge shifts next bit of data into the shift register

   	 PORTC |= 0x02;  

    }

    // set RCLK = 1. Rising edge copies data from “Shift” register to “Storage” register

    PORTC |= 0x04;

    // clears all lines in preparation of a new transmission

    PORTC = 0x00;

}

enum States{start, init, inc, waitInc, dec, waitDec, reset} state;

unsigned short number;
unsigned char button;

void Tick(){
	switch(state) { //transitions
		case start:
			state = init;
			break;
		case init:
			if(button == 0x01){
				state = inc;
			}
			else if(button == 0x02){
				state = dec;
			}
			else if(button == 0x03){
				state = reset;
			}
			break;
		case inc:
	 		state = waitInc;
			break;
		case waitInc:
			if(button == 0x02){
				state = dec;
			}
			else if(button == 0x03){
				state = reset;
			}
			else if(button == 0x00){
				state = init;
			}
			break;
		case dec:
			state = waitDec;
			break;
		case waitDec:
			if(button == 0x01){
				state = inc;
			}
			else if(button == 0x03){
				state = reset;
			}
			else if(button == 0x00){
				state = init;
			}
			break;
		case reset:
			if(button == 0x00){
				state = init;
			}
			else if(button == 0x01){
				state = inc;
			}
			else if(button == 0x02){
				state = dec;
			}
			break;
		default:
			state = start;
			break;
		}
	switch(state){//actions
		case start:
			break;
		case init:
			transmit_data(number);
			break;
		case inc:
			if(number < 0x7FFF){
				number++;
			}
			transmit_data(number);
			break;
		case waitInc:
		//	transmit_data(number);
			break;
		case dec:
			if(number > 0x00){
				number--;
			}
			 transmit_data(number);
			break;
		case waitDec:
		//	transmit_data(number);
			break;
		case reset:
			number = 0x00;
			 transmit_data(number);
			break;
		default:
			break;
	}
}

int main(void) {
    /* Insert DDR and PORT initializations */
	DDRA = 0x00; PORTA = 0xff;
	DDRB = 0xff; PORTB = 0x00;
	DDRC = 0xff; PORTC = 0x00;
    /* Insert your solution below */
	state = start;
	number = 0x00;
	button = 0x00;

    while (1) {
	button = ~PINA;
	Tick();
    }
    return 1;
}
