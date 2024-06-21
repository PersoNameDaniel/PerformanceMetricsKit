# 1 "C:\\Users\\Daniel\\Documents\\GitHub\\PerformanceMetricsKit\\shifting\\shifting.ino"
const int upShiftPin = 2;
const int downShiftPin = 3;
const int shiftSolenoid12 = 4;
const int shiftSolenoid34 = 5;
const int crankSensorPin = 18;

const int maxGear = 4;
const int minimumRPM = 800;
const int maximumRPM = 6000;
//const int finalDriveRatio = 2.93;
const float firstGearRatio = 2.921;
const float secondGearRatio = 1.568;
const float thirdGearRatio = 1.000;
const float fourthGearRatio = 0.705;

int gear = 1;
int shift = 0;
int crankCount = 0;
float engineRPM = -1.0;
unsigned long startTime = millis();

void setup() {
    Serial.begin(115200);
    pinMode(upShiftPin, 0x0);
    attachInterrupt(((upShiftPin) == 2 ? 0 : ((upShiftPin) == 3 ? 1 : ((upShiftPin) >= 18 && (upShiftPin) <= 21 ? 23 - (upShiftPin) : -1))), incrementShift, 3);
    pinMode(downShiftPin, 0x0);
    attachInterrupt(((downShiftPin) == 2 ? 0 : ((downShiftPin) == 3 ? 1 : ((downShiftPin) >= 18 && (downShiftPin) <= 21 ? 23 - (downShiftPin) : -1))), decrementShift, 3);
    pinMode(crankSensorPin, 0x0);
    attachInterrupt(((crankSensorPin) == 2 ? 0 : ((crankSensorPin) == 3 ? 1 : ((crankSensorPin) >= 18 && (crankSensorPin) <= 21 ? 23 - (crankSensorPin) : -1))), incrementCrankCount, 1);

    pinMode(shiftSolenoid12, 0x1);
    pinMode(shiftSolenoid34, 0x1);
}

void loop() {
    float engineRPM = getEngineRPM();
    //Serial.println(crankCount);
    Serial.print(engineRPM);
    Serial.println(" RPM");

    if (shift > 0) {
        upShift();
    } else if (shift < 0) {
        downShift();
    }

    delay(100);
}

void incrementCrankCount() {
    crankCount++;
}

void incrementShift() {
    shift++;
}

void decrementShift() {
    shift--;
}

int getEngineRPM() {
    unsigned long currentTime = millis();
    float timeInSeconds = (currentTime - startTime) / 1000.0; // convert milliseconds to seconds

    float frequency = crankCount / timeInSeconds; // calculate frequency in Hz
    engineRPM = (frequency / 18) * 60; // convert Hz to RPM

    //Serial.println(crankCount);
    //Serial.println(timeInSeconds);

    crankCount = 0;
    startTime = currentTime;

    return engineRPM;
}

double getGearRatio(int gear) {
    switch (gear) {
        case 1:
            return firstGearRatio;
        break;
        case 2:
            return secondGearRatio;
        break;
        case 3:
            return thirdGearRatio;
        break;
        case 4:
            return fourthGearRatio;
        break;
        default: return -1.0; // Default or error case
    }
}

float getNewEngineRPM(int newGear) {
    //float newEngineRPM = engineRPM * (getGearRatio(gear) / getGearRatio(newGear));
    float newEngineRPM = engineRPM * (getGearRatio(newGear) / getGearRatio(gear));
    Serial.println(newEngineRPM);
    return newEngineRPM;
}

void upShift() {
    if (gear < maxGear) {
        switch (gear){
            case 1:
                if (getNewEngineRPM(2) > minimumRPM) {
                    shiftToSecond();
                } else {
                    Serial.println("Engine speed low for upshift");
                }
            break;
            case 2:
                if (getNewEngineRPM(3) > minimumRPM) {
                    shiftToThird();
                } else {
                    Serial.println("Engine speed low for upshift");
                }
            break;
            case 3:
                if (getNewEngineRPM(4) > minimumRPM) {
                    shiftToFourth();
                } else {
                    Serial.println("Engine speed low for upshift");
                }
            break;
        }
    } else {
        Serial.print("up shift invalid: current gear is ");
        Serial.println(gear);
    }
    shift -= 1;
}

void downShift() {
    if (gear > 1) {
        switch (gear){
            case 2:
                if (getNewEngineRPM(1) < maximumRPM) {
                    shiftToFirst();
                } else {
                    Serial.println("Engine speed high for downshift");
                }
            break;
            case 3:
                if (getNewEngineRPM(2) < maximumRPM) {
                    shiftToSecond();
                } else {
                    Serial.println("Engine speed high for downshift");
                }
            break;
            case 4:
                if (getNewEngineRPM(3) < maximumRPM) {
                    shiftToThird();
                } else {
                    Serial.println("Engine speed high for downshift");
                }
            break;
        }
    } else {
        Serial.print("down shift invalid: current gear is ");
        Serial.println(gear);
    }
    shift += 1;
}

void shiftToFirst() {
    digitalWrite(shiftSolenoid12, 0x0);
    digitalWrite(shiftSolenoid34, 0x0);
    gear = 1;
    Serial.println("Shifted into first gear");
}

void shiftToSecond() {
    digitalWrite(shiftSolenoid12, 0x0);
    digitalWrite(shiftSolenoid34, 0x1);
    gear = 2;
    Serial.println("Shifted into second gear");
}

void shiftToThird() {
    digitalWrite(shiftSolenoid12, 0x0);
    digitalWrite(shiftSolenoid34, 0x0);
    gear = 3;
    Serial.println("Shifted into third gear");
}

void shiftToFourth() {
    digitalWrite(shiftSolenoid12, 0x1);
    digitalWrite(shiftSolenoid34, 0x1);
    gear = 4;
    Serial.println("Shifted into fourth gear");
}
