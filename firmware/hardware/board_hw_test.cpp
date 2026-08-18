#include "pch.h"
#include "hellen_meta.h"

static Gpio OUTPUTS[] = {
	// low-side outputs
	Gpio::MM100_INJ1, // J5-4 Injector 1
	Gpio::MM100_INJ2, // J5-5 Injector 2
	Gpio::MM100_INJ3, // J5-6 Injector 3
	Gpio::MM100_INJ4, // J14-4 Injector 4
	Gpio::MM100_INJ5, // J14-5 Injector 5
	Gpio::MM100_INJ6, // J14-6 Injector 6
	Gpio::MM100_INJ7, // J15-7 Low Side 1
	Gpio::MM100_INJ8, // J15-8 Low Side 2
	Gpio::MM100_IGN7, // J3-7 Low Side 3
	Gpio::MM100_IGN8, // J8-1 Low Side 4
	Gpio::MM100_USB1ID, // J18-7 Low Side 5
	Gpio::MM100_SPI3_MOSI, // J20-1 Low Side 6
	// J16-6 Low Side 7 is module pad IO2, STM32 pin TBD
	Gpio::MM100_LED4_YELLOW, // J16-7 Low Side 8
	Gpio::MM100_OUT_PWM1, // J15-5 Low Side 9
	Gpio::MM100_OUT_PWM2, // J15-6 Low Side 10
	// logic-level outputs
	Gpio::MM100_IGN1, // J5-1 Coil 1
	Gpio::MM100_IGN2, // J5-2 Coil 2
	Gpio::MM100_IGN3, // J5-3 Coil 3
	Gpio::MM100_IGN4, // J14-1 Coil 4
	Gpio::MM100_IGN5, // J14-2 Coil 5
	Gpio::MM100_IGN6, // J14-3 Coil 6
};

int getBoardMetaOutputsCount() {
    return efi::size(OUTPUTS);
}

int getBoardMetaLowSideOutputsCount() {
    return getBoardMetaOutputsCount() - 6;
}

Gpio* getBoardMetaOutputs() {
    return OUTPUTS;
}

// two TLE9201 H-bridges on the motor-driver module
int getBoardMetaDcOutputsCount() {
    return 2;
}
