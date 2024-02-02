

### New features with this branch:
* Drag pin inactivation are now handled smartly, by actually sensing if pin is up before sending acknowledge; this means that static delays are no longer needed in OpenPnP setup, and a  much more robust drag pin operation.
* Anti Stiction Wiggle (ASW). If dragpin gets stuck, FW automatically tries to free it by quickly moving dragpin in a X/Y back/forth/left/righ pattern until it is free (or give up if it is not freed). At any time ASW has been engaged, the 'ok' back to OpenPnP has a comment attached to it, detailing what the ASW result was - e.g. "2023-12-13 11:22:54.328 GcodeDriver$ReaderThread TRACE: [GcodeDriver:ttyUSB0] << ok  ; ASW: l1,t4 (G1 X-0.1 Y-0.05)".
ASW is dependant on the smart drag pin activation code. Both can be enabled by adding a new property "switch.dragpin.dragpin true" to the group of switch.dragpin in the config.default. This property tells the generic code, that this pin is connected to a dragpin, and to activate the advanced mechanisms. Setting its value to false disables andy advanced logic.

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


