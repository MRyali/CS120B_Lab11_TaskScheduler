/*	Author: Mayur Ryali
 *  Partner(s) Name:
 *	Lab Section: 21
 *	Assignment: Lab #11  Exercise #1
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */

// Demo: https://drive.google.com/open?id=12wiU9odFQGrlToBG4U9ND3i0MtCm7nCl

#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#include "keypad.h"
#include "scheduler.h"
#include "io.h"
#include "timer.h"
#endif

enum States {Start, runSM} state;

unsigned char keypadVal;
unsigned char tempB;

int tick(int state) {
    keypadVal = GetKeypadKey();
    switch(state) {
        case Start:
            state = runSM;
            break;
        case runSM:
            switch(keypadVal) { //get keypad val then wait for next keypad val upon returning to the same state
                case '\0':
      			       tempB = 0x1F;
                       break;
      			case '1':
      			       tempB = 0x01;
                       break;
      			case '2':
      			       tempB = 0x02;
      			       break;
      			case '3':
      			       tempB = 0x03;
      		           break;
      			case '4':
      			       tempB = 0x04;
      		           break;
      			case '5':
      			       tempB = 0x05;
      		           break;
      			case '6':
      			       tempB = 0x06;
      		           break;
      			case '7':
      			       tempB = 0x07;
      		           break;
      			case '8':
      			       tempB = 0x08;
      			       break;
      			case '9':
      			       tempB = 0x09;
      			       break;
      			case 'A':
      			       tempB = 0x0A;
      			       break;
      			case 'B':
      			       tempB = 0x0B;
      			       break;
      			case 'C':
      			       tempB = 0x0C;
      			       break;
      			case 'D':
      			       tempB = 0x0D;
      			       break;
      			case '*':
      			       tempB = 0x0E;
      			       break;
      			case '0':
      			       tempB = 0x00;
      			       break;
      			case '#':
      			       tempB = 0x0F;
      			       break;
      			default:
      			       tempB = 0x1B;
      			       break;
            }
            state = runSM; //keep getting input from kepyad
            break;
        default:
            state = Start;
            break;
    }
    switch(state) {
        case Start:
            PORTB = 0x1F; //starting value
            break;
        case runSM:
            PORTB = tempB;
            break;
        default:
            PORTB = tempB;
            break;
    }
    return state;
}

int main(void) {
    DDRA = 0xF0; PORTA = 0x0F; //input
    DDRB = 0xFF; PORTB = 0x00; //output
    DDRD = 0xFF; PORTD = 0x00; //output;

    // unsigned long int
    static task task1;
    task *tasks[] = { &task1};
    const unsigned short numTasks = sizeof(tasks)/sizeof(task*);

	   // Task 1
	task1.state = 0;//Task initial state.
	task1.period = 2;//Task Period.
	task1.elapsedTime = 2;//Task current elapsed time.
    task1.TickFct = &tick;//Function pointer for the tick.

    TimerSet(10);
	TimerOn();

    unsigned short i; // Scheduler for-loop iterator
   	while(1) {
   	    for ( i = 0; i < numTasks; i++ ) {
   		       if ( tasks[i]->elapsedTime == tasks[i]->period ) {
   			          tasks[i]->state = tasks[i]->TickFct(tasks[i]->state);
   				      tasks[i]->elapsedTime = 0;
   			}
   			tasks[i]->elapsedTime += 1;
   		}
   		while(!TimerFlag);
   		TimerFlag = 0;
   	}
    return 0;
}
