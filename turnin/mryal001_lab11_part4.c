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

enum States1 {Start1, press, release} state1;
enum States2 {Start2, display, hold} state2;

unsigned char keypadVal = 0x00;
unsigned char letterChange = 'C';
unsigned char signal = 0;
unsigned char cursor = 0;
/*
unsigned char display[10]={'S', 'U', 'M', 'M', 'E', 'R', 'T', 'I', 'M', 'E'};

int tick(int state) {
    keypadVal = GetKeypadKey();
    switch(state) {
        case Start:
            state = runSM;
            break;
        case runSM:
                switch(keypadVal) {
                    case '1':
              		    display[cursor] = '1';
                        cursor++;
                           break;
              		case '2':
                        display[cursor] = '2';
                        cursor++;
              		    break;
              		case '3':
                        display[cursor] = '3';
                        cursor++;
              	           break;
              		case '4':
                        display[cursor] = '4';
                        cursor++;
              	           break;
              		case '5':
                        display[cursor] = '5';
                        cursor++;
              	           break;
              		case '6':
                        display[cursor] = '6';
                        cursor++;
              	           break;
              		case '7':
                        display[cursor] = '7';
                        cursor++;
              	           break;
              		case '8':
                        display[cursor] = '8';
                        cursor++;
              		       break;
              		case '9':
                        display[cursor] = '9';
                        cursor++;
              		       break;
              		case 'A':
                        display[cursor] = 'A';
                        cursor++;
              		       break;
              		case 'B':
                        display[cursor] = 'B';
                        cursor++;
              		       break;
              		case 'C':
                        display[cursor] = 'C';
                        cursor++;
              		       break;
              		case 'D':
                        display[cursor] = 'D';
                        cursor++;
              		       break;
              		case '*':
                        display[cursor] = '*';
                        cursor++;
              		       break;
              		case '0':
                        display[cursor] = '0';
                        cursor++;
              		       break;
              		case '#':
                        display[cursor] = '#';
                        cursor++;
              		    break;
                    }
                LCD_Cursor(i); //cursor
                LCD_WriteData(display[i - 1]); //prints whole word
                if (i < 11) {
                    i++;
                }
                else {
                    i = 1;
                }
            state = runSM;
            break;
        default:
            state = Start;
            break;
    }
    switch(state) {
        case Start:
            LCD_ClearScreen();
            break;
        case runSM:
            break;
        default:
            break;
    }
    return state;
}
*/

int tick1(int state) {
    keypadVal = GetKeypadKey();
    switch(state1) {
        case Start1:
            if (keypadVal != '0') {
                state1 = press;
            }
            break;
        case press:
            if (keypadVal != '0') {
                state1 = release;
            }
            /*
            else {
                state1 = press;
            }*/
            break;
        case release:
            if (keypadVal == '0') {
                state1 = press;
            }/*
            else {
                state1 = release;
            }*/
            else {
                break;
            }
        default:
            state = Start1;
            break;
    }
    switch(state1) {
        case Start1:
            break;
        case press:
            if (keypadVal != '\0'){ //if a button was pressed
                letterChange = keypadVal;//store the letter
                signal = 1; //tells that button was pressed
                if (cursor == 16) { //update the position
                    cursor = 0;
                }/*
                else {
                    cursor = 0; //index for where letter will change
                }*/
                cursor++;
            }
            break;
        case release:
            break;
        default:
            break;
    }
    return state1;
}

int tick2(int state) {
	switch (state2) {
        case Start2:
            LCD_DisplayString(1, "SummerTime2020!!");
            state2 = hold;
            break;
		case display:
			state2 = hold;
			break;
		case hold:
            if (signal == 1){
                state2 = display;
            }/*
            else {
                state2 = hold;
            }*/
            break;
		default:
			state2 = Start2;
			break;
	}
	switch(state2) {
		case Start2:
			break;
		case display:
			LCD_Cursor(cursor); //changes which letter is being changed
			LCD_WriteData(letterChange); //the letter we are inserting
			signal = 0;
			break;
		case hold:
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
    task *tasks[] = { &task1, &task2 };
    const unsigned short numTasks = sizeof(tasks)/sizeof(task*);

    // Task 1
    task1.state = 0;//Task initial state.
    task1.period = tick1_period;//Task Period.  //1
    task1.elapsedTime = tick1_period; // Task current elasped time. //1
    task1.TickFct = &tick1; // Function pointer for the tick.

    // Task 2
    task2.state = 0;//Task initial state.
    task2.period = tick2_period;//Task Period.   //2
    task2.elapsedTime = tick2_period; // Task current elasped time. //2
    task2.TickFct = &tick2; // Function pointer for the tick.

    TimerSet(smGCD);
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
    	while(!TimerFlag) {
    	   TimerFlag = 0;
    	}
    }
    // Error: Program should not exit!
    return 0;
}
