// For OLED LCD
#define I2C_SDA 21
#define I2C_SCL 22

// GPS
// #undef GPS_RX_PIN
// #undef GPS_TX_PIN
// #define GPS_RX_PIN 12
// #define GPS_TX_PIN 15
// #define GPS_UBLOX

// #define BATTERY_PIN 35 // A battery voltage measurement pin, voltage divider connected here to measure battery voltage
// #define ADC_CHANNEL ADC1_GPIO35_CHANNEL
// #define ADC_MULTIPLIER 1.85 // (R1 = 470k, R2 = 680k)
//#define EXT_PWR_DETECT 4    // Pin to detect connected external power source for LILYGO® TTGO T-Energy T18 and other DIY boards
//#define EXT_NOTIFY_OUT 12   // Overridden default pin to use for Ext Notify Module (#975).


#define LED_PIN 27    // // add status LED (compatible with core-pcb and DIY targets)
#define BUTTON_PIN 0 //If defined, this will be used for user button presses
#define BUTTON_NEED_PULLUP


//#define LORA_DIO0 26  // a No connect on the SX1262/SX1268 module
#define LORA_RESET 14 // RST for SX1276, and for SX1262/SX1268
#define LORA_DIO1 25  // IRQ for SX1262/SX1268
#define LORA_DIO2 26  // BUSY for SX1262/SX1268
#define LORA_DIO3     // Not connected on PCB, but internally on the TTGO SX1262/SX1268, if DIO3 is high the TXCO is enabled

// In transmitting, set TXEN as high communication level，RXEN pin is low level;
// In receiving, set RXEN as high communication level, TXEN is lowlevel;
// Before powering off, set TXEN、RXEN as low level.
#define LORA_RXEN 13 // Input - RF switch RX control, connecting external MCU IO, valid in high level
#define LORA_TXEN 12 // Input - RF switch TX control, connecting external MCU IO or DIO2, valid in high level

#undef LORA_SCK
#define LORA_SCK 18
#undef LORA_MISO
#define LORA_MISO 19
#undef LORA_MOSI
#define LORA_MOSI 23
#undef LORA_CS
#define LORA_CS 17

// RX/TX for RFM95/SX127x
// #define RF95_RXEN LORA_RXEN
// #define RF95_TXEN LORA_TXEN
// #define RF95_TCXO <GPIO#>

// supported modules list
//#define USE_RF95 // RFM95/SX127x
#define USE_SX1262
//#define USE_SX1268
//#define USE_LLCC68

// common pinouts for SX126X modules
#define SX126X_CS LORA_CS
#define SX126X_DIO1 LORA_DIO1
#define SX126X_BUSY LORA_DIO2
#define SX126X_RESET LORA_RESET
#define SX126X_RXEN LORA_RXEN
#define SX126X_TXEN LORA_TXEN

// Set lora.tx_power to 13 for Hydra or other E22 900M30S target due to PA
#define SX126X_MAX_POWER 22

#ifdef EBYTE_E22
// Internally the TTGO module hooks the SX126x-DIO2 in to control the TX/RX switch
// (which is the default for the sx1262interface code)
#define SX126X_DIO3_TCXO_VOLTAGE 1.8
#endif
