// #include "CyrillicExtension.h"

// bool CyrillicExtension::isCyrillic = false;

// // Таблица маппинга: ASCII -> UTF-8 (Кириллица)
// // Индексы соответствуют ASCII кодам от ' ' (32) до '~' (126)
// static const char* ua_map[] = {
//     // Индексы 0-7 (ASCII 32-39: Пробел, !, ", #, $, %, &, ')
//     " ",   "!",   "Є",   "№",   ";",   "%",   "?",   "є",   
//     // Индексы 8-15 (ASCII 40-47: (, ), *, +, ,, -, ., /)
//     "(",   ")",   "*",   "+",   "б",   "-",   "ю",   "/",   
//     // Индексы 16-23 (ASCII 48-55: Цифры 0-7)
//     "0",   "1",   "2",   "3",   "4",   "5",   "6",   "7",   
//     // Индексы 24-31 (ASCII 56-63: 8, 9, :, ;, <, =, >, ?)
//     "8",   "9",   "Ж",   "ж",   "Б",   "=",   "Ю",   ",",   
//     // Индексы 32-39 (ASCII 64-71: @, A, B, C, D, E, F, G)
//     "\"",  "Ф",   "И",   "С",   "В",   "У",   "А",   "П",   
//     // Индексы 40-47 (ASCII 72-79: H, I, J, K, L, M, N, O)
//     "Р",   "Ш",   "О",   "Л",   "Д",   "Ь",   "Т",   "Щ",   
//     // Индексы 48-55 (ASCII 80-87: P, Q, R, S, T, U, V, W)
//     "З",   "Й",   "К",   "І",   "Е",   "Г",   "М",   "Ц",   
//     // Индексы 56-63 (ASCII 88-95: X, Y, Z, [, \, ], ^, _)
//     "Ч",   "Н",   "Я",   "х",   "ґ",   "ї",   ":",   "_",   
//     // Индексы 64-71 (ASCII 96-103: `, a, b, c, d, e, f, g)
//     "ґ",   "ф",   "и",   "с",   "в",   "у",   "а",   "п",   
//     // Индексы 72-79 (ASCII 104-111: h, i, j, k, l, m, n, o)
//     "р",   "ш",   "о",   "л",   "д",   "ь",   "т",   "щ",   
//     // Индексы 80-87 (ASCII 112-119: p, q, r, s, t, u, v, w)
//     "з",   "й",   "к",   "і",   "е",   "г",   "м",   "ц",   
//     // Индексы 88-94 (ASCII 120-126: x, y, z, {, |, }, ~)
//     "ч",   "н",   "я",   "Х",   "Ґ",   "Ї",   "Є"           
// };

// void CyrillicExtension::toggleLayout() {
//     isCyrillic = !isCyrillic;
// }

// const char* CyrillicExtension::getLayoutName() {
//     return isCyrillic ? "UA" : "EN";
// }

// const char* CyrillicExtension::translateKey(char key) {
//     if (!isCyrillic) return nullptr;
    
//     // Если это печатаемый символ из нашего диапазона
//     if (key < 32 || key > 126) return nullptr;
//     // Получаем индекс: пробел(32) станет 0, '!' станет 1 и так далее
//     int index = (int)key - 32;
//     return ua_map[index];
// }

// int CyrillicExtension::getPrevUtf8Index(const String& s, int idx) {
//     if (idx <= 0) return 0;
//     const char* buf = s.c_str();
//     int i = idx - 1;
//     // В UTF-8 байты продолжения символа всегда начинаются с 10xxxxxx (0x80)
//     while (i > 0 && (((uint8_t)buf[i] & 0xC0) == 0x80)) {
//         i--;
//     }
//     return i;
// }





#include "CyrillicExtension.h"

bool CyrillicExtension::isCyrillic = false;

// Таблица маппинга: ASCII -> UTF-8 (Кириллица)
// Индексы соответствуют ASCII кодам от ' ' (32) до '~' (126)
static const char* ua_map[] = {
    // Индексы 0-7 (ASCII 32-39: Пробел, !, ", #, $, %, &, ')
    " ",   "!",   "Є",   "№",   ";",   "%",   "?",   "є",   
    // Индексы 8-15 (ASCII 40-47: (, ), *, +, ,, -, ., /)
    // '.' и '/' — литералы (NumPad + пунктуация с RP2350).
    // «ю» больше НЕ на '.': иначе NumPad '.' печатал «ю».
    "(",   ")",   "*",   "+",   "б",   "-",   ".",   "/",   
    // Индексы 16-23 (ASCII 48-55: Цифры 0-7)
    "0",   "1",   "2",   "3",   "4",   "5",   "6",   "7",   
    // Индексы 24-31 (ASCII 56-63: 8, 9, :, ;, <, =, >, ?)
    // '>' → «Ю» (заглавная); строчная «ю» — с '`' (см. ниже)
    "8",   "9",   "Ж",   "ж",   "Б",   "=",   "Ю",   ",",   
    // Индексы 32-39 (ASCII 64-71: @, A, B, C, D, E, F, G)
    "\"",  "Ф",   "И",   "С",   "В",   "У",   "А",   "П",   
    // Индексы 40-47 (ASCII 72-79: H, I, J, K, L, M, N, O)
    "Р",   "Ш",   "О",   "Л",   "Д",   "Ь",   "Т",   "Щ",   
    // Индексы 48-55 (ASCII 80-87: P, Q, R, S, T, U, V, W)
    "З",   "Й",   "К",   "І",   "Е",   "Г",   "М",   "Ц",   
    // Индексы 56-63 (ASCII 88-95: X, Y, Z, [, \, ], ^, _)
    "Ч",   "Н",   "Я",   "х",   "ґ",   "ї",   ":",   "_",   
    // Индексы 64-71 (ASCII 96-103: `, a, b, c, d, e, f, g)
    // '`' → «ю» (RP2350 шлёт '`' с физической клавиши '.' в раскладке UA)
    "ю",   "ф",   "и",   "с",   "в",   "у",   "а",   "п",   
    // Индексы 72-79 (ASCII 104-111: h, i, j, k, l, m, n, o)
    "р",   "ш",   "о",   "л",   "д",   "ь",   "т",   "щ",   
    // Индексы 80-87 (ASCII 112-119: p, q, r, s, t, u, v, w)
    "з",   "й",   "к",   "і",   "е",   "г",   "м",   "ц",   
    // Индексы 88-94 (ASCII 120-126: x, y, z, {, |, }, ~)
    "ч",   "н",   "я",   "Х",   "Ґ",   "Ї",   "Є"           
};

void CyrillicExtension::toggleLayout() {
    isCyrillic = !isCyrillic;
}

const char* CyrillicExtension::getLayoutName() {
    return isCyrillic ? "UA" : "EN";
}

const char* CyrillicExtension::translateKey(char key) {
    if (!isCyrillic) return nullptr;
    
    // Если это печатаемый символ из нашего диапазона
    if (key < 32 || key > 126) return nullptr;
    // Получаем индекс: пробел(32) станет 0, '!' станет 1 и так далее
    int index = (int)key - 32;
    return ua_map[index];
}

int CyrillicExtension::getPrevUtf8Index(const String& s, int idx) {
    if (idx <= 0) return 0;
    const char* buf = s.c_str();
    int i = idx - 1;
    // В UTF-8 байты продолжения символа всегда начинаются с 10xxxxxx (0x80)
    while (i > 0 && (((uint8_t)buf[i] & 0xC0) == 0x80)) {
        i--;
    }
    return i;
}