/*	Author: Mayur Ryali
 *  Partner(s) Name:
 *	Lab Section: 21
 *	Assignment: Lab #11  Exercise #3
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */

// Demo:

#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#include "keypad.h"
#include "scheduler.h"
#include "io.h"
#include "timer.h"
#endif


int main(void) {
    DDRC = 0xF0; PORTC = 0x0F; //input
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
