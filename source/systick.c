/*
 * File name: systick.c
 * Created on: Nov 22, 2022
 * Author: Shashank Chandrasekaran
 * Description: This C file contains the definitions of Systick timer related functions
 * Reference: Assignment 4 Timing.c
 */

#include <stdio.h>
#include "MKL25Z4.h"
#include "systick.h"

#define SYSTICK_LOAD (187500) //Load value for obtaining 62.5 msec delay
#define MS_62_5 (62.5) //Value used to obtain time in multiples of 62.5 msec

ticktime_t TimerMS_62_5=0, Start_Timer=0; //Variables used for timing calculations

/***************************Initializing SysTick Timer**************************************/
void Init_SysTick(void)
{
	SysTick->LOAD=SYSTICK_LOAD; //Set reload to get 62.5 ms delay
	NVIC_SetPriority(SysTick_IRQn,3); //Set interrupt priority to 3
	SysTick->VAL=0; //Force load of reload value
	SysTick->CTRL=SysTick_CTRL_TICKINT_Msk| //Enable Interrupt, alt. clock source
			      SysTick_CTRL_ENABLE_Msk; //Enable SysTick timer
}

/***************************SysTick interrupt handler**************************************/
void SysTick_Handler(void)
{
	TimerMS_62_5++;
}

/*************************Returns time since Startup**************************************/
ticktime_t now(void)
{
	return (TimerMS_62_5)*(MS_62_5);
}

/*************************Resets the Start timer variable**********************************/
void reset_timer(void)
{
	Start_Timer=TimerMS_62_5;
}

/***************Returns Start variable count since last call to reset ********************/
ticktime_t get_timer(void)
{
	return (TimerMS_62_5 - Start_Timer);
}
