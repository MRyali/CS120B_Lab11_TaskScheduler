/*	Author: Mayur Ryali
 *  Partner(s) Name:
 *	Lab Section: 21
 *	Assignment: Lab #11  Exercise #4
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

enum States {Start, display, changeLetter} state;

unsigned char keypadVal;
unsigned char tempB;
unsigned char i;
unsigned char cursor = 1;

const unsigned char display[10]={'S', 'U', 'M', 'M', 'E', 'R', 'T', 'I', 'M', 'E'};

int tick(int state) {
    keypadVal = GetKeypadKey();
    switch(state) {
        case Start:
            state = display;
            break;
        case runSM:
            for (i = 1; i < 11; i++) {
                LCD_Cursor(i); //cursor
                LCD_WriteData(display[i - 1]); //prints whole word
            }
            state = changeLetter;
            break;
        case changeLetter:
            LCD_Cursor(cursor);
            switch(keypadVal) {
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
                }
                LCD_WriteData(tempB + '0');
                cursor++;
                if (cursor == 11) {
                    cursor = 1;
                }
                state = changeLetter;
                break;
        default:
            state = Start;
            break;
    }
    switch(state) {
        case Start:
            break;
        case runSM:
            break;
        default:
            break;
    }
    return state;
}

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
