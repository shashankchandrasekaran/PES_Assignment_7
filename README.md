# PES_Assignment_7
PES Assignment 7: Created by: Shashank Chandrasekaran Decription: Musical Notes implementation using DAC and ADC

Following are the .c files created:

Audio_output.c : This file contains the definitions of functions like DAC0, TPM0 and DMA0. The flow is such that TPM0 is generating a delay based on 48 KHz sampling 
frequency. Each time the TPM0 overflows, one sample from a pre-calculated buffer of sample values will be copied by DMA from the current buffer address to the destination address. Once the entire buffer is copied, the DMA interrupt will be called and the entire process will restart. The buffer is calculated based on the sample values returned by sine function.

Audio_input.c : This file contains the definitions of functions like ADC0 and TPM1. The flow is such that ADC0 samples the analog output generated by DAC and these sample values are stored in a buffer. The buffer in turn is passed to an autocorrelate function which returns period from which we can find frequency.

systick.c : This file uses the systick timer to generate interrupt every 62.5 ms. The reset_timer() and get_timer() function can be used to get the 1 second delay for each tone.

PES_Assignment_Musical_Notes.c : Contains the main subroutine that calls the major functions discussed above by integrating them.

The DAC portion was tested first for one sine wave. The sample values that were obtained from sine function was plotted in excel and a proper sine wave could be seen. This was the first step that assured the sample values were correct. The next step was to introduce the transition logic, for which systick timer was used for obtaining accurate timing of 1 sec. The approach was such that as systick would check that 1000 msec has elapsed, the tone would be changed immediately. But the problem faced here was that there was clipping at the transitions. To solve this issue, the elapsed time was checked inside the DMA interrupt handler and tone was changed once remaining cycles was completed before beggining new cycles of the next tone. This eliminated the clippings between tones. Since systick timer was used, the transition is very close to 1000 msec.
Once the DAC was implemented , the ADC was configured and it was checked if the frequency obtained by ADC samples matched the actual frequency.
Also, the Pin 30 was connected to oscilloscope to check the frequency. It was concluded that the values observed were 444Hz, 592Hz, 666Hz and 888Hz which is within the +- 1.5% offset.

References were taken from Assignment 4 submission for systick and Dean's github link which are mentioned in the source files.

The screenshots for the 4 tones are added to the main branch. These can be found as 4 .PNG files.
