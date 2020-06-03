/*	Author: Mayur Ryali
 *  Partner(s) Name:
 *	Lab Section: 21
 *	Assignment: Lab #11  Exercise #5
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

unsigned char i = 0;

const unsigned char display[30]={' ', ' ', ' ', '#', ' ', ' ', '#', ' ', '#', '#', ' ', ' ', '#', };

int gameSM() {
    case runSM:
        for (i = 1; i <= 32; i++) {
            LCD_Cursor(i); //cursor
            LCD_WriteData(display[(i - 2) + scroll]); //"starts at index 1 first time, then index 2, etc simulating a scroll"
            if ((scroll + i + 1) == 66) {
                scroll = 1;
            }
        }
        scroll++; //responible for "scrolling text"
        state = runSM;
        break;ss
}

int playerSM() {
    switch(state2) {
        case Start2:
            state2 = player;
            break;
        case player:
            if (buttonUp) {
                state2 = cursorUp;
            }
            else if (buttonDown) {
                state2 = cursorDown;
            }
            else {
                state2 = player;
            }
            break;
        case cursorUp:
            if (!buttonUp) {
                state2 = player;
            }
            else {
                state2 = cursorUp;
            }
            break;
        case cursorDown:
            if (!buttonDown) {
                state2 = player;
            }
            else {
                state2 = cursorDown;
            }
            break;
        default:
            state2 = Start2;
            break;
    }
    switch(state2) {
        case Start2:
            LCD_Cursor(1);
            break;
        case player:
            break;
        case cursorUp:
            LCD_Cursor(1); // top row first position
            break;
        case cursorDown:
            LCD_Cursor(17); //botton row first position
            break;
        default:
            break;
    }
}

int main(void) {
    DDRC = 0xF0; PORTC = 0x0F; //input
    DDRB = 0xFF; PORTB = 0x00; //output
    DDRD = 0xFF; PORTD = 0x00; //output;

    // unsigned long int
    static task task1;
    task *tasks[] = { &task1, &task2};
    const unsigned short numTasks = sizeof(tasks)/sizeof(task*);

	   // Task 1
	task1.state = 0;//Task initial state.
	task1.period = 2;//Task Period.
	task1.elapsedTime = 2;//Task current elapsed time.
    task1.TickFct = &gameSM;//Function pointer for the tick.

    task1.state = 0;//Task initial state.
	task1.period = 2;//Task Period.
	task1.elapsedTime = 2;//Task current elapsed time.
    task1.TickFct = &playerSM;//Function pointer for the tick.

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
