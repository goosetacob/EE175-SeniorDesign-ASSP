#include <avr/io.h>
#include <avr/interrupt.h>
#include "lib/bit.h"
#include "lib/timer.h"
#include <stdio.h>
#include "lib/lcd_8bit_task.h"

//--------Find GCD function --------------------------------------------------
unsigned long int findGCD(unsigned long int a, unsigned long int b)
{
    unsigned long int c;
    while(1){
        c = a%b;
        if(c==0){return b;}
        a = b;
b = c;
    }
    return 0;
}
//--------End find GCD function ----------------------------------------------

//--------Task scheduler data structure---------------------------------------
// Struct for Tasks represent a running process in our simple real-time operating system.
typedef struct _task {
    /*Tasks should have members that include: state, period,
        a measurement of elapsed time, and a function pointer.*/
    signed char state; //Task's current state
    unsigned long int period; //Task period
    unsigned long int elapsedTime; //Time elapsed since last task tick
    int (*TickFct)(int); //Task tick function
} task;

//--------End Task scheduler data structure-----------------------------------

//--------User defined FSMs---------------------------------------------------

//Enumeration of states.
enum SM2_States { SM2_init };

unsigned char tmp = 0x00;

int SMTick2(int state) {
    //State machine transitions
    switch (state) {
    case -1: // Init
      state = SM2_init;
        break;

   case SM2_init:
      tmp = 'test/n';

      state = SM2_init;
      break;
    default:
        state = -1;
        break;
    }

    //State machine actions
    switch(state) {
    case -1: // Init
        break;

    default:
        break;
    }

    return state;
}

//Enumeration of states.
enum SM3_States { SM3_init };

enum LT_States { LT_s0, LT_WaitLcdRdy, LT_WaitButton, LT_FillAndDispString,
LT_HoldGo1, LT_WaitBtnRelease } LT_State;

void LT_Tick() {
    static unsigned short j;
    static unsigned char i, x, c;
    switch(LT_State) { // Transitions
        case -1:
            LT_State = LT_s0;
            break;
        case LT_s0:
            LT_State = LT_WaitLcdRdy;
            break;
        case LT_WaitLcdRdy:
            if (!LCD_rdy_g) {
                LT_State = LT_WaitLcdRdy;
            }
            else if (LCD_rdy_g) {
                LT_State = LT_FillAndDispString;
            }
            break;
        case LT_FillAndDispString:
            LT_State = LT_HoldGo1;
            break;
        case LT_HoldGo1:
            LCD_go_g=0;
            LT_State = LT_WaitBtnRelease;
            break;
        case LT_WaitBtnRelease:
                LT_State = LT_WaitLcdRdy;
            break;
        default:
            LT_State = LT_s0;
        } // Transitions

    switch(LT_State) { // State actions
        case LT_s0:
            LCD_go_g=0;
            strcpy(LCD_string_g, "1234567890123456"); // Init, but never seen, shows use of strcpy though
            break;
        case LT_WaitLcdRdy:
            break;
        case LT_WaitButton:
            break;
        case LT_FillAndDispString:
         LCD_write_str = 0;
            for (i=0; i<16; i++) { // Fill string with c
                LCD_string_g[i] = tmp;
            }
            LCD_string_g[i] = '\0'; // End-of-string char
            LCD_go_g = 1; // Display string*/
            break;
        case LT_HoldGo1:
            break;
        case LT_WaitBtnRelease:
            break;
        default:
            break;
    } // State actions
}




// --------END User defined FSMs-----------------------------------------------

// Implement scheduler code from PES.
int main()
{
// Set Data Direction Registers
// Buttons PORTA[0-7], set AVR PORTA to pull down logic
DDRB = 0xFF; // Set port B to output
DDRD = 0xFF; // Set port D to output
DDRA = 0x00; PORTA = 0xFF;
DDRC = 0xF0; PORTC = 0x0F; // PC7..4 outputs init 0s, PC3..0 inputs init 1s
// . . . etc

// Period for the tasks
unsigned long int SMTick1_calc = 10;
unsigned long int SMTick2_calc = 50;
unsigned long int SMTick3_calc = 10;

//Calculating GCD
unsigned long int tmpGCD = 1;
tmpGCD = findGCD(SMTick1_calc, SMTick2_calc);
tmpGCD = findGCD(tmpGCD, SMTick3_calc);

//Greatest common divisor for all tasks or smallest time unit for tasks.
unsigned long int GCD = tmpGCD;

//Recalculate GCD periods for scheduler
unsigned long int SMTick1_period = SMTick1_calc/GCD;
unsigned long int SMTick2_period = SMTick2_calc/GCD;
unsigned long int SMTick3_period = SMTick3_calc/GCD;

//Declare an array of tasks
static task task1, task2, task3;
task *tasks[] = { &task1, &task2  , &task3};
const unsigned short numTasks = sizeof(tasks)/sizeof(task*);

// Task 1
task1.state = -1;//Task initial state.
task1.period = SMTick1_period;//Task Period.
task1.elapsedTime = SMTick1_period;//Task current elapsed time.
task1.TickFct = &LCDI_SMTick;//Function pointer for the tick.

// Task 2
task2.state = -1;//Task initial state.
task2.period = SMTick2_period;//Task Period.
task2.elapsedTime = SMTick2_period;//Task current elapsed time.
task2.TickFct = &SMTick2;//Function pointer for the tick.

// Task 2
task3.state = -1;//Task initial state.
task3.period = SMTick3_period;//Task Period.
task3.elapsedTime = SMTick3_period;//Task current elapsed time.
task3.TickFct = &LT_Tick;//Function pointer for the tick.



// Set the timer and turn it on
TimerSet(GCD);
TimerOn();

unsigned short i; // Scheduler for-loop iterator
while(1) {

   // Scheduler code
    for ( i = 0; i < numTasks; i++ ) {
        // Task is ready to tick
        if ( tasks[i]->elapsedTime == tasks[i]->period ) {
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
