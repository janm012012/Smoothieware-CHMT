

### This fork is about DMA on the serial/RS232 hardware with (or without) handshaking and increased + more reliable throughput, as well as drag pin enhancements. 

With this branch, DMA is implemented on rx as well as tx with hardware flow control.
Hardware flow control can be disabled by setting rts_cts_handshake to false in config.defaults. So in theory, this branch should work with stock machine, up to 115200 Baud (CHM-T36), as long as confirmation flow control is still enabled in OpenPnP. (115200 comes from the limitation of the rs232 level shifter, U33, populated on the controller board). A CHM-T48 should be able to achieve 480kBaud, limited by the rs422 level shifter.

Note; the author has long since updated his machine to RTS/CTS and non-RS232/422 levels, bitrate of 4mbit, so using a stock machine is currently untested. 

There are still benefits to use this code on a stock machine, just not very noticable. 
In theory, a CHMT36 should be able to be set-up to use RTS control without doing any board changes, by specifying the UART2 tx pin in Kernel.cpp as rts pin (set cts to NC). This has (not yet) been tested. Then you would at least no longer need "Confirmation Flow Control".


In order to benefit from higher thoughput and hardware flow control, you will need to modify your control board.
The changes needed, can be defined in two groups; one for the actual hardware flow control, and one for increased bitrate.
The latter probably needs the former to be useful.

This guide presumes that you want to go all in, and go for logic signalling levels directly to the isolators. Normally 3.3V signalling level.

Both 36 and 48 models share the same control board, with a little difference; the 48 has a native rs422 interface populated, whereas the 36 has rs232.
#### For the 48 models, the following needs to be done;
* Add a 1+1 channel isolator chip to unpopulated position U31 e.g. ADUM121NOBRZ-RL7. Note; put a piece of kapton tape on pads 2 & 3, since they shall not be soldered to the board pads, but instead be connected to the rts/cts wires described below.
* Pull two wires from an unpopulated U18 pin 4 & 1 (SO8) (rs485 transciever) to pin 2 & 3 of the new isosolator chip (not to the pads!). These are the RTS/CTS signals.
* Position U33 is made for a rs232 level shifter. The rx/tx/rts/cts path needs to be connected by wires, since we don't want 232 levels, in order to achieve higher speeds. See picture. 
* Add a 4 pole through hole mount JST connector (board uses China brand Yeonho Electronics SMW250/SMH250 throughout, so if you have those at hand that would be nicer, but the JST are very similar in all aspects but the locking).
* Move the 0R resistor from position R132 to position R131. This will connect the rx signal from the rs232 input instead of the original rs422.
* Remove (or keep) the ESD protection network close to the connectors. (See benefits below).

#### For the 36 models, which already have the rs232 you need to;
* disconnect pin 2 & 3 on U31 from board pads.
* Pull two wires from an unpopulated U18 pin 4 & 1 (SO8) (rs485 transciever) to pin 2 & 3 of U31 (not to the pads!). These are the RTS/CTS signals.
* Remove U33 (see above) and connect 4 wires in its place.
* Remove (or keep) the ESD protection network close to the connectors. (See benefits below).

#### For both machines;
* The USB-serial adapter is best kept as close to the controller boardd as possible, this is especially true if you decide to go for 3.3V signalling since it is very fast and more susceptible to external noise. 
* I have tested several USB-serial adapters (bridges), and found that the ones based on XR21B1420 works best (tested in linux only). So far I have been using a XR21B1420IL28-0A-EVB, that I have stuffed just beside the control board with short wires. The serial adapter needs to power the isolators (needs be the same Volage as the signalling level of the serial bridge) e.g. 3.3V.
* The ESD protection components are meant to protect the interface. This is needed especially if RS232 signalling levels are selected, and the RS232 are pulled outside the chmt casing. If 3,3V signalling levels, you probably should remove them, since the USB interface will be the interface to the outer world, and normally it already has protection. I have not tested to run my board with the ESD components fitted, so I don't know if it will work with them in place. But remember that orignial machine was for 115kbits, now we are running several mbits. If you remove them, you still need to make sure to put 0R resistors or solder blobs to complete the signal path.
* Option: If you like to stay with rs232 levels for whatever reason, you can populate U33 with e.g. SN65C3232EDR instead, which will allow speeds up to 921600bps. You then also will need to add a few SMD 100nF caps around U33 on the unpopulated posistions.


#### How to configure the serial port 
If you did add RTS/CTS signals above;
In config.default there are two relevant lines; one for specifying the baudrate you wish to run, and one setting for enabling RTS/CTS hardware flow control.
In OpenPnP you will need to select RTS/CTS flow control, and uncheck the "Confirmation Flow Control" since we will not need it any more.

If you have a standard board, you need to set the baudrate to 115200/460k and set flow control to false in the config.defaults.

#### A note of warning 
Chmt is rather noisy, and chasing higer bitrates might mean that the serial lines gets disturbed. Keep the wires between the USB-Serial board and the the mainboard as short as possible.


