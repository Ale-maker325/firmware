#include "RelayModule.h"
#include "Channels.h"
#include "MeshService.h"
#include "NodeDB.h"
#include "configuration.h"
#include "main.h"

#include <ctype.h>
#include <stdlib.h>
#include <strings.h>

#ifdef RELAY_PCA9685
#include <Wire.h>

// Регістри PCA9685 (даташит NXP, розд. 7.3)
#define PCA_MODE1 0x00
#define PCA_LED0 0x06 // ON_L, ON_H, OFF_L, OFF_H — по 4 байти на вихід
#define PCA_ALL_LED 0xFA
#define PCA_PRESCALE 0xFE
#define PCA_FULL 0x10                      // біт «повністю ввімкнено / вимкнено» в ON_H / OFF_H
#define PCA_PRESCALE_50HZ 121              // 25 МГц / (4096 × 50 Гц) − 1
#define PCA_PERIOD_US 20000

static bool pcaWrite(uint8_t reg, const uint8_t *data, uint8_t len)
{
    Wire1.beginTransmission(RELAY_PCA9685);
    Wire1.write(reg);
    Wire1.write(data, len);
    return Wire1.endTransmission() == 0;
}

static bool pcaWriteByte(uint8_t reg, uint8_t value)
{
    return pcaWrite(reg, &value, 1);
}

static void pcaSetChannel(uint8_t ch, uint16_t on, uint16_t off)
{
    const uint8_t regs[4] = {(uint8_t)on, (uint8_t)(on >> 8), (uint8_t)off, (uint8_t)(off >> 8)};
    pcaWrite(PCA_LED0 + 4 * ch, regs, 4);
}

static void writeOutput(uint8_t idx, bool high)
{
    if (high)
        pcaSetChannel(idx, PCA_FULL << 8, 0);
    else
        pcaSetChannel(idx, 0, PCA_FULL << 8);
}

void relayHardwareInit()
{
    Wire1.begin();
    // Сон (потрібен для зміни частоти), ALLCALL вимкнено: інакше відгук на 0x70 сканер прийме за датчик SHTC3
    if (!pcaWriteByte(PCA_MODE1, 0x10)) {
        LOG_ERROR("PCA9685 not found at 0x%x", RELAY_PCA9685);
        return;
    }
    pcaWriteByte(PCA_PRESCALE, PCA_PRESCALE_50HZ);
    pcaWriteByte(PCA_ALL_LED + 0, 0);
    pcaWriteByte(PCA_ALL_LED + 1, 0);
    pcaWriteByte(PCA_ALL_LED + 2, 0);
    pcaWriteByte(PCA_ALL_LED + 3, PCA_FULL);
    pcaWriteByte(PCA_MODE1, 0x20); // автоінкремент адреси, вихід зі сну
    delay(1);
    for (uint8_t i = 0; i < RELAY_COUNT; i++)
        writeOutput(i, RELAY_ACTIVE_LOW != 0);
    LOG_INFO("PCA9685 at 0x%x: %u outputs", RELAY_PCA9685, RELAY_COUNT);
}

#else
static const uint8_t relayPins[RELAY_COUNT] = RELAY_PINS;

static void writeOutput(uint8_t idx, bool high)
{
    digitalWrite(relayPins[idx], high ? HIGH : LOW);
}

void relayHardwareInit()
{
    for (uint8_t i = 0; i < RELAY_COUNT; i++) {
        pinMode(relayPins[i], OUTPUT);
        writeOutput(i, RELAY_ACTIVE_LOW != 0);
    }
}
#endif

RelayModule::RelayModule() : SinglePortModule("relay", meshtastic_PortNum_TEXT_MESSAGE_APP)
{
#ifdef RELAY_PCA9685
    for (int16_t &a : servoAngle)
        a = -1;
#endif
}

void RelayModule::setRelay(uint8_t idx, bool on)
{
    state[idx] = on;
#ifdef RELAY_PCA9685
    servoAngle[idx] = -1;
#endif
    writeOutput(idx, on != (RELAY_ACTIVE_LOW != 0));
    LOG_INFO("Relay R%u -> %s", idx + 1, on ? "ON" : "OFF");
}

#ifdef RELAY_PCA9685
void RelayModule::setServo(uint8_t idx, uint8_t angle)
{
    state[idx] = false;
    servoAngle[idx] = angle;
    uint32_t us = SERVO_MIN_US + (uint32_t)(SERVO_MAX_US - SERVO_MIN_US) * angle / 180;
    pcaSetChannel(idx, 0, us * 4096 / PCA_PERIOD_US);
    LOG_INFO("Servo S%u -> %u deg (%u us)", idx + 1, angle, us);
}
#endif

bool RelayModule::isAllowed(NodeNum from) const
{
#ifdef RELAY_ALLOWED_NODES
    static const NodeNum allowed[] = RELAY_ALLOWED_NODES;
    for (NodeNum n : allowed)
        if (n == from)
            return true;
    return false;
#else
    return true;
#endif
}

