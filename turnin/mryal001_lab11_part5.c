/*	Author: Mayur Ryali
 *  Partner(s) Name:
 *	Lab Section: 21
 *	Assignment: Lab #11  Exercise #5
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */

// Demo: https://drive.google.com/open?id=13A2sWmmBppTPxSY9QQqZPmkRV2Z7JxJg

#include <avr/io.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#include "keypad.h"
#include "scheduler.h"
#include "io.h"
#include "timer.h"
#endif

enum cursorSM {Start1, cursorUp, cursorDown} state1;
enum objectSM {Start2, move} state2;
enum gameSM {Start3, playGame, gameOver, wait, gameReset} state3;

unsigned char buttonUp;
unsigned char buttonDown;
unsigned char buttonReset;
int r = 0; //random
unsigned char i = 0; //temp variable allows to check if game is paused
// unsigned char currChar = 0;
unsigned char playerCursor = 0; //user position (top is 1, bottom is 17)
unsigned char topPositionLCD;
unsigned char bottomPositionLCD;
unsigned char topPositionObject;
unsigned char bottomPositionObject;

/*
const unsigned char display[30]={' ', ' ', ' ', '#', ' ', ' ', '#', ' ', '#', '#', ' ', ' ', '#', };

int gameSM() {
    switch (state1) {
        case Start1:
            state1 = runSM;
            break;
        case runSM:
            for (i = 1; i <= 32; i++) {
                LCD_Cursor(i); //cursor
                LCD_WriteData(display[(i - 2) + scroll]); //"starts at index 1 first time, then index 2, etc simulating a scroll"
                currChar = display[(i - 2) + scroll];
                if ((scroll + i + 1) == 66) {
                    scroll = 1;
                }
            }
            scroll++; //responible for "scrolling text"
            state = runSM;
            break;
        default:
            state1 = Start;
            break;
    }
    switch(state1) {
        case Start1:
            break;
        case runSM:
            break;
        default:
            break;
    }
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
            playerCursor = 1;
            if (!buttonUp) {
                state2 = player;
            }
            else {
                state2 = cursorUp;
            }
            break;
        case cursorDown:
            playerCursor = 16;
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
*/

void playerCursorSM () { //allows player to move sursor up and down
    switch (state1) {
        case Start1:
            state1 = cursorUp;
            break;
        case cursorUp: //play game
            if (buttonDown) {
                state1 = cursorDown;
            }
            else {
                state1 = cursorUp;
            }
            break;
        case cursorDown:
            if (buttonUp) {
                state1 = cursorUp;
            }
            else {
                state1 = cursorDown;
            }
            break;
        default:
            state1 = Start1;
            break;
    }
    switch (state1) {
        case Start1:
            break;
        case cursorUp:
            playerCursor = 1; //store position for comparison
            LCD_Cursor(playerCursor);
            break;
        case cursorDown:
            playerCursor = 17; //store position for comparison
            LCD_Cursor(playerCursor);
            break;
        default:
            break;
    }
}

void objectMoveSM() {
    switch(state2) {
        case Start2:
            state2 = move;
            break;
        case move:
            if ((topPositionLCD != 1) && (bottomPositionLCD != 17)) {
                topPositionLCD -= 1; //decrementing by one gives smooth transition
                bottomPositionLCD -= 1;
            }
            else if (topPositionLCD == 1) { //reset position upon reaching end of screen
                topPositionLCD = 16;
            }
            else if (bottomPositionLCD == 17) {
                bottomPositionLCD = 32;
            }
            break;
        default:
            state2 = Start2;
            break;
    }
    switch(state2) {
        case Start2:
            break;
        case move:
            LCD_ClearScreen();
            LCD_Cursor(topPositionLCD); //top object
            LCD_WriteData('#');
            topPositionObject = topPositionLCD; //store position of object
            LCD_Cursor(bottomPositionLCD); //bottom object
            LCD_WriteData('#');
            bottomPositionObject = bottomPositionLCD; //store the position of object
            break;
        default:
            break;
    }
}

void playGameSM() {
    switch(state3) { // state transitions
        case Start3:
            state3 = wait;
            break;
        case playGame:
            if (playerCursor == topPositionObject || playerCursor== bottomPositionObject) { //game is lost
                state3 = gameOver;
            }
            else {
                state3 = playGame;
            }
            break;
        case gameOver:
            state3 = wait;
            break;
        case wait:
            if (buttonReset) {
                state3 = gameReset;
            }
            else {
                state3 = wait; //wait for game to start over
            }
            break;
        case gameReset:
            state3 = playGame; //reset the game
            break;
    }
    switch(state3) { // state actions
        case Start3:
            break;
        case playGame:
            playerCursorSM(); //allows player to move the cursor
            break;
        case gameOver:
            LCD_DisplayString(1, (unsigned char*)"Get Good Lol"); //game over message
            i = 1;
            break;
        case wait:
            break;
        case gameReset: //reset game
            LCD_ClearScreen();
            i = 0;
            break;
    }
}

int main(void) {
    DDRA = 0x00; PORTA = 0xFF; //input
    DDRC = 0xFF; PORTC = 0x00; //output
    DDRB = 0xFF; PORTB = 0x00; //output
    DDRD = 0xFF; PORTD = 0x00; //output;

    srand(time(0));  //set seed as current time

    int rPosTop = (rand() % 16) + 5; //random starting pos for top
    int rPosBot = (rand() % 32) + 5; //random starting pos for bot
    r = (rand() % 1) + 1;      // Returns a pseudo-random integer between 0 and RAND_MAX.

    // initialize global variables
    topPositionLCD = rPosTop;
    bottomPositionLCD= rPosBot;
    i = 1;
    // initialize local variables
    unsigned long cursor_time = 50;
    unsigned long obstacle_time = 300;
    const unsigned long timer_period = 50;

    state1 = Start1;
    state2 = Start2;
    state3 = Start3;

    // initialize timer
    TimerSet(timer_period);
    TimerOn();
    LCD_init();
    LCD_ClearScreen();

    while (1) {
        buttonUp = ~PINA & 0x01; //up
        buttonDown = ~PINA & 0x02; //down
        buttonReset = ~PINA & 0x04; //reset
        if (buttonReset) { // use PB2 to start/restart the game
            i = 0;
        }
        if (i == 0) { //game is running
            if (cursor_time >= 50) {
                playGameSM();
                cursor_time = 0;
            }
            if (obstacle_time >= 300) {
                objectMoveSM();
                obstacle_time = 0;
            }
        }
        while (!TimerFlag) {}  // Wait for timer period
        TimerFlag = 0;        // Lower flag
        cursor_time += timer_period;
        obstacle_time += timer_period;
    }
    return 0;
}
