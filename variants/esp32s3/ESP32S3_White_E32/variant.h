#define LED_PIN 48

//*************************************************   OLED   ****************************************************/
// Дисплей OLED
// #define HAS_SCREEN 1
#define USE_SSD1306
// #define USE_SH1106 1
#define I2C_SCL 38
#define I2C_SDA 39
//***********************************************   END OLED  ***************************************************/


//*************************************************   I2C2   ****************************************************/
// Другий I2C для підключення зовнішніх пристроїв
#define I2C_SDA1 15
#define I2C_SCL1 16
//*************************************************  END I2C2  ***************************************************/



//*************************************************   BUTTON   ****************************************************/
// Пін кнопки
#define BUTTON_PIN 0   
#define BUTTON_NEED_PULLUP

//*************************************************  END BUTTON  ***************************************************/

//***************************************************   UART   *****************************************************/
// UART
#define UART_TX 43
#define UART_RX 44

//UART (GPS)
//RX (для GPS TX)	36	Безопасный, не ADC2/Flash.
//TX (для GPS RX)	35	Безопасный, не ADC2/Flash.

//*************************************************   END UART   ****************************************************/




//Тип радіомодема
#define USE_RF95

#define LORA_DIO0 11 // IRQ
#define LORA_DIO1 10 // BUSY
#define LORA_RESET 12

/**конфигурация LoRa SPI (SCK 17, MISO 18) намного безопаснее! SPI-пины Heltec (9, 10, 11) часто
 * используются для внутренней Flash/PSRAM на не-Heltec платах и могут вызвать нестабильность.
 * Ваше решение об использовании GPIO 17/18 полностью устраняет этот риск. 
*/
#define LORA_SCK 17
#define LORA_MISO 18
#define LORA_MOSI 8
#define LORA_CS 9
#define LORA_TXEN 13
#define LORA_RXEN 14

//Піни для управління завданим радіомодемом
// #define USE_RF95_RFO
#define RF95_CS LORA_CS
#define RF95_IRQ LORA_DIO0
#define RF95_DIO1 LORA_DIO1
#define RF95_TXEN LORA_TXEN
#define RF95_RXEN LORA_RXEN
#define RF95_RESET LORA_RESET
#define RF95_MAX_POWER 17



// #define ADC_CTRL 37
// #define ADC_CTRL_ENABLED LOW
// #define BATTERY_PIN 1 // A battery voltage measurement pin, voltage divider connected here to measure battery voltage
// #define ADC_CHANNEL ADC1_GPIO1_CHANNEL
// #define ADC_ATTENUATION ADC_ATTEN_DB_2_5 // lower dB for high resistance voltage divider
// #define ADC_MULTIPLIER 4.9 * 1.045