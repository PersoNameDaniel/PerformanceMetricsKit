# 1 "C:\\Users\\Daniel\\Documents\\GitHub\\PerformanceMetricsKit\\shifting\\shifting.ino"
# 2 "C:\\Users\\Daniel\\Documents\\GitHub\\PerformanceMetricsKit\\shifting\\shifting.ino" 2
# 3 "C:\\Users\\Daniel\\Documents\\GitHub\\PerformanceMetricsKit\\shifting\\shifting.ino" 2

void setup() {
    Serial.begin(115200);
    pinMode(getUpshiftPin(), 0x0);
    attachInterrupt(((getUpshiftPin()) == 2 ? 0 : ((getUpshiftPin()) == 3 ? 1 : ((getUpshiftPin()) >= 18 && (getUpshiftPin()) <= 21 ? 23 - (getUpshiftPin()) : -1))), incrementShift, 3);
    pinMode(getDownshiftPin(), 0x0);
    attachInterrupt(((getDownshiftPin()) == 2 ? 0 : ((getDownshiftPin()) == 3 ? 1 : ((getDownshiftPin()) >= 18 && (getDownshiftPin()) <= 21 ? 23 - (getDownshiftPin()) : -1))), decrementShift, 3);
    pinMode(getCrankSensorPin(), 0x0);
    attachInterrupt(((getCrankSensorPin()) == 2 ? 0 : ((getCrankSensorPin()) == 3 ? 1 : ((getCrankSensorPin()) >= 18 && (getCrankSensorPin()) <= 21 ? 23 - (getCrankSensorPin()) : -1))), incrementCrankCount, 1);

    pinMode(getShiftSolenoid12(), 0x1);
    pinMode(getShiftSolenoid34(), 0x1);
}

void loop() {
    if ((millis() - getStartTime()) > 86400000) {
        getEngineRPM();
    }
    int TPS = analogRead(getThrottlePositionSensorPin());
    if (TPS > 300) {
    //if (analogRead(getThrottlePositionSensorPin()) > 300) {
        lockoutFourthGear();
    } else {
        unlockFourthGear();
    }
    //Serial.println(crankCount);
    //Serial.print(engineRPM);
    //Serial.println(" RPM");
    if (millis() % 1000 == 0) {
        Serial.print("TPS: ");
        Serial.println(TPS);
    }

    if (getShiftsRequested() > 0) {
        upShift();
    } else if (getShiftsRequested() < 0) {
        downShift();
    }
}
