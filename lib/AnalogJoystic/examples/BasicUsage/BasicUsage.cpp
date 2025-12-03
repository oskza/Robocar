#include <Arduino.h>
#include <AnalogJoystic.h>

#define JOYSTIC_HORZ_PIN 35 //X
#define JOYSTIC_VERT_PIN 34 //Y
#define JOYSTIC_DEADZONE 80

AnalogJoystic joystic(JOYSTIC_VERT_PIN, JOYSTIC_HORZ_PIN, JOYSTIC_DEADZONE);

void setup() {
  Serial.begin(115200);
  while (!Serial);

  joystic.calibrateCenter();
}

void loop() {
  Serial.print("Vert: "); Serial.println(joystic.readVertical());
  Serial.print("Horz: "); Serial.println(joystic.readHorizontal());
}
