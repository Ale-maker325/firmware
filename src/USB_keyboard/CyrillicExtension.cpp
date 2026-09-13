#include "CyrillicExtension.h"

bool CyrillicExtension::isCyrillic = false;

// ---------------------------------------------------------------------------
// ASCII (32..126) → UTF-8 для украинской раскладки.
// ВСЕ не-ASCII строки заданы через \xXX, чтобы файл не зависел от кодировки
// редактора (иначе ґ/№/ї после копирования на Windows превращались в мусор
// и на экране появлялись «їґ», «ї» вместо № и т.п.).
//
// UTF-8 справка:
//   а-я/А-Я обычные — 2 байта D0..D1
//   є Є і І ї Ї ґ Ґ — 2 байта
//   № (U+2116)     — 3 байта E2 84 96
// ---------------------------------------------------------------------------
static const char *const ua_map[] = {
    // 32-39: space ! " # $ % & '
    " ",
    "!",
    "\xD0\x84",             // " → Є
    "\xE2\x84\x96",         // # → №
    ";",                    // $ → ;
    "%",
    "?",                    // & → ?
    "\xD1\x94",             // ' → є

    // 40-47: ( ) * + , - . /
    "(",
    ")",
    "*",
    "+",
    "\xD0\xB1",             // , → б
    "-",
    ".",                    // . литерал (NumPad / точка с «/»)
    "/",                    // / литерал (NumPad)

    // 48-55: 0-7
    "0", "1", "2", "3", "4", "5", "6", "7",

    // 56-63: 8 9 : ; < = > ?
    "8",
    "9",
    "\xD0\x96",             // : → Ж
    "\xD0\xB6",             // ; → ж
    "\xD0\x91",             // < → Б
    "=",
    "\xD0\xAE",             // > → Ю
    ",",                    // ? → запятая (Shift+/ с RP2350)

    // 64-71: @ A B C D E F G
    "\"",                   // @ → "
    "\xD0\xA4",             // A → Ф
    "\xD0\x98",             // B → И
    "\xD0\xA1",             // C → С
    "\xD0\x92",             // D → В
    "\xD0\xA3",             // E → У
    "\xD0\x90",             // F → А
    "\xD0\x9F",             // G → П

    // 72-79: H I J K L M N O
    "\xD0\xA0",             // H → Р
    "\xD0\xA8",             // I → Ш
    "\xD0\x9E",             // J → О
    "\xD0\x9B",             // K → Л
    "\xD0\x94",             // L → Д
    "\xD0\xAC",             // M → Ь
    "\xD0\xA2",             // N → Т
    "\xD0\xA9",             // O → Щ

    // 80-87: P Q R S T U V W
    "\xD0\x97",             // P → З
    "\xD0\x99",             // Q → Й
    "\xD0\x9A",             // R → К
    "\xD0\x86",             // S → І
    "\xD0\x95",             // T → Е
    "\xD0\x93",             // U → Г
    "\xD0\x9C",             // V → М
    "\xD0\xA6",             // W → Ц

    // 88-95: X Y Z [ \ ] ^ _
    "\xD0\xA7",             // X → Ч
    "\xD0\x9D",             // Y → Н
    "\xD0\xAF",             // Z → Я
    "\xD1\x85",             // [ → х
    "\xD2\x91",             // \ → ґ   (один символ U+0491)
    "\xD1\x97",             // ] → ї
    ":",                    // ^ → :
    "_",

    // 96-103: ` a b c d e f g
    "\xD1\x8E",             // ` → ю   (клавиша «.» с RP2350 в UA)
    "\xD1\x84",             // a → ф
    "\xD0\xB8",             // b → и
    "\xD1\x81",             // c → с
    "\xD0\xB2",             // d → в
    "\xD1\x83",             // e → у
    "\xD0\xB0",             // f → а
    "\xD0\xBF",             // g → п

    // 104-111: h i j k l m n o
    "\xD1\x80",             // h → р
    "\xD1\x88",             // i → ш
    "\xD0\xBE",             // j → о
    "\xD0\xBB",             // k → л
    "\xD0\xB4",             // l → д
    "\xD1\x8C",             // m → ь
    "\xD1\x82",             // n → т
    "\xD1\x89",             // o → щ

    // 112-119: p q r s t u v w
    "\xD0\xB7",             // p → з
    "\xD0\xB9",             // q → й
    "\xD0\xBA",             // r → к
    "\xD1\x96",             // s → і
    "\xD0\xB5",             // t → е
    "\xD0\xB3",             // u → г
    "\xD0\xBC",             // v → м
    "\xD1\x86",             // w → ц

    // 120-126: x y z { | } ~
    "\xD1\x87",             // x → ч
    "\xD0\xBD",             // y → н
    "\xD1\x8F",             // z → я
    "\xD0\xA5",             // { → Х
    "\xD2\x90",             // | → Ґ   (один символ U+0490)
    "\xD0\x87",             // } → Ї
    "'"                     // ~ → апостроф (клавиша HID 0x35 в UA)
};

void CyrillicExtension::toggleLayout()
{
    isCyrillic = !isCyrillic;
}

const char *CyrillicExtension::getLayoutName()
{
    return isCyrillic ? "UA" : "EN";
}

const char *CyrillicExtension::translateKey(char key)
{
    if (!isCyrillic)
        return nullptr;

    if (key < 32 || key > 126)
        return nullptr;

    int index = (int)(unsigned char)key - 32;
    if (index < 0 || index >= (int)(sizeof(ua_map) / sizeof(ua_map[0])))
        return nullptr;

    return ua_map[index];
}

int CyrillicExtension::getPrevUtf8Index(const String &s, int idx)
{
    if (idx <= 0)
        return 0;
    const char *buf = s.c_str();
    int i = idx - 1;
    while (i > 0 && (((uint8_t)buf[i] & 0xC0) == 0x80)) {
        i--;
    }
    return i;
}
