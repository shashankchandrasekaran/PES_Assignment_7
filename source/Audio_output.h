/*
 * Filename: Audio_output.h
 * Created on: Nov 22, 2022
 * Author: Shashank Chandrasekaran
 * Description: This H file contains the function declarations related to DAC, DMA and TPM0
 * Reference: None
 */

#ifndef AUDIO_OUTPUT_H_
#define AUDIO_OUTPUT_H_

#define BUFFER_SIZE (1024) //size of buffer array

//Define structure containing tone related variables
typedef struct buffers
{
	int frequency;
	int samples_period;
	int total_cycles;
	uint32_t total_samples;
	uint16_t output_buffers[BUFFER_SIZE];
}sample_buffers;


/*
 * Function for initialization of DAC0 for generating analog output
 * Parameter passed: None
 * Parameters returned: None
 */
void Init_DAC0(void);


/*
 * Function for initialization of TPM0 to overflow at 48KHz sampling frequency
 * Parameter passed: None
 * Parameters returned: None
 */
void Init_TPM0(void);


/*
 * Function for initialization of DMA0 to copy samples from source to destination addresses
 * Parameter passed: None
 * Parameters returned: None
 */
void Init_DMA0(void);


/*
 * Function for computing samples from sine function and store in a buffer
 * Parameter passed: out_buff: Structure containing the buffer where samples
 * need to be stored
 * Parameters returned: None
 */
void tone_to_samples(sample_buffers *out_buff);


/*
 * Function for copying start address of buffer to source address and byte count to
 * a variable later passed in BCR register
 * Parameter passed: out_buff: Structure containing the buffer containing samples
 * Parameters returned: None
 */
void Copy_buffer_DMA0(sample_buffers *out_buff);


/*
 * Function for starting DMA0 to define source, destination address and byte counts
 * Parameter passed: None
 * Parameters returned: None
 */
void Start_DMA0(void);


/*
 * Function for returning tone status based on whether one second is elapsed or not
 * Parameter passed: None
 * Parameters returned: 1 if one second has elapsed and 0 if not
 */
int tone_status_return(void);


/*
 * DMA0 interrupt handler called every time entire buffer is copied from source
 * to destination address
 * Parameter passed: None
 * Parameters returned: None
 */
void DMA0_IRQHandler(void);

#endif /* AUDIO_OUTPUT_H_ */