A picture of the patch prior removing the rs232 (U33) chip;
![rts_cts_patch](https://user-images.githubusercontent.com/18227864/158996475-5d222994-015a-4fb8-b81a-a45bb956cf9d.jpg)



#### Also included in this branch is;
* Drag pin deactivation now waits for drag pin to arrive up, before returning ok to openpnp, waiting up to 1 second, before giving up. Be sure to add a booelan into config file, "dragpin true", to let the code know that the actuator infact is a drag pin, and that it should wait for it coming up.
* Actuators are now not waiting for motion queues to be empty before actuating. This along with drag pin enhancement above generally saves about 500ms on a typical drag/feed operation.
* Based on Chris Riegel's fork
* Jan's (janm012012) additions for ligthing for down camera, and increased z-limits etc.
* A reboot check in gcode dispatch, that will halt the machine if a software/watchdog etc reset has occurred. (non-power on start) and send a message why onto OpenPnP. (Clear HALT with M999).
* A minor memory leak fix from smootheware upstream (M115)




## Old STM32/CHMT Notes from upstream

To build, follow normal smoothie build process to get setup.  Then checkout chmt branch and rebuild.

### Port Status:
* mbed hooks - Added, compiles, tested
* stm32f4xx libs - Added, compiles, tested
* timers - Ported, compiles, tested
* wdt - Ported, compiles, tested
* gpio - Ported, compiles, tested
* adc - Ported, compiles, tested
* pwm - Ported, compiles, tested
* build scripts - Added, project builds successfully

### CHMT Status:
* config file - 48VB Complete
* pin map - 48VB/36VA Complete
* operation/verfication - All System Functions Operational (excl. axis encoders)
* machine testing - 48VB All Systems Operational

### TODO:
* DONE: Target initialization and board bringup (clocks, mpu, etc)
* DONE: Verification of ported peripherals (step generation, watchdog, gpios)
* DONE: Debug/Comm uart setup
* DONE: CHMT controller specific configuration

### Notes/Caveats/Gotchas:
* smoothie mbed was ancient, so the oldest stm32 mbed available was integrated to reduce friction -- incompatibilities, and bugs from dated mbed may have been introduced
* MRI (gdb over serial) is not supported on stm32, use SWD/JTAG
* config file must be hardcoded into firmware build

### Next Steps/Priority
* CHMT Pinout Reversing -- Complete
* CHMT Config File Development -- 48VB Complete
* CHMT Machine Testing -- All Base Functions Operational, Long term and stability testing required.
* Synchronize System GCODEs to OpenPNP standards
* Stability Testing Required.
* WDT rewrite for longer timeout

# Smoothie

## Overview
Smoothie is a free, opensource, high performance G-code interpreter and CNC controller written in Object-Oriented C++ for the LPC17xx micro-controller ( ARM Cortex M3 architecture ). It will run on a mBed, a LPCXpresso, a SmoothieBoard, R2C2 or any other LPC17xx-based board. The motion control part is a port of the awesome grbl.

Documentation can be found here : [[http://smoothieware.org/]]

NOTE it is not necessary to build Smoothie yourself unless you want to. prebuilt binaries are available here: [[http://triffid-hunter.no-ip.info/Smoothie.html|Nightly builds]] and here: [[https://github.com/Smoothieware/Smoothieware/blob/edge/FirmwareBin/firmware.bin?raw=true|recent stable build]]

## Quick Start
These are the quick steps to get Smoothie dependencies installed on your computer:
* Pull down a clone of the Smoothie github project to your local machine.
* In the root subdirectory of the cloned Smoothie project, there are install scripts for the supported platforms.  Run the install script appropriate for your platform:
** Windows: win_install.cmd
** OS X: mac_install
** Linux: linux_install
* You can then run the BuildShell script which will be created during the install to properly configure the PATH environment variable to point to the required version of GCC for ARM which was just installed on your machine.  You may want to edit this script to further customize your development environment.

## Building Smoothie
Follow this guide... [[http://smoothieware.org/compiling-smoothie]]

In short...
From a shell, switch into the root Smoothie project directory and run:
{{{
make clean
make all
}}}

To upload you can do

{{{
make upload
}}}

if you have dfu-util installed.

Alternatively copy the file LPC1768/main.bin to the sdcard calling it firmware.bin and reset.

## Filing issues (for bugs ONLY)
Please follow this guide [[https://github.com/Smoothieware/Smoothieware/blob/edge/ISSUE_TEMPLATE.md]]

## Contributing

Please take a look at : 

* http://smoothieware.org/coding-standards
* http://smoothieware.org/developers-guide
* http://smoothieware.org/contribution-guidlines

Contributions very welcome !

## Donate
The Smoothie firmware is free software developed by volunteers. If you find this software useful, want to say thanks and encourage development, please consider a 
[[https://www.paypal.com/cgi-bin/webscr?cmd=_donations&business=9QDYFXXBPM6Y6&lc=US&item_name=Smoothieware%20development&currency_code=USD&bn=PP%2dDonationsBF%3abtn_donate_SM%2egif%3aNonHosted|Donation]]

## License

Smoothieware is released under the GNU GPL v3, which you can find at http://www.gnu.org/licenses/gpl-3.0.en.html


