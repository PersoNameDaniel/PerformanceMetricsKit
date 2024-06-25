#line 1 "C:\\Users\\Daniel\\Documents\\GitHub\\PerformanceMetricsKit\\shifting\\shifting.ino"
#include <Arduino.h>
#include "functions.h"

#line 4 "C:\\Users\\Daniel\\Documents\\GitHub\\PerformanceMetricsKit\\shifting\\shifting.ino"
void setup();
#line 17 "C:\\Users\\Daniel\\Documents\\GitHub\\PerformanceMetricsKit\\shifting\\shifting.ino"
void loop();
#line 4 "C:\\Users\\Daniel\\Documents\\GitHub\\PerformanceMetricsKit\\shifting\\shifting.ino"
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
    //int TPS = analogRead(getThrottlePositionSensorPin());
    //if (TPS > 300) {
    if (analogRead(getThrottlePositionSensorPin()) > 300) {
        lockoutFourthGear();
    } else {
        unlockFourthGear();
    }
    //Serial.println(crankCount);
    //Serial.print(engineRPM);
    //Serial.println(" RPM");
    //if (millis() % 1000 == 0) {
    //    Serial.print("TPS: ");
    //    Serial.println(TPS);
    //}

    if (getShiftsRequested() > 0) {
        upShift();
    } else if (getShiftsRequested() < 0) {
        downShift();
    }
}
