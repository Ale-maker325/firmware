#ifndef CYRILLIC_EXTENSION_H
#define CYRILLIC_EXTENSION_H

#include <Arduino.h>

// Используем фиксированное значение для события, чтобы избежать конфликтов с будущими обновлениями.
// Значения выше 0xB0 редко используются разработчиками Meshtastic.
#define INPUT_BROKER_LAYOUT_CHANGE_CODE 0xB2 

class CyrillicExtension {
public:
    static bool isCyrillic;
    
    // Переключить раскладку
    static void toggleLayout();
    
    // Получить имя текущей раскладки для экрана
    static const char* getLayoutName();
    
    // Обработать нажатие клавиши: вернуть UTF-8 строку или NULL, если трансляция не нужна
    static const char* translateKey(char key);
    
    // Вспомогательные функции для корректной работы Backspace с UTF-8
    static int getPrevUtf8Index(const String& s, int idx);
};

#endif