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

    // Вставить нажатую клавишу в text по позиции cursor: если для неё есть
    // кириллическая замена (см. translateKey) — вставляется она (2-3 байта
    // UTF-8), иначе — исходный ASCII-символ (1 байт). cursor сдвигается на
    // длину фактически вставленных байт. Вынесено сюда, чтобы не дублировать
    // один и тот же кусок кода в нескольких местах CannedMessageModule.cpp.
    // Тип параметра — unsigned int&, т.к. именно так объявлено поле
    // CannedMessageModule::cursor (см. CannedMessageModule.h) — ссылка должна
    // совпадать по типу, иначе компилятор не может её связать с полем.
    static void insertKey(String& text, unsigned int& cursor, uint8_t kbchar);
};

#endif