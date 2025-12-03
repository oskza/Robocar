#include <Arduino.h>
#include <CompassBMM150.h>

CompassBMM150 compass;

void setup() {
  if (!compass.init()) {/*...*/}
}

void loop() {
  float degree = compass.getCompassDegree();
  //...
}
