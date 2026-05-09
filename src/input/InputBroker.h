#pragma once

#include "Observer.h"
#include "freertosinc.h"


enum input_broker_event {
    INPUT_BROKER_NONE = 0,
    INPUT_BROKER_SELECT = 10,
    INPUT_BROKER_SELECT_LONG = 11,
    INPUT_BROKER_UP_LONG = 12,
    INPUT_BROKER_DOWN_LONG = 13,
    INPUT_BROKER_UP = 17,
    INPUT_BROKER_DOWN = 18,
    INPUT_BROKER_LEFT = 19,
    INPUT_BROKER_RIGHT = 20,
    INPUT_BROKER_CANCEL = 24,
    INPUT_BROKER_BACK = 27,
    INPUT_BROKER_USER_PRESS,
    INPUT_BROKER_ALT_PRESS,
    INPUT_BROKER_ALT_LONG,
    INPUT_BROKER_SHUTDOWN = 0x9b,
    INPUT_BROKER_GPS_TOGGLE = 0x9e,
    INPUT_BROKER_SEND_PING = 0xaf,
    INPUT_BROKER_MATRIXKEY = 0xFE,
    INPUT_BROKER_ANYKEY = 0xff,
    // Наша изоляция:
    // Просто добавляем константу. Число 0xB2 мы выбрали как безопасное.
    INPUT_BROKER_LAYOUT_CHANGE = 0xB2,
};

#define INPUT_BROKER_MSG_BRIGHTNESS_UP 0x11
#define INPUT_BROKER_MSG_BRIGHTNESS_DOWN 0x12
#define INPUT_BROKER_MSG_REBOOT 0x90
#define INPUT_BROKER_MSG_MUTE_TOGGLE 0xac
#define INPUT_BROKER_MSG_FN_SYMBOL_ON 0xf1
#define INPUT_BROKER_MSG_FN_SYMBOL_OFF 0xf2
#define INPUT_BROKER_MSG_BLUETOOTH_TOGGLE 0xAA
#define INPUT_BROKER_MSG_TAB 0x09
#define INPUT_BROKER_MSG_EMOTE_LIST 0x8F

/**
 * @brief Анализ структуры InputEvent
 * В файле InputBroker.h описано, как именно Meshtastic упаковывает каждое нажатие клавиши:
 *  - unsigned char kbchar: Вот наш главный "подозреваемый". Тип unsigned char в C++ — это строго 8 бит (один байт). 
 *    Это подтверждает, что за один раз система может передать только значение от 0 до 255.
 *  - input_broker_event inputEvent: Это тип события (нажатие, долгое нажатие и т.д.). Для обычных клавиш здесь используется значение
 *    INPUT_BROKER_ANYKEY = 0xff.  
 *  - const char *source: Имя устройства, которое прислало данные (например, "CardKB").  
 * 2. Как работает InputBroker
 * Этот модуль работает как диспетчер или "почтовое отделение":
 *  - Регистрация: Различные драйверы (как тот I2C-драйвер, что мы смотрели раньше) регистрируются через registerSource().
 *  - Обработка: Когда ты нажимаешь клавишу, вызывается handleInputEvent().
 *  - Пробуждение: Любой ввод вызывает powerFSM.trigger(EVENT_INPUT), что заставляет устройство выйти из режима сна.  
 *  - Рассылка: Самое важное — команда this->notifyObservers(event). InputBroker сам не решает, что делать с буквой "А" или "Ї". 
 *    Он просто кричит всем остальным модулям: "Эй, пришел байт такой-то, кто его ждет?". 
 * 
 */

typedef struct _InputEvent {
    const char *source;
    input_broker_event inputEvent;
    unsigned char kbchar;
    uint16_t touchX;
    uint16_t touchY;
} InputEvent;

class InputPollable
{
  public:
    virtual void pollOnce() = 0;
};

class InputBroker : public Observable<const InputEvent *>
{
    CallbackObserver<InputBroker, const InputEvent *> inputEventObserver =
        CallbackObserver<InputBroker, const InputEvent *>(this, &InputBroker::handleInputEvent);

  public:
    InputBroker();
    void registerSource(Observable<const InputEvent *> *source);
    void injectInputEvent(const InputEvent *event) { handleInputEvent(event); }
#if defined(HAS_FREE_RTOS) && !defined(ARCH_RP2040)
    void requestPollSoon(InputPollable *pollable);
    void queueInputEvent(const InputEvent *event);
    void processInputEventQueue();
#endif

  protected:
    int handleInputEvent(const InputEvent *event);

  private:
#if defined(HAS_FREE_RTOS) && !defined(ARCH_RP2040)
    QueueHandle_t inputEventQueue;
    QueueHandle_t pollSoonQueue;
    TaskHandle_t pollSoonTask;
    static void pollSoonWorker(void *p);
#endif
};

extern InputBroker *inputBroker;