// Відповідь: "ON: R1 R3" або "ALL OFF", далі положення сервоприводів, наприклад " S5=90"
void RelayModule::sendStatus(const meshtastic_MeshPacket &req)
{
    char text[200] = "";
    size_t len = 0;
    for (uint8_t i = 0; i < RELAY_COUNT; i++)
        if (state[i])
            len += snprintf(text + len, sizeof(text) - len, "%sR%u", len ? " " : "ON: ", i + 1);
    if (len == 0)
        len = snprintf(text, sizeof(text), "ALL OFF");
#ifdef RELAY_PCA9685
    for (uint8_t i = 0; i < RELAY_COUNT && len < sizeof(text); i++)
        if (servoAngle[i] >= 0)
            len += snprintf(text + len, sizeof(text) - len, " S%u=%d", i + 1, servoAngle[i]);
#endif
    len = strnlen(text, sizeof(text));

    meshtastic_MeshPacket *p = allocDataPacket();
    setReplyTo(p, req);
    p->decoded.request_id = 0; // звичайне текстове повідомлення, а не відповідь на запит
    if (isBroadcast(req.to)) {
        // Команда прийшла в канал керування — відповідаємо в той самий канал
        p->to = NODENUM_BROADCAST;
        p->want_ack = false;
    }
    p->decoded.payload.size = len;
    memcpy(p->decoded.payload.bytes, text, len);
    service->sendToMesh(p);
}

// Канал керування: назва починається з RELAY_CHANNEL_PREFIX (регістр не важливий) і має власний ключ, а не стандартний
bool RelayModule::isRelayChannel(uint8_t chIndex) const
{
    if (chIndex >= MAX_NUM_CHANNELS)
        return false;
    const meshtastic_Channel &ch = channels.getByIndex(chIndex);
    if (ch.role == meshtastic_Channel_Role_DISABLED || ch.settings.psk.size < 16)
        return false;
    return strncasecmp(ch.settings.name, RELAY_CHANNEL_PREFIX, strlen(RELAY_CHANNEL_PREFIX)) == 0;
}

// Розбиває текст на слова з латинських літер і числа; решта символів — роздільники: "r12on" → R 12 ON, "s3 90" → S 3 90
static uint8_t tokenize(const uint8_t *src, size_t len, char tok[][8], uint8_t maxTok)
{
    uint8_t n = 0, l = 0, prev = 0;
    for (size_t i = 0; i <= len; i++) {
        uint8_t c = i < len ? src[i] : 0;
        uint8_t cls = isalpha(c) ? 1 : isdigit(c) ? 2 : 0;
        if (cls != prev && l) {
            tok[n++][l] = '\0';
            l = 0;
            if (n == maxTok)
                break;
        }
        if (cls && l < 7)
            tok[n][l++] = toupper(c);
        prev = cls;
    }
    return n;
}

ProcessMessage RelayModule::handleReceived(const meshtastic_MeshPacket &mp)
{
    // Особисті повідомлення або загальні повідомлення в каналі керування; решта каналів ігнорується
    bool viaChannel = isBroadcast(mp.to) && isRelayChannel(mp.channel);
    if (!(isToUs(&mp) || viaChannel) || isFromUs(&mp) || !isAllowed(mp.from))
        return ProcessMessage::CONTINUE;

    // Регістр, пробіли та розділові знаки не мають значення: "r1 on", "R1ON", "Status." — усе одне
    char tok[4][8];
    uint8_t n = tokenize(mp.decoded.payload.bytes, mp.decoded.payload.size, tok, 4);

    if (n == 1 && strcmp(tok[0], "STATUS") == 0) {
        sendStatus(mp);
        return ProcessMessage::CONTINUE;
    }

    if ((n == 1 && strcmp(tok[0], "ALLOFF") == 0) || (n == 2 && strcmp(tok[0], "ALL") == 0 && strcmp(tok[1], "OFF") == 0)) {
        for (uint8_t i = 0; i < RELAY_COUNT; i++)
            setRelay(i, false);
        sendStatus(mp);
        return ProcessMessage::CONTINUE;
    }

    // R<номер> ON|OFF, S<номер> <кут 0–180>
    if (n != 3 || !isdigit((unsigned char)tok[1][0]))
        return ProcessMessage::CONTINUE;
    int idx = atoi(tok[1]) - 1;
    if (idx < 0 || idx >= RELAY_COUNT)
        return ProcessMessage::CONTINUE;

    if (strcmp(tok[0], "R") == 0 && strcmp(tok[2], "ON") == 0)
        setRelay(idx, true);
    else if (strcmp(tok[0], "R") == 0 && strcmp(tok[2], "OFF") == 0)
        setRelay(idx, false);
#ifdef RELAY_PCA9685
    else if (strcmp(tok[0], "S") == 0 && isdigit((unsigned char)tok[2][0]) && atoi(tok[2]) <= 180)
        setServo(idx, atoi(tok[2]));
#endif
    else
        return ProcessMessage::CONTINUE;

    sendStatus(mp);
    return ProcessMessage::CONTINUE;
}
