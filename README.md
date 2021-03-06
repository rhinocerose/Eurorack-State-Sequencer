# Eurorack-State-Sequencer

## Description
The state sequencer is an NXP LPC804 (ARM Cortex-M0+) based stored-voltage sequencer for the Eurorack modular synthesis system. This was created for the EECS3215 (Embedded Systems) class as part of my Bachelor's of Electrical Engineering degree at the Lassonde School of Engineering at York University.

## Features
- 8-channel 14-bit output
  - TI DAC8168 
  - Theoretically accurate to 0.00061V (6.1 mV)
  - pin and library compatible with DAC7568, DAC8568)
- States displayed on 132x64 1.3" OLED (SH1106)
- Reverse power protection via Schottky diodes

## Development

### Process
I decided on the DAC8168 because it exists in pin-compatible variants from TI and so allowed for increasing or decreasing resolution as required. This DAC is only avaiable in SMD so in order to breadboard the circuit I decided to print some breakout PCBs that included the recommended conditioning as per the data sheet.

### uProcessor Considerations
I was initially hesitant to use the LPC804 as it seemed underpowered and unsupported for the application at hand. After working with it, I realized that with a bit of finessing it is quite a capable little beast. So I wrote out some interfacing functions.

### Middleware
I focused primarily on 

## Acknowledgments
