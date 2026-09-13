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




// Прошивка одна и та же для двух совместимых по ногам модулей — E32 (RF95/
// SX1278) и E22 (чипы SX1262/SX1268/LLCC68). При старте прошивка сама
// определяет, что физически распаяно (см. src/mesh/RadioInterface.cpp и
// раздел 1 в PROMPT_PROJECT_MESH.md).

#define LORA_DIO0 11 // IRQ у SX127x (E32) / BUSY у SX126x (E22)
#define LORA_DIO1 10 // DIO1 — общий и у SX127x, и у SX126x
#define LORA_RESET 12
#define LORA_RXEN 14  // общий RXEN — остаётся на MCU в обеих схемах ниже

/**конфигурация LoRa SPI (SCK 17, MISO 18) намного безопаснее! SPI-пины Heltec (9, 10, 11) часто
 * используются для внутренней Flash/PSRAM на не-Heltec платах и могут вызвать нестабильность.
 * Ваше решение об использовании GPIO 17/18 полностью устраняет этот риск.
*/
#define LORA_SCK 17
#define LORA_MISO 18
#define LORA_MOSI 8
#define LORA_CS 9

// --- SX126x (E22-400M30S и совместимые по ногам SX1262/LLCC68) ---
#define SX126X_CS     LORA_CS
#define SX126X_RESET  LORA_RESET
#define SX126X_DIO1   LORA_DIO1
#define SX126X_BUSY   LORA_DIO0
#define SX126X_RXEN   LORA_RXEN
#define SX126X_MAX_POWER 22

#define SX126X_DIO3_TCXO_VOLTAGE 1.8
// TCXO_OPTIONAL обязателен вместе с SX126X_DIO3_TCXO_VOLTAGE — без него TCXO
// молча не включится (баг апстрима, см. PROMPT_PROJECT_MESH.md, раздел 1.2).
#define TCXO_OPTIONAL

// Схема управления РЧ-переключателем SX126x. По умолчанию — полное подключение,
// как разведено на плате сейчас: TXEN тоже на отдельном GPIO контроллера.
// На будущей ревизии платы, если решишь сэкономить один пин и развести
// управление через встроенный DIO2 чипа (как на nRF52840_Ebyte_modems) —
// раскомментируй строку ниже.
// ВНИМАНИЕ: модем E32/RF95 не умеет управлять переключателем через DIO2 (это
// возможность только чипов SX126x) — при включении этого режима поддержка
// E32/RF95 автоматически отключается (см. #else ниже).
// #define SX126X_DIO2_AS_RF_SWITCH

#ifdef SX126X_DIO2_AS_RF_SWITCH
    #define SX126X_TXEN RADIOLIB_NC // TX-плечо переключает сам чип через DIO2, GPIO не нужен
#else
    #define LORA_TXEN 13
    #define SX126X_TXEN LORA_TXEN

    //Тип радіомодема
    #define USE_RF95

    //Піни для управління завданим радіомодемом
    // #define USE_RF95_RFO
    #define RF95_CS LORA_CS
    #define RF95_IRQ LORA_DIO0
    #define RF95_DIO1 LORA_DIO1
    #define RF95_TXEN LORA_TXEN
    #define RF95_RXEN LORA_RXEN
    #define RF95_RESET LORA_RESET
    #define RF95_MAX_POWER 17
#endif

// Модули E22 могут поставляться на разных чипах при одинаковой распиновке —
// включаем все три сразу, чтобы автоопределение сработало для любого из них.
#define USE_SX1262
#define USE_SX1268
#define USE_LLCC68



// #define ADC_CTRL 37
// #define ADC_CTRL_ENABLED LOW
// #define BATTERY_PIN 1 // A battery voltage measurement pin, voltage divider connected here to measure battery voltage
// #define ADC_CHANNEL ADC1_GPIO1_CHANNEL
// #define ADC_ATTENUATION ADC_ATTEN_DB_2_5 // lower dB for high resistance voltage divider
// #define ADC_MULTIPLIER 4.9 * 1.045