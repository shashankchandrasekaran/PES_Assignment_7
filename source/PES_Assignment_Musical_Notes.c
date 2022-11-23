/*
 * Filename: PES_Assignment_Musical_Notes.c
 * Created on: Nov 22, 2022
 * Author: Shashank Chandrasekaran
 * Description: This C file contains the main subroutine that integrates all elements together
 * Reference: None
 */

#include <stdio.h>
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "MKL25Z4.h"
#include "fsl_debug_console.h"
#include "systick.h"
#include "Audio_output.h"
#include "Audio_input.h"

#define BUFFER_ARRAY_SIZE (4) //Number of tones
#define RESET_TONE (3) //Restart tone from 440Hz after this value

//4 tones
#define TONE1 (0)
#define TONE2 (1)
#define TONE3 (2)
#define TONE4 (3)

//Frequencies of 4 tones
#define TONE1_FREQ (440)
#define TONE2_FREQ (587)
#define TONE3_FREQ (659)
#define TONE4_FREQ (880)

//Array of structures to store contents of each tone
sample_buffers tones[BUFFER_ARRAY_SIZE];

//Main subroutine
int main(void)
{
	/* Init board hardware. */
	BOARD_InitBootPins();
	BOARD_InitBootClocks();
	BOARD_InitBootPeripherals();
#ifndef BOARD_INIT_DEBUG_CONSOLE_PERIPHERAL
	/* Init FSL debug console. */
	BOARD_InitDebugConsole();
#endif

	int tone_track=0; //To keep track of current tone
	uint16_t receive_buffer[BUFFER_SIZE]; //To store samples obtained by ADC

	//Pre-calculate the 4 buffers
	tones[TONE1].frequency=TONE1_FREQ;
	tones[TONE2].frequency=TONE2_FREQ;
	tones[TONE3].frequency=TONE3_FREQ;
	tones[TONE4].frequency=TONE4_FREQ;
	for(int i=0;i<BUFFER_ARRAY_SIZE;i++)
		tone_to_samples(&tones[i]);

	//Initialization sequence
	Init_DAC0();
	Init_DMA0();
	Init_TPM0();
	Init_TPM1();
	Init_ADC0();
	Copy_buffer_DMA0(&tones[0]); //Copy contents of tone 0
	printf("\rGenerated %d samples at %d Hz; computed period=%d samples\r\n",
			tones[0].total_samples,tones[0].frequency,tones[0].samples_period);
	Init_SysTick();
	TPM0->SC |= TPM_SC_CMOD(1); //Start TPM0
	Start_DMA0(); //Start DMA0
	ADC0_samples_buffer(&receive_buffer[0]); //Sample analog output for 440Hz tone
	ADC0_print_samples(&receive_buffer[0]); //Print frequency and other parameters based on ADC samples
	while(1)
	{
		if(tone_status_return()==1) //If 1 second has elapsed
		{
			tone_track++; //Change to next tone
			Copy_buffer_DMA0(&tones[tone_track]); //Copy next tone contents
			reset_timer(); //Reset timer
			Start_DMA0(); //Start DMA again
			ADC0_samples_buffer(&receive_buffer[0]); //Sample the analog output
			printf("\rGenerated %d samples at %d Hz; computed period=%d samples\r\n",
					tones[tone_track].total_samples,tones[tone_track].frequency,tones[tone_track].samples_period);
			ADC0_print_samples(&receive_buffer[0]);
		}
		if(tone_track==RESET_TONE) //If last tone is reached, reset the tone
		{
			tone_track=-1;
		}
	}
	return 0;
}
