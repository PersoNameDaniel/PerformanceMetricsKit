#include <Arduino.h>
#include "functions.h"

void setup() {
    Serial.begin(115200);
    pinMode(getUpshiftPin(), INPUT);
    attachInterrupt(digitalPinToInterrupt(getUpshiftPin()), incrementShift, RISING);
    pinMode(getDownshiftPin(), INPUT);
    attachInterrupt(digitalPinToInterrupt(getDownshiftPin()), decrementShift, RISING);
    pinMode(getCrankSensorPin(), INPUT);
    attachInterrupt(digitalPinToInterrupt(getCrankSensorPin()), recordCrankTime, CHANGE);

    pinMode(getShiftSolenoid12(), OUTPUT);
    pinMode(getShiftSolenoid34(), OUTPUT);
    pinMode(getVehicleComputerBypassRelayPin(), OUTPUT);

    digitalWrite(getVehicleComputerBypassRelayPin(), LOW);
}

void loop() {

    // fail safe logic
    if (gear < 1 || gear > maxSafeGear) {
        failSafe();
    }

    // get engine RPM to prevent overflow
    if ((millis() - getStartTime()) > 86400000) {
        getEngineRPM();
    }

    // lockout fourth gear if throttle is above 300
    if (analogRead(getThrottlePositionSensorPin()) > 300) {
        lockoutFourthGear();
    } else {
        unlockFourthGear();
    }

    // shift logic
    if (getShiftsRequested() > 0) {
        upShift();
    } else if (getShiftsRequested() < 0) {
        downShift();
    }
}