#include <Arduino.h>
#include <Encoder.h>

#define ENCODER_R_PIN 32
#define ENCODER_L_PIN 33

#define ENCODER_SLOTS 20

Encoder encoderR(ENCODER_R_PIN);
Encoder encoderL(ENCODER_L_PIN);

void IRAM_ATTR onRightEncoder() { encoderR.tick(); }
void IRAM_ATTR onLeftEncoder()  { encoderL.tick(); }

void setup() {
  encoderR.init(onRightEncoder);
  encoderL.init(onLeftEncoder);
}

void loop() {
  if (encoderR.getCount() >= ENCODER_SLOTS) {
    encoderR.reset();
    //...
  }

  if (encoderL.getCount() >= ENCODER_SLOTS) {
    encoderL.reset();
    //...
  }
}
