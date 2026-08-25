#ifndef Pins_Arduino_h
#define Pins_Arduino_h

#include <stdint.h>
#include <variant.h>

// Classic ESP32 (not S3)
// Default UART0 pins used by Serial / USB
static const uint8_t TX = 1;
static const uint8_t RX = 3;

// Default SPI mapped to LoRa module
static const uint8_t SS   = LORA_CS;
static const uint8_t SCK  = LORA_SCK;
static const uint8_t MOSI = LORA_MOSI;
static const uint8_t MISO = LORA_MISO;

// Default I2C (OLED)
static const uint8_t SDA = I2C_SDA;
static const uint8_t SCL = I2C_SCL;

#endif /* Pins_Arduino_h */