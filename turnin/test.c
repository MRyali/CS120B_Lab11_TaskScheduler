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
#include <stdio.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#include "keypad.h"
#include "scheduler.h"
#include "io.h"
#include "timer.h"
#include "bit.h"
#endif

//enum LetterChangeSM {Start1, press, release} state1;
enum DisplaySM{Start2, display,} state2;

unsigned char keypadVal = 0x00;
unsigned char letterChange = 'C';
unsigned char signal = 0;
unsigned char cursor = 0;

unsigned char display[16]={'S', 'U', 'M', 'M', 'E', 'R', 'T', 'I', 'M', 'E', '2', '0', '2', '0', '!', '!'};

unsigned char word[5] = {'H', 'E', 'L', 'L', 'O'};
int LetterChangeSM(int state) {
    switch(state2) {
        case Start2:
            state2 = display;
            break;
        case display:
            LCD_Cursor(1);
            LCD_WriteData(word[0]);
            LCD_Cursor(2);
            LCD_WriteData(word[1]);
            LCD_Cursor(3);
            LCD_WriteData(word[2]);
            LCD_Cursor(4);
            LCD_WriteData(word[3]);
            LCD_Cursor(5);
            LCD_WriteData(word[4]);
            state2 = display;
        default:
            state2 = Start2;
            break;
    }
}


int main(void) {
    DDRA = 0xF0; PORTA = 0x0F; //input
    DDRC = 0xFF; PORTC = 0x00; //output
    DDRB = 0xFF; PORTB = 0x00; //output
    DDRD = 0xFF; PORTD = 0x00; //output;

    unsigned long int tick1Val= 2;
    unsigned long int tick2Val= 1;

    unsigned long int tempGCD = findGCD(tick1Val, tick2Val);

    //Greatest common divisor for all tasks or smallest time unit for tasks.
    unsigned long int smGCD = tempGCD;

    //Recalculate GCD periods for scheduler
    unsigned long int tick1_period = tick1Val/smGCD;// 4
    unsigned long int tick2_period = tick2Val/smGCD;// 1

    //Declare an array of tasks
    static task task1;
    task *tasks[] = { &task1};
    const unsigned short numTasks = sizeof(tasks)/sizeof(task*);

    // Task 1
    task1.state = 0;//Task initial state.
    task1.period = tick1_period;//Task Period.  //1
    task1.elapsedTime = tick1_period; // Task current elasped time. //1
    task1.TickFct = &LetterChangeSM; // Function pointer for the tick.
/*
    // Task 2
    task2.state = 0;//Task initial state.
    task2.period = tick2_period;//Task Period.   //2
    task2.elapsedTime = tick2_period; // Task current elasped time. //2
    task2.TickFct = &tick2; // Function pointer for the tick.
*/
    TimerSet(10);
    TimerOn();

    LCD_init();
    LCD_ClearScreen();

    unsigned short i; // Scheduler for-loop iterator

    while(1) {
    	// Scheduler code
    	for ( i = 0; i < numTasks; i++ ) {
    		// Task is ready to tick
    		if ( tasks[i]->elapsedTime >= tasks[i]->period ) {
    			// Setting next state for task
    			tasks[i]->state = tasks[i]->TickFct(tasks[i]->state);
    			// Reset the elapsed time for next tick.
    			tasks[i]->elapsedTime = 0;
    		}
    		tasks[i]->elapsedTime += 1;
    	}
    	while(!TimerFlag);
    	   TimerFlag = 0;
    }
    // Error: Program should not exit!
    return 0;
}
