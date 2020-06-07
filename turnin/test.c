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

enum LetterChangeSM {Start1, wait, press, release} state1;
enum DisplaySM{Start2, display} state2;

unsigned char keypadVal = 0x00;
unsigned char index = 0;

//unsigned char display[16]={'S', 'U', 'M', 'M', 'E', 'R', 'T', 'I', 'M', 'E', '2', '0', '2', '0', '!', '!'};

unsigned char word[5] = {'H', 'E', 'L', 'L', 'O'};

int LetterChangeSM (int state) {
    keypadVal = GetKeypadKey();
    switch(state1) {
        case Start1:
            state1 = wait;
            break;
        case wait:
            if (keypadVal != '0') {
                state1 = press;
            }
            else {
                state1 = wait;
            }
            break;
        case press:
            if (keypadVal == '0') {
                state1 = release;
            }
            else {
                state1 = press;
            }
            break;
        case release:
            switch(keypadVal) {
      			case '1':
      			       word[index] = '1';
                       break;
      			case '2':
      			       word[index] = '2';
      			       break;
      			case '3':
      			       word[index] = '3';
      		           break;
      			case '4':
      			       word[index] = '4';
      		           break;
      			case '5':
      			       word[index] = '5';
      		           break;
      			case '6':
      			       word[index] = '6';
      		           break;
      			case '7':
      			       word[index] = '7';
      		           break;
      			case '8':
      			       word[index] = '8';
      			       break;
      			case '9':
      			       word[index] = '9';
      			       break;
      			case 'A':
      			       word[index] = 'A';
      			       break;
      			case 'B':
      			       word[index] = 'B';
      			       break;
      			case 'C':
      			       word[index] = 'C';
      			       break;
      			case 'D':
      			       word[index] = 'D';
      			       break;
      			case '*':
      			       word[index] = '*';
      			       break;
      			case '0':
      			       word[index] = '0';
      			       break;
      			case '#':
      			       word[index] = '#';
      			       break;
            }
            state1 = wait;
            break;
        default:
            state1 = Start1;
            break;
    }
    switch(state1) {
        case Start1:
            break;
        case wait:
            break;
        case press:
            break;
        case release:
            if (index < 4) {
                index++;
            }
            else {
                index = 0;
            }
            /*if (keypadVal == '1') {
                word[index] = '1';
            }
            if (keypadVal == '1') {
                word[index] = '1';
            }
            if (keypadVal == '2') {
                word[index] = '2';
            }
            if (keypadVal == '3') {
                word[index] = '3';
            }
            if (keypadVal == '4') {
                word[index] = '4';
            }
            if (keypadVal == '5') {
                word[index] = '5';
            }
            if (keypadVal == '6') {
                word[index] = '6';
            }
            if (keypadVal == '7') {
                word[index] = '7';
            }
            if (keypadVal == '8') {
                word[index] = '8';
            }
            if (keypadVal == '9') {
                word[index] = '9';
            }
            if (keypadVal == 'A') {
                word[index] = 'A';
            }
            if (keypadVal == 'B') {
                word[index] = 'B';
            }
            if (keypadVal == 'C') {
                word[index] = 'C';
            }
            if (keypadVal == 'D') {
                word[index] = 'D';
            }
            if (keypadVal == '#') {
                word[index] = '#';
            }
            if (keypadVal == '*') {
                word[index] = '*';
            }*/
            break;
        default:
            break;
    }
    return state1;
}

int DisplaySM(int state) {
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
            break;
        default:
            state2 = Start2;
            break;
    }
    switch(state2) {
        case Start2:
            break;
        case display:
            break;
        default:
            break;
    }
    return state2;
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
    static task task1, task2;
    task *tasks[] = { &task1, &task2};
    const unsigned short numTasks = sizeof(tasks)/sizeof(task*);

    // Task 1
    task1.state = 0;//Task initial state.
    task1.period = tick1_period;//Task Period.  //1
    task1.elapsedTime = tick1_period; // Task current elasped time. //1
    task1.TickFct = &LetterChangeSM; // Function pointer for the tick.

    // Task 2
    task2.state = 0;//Task initial state.
    task2.period = tick2_period;//Task Period.   //2
    task2.elapsedTime = tick2_period; // Task current elasped time. //2
    task2.TickFct = &DisplaySM; // Function pointer for the tick.

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
