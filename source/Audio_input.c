/*
 * Filename: Audio_input.c
 * Created on: Nov 22, 2022
 * Author: Shashank Chandrasekaran
 * Description: This C file contains the function definitions related to ADC0 and TPM1
 * Reference: Dean Chapter 6 Listing 6.5 and 6.6
 */

#include <stdio.h>
#include "MKL25Z4.h"
#include "Audio_input.h"
#include "autocorrelate.h"

#define CLOCK (48000000) //Clock frequency
#define INPUT_SAMPLE_RATE (96000) //ADC sampling frequency
#define BUFF_SIZE (1024) //Number of samples to be stored in buffer
#define SINGLE_ENDED_CONV (3) //For single ended conversion
#define INPUT_CHANNEL_SEL (23) //For selecting AD0 input channel 23
#define SEL_TPM1_OVFL (9) //For selecting TPM1 overflow for ADC trigger


/**********************************Initialize TPM1*******************************************/
void Init_TPM1(void)
{
	//Turn on clock to TPM1
	SIM->SCGC6 |= SIM_SCGC6_TPM1_MASK;

	//Disable TPM
	TPM1->SC = 0;

	//Load the counter and mod
	TPM1->CNT = 0;
	TPM1->MOD = TPM_MOD_MOD((CLOCK/INPUT_SAMPLE_RATE)-1);  //Generates delay at 96KHz
}


/**********************************Initialize ADC0*******************************************/
void Init_ADC0(void)
{
	//Turn on clock to ADC0
	SIM->SCGC6 |= SIM_SCGC6_ADC0_MASK;

	//Normal power config, divide ratio 1, short sampling period
	ADC0->CFG1 = ADC_CFG1_ADLPC(0) | ADC_CFG1_ADIV(0) | ADC_CFG1_ADLSMP(0) |
			ADC_CFG1_MODE(SINGLE_ENDED_CONV) | ADC_CFG1_ADICLK(0); // single-ended 16-bit conversion, select bus clock

	//Disabled Compare function, DMA disabled, default reference voltage
	ADC0->SC2 = ADC_SC2_ADTRG(1) | ADC_SC2_ACFE(0) | ADC_SC2_DMAEN(0) | ADC_SC2_REFSEL(0);

	//ADC interrupt enabled, Single ended conversion 10111 When DIFF=0, AD23 is selected as input for ADC
	ADC0->SC1[0] = ADC_SC1_AIEN(0) | ADC_SC1_DIFF(0) | ADC_SC1_ADCH(INPUT_CHANNEL_SEL);

	//Select TPM1 for ADC0
	SIM->SOPT7 = SIM_SOPT7_ADC0ALTTRGEN(1) | SIM_SOPT7_ADC0TRGSEL(SEL_TPM1_OVFL) |
				 SIM_SOPT7_ADC0PRETRGSEL(0);
}

/**********************************ADC0 samples buffer***************************************/
void ADC0_samples_buffer(uint16_t *inp_buff)
{
	TPM1->SC |= TPM_SC_CMOD(1); //Start TPM1
	for(int i=0;i<BUFF_SIZE;i++) //For 1024 samples
	{
		//Wait for conversion to finish
		while (!(ADC0->SC1[0] & ADC_SC1_COCO_MASK));
		inp_buff[i] = ADC0->R[0]; //Store sampled value in buffer
	}
	TPM1->SC &= ~TPM_SC_CMOD_MASK; //Stop TPM1
}

/**********************************Print ADC samples*****************************************/
void ADC0_print_samples(uint16_t *inp_buff)
{
	int sum=0,min,max,period; //To calculate minimum, maximum, sum and period of samples
	//Calculate min, max and total sum
	min=max=inp_buff[0];
	for(int i=0;i<BUFF_SIZE;i++)
	{
		if(min>inp_buff[i])
			min=inp_buff[i];
		if(max<inp_buff[i])
			max=inp_buff[i];
		sum+=inp_buff[i];
	}
	//obtain period from autocorrelate function
	period=autocorrelate_detect_period(inp_buff,BUFF_SIZE,kAC_16bps_unsigned);
	printf("\rmin=%d max=%d avg=%d period=%d samples frequency=%d Hz\n\n",
			min,max,sum/BUFF_SIZE,period,INPUT_SAMPLE_RATE/period);
}
