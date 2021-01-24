////////////////////////////////////////////
// Author: 	Linus Eriksson
// Date:	2017-04-22
// 
// Implements some basic functionality for
// using the Arduino interface on the
// DE10-Lite board.
// 
// Set mode of pin by using function: arduino_pin_mode
//
// Read from digital pin with function: arduino_digital_read
//
// Write to digital pin with function: arduino_digital_write
//
// Enable analog input by calling: arduino_analog_enable 
// and specify base addresses for sample store and sequencer
//
// Default state of pins is PIN_INPUT. Unused pins value are
// unknown. No internal pull-ups.

#ifndef DE10LITEARDUINODRIVER_H_INCLUDED
#define DE10LITEARDUINODRIVER_H_INCLUDED

#include <system.h>
#include <io.h>
#include <alt_types.h>
#include <altera_modular_adc.h>

#define PIN_D0 0
#define PIN_D1 1
#define PIN_D2 2
#define PIN_D3 3
#define PIN_D4 4
#define PIN_D5 5
#define PIN_D6 6
#define PIN_D7 7
#define PIN_D8 8
#define PIN_D9 9
#define PIN_D10 10
#define PIN_D11 11
#define PIN_D12 12
#define PIN_D13 13
#define PIN_D14 14
#define PIN_D15 15
#define PIN_A0 0
#define PIN_A1 1
#define PIN_A2 2
#define PIN_A3 3
#define PIN_A4 4
#define PIN_A5 5
#define PIN_INPUT 0
#define PIN_OUTPUT 1
#define UART_ON 1
#define UART_OFF 0
#define SIGNAL_LOW 0
#define SIGNAL_HIGH 1
#define ANALOG_VOLTAGE_REFERENCE 2.5f

static alt_u32 sequencer;
static alt_u32 sample_store;

// Set digital pin 1&2 as UART whit UART_ON, UART_OFF
void arduino_uart_mode(alt_u8 mode) {
	IOWR_32DIRECT(DE10_LITE_ARDUINO_IP_0_BASE,8,mode);
}

// Set digital pin as either PIN_INPUT or PIN_OUTPUT
void arduino_pin_mode(alt_u8 pin,alt_u8 mode) {
	IOWR_32DIRECT(DE10_LITE_ARDUINO_IP_0_BASE,0,((pin<<1)|mode));
}

// Write to digital pin specified as output
void arduino_digital_write(alt_u8 pin,alt_u8 signal) {
	IOWR_32DIRECT(DE10_LITE_ARDUINO_IP_0_BASE,4,((pin<<1)|signal));
}

// Read from digital pin specified as input
alt_u8 arduino_digital_read(alt_u8 pin) {
	return IORD_32DIRECT(DE10_LITE_ARDUINO_IP_0_BASE,pin*4);
}

// Start ADC in continuous mode
void arduino_analog_enable(alt_u32 sequencer_base,alt_u32 sample_store_base) {
	sequencer=sequencer_base;
	sample_store=sample_store_base;
	adc_stop(sequencer);
	adc_set_mode_run_continuously(sequencer);
	adc_start(sequencer);
}

// Read analog pin, DE10-Lite accepts up to 5.0v as input.
// Downscaled internally to max 2.5v. See voltage read function
// below for info on how to use values.
alt_u32 arduino_analog_read(alt_u8 pin) {
	alt_u32 reading;
	alt_adc_word_read(sample_store+(pin*4),&reading,1);
	return reading;
}

// Read analog pin and convert to voltage level
float arduino_analog_read_voltage(alt_u8 pin) {
	alt_u32 reading;
	alt_adc_word_read(sample_store+(pin*4),&reading,1);
	return ((((float)(reading<<1))*ANALOG_VOLTAGE_REFERENCE)/4096.0f);
}

#endif // DE10LITEARDUINODRIVER_H_INCLUDED
