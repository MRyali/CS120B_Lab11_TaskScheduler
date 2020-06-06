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

enum States {Start, runSM, pressed} state;

unsigned char keypadVal;
unsigned char tempB; //value for display
unsigned char store; //value to keep display until new button pressed
unsigned char key; //tells whicg value to add to tempB in LCD_WriteData

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
                       key = 0;
                       LCD_Cursor(1);
                       LCD_WriteData(tempB + '0');
                       break;
      			case '1':
      			       tempB = 0x01;
                       key = 0;
                       LCD_Cursor(1);
                       LCD_WriteData(tempB + '0');
                       break;
      			case '2':
      			       tempB = 0x02;
                       key = 0;
                       LCD_Cursor(1);
                       LCD_WriteData(tempB + '0');
      			       break;
      			case '3':
      			       tempB = 0x03;
                       key = 0;
                       LCD_Cursor(1);
                       LCD_WriteData(tempB + '0');
      		           break;
      			case '4':
      			       tempB = 0x04;
                       key = 0;
                       LCD_Cursor(1);
                       LCD_WriteData(tempB + '0');
      		           break;
      			case '5':
      			       tempB = 0x05;
                       key = 0;
                       LCD_Cursor(1);
                       LCD_WriteData(tempB + '0');
      		           break;
      			case '6':
      			       tempB = 0x06;
                       key = 0;
                       LCD_Cursor(1);
                       LCD_WriteData(tempB + '0');
      		           break;
      			case '7':
      			       tempB = 0x07;
                       key = 0;
                       LCD_Cursor(1);
                       LCD_WriteData(tempB + '0');
      		           break;
      			case '8':
      			       tempB = 0x08;
                       key = 0;
                       LCD_Cursor(1);
                       LCD_WriteData(tempB + '0');
      			       break;
      			case '9':
      			       tempB = 0x09;
                       key = 0;
                       LCD_Cursor(1);
                       LCD_WriteData(tempB + '0');
      			       break;
      			case 'A':
      			       tempB = 0x0A;
                       key = 1;
                       LCD_Cursor(1);
                       LCD_WriteData(tempB + 0x37);
      			       break;
      			case 'B':
      			       tempB = 0x0B;
                       key = 1;
                       LCD_Cursor(1);
                       LCD_WriteData(tempB + 0x37);
      			       break;
      			case 'C':
      			       tempB = 0x0C;
                       key = 1;
                       LCD_Cursor(1);
                       LCD_WriteData(tempB + 0x37);
      			       break;
      			case 'D':
      			       tempB = 0x0D;
                       key = 1;
                       LCD_Cursor(1);
                       LCD_WriteData(tempB + 0x37);
      			       break;
      			case '*':
      			       tempB = 0x0E;
                       key = 2;
                       LCD_Cursor(1);
                       LCD_WriteData(tempB + 0x1C);
      			       break;
      			case '0':
      			       tempB = 0x00;
                       key = 0;
                       LCD_Cursor(1);
                       LCD_WriteData(tempB + '0');
      			       break;
      			case '#':
      			       tempB = 0x0F;
                       key = 2;
                       LCD_Cursor(1);
                       LCD_WriteData(tempB + 0x14);
      			       break;
      			default:
      			       tempB = 0x1B;
                       key = 0;
                       LCD_Cursor(1);
                       LCD_WriteData(tempB + '0');
      			       break;
            }
            store = tempB;
            state = pressed;
            /*if (keypadVal == '\0') {
                state = pressed; //keep getting input from kepyad
            }
            else {
                state = runSM;
            }*/
            break;
        case pressed: //keeps value displayed unti new push
            switch (key) {
                case 0:
                    LCD_Cursor(1);
                    LCD_WriteData(store + '0');
                    break;
                case 1:
                    LCD_Cursor(1);
                    LCD_WriteData(store + '0');
                    break;
                case 2:
                    LCD_Cursor(1);
                    LCD_WriteData(store + '0');
                    break;
            }
            if (keypadVal == '\0') {
                state = pressed; //keep getting input from kepyad
            }
            else {
                state = runSM;
            }
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
        case pressed:
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
    task *tasks[] = { &task1};
    const unsigned short numTasks = sizeof(tasks)/sizeof(task*);

	   // Task 1
	task1.state = 0;//Task initial state.
	task1.period = 2;//Task Period.
	task1.elapsedTime = 2;//Task current elapsed time.
    task1.TickFct = &tick;//Function pointer for the tick.

    TimerSet(10);
	TimerOn();
    LCD_init();

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
