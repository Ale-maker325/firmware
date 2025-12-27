// For OLED LCD
#define I2C_SDA 21
#define I2C_SCL 22

// GPS
// #undef GPS_RX_PIN
// #undef GPS_TX_PIN
// #define GPS_RX_PIN 12
// #define GPS_TX_PIN 15
// #define GPS_UBLOX

#define BUTTON_PIN 0
//#define BATTERY_PIN 35 // A battery voltage measurement pin, voltage divider connected here to measure battery voltage
//#define ADC_CHANNEL ADC1_GPIO35_CHANNEL
//#define ADC_MULTIPLIER 1.85 // (R1 = 470k, R2 = 680k)
//#define EXT_PWR_DETECT 4    // Pin to detect connected external power source for LILYGO® TTGO T-Energy T18 and other DIY boards
//#define EXT_NOTIFY_OUT 12   // Overridden default pin to use for Ext Notify Module (#975).
#define LED_PIN 27           // add status LED (compatible with core-pcb and DIY targets)


// supported modules list
#define USE_RF95 // RFM95/SX127x

#define LORA_DIO0 26  // a No connect on the SX1262/SX1268 module
#define LORA_RESET 14 // RST for SX1276, and for SX1262/SX1268
#define LORA_DIO1 25  // IRQ for SX1262/SX1268

#define LORA_SCK 18
#define LORA_MISO 19
#define LORA_MOSI 23
#define LORA_CS 17

#define LORA_RXEN 13
#define LORA_TXEN 12


//Піни для управління завданим радіомодемом
#define RF95_CS LORA_CS
#define RF95_IRQ LORA_DIO0
#define RF95_DIO1 LORA_DIO1
#define RF95_TXEN LORA_TXEN
#define RF95_RXEN LORA_RXEN
#define RF95_RESET LORA_RESET
#define RF95_MAX_POWER 20