Hardware requirements

=====================

- Mini/micro USB cable

- MIMXRT1050-EVK
- Personal Computer
- 3.5mm headset



- Loopback network connector
- Speaker(8¦¸ 1W)



Prepare the Demo

===============

1.  Connect a USB cable between the host PC and the OpenSDA USB port on the target board.
2.  Open a serial terminal with the following settings:

- 115200 baud rate
    
- 8 data bits
    
- No parity
    
- One stop bit
    
- No flow control


3.  Download the program to the target board.
4.  Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.



Running the demo

================

The log below shows the output of the hello world demo in the terminal window:



***********************************************************
        FAT Tool for iMXRT1050
***********************************************************

***********************************************************
CPU:        600000000 MHz
SDR:        132000000 MHz
IPG:        150000000 MHz
DEBUG UART: 24000000 MHz
***********************************************************


1 - Do All Tests
2 - SDRAM RW Test
3 - Ethernet Loopback Test
4 - Audio Play Test
5 - Motion Sensor ReadID Test
Please input index for test (1 - 5), 0 for restart:

Then you can start the FAT test!
