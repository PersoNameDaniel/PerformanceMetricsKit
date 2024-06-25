#include <Arduino.h>
#include "functions.h"

void setup() {
    Serial.begin(115200);
    pinMode(getUpshiftPin(), INPUT);
    attachInterrupt(digitalPinToInterrupt(getUpshiftPin()), incrementShift, RISING);
    pinMode(getDownshiftPin(), INPUT);
    attachInterrupt(digitalPinToInterrupt(getDownshiftPin()), decrementShift, RISING);
    pinMode(getCrankSensorPin(), INPUT);
    attachInterrupt(digitalPinToInterrupt(getCrankSensorPin()), incrementCrankCount, CHANGE);

    pinMode(getShiftSolenoid12(), OUTPUT);
    pinMode(getShiftSolenoid34(), OUTPUT);
}

void loop() {
    if ((millis() - getStartTime()) > 86400000) {
        getEngineRPM();
    }
    if (analogRead(getThrottlePositionSensorPin()) > 300) {
        lockoutFourthGear();
    }
    //Serial.println(crankCount);
    //Serial.print(engineRPM);
    //Serial.println(" RPM");

    if (getShiftsRequested() > 0) {
        upShift();
    } else if (getShiftsRequested() < 0) {
        downShift();
    }
}