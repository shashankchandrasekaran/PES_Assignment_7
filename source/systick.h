/*
 * File name:systick.h
 * Created on: Nov 22, 2022
 * Author: Shashank Chandrasekaran
 * Description: This H file contains the declaration of Systick timer related functions
 * Reference: Assignment 4 Timing.h
 */

#ifndef TIMING_H_
#define TIMING_H_

typedef uint32_t ticktime_t; //Time since boot, in sixteenth of a second

/*
 * Function for initialization of SysTick timer
 * Parameter passed: None
 * Parameters returned: None
 */
void Init_SysTick(void);


/*
 * Function to define operation to be performed when SysTick interrupt is called
 * every 63.5 msec
 * Parameter passed: None
 * Parameters returned: None
 */
void SysTick_Handler(void);


/*
 * Function to return time since startup, in sixteenth of a second
 * Parameter passed: None
 * Parameters returned: Total time since startup: TimerMS_62_5++
 */
ticktime_t now(void);


/*
 * Function to reset a variable that is used for calculating certain delay.
 * Resets a variable every time it is called,so that the counting can start again.
 * But it doesn't affect the now() values
 * Parameter passed: None
 * Parameters returned: None
 */
void reset_timer(void);


/*
 * Function to return the variable count (used for calculating delay) since
 * last call to reset function
 * Parameter passed: None
 * Parameters returned: Variable's count value since reset: TimerMS_62_5 - Start_Timer
 */
ticktime_t get_timer(void);

#endif /* TIMING_H_ */
