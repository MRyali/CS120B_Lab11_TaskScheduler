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

enum States1 {Start1, press, release} state1;
enum States2 {Start2, display, hold} state1;

unsigned char keypadVal;
unsigned char changeLetter;
unsigned char signal;
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
        case Start:
            state1 = press;
            break;
        case press:
            if (kepyadVal != '0') {
                state1 = release;
            }
            else {
                state1 = press;
            }
            break;
        case release:
            if (keypadVal == '0') {
                state1 = press;
            }
            else {
                state1 = release;
            }
            break;
        default:
            state = Start;
            break;
    }
    switch(state) {
        case Start:
            break;
        case press:
            if (kepyadVal != '\0'){ //if a button was pressed
                letterChange = keypadVal;//store the letter
                signal = 1; //tells that button was pressed
                if (cursor != 16) { //update the position
                    cursor++;
                }
                else {
                    cursor++; //index for where letter will change
                }
            }
            break;
        case release:
            break;
        default:
            break;
    }
    return state;
}

int tick2(int state) {
	switch (state2) {
        case Start2:
            LCD_DisplayString(1, "SummerTime2020!!");
            state2 = display;
            break;
		case display:
			state2 = hold;
			break;
		case hold:
            if (signal == 1){
                state2 = display;
            }
            else {
                state2 = hold;
            }
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
	return state;
}


int main(void) {
    DDRA = 0xF0; PORTA = 0x0F; //input
    DDRC = 0xFF; PORTC = 0x00; //output
    DDRB = 0xFF; PORTB = 0x00; //output
    DDRD = 0xFF; PORTD = 0x00; //output;

    // unsigned long int
    static task task1;
    task *tasks[] = { &task1, &task2};
    const unsigned short numTasks = sizeof(tasks)/sizeof(task*);

	// Task 1
	task1.state = 0;//Task initial state.
	task1.period = 10;//Task Period.
	task1.elapsedTime = 2;//Task current elapsed time.
    task1.TickFct = &tick1;//Function pointer for the tick.

    // Task 1
    task2.state = 0;//Task initial state.
    task2.period = 10;//Task Period.
    task2.elapsedTime = 2;//Task current elapsed time.
    task2.TickFct = &tick2;//Function pointer for the tick.

    TimerSet(30);
	TimerOn();
    LCD_init();
    LCD_ClearScreen();

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
