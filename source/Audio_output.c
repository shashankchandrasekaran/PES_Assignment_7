/*
 * Filename: Audio_output.c
 * Created on: Nov 22, 2022
 * Author: Shashank Chandrasekaran
 * Description: This C file contains the function definitions related to DAC, DMA and TPM0
 * Reference: https://github.com/alexander-g-dean/ESF/blob/master/NXP/Code/Chapter_9/DMA_Examples/Source/DMA.c
 */

#include <stdio.h>
#include <stdint.h>
#include "MKL25Z4.h"
#include "Audio_output.h"
#include "systick.h"
#include "fp_trig.h"

#define DAC_POS (30) //Pin 30 is for analog output
#define CLOCK (48000000) //Clock frequency
#define OUTPUT_SAMPLE_RATE (48000) //DAC sampling frequency
#define SOURCE_SIZE (2) //Source size
#define DEST_SIZE (2) //Destination size
#define SEL_TPM0_OVFL (54) //For selecting TPM0 overflow for DMA
#define BCR_LIM (2) //To increase number of bytes stored in DMA0 BCR register
#define ONE_SEC_ELAP (16) //To indicate 1 second has elapsed

int tone_transition_req=0; //Set after 1 second is elapsed
uint16_t *Reload_DMA_Source; //Stores buffer's source address
uint32_t Reload_DMA_Byte_Count=0; //Stores total number of samples in buffer

/**********************************Initialize DAC0******************************************/
void Init_DAC0(void)
{
	//Turn on clock to DAC0 and Port E
	SIM->SCGC6 |= (1UL << SIM_SCGC6_DAC0_SHIFT);
	SIM->SCGC5 |= (1UL << SIM_SCGC5_PORTE_SHIFT);

	//Select analog
	PORTE->PCR[DAC_POS] &= ~(PORT_PCR_MUX(7));

	//Disable buffer mode
	DAC0->C1 = 0;
	DAC0->C2 = 0;

	//Enable DAC, select VDDA as reference voltage
	DAC0->C0 = (1 << DAC_C0_DACEN_SHIFT) |
			   (1 << DAC_C0_DACRFS_SHIFT);
}

/**********************************Initialize TPM0*****************************************/
void Init_TPM0(void)
{
	//Turn on clock to TPM0
	SIM->SCGC6 |= SIM_SCGC6_TPM0_MASK;

	//Set clock source for TPM0
	SIM->SOPT2 |= (SIM_SOPT2_TPMSRC(1) | SIM_SOPT2_PLLFLLSEL_MASK);

	//Disable TPM0
	TPM0->SC = 0;

	//Load the counter and mod
	TPM0->CNT = 0;
	TPM0->MOD = TPM_MOD_MOD((CLOCK/OUTPUT_SAMPLE_RATE)-1); //Generates delay at 48KHz

	//Set TPM0 to count up and divide clock by no prescaler
	TPM0->SC = (TPM_SC_DMA_MASK | TPM_SC_PS(0));
}

/**********************************Initialize DMA0*****************************************/
void Init_DMA0(void)
{
	//Gate clocks to DMA and DMAMUX
	SIM->SCGC7 |= SIM_SCGC7_DMA_MASK;
	SIM->SCGC6 |= SIM_SCGC6_DMAMUX_MASK;

	//Disable DMA channel to allow configuration
	DMAMUX0->CHCFG[0] = 0;

	// Generate DMA interrupt when done
	// Increment source, transfer words (16 bits)
	// Enable peripheral request
	DMA0->DMA[0].DCR = DMA_DCR_EINT_MASK | DMA_DCR_SINC_MASK |
					   DMA_DCR_SSIZE(SOURCE_SIZE) | DMA_DCR_DSIZE(DEST_SIZE) |
					   DMA_DCR_ERQ_MASK | DMA_DCR_CS_MASK;

	//Configure NVIC for DMA ISR
	NVIC_SetPriority(DMA0_IRQn, 2);
	NVIC_ClearPendingIRQ(DMA0_IRQn);
	NVIC_EnableIRQ(DMA0_IRQn);

	//Enable DMA MUX channel with TPM0 overflow as trigger
	DMAMUX0->CHCFG[0] = DMAMUX_CHCFG_SOURCE(SEL_TPM0_OVFL);
}

/**********************************Compute buffers******************************************/
void tone_to_samples(sample_buffers *out_buff)
{
	out_buff->samples_period=OUTPUT_SAMPLE_RATE/out_buff->frequency; //Samples per period
	out_buff->total_cycles=BUFFER_SIZE/out_buff->samples_period; //Total cycles in buffer
	out_buff->total_samples=out_buff->total_cycles*out_buff->samples_period; //Total samples in buffer
	for(int i=0;i<out_buff->total_samples;i++)
		//Stores the output of sine function in buffer as a sample
		out_buff->output_buffers[i]=(fp_sin(i*TWO_PI/out_buff->samples_period)+TRIG_SCALE_FACTOR)&0xFFFF;
}

/*************************Copy buffer start address to DMA source***************************/
void Copy_buffer_DMA0(sample_buffers *out_buff)
{
	TPM0->SC &= ~TPM_SC_CMOD_MASK; //Stop TPM0
	Reload_DMA_Source=&out_buff->output_buffers[0]; //Store start address
	Reload_DMA_Byte_Count=out_buff->total_samples; //Store total number of samples
	TPM0->SC |= TPM_SC_CMOD(1); //Start TPM0
}

/************************************Start DMA0********************************************/
void Start_DMA0(void)
{
	//Initialize source and destination pointers
	DMA0->DMA[0].SAR = DMA_SAR_SAR((uint32_t) Reload_DMA_Source);
	DMA0->DMA[0].DAR = DMA_DAR_DAR((uint32_t) (&(DAC0->DAT[0])));

	//Byte count
	DMA0->DMA[0].DSR_BCR = DMA_DSR_BCR_BCR(Reload_DMA_Byte_Count*BCR_LIM);

	//Clear done flag
	DMA0->DMA[0].DSR_BCR &= ~DMA_DSR_BCR_DONE_MASK;

	//Set enable flag
	DMAMUX0->CHCFG[0] |= DMAMUX_CHCFG_ENBL_MASK;
}

/************************************Start DMA0********************************************/
int tone_status_return(void)
{
	if(tone_transition_req==0)
		return 0;
	else if(tone_transition_req==1) //If 1 second has elapsed
	{
		tone_transition_req=0; //Reset flag
		return 1; //Return 1 indicating transition of tone required
	}
	return 0;
}

/*******************************DMA0 interrupt handler************************************/
void DMA0_IRQHandler(void)
{
	//Clear done flag
	DMA0->DMA[0].DSR_BCR |= DMA_DSR_BCR_DONE_MASK;

	//Check if 1 second has elapsed
	if(get_timer()>=ONE_SEC_ELAP)
	{
		tone_transition_req=1;
	}
	else
		//Start the next DMA cycle again
		Start_DMA0();
}
