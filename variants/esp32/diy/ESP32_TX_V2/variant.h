// ============================================================
// ESP32_TX_V2
// Поддержка: E22-400M30S (SX1268) — активен по умолчанию
//            E32-400M30S (SX127x) — можно включить позже
// ============================================================

// ----- Дисплей OLED (I2C) -----
#define I2C_SDA 21
#define I2C_SCL 22
// #define USE_SSD1306          // раскомментируй, если нужно явно

// ----- Кнопка -----
#define BUTTON_PIN 0
// #define BUTTON_NEED_PULLUP   // обычно на GPIO0 уже есть подтяжка

// ----- LED -----
//#define LED_PIN 27
//#define LED_POWER 27           // для совместимости с разными частями прошивки

// ----- MOSFET (GPIO16) -----
// Можно использовать позже через External Notification или Module Config
// #define EXT_NOTIFY_OUT 16

// ----- GPS (пока отключён) -----
#define HAS_GPS 0
// #define GPS_RX_PIN 15
// #define GPS_TX_PIN 12

// ----- UART -----
// Стандартные пины ESP32:
// UART0 (Serial / USB): TX = 1, RX = 3
// UART2 (свободный):    TX = 17, RX = 16  ← заняты NSS и MOSFET
// Переназначать не нужно, если GPS нет.

// ============================================================
// Радио — общие пины
// ============================================================
#define LORA_SCK  18
#define LORA_MISO 19
#define LORA_MOSI 23
#define LORA_CS   17          // NSS

#define LORA_RESET 14         // RST
#define LORA_DIO1  25         // IRQ (DIO1)
#define LORA_DIO0  26         // BUSY у E22 / DIO0 у E32

#define LORA_RXEN  13         // RXEN
#define LORA_TXEN  12         // TXEN

// ============================================================
// SX126x (E22-400M30S) — АКТИВЕН
// ============================================================
#define USE_SX1268

#define SX126X_CS     LORA_CS
#define SX126X_SCK    LORA_SCK
#define SX126X_MISO   LORA_MISO
#define SX126X_MOSI   LORA_MOSI
#define SX126X_RESET  LORA_RESET
#define SX126X_DIO1   LORA_DIO1
#define SX126X_BUSY   LORA_DIO0     // у E22 BUSY = DIO0
#define SX126X_RXEN   LORA_RXEN
#define SX126X_TXEN   LORA_TXEN

#define SX126X_MAX_POWER 22
#define SX126X_DIO3_TCXO_VOLTAGE 1.8
#define TCXO_OPTIONAL

// ============================================================
// RF95 / SX127x (E32-400M30S) — пока выключен
// Чтобы включить — раскомментируй USE_RF95 и закомментируй USE_SX1268
// ============================================================
// #define USE_RF95

#ifdef USE_RF95
#define RF95_CS     LORA_CS
#define RF95_IRQ    LORA_DIO0     // у E32 DIO0 = IRQ
#define RF95_RESET  LORA_RESET
#define RF95_TXEN   LORA_TXEN
#define RF95_RXEN   LORA_RXEN
#define RF95_MAX_POWER 20
#endif
