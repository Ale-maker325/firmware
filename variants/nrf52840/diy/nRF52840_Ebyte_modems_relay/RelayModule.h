#pragma once
#include "SinglePortModule.h"
#include "variant.h"

#ifndef RELAY_CHANNEL_PREFIX
#define RELAY_CHANNEL_PREFIX "relay"
#endif

// Початковий стан виходів реле («вимкнено»); для PCA9685 — з earlyInitVariant(), до сканування I2C
void relayHardwareInit();

// Керування реле текстовими командами (R1 ON, R1 OFF, ALL OFF, S1 90, STATUS) в особистих повідомленнях
// або в каналі керування
class RelayModule : public SinglePortModule
{
  public:
    RelayModule();

  protected:
    ProcessMessage handleReceived(const meshtastic_MeshPacket &mp) override;

  private:
    bool state[RELAY_COUNT] = {};
#ifdef RELAY_PCA9685
    int16_t servoAngle[RELAY_COUNT]; // -1 — вихід працює як реле
#endif

    void setRelay(uint8_t idx, bool on);
#ifdef RELAY_PCA9685
    void setServo(uint8_t idx, uint8_t angle);
#endif
    bool isAllowed(NodeNum from) const;
    bool isRelayChannel(uint8_t chIndex) const;
    void sendStatus(const meshtastic_MeshPacket &req);
};
