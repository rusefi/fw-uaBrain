https://github.com/rusefi/rusefi/wiki/uabrain

See https://github.com/rusefi/rusefi/wiki/Custom-Firmware

# uaBrain

uaBrain is a Hellen-One platform "brain" module

## Pin mapping

Following [hellen-board-mapping.md](ext/rusefi/docs/hellen-board-mapping.md),
each edge connector has a `connectors/J*.yaml` that maps connector pins to
`MM100_*` meta names from
`ext/rusefi/firmware/config/boards/hellen_mm100_meta.h`; `board.mk` includes
`hellen-common100.mk` (sets `HW_HELLEN`, board-ID pins, critical-error LED) and
`board_configuration.cpp` assigns defaults using only meta names.

Connector overview (extracted from `hw-uaBrain.kicad_pcb` pad→net data):

| Connector | Contents |
|-----------|----------|
| J5 (right) | Coils 1-3, Injectors 1-3, Knock 1, GNDA |
| J14 (top) | Coils 4-6, Injectors 4-6, Knock 2, GNDA |
| J15 (left) | +12V, key voltage (VIGN), GND, Low Side 9/10/1/2 |
| J2 (left) | PPS1/PPS2, +5VP, GNDA, CAN1 H/L, Buttons 1/2 |
| J20 (left) | Low Side 6, Button 3, +5VP, GNDA, 3.3VA, 3.3V, 5VA, 5V |
| J8 (right) | Low Side 4, AIN1, +5VP, GNDA, 3.3VA, 3.3V, 5VA, 5V |
| J3 (right) | VR discrete +/-, HALL1, Low Side 3, GND |
| J18 (top) | VR MAX9924 +/-, HALL3 (SENT), Low Side 5, GND |
| J4 (top) | DC1 +/-, TPS1/TPS2, CLT, +5VP, GNDA |
| J21 (top) | DC2 +/-, AIN2, MAP, +5VP, GNDA |
| J16 (top) | IAT, HALL2, CAN2 H/L, Low Side 7/8, GND |
| J10 (left) | PROG button, USB, Flex, +5VP, GNDA |
| J19 (right) | WBO1 LSU 4.9, EGT1, +12V RAW |
| J13 (top) | WBO2 LSU 4.9, EGT2, +12V RAW |

Signal-to-STM32 notes:

- Low Side 1..10 map to module pads INJ7, INJ8, IGN7, IGN8, USB1ID, SPI3_MOSI,
  IO2 (pin TBD), LED4_YELLOW, OUT_PWM1, OUT_PWM2.
- DC1 = OUT_PWM3/OUT_PWM4/SPI2_MISO, DC2 = OUT_PWM5/SPI2_MOSI/LED2_GREEN
  (TLE9201 PWM/DIR/DIS).
- Trigger conditioners feed MM100_UART8_TX (MAX9924) and MM100_UART8_RX
  (discrete), threshold on OUT_PWM6.
- Knock 1/2 on PA2/PA1 (software knock, ADC3), see `knock_config.h`.
- CAN1 uses the module's on-board transceiver (PD0/PD1); CAN2 is PB12/PB13
  through the on-board CAN module.
- EGT via two MAX31855 on SPI3: EGT2 CS = SPI3_CS (PA15); EGT1 CS = module pad
  IO1 - STM32 pin still to be confirmed (see TODO in board_configuration.cpp).
- UART2 (PD5/PD6) connects to the on-board ESP32-S2 bridge.
