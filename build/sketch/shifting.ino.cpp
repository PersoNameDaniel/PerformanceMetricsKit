#line 1 "C:\\Users\\Daniel\\Documents\\GitHub\\PerformanceMetricsKit\\shifting\\shifting.ino"
#include <Arduino.h>
#include "functions.h"

/*
int getUpshiftPin();
int getDownshiftPin();
int getShiftSolenoid12();
int getShiftSolenoid34();
int getCrankSensorPin();
void incrementCrankCount();
void incrementShift();
void decrementShift();
unsigned long getStartTime();
int getShiftsRequested();
int getEngineRPM();
double getGearRatio(int gear);
float getNewEngineRPM(int newGear);
void upShift();
void downShift();
void shiftToFirst();
void shiftToSecond();
void shiftToThird();
void shiftToFourth();
*/

#line 26 "C:\\Users\\Daniel\\Documents\\GitHub\\PerformanceMetricsKit\\shifting\\shifting.ino"
void setup();
#line 39 "C:\\Users\\Daniel\\Documents\\GitHub\\PerformanceMetricsKit\\shifting\\shifting.ino"
void loop();
#line 26 "C:\\Users\\Daniel\\Documents\\GitHub\\PerformanceMetricsKit\\shifting\\shifting.ino"
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
