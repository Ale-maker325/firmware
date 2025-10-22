// pins_arduino.h

// Need this file for ESP32-S3
// No need to modify this file, changes to pins imported from variant.h
// Most is similar to https://github.com/espressif/arduino-esp32/blob/master/variants/esp32s3/pins_arduino.h

#ifndef Pins_Arduino_h
#define Pins_Arduino_h

#include <stdint.h>
#include <variant.h> // Этот файл содержит все ваши #define константы

#define USB_VID 0x303a
#define USB_PID 0x1001

// ----------------------
// UART0 (Консоль/USB)
// ----------------------
static const uint8_t TX = UART_TX; // TX = 43
static const uint8_t RX = UART_RX; // RX = 44

// ----------------------
// SPI (LoRa)
// ----------------------
// Стандартные константы Arduino SPI, перенаправлены на ваши пины LoRa
static const uint8_t SS = LORA_CS;   // SS = 9
static const uint8_t SCK = LORA_SCK; // SCK = 17
static const uint8_t MOSI = LORA_MOSI; // MOSI = 8
static const uint8_t MISO = LORA_MISO; // MISO = 18

// ----------------------
// I2C0 (OLED)
// ----------------------
// Стандартные константы Arduino Wire (I2C)
static const uint8_t SCL = I2C_SCL; // SCL = 38
static const uint8_t SDA = I2C_SDA; // SDA = 39

// ----------------------
// Дополнительные дефайны (LED, BUTTON, GPS и т.д.)
// ----------------------
// Они уже определены в variant.h и доступны напрямую.
// В pins_arduino.h их можно не дублировать.

#endif /* Pins_Arduino_h */
