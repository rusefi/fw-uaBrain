/**
 * @file board_configuration.cpp
 *
 * uaBrain - a Hellen-One platform "brain" module with castellated edge connectors J2..J21.
 *
 * CPU module: mega-mcu100-f7 rev 0.3 (STM32F765, MM100_* namespace, see
 * ext/rusefi/firmware/config/boards/hellen_mm100_meta.h and
 * ext/rusefi/docs/hellen-board-mapping.md). Connector pin mapping lives in
 * connectors/J*.yaml.
 */

#include "pch.h"
#include "defaults.h"
#include "hellen_meta.h"
#include "board_overrides.h"

// blue LED is the only module LED still free: green (PD7) drives DC2 disable,
// yellow (PE8) drives Low Side output 8
Gpio getCommsLedPin() {
	return Gpio::MM100_LED3_BLUE;
}

Gpio getRunningLedPin() {
	return Gpio::Unassigned;
}

Gpio getWarningLedPin() {
	return Gpio::Unassigned;
}

static void setupDefaultSensorInputs() {
	// J4-3 TPS1, J4-4 TPS2
	engineConfiguration->tps1_1AdcChannel = MM100_IN_TPS_ANALOG;
	engineConfiguration->tps1_2AdcChannel = MM100_IN_AUX1_ANALOG;

	// J2-1 PPS1, J2-2 PPS2
	setPPSInputs(MM100_IN_PPS_ANALOG, MM100_IN_AUX2_ANALOG);

	// J21-6 external MAP; on-board MAP option sits on MM100_IN_MAP2_ANALOG (PC1)
	engineConfiguration->map.sensor.hwChannel = MM100_IN_MAP1_ANALOG;

	// J4-7 CLT, J16-1 IAT
	engineConfiguration->clt.adcChannel = MM100_IN_CLT_ANALOG;
	engineConfiguration->iat.adcChannel = MM100_IN_IAT_ANALOG;

	// J18-1/2 VR MAX9924 conditioner output - the safer default trigger
	engineConfiguration->triggerInputPins[0] = Gpio::MM100_UART8_TX;
	// J3-1/2 VR discrete conditioner output is available on Gpio::MM100_UART8_RX
	engineConfiguration->camInputs[0] = Gpio::MM100_IN_D1; // J3-4 HALL1
	engineConfiguration->camInputs[1] = Gpio::MM100_IN_D2; // J16-3 HALL2
	// J18-4 HALL3 (Gpio::MM100_IN_D3, SENT capable) left unassigned by default

	// J10-6 flex fuel input
	engineConfiguration->flexSensorPin = Gpio::MM100_IN_VSS;
}

static void uaBrain_boardConfigOverrides() {
	setHellenMegaEnPin();
	setHellenVbatt();

	hellenMegaSdWithAccelerometer();

	// VR discrete conditioner threshold, module pad OUT_PWM6 (PD14)
	engineConfiguration->vrThreshold[0].pin = Gpio::MM100_OUT_PWM6;

	// CAN1 on the module's on-board transceiver (PD0/PD1), J2-5/J2-6
	setHellenCan();

	setDefaultHellenAtPullUps();
}

static void uaBrain_boardInitHardware() {
	setupHellenSharedInputs();
}

static void uaBrain_boardDefaultConfiguration() {
	engineConfiguration->injectionPins[0] = Gpio::MM100_INJ1;
	engineConfiguration->injectionPins[1] = Gpio::MM100_INJ2;
	engineConfiguration->injectionPins[2] = Gpio::MM100_INJ3;
	engineConfiguration->injectionPins[3] = Gpio::MM100_INJ4;
	engineConfiguration->injectionPins[4] = Gpio::MM100_INJ5;
	engineConfiguration->injectionPins[5] = Gpio::MM100_INJ6;

	engineConfiguration->ignitionPins[0] = Gpio::MM100_IGN1;
	engineConfiguration->ignitionPins[1] = Gpio::MM100_IGN2;
	engineConfiguration->ignitionPins[2] = Gpio::MM100_IGN3;
	engineConfiguration->ignitionPins[3] = Gpio::MM100_IGN4;
	engineConfiguration->ignitionPins[4] = Gpio::MM100_IGN5;
	engineConfiguration->ignitionPins[5] = Gpio::MM100_IGN6;

	// two TLE9201 H-bridges on the motor-driver module
	// DC1: PWM=OUT_PWM3 (PC7), DIR=OUT_PWM4 (PC8), DIS=SPI2_MISO (PB14)
	setupTLE9201IncludingStepper(Gpio::MM100_OUT_PWM3, Gpio::MM100_OUT_PWM4, Gpio::MM100_SPI2_MISO);
	// DC2: PWM=OUT_PWM5 (PC9), DIR=SPI2_MOSI (PB15), DIS=LED_GREEN (PD7)
	setupTLE9201IncludingStepper(Gpio::MM100_OUT_PWM5, Gpio::MM100_SPI2_MOSI, Gpio::MM100_LED2_GREEN, 1);
	engineConfiguration->etbFunctions[0] = DC_Throttle1;

	// module on-board barometer on I2C PB10/PB11
	setHellenMMbaro();

	engineConfiguration->displayLogicLevelsInEngineSniffer = true;
	engineConfiguration->isSdCardEnabled = true;

	// two Hellen knock modules: J5-7 -> PA2, J14-7 -> PA1, see knock_config.h
	engineConfiguration->enableSoftwareKnock = true;

	engineConfiguration->canTxPin = Gpio::MM100_CAN_TX;
	engineConfiguration->canRxPin = Gpio::MM100_CAN_RX;
	// real CAN2 on PB12/PB13 through the on-board transceiver module, J16-4/J16-5
	setHellenCan2();

	// two on-board MAX31855 EGT chips on SPI3 (one-way, MOSI not needed)
	engineConfiguration->is_enabled_spi_3 = true;
	engineConfiguration->spi3misoPin = Gpio::MM100_SPI3_MISO;
	engineConfiguration->spi3sckPin = Gpio::MM100_SPI3_SCK;
	engineConfiguration->max31855spiDevice = SPI_DEVICE_3;
	// EGT2 chip select is the module SPI3_CS pad (PA15)
	engineConfiguration->max31855_cs[1] = Gpio::MM100_SPI3_CS;
	// TODO: EGT1 chip select is the mega-mcu100-f7 module pad IO1 - STM32 pin not yet
	// confirmed (PD8/PD9 are the prime candidates since this module has no board-ID pads);
	// set max31855_cs[0] once the module schematic mapping for IO1/IO2 is verified.

	setupDefaultSensorInputs();

	// two on-board CAN wideband controllers
	hellenWbo();
	engineConfiguration->enableVerboseCanTx = true;

	setCrankOperationMode();
	setAlgorithm(engine_load_mode_e::LM_SPEED_DENSITY);
	engineConfiguration->injectorCompensationMode = ICM_FixedRailPressure;

#ifndef EFI_BOOTLOADER
	setCommonNTCSensor(&engineConfiguration->clt, HELLEN_DEFAULT_AT_PULLUP);
	setCommonNTCSensor(&engineConfiguration->iat, HELLEN_DEFAULT_AT_PULLUP);
#endif // EFI_BOOTLOADER
}

void setup_custom_board_overrides() {
	custom_board_InitHardware = uaBrain_boardInitHardware;
	custom_board_DefaultConfiguration = uaBrain_boardDefaultConfiguration;
	custom_board_ConfigOverrides = uaBrain_boardConfigOverrides;
}
