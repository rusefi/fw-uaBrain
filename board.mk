include $(BOARD_DIR)/firmware/firmware.mk


BOARDINC += $(BOARD_DIR)/generated/controllers/generated

# defines SHORT_BOARD_NAME
include $(BOARD_DIR)/meta-info.env

# uaBrain is a Hellen-One platform board built around the mega-mcu100-f7 CPU module
# (STM32F765, MM100_* namespace, see ext/rusefi/firmware/config/boards/hellen_mm100_meta.h
# and ext/rusefi/docs/hellen-board-mapping.md)
DDEFS += -DFIRMWARE_ID=\"uaBrain\" $(VAR_DEF_ENGINE_TYPE)

# software knock on ADC3: two Hellen knock modules, PA2 (knock 1) + PA1 (knock 2)
DDEFS += -DEFI_SOFTWARE_KNOCK=TRUE -DSTM32_ADC_USE_ADC3=TRUE

# two on-board MAX31855 EGT chips on SPI3
DDEFS += -DEFI_MAX_31855=TRUE

# Format stays the F7 default (compressed MSD).
DEFS += -DEFI_EMBED_INI_MSD=TRUE

# the mega-mcu100-f7 module does not expose the Hellen board-ID resistor pads
DDEFS += -DHW_HELLEN_SKIP_BOARD_TYPE=TRUE
# TODO: request a proper static board id in rusefi/hellen-one board_id registry
DDEFS += -DSTATIC_BOARD_ID=0

# ESP32 is attached to CAN2 on this board
DDEFS += -DEFI_SERIAL_CAN_BUS=2

DDEFS += -DSTM32_ICU_USE_TIM1=TRUE -DSTM32_PWM_USE_TIM1=FALSE
# HALL3 (MM100_IN_D3) is SENT-capable
DDEFS += -DEFI_SENT_SUPPORT=TRUE

# SD card and accelerometer share SPI1 on the mega module
ONBOARD_MEMS_TYPE=LIS2DH12

include $(PROJECT_DIR)/hw_layer/ports/stm32/2mb_flash.mk

# hellen-common100.mk pulls in hellen-common.mk: HW_HELLEN=1, inverted LEDs,
# critical-error LED on MM100_LED1_RED, hellen_common.cpp + hellen_board_id.cpp
include $(BOARDS_DIR)/hellen/hellen-common100.mk

# UART2 (PD5/PD6) is wired to the on-board ESP32 serial bridge
DDEFS += $(PRIMARY_COMMUNICATION_PORT_USART2)

DDEFS += -DUSB_DESCRIPTOR_STRING_CONTENT="'r', 0, 'u', 0, 's', 0, 'E', 0, 'F', 0, 'I', 0, ' ', 0, 'u', 0, 'a', 0, 'B', 0, 'r', 0, 'a', 0, 'i', 0, 'n', 0"
