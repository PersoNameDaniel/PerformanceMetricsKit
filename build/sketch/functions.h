#line 1 "C:\\Users\\Daniel\\Documents\\GitHub\\PerformanceMetricsKit\\shifting\\functions.h"
const int upShiftPin = 2;
const int downShiftPin = 3;
const int shiftSolenoid12 = 4;
const int shiftSolenoid34 = 5;
const int crankSensorPin = 18;

const int pulsesPerRevolution = 18;
const int maxGear = 4;
const int minimumRPM = 800;
const int maximumRPM = 6000;
const float safetyTolerance = 0.1;
const float firstGearRatio = 2.921;
const float secondGearRatio = 1.568;
const float thirdGearRatio = 1.000;
const float fourthGearRatio = 0.705;

int gear = 1;
unsigned long startTime = millis();
unsigned long crankCount = 0;
int shiftsRequested = 0;

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

int getUpshiftPin() {
    return upShiftPin;
}

int getDownshiftPin() {
    return downShiftPin;
}

int getShiftSolenoid12() {
    return shiftSolenoid12;
}

int getShiftSolenoid34() {
    return shiftSolenoid34;
}

int getCrankSensorPin() {
    return crankSensorPin;
}

void incrementCrankCount() {
    crankCount++;
}

void incrementShift() {
    shiftsRequested++;
}

void decrementShift() {
    shiftsRequested--;
}

unsigned long getStartTime() {
    return startTime;
}

int getShiftsRequested() {
    return shiftsRequested;
}

int getEngineRPM() {
    unsigned long currentTime = millis();
    float timeInSeconds = (currentTime - startTime) / 1000.0;
    
    float frequency = crankCount / timeInSeconds;
    float engineRPM = (frequency / pulsesPerRevolution) * 60;

    //Serial.println(crankCount);
    //Serial.println(timeInSeconds);

    crankCount = 0;
    startTime = currentTime;

    Serial.println(engineRPM);
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
        default: return -1.0;
    }
}

float getNewEngineRPM(int newGear) {
    float newEngineRPM = getEngineRPM() * (getGearRatio(newGear) / getGearRatio(gear));
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
    shiftsRequested -= 1;
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
    shiftsRequested += 1;
}

void shiftToFirst() {
    digitalWrite(shiftSolenoid12, LOW);
    digitalWrite(shiftSolenoid34, LOW);
    gear = 1;
    Serial.println("Shifted into first gear");
}

void shiftToSecond() {
    digitalWrite(shiftSolenoid12, HIGH);
    digitalWrite(shiftSolenoid34, LOW);
    gear = 2;
    Serial.println("Shifted into second gear");
}

void shiftToThird() {
    digitalWrite(shiftSolenoid12, HIGH);
    digitalWrite(shiftSolenoid34, HIGH);
    gear = 3;
    Serial.println("Shifted into third gear");
}

void shiftToFourth() {
    digitalWrite(shiftSolenoid12, LOW);
    digitalWrite(shiftSolenoid34, HIGH);
    gear = 4;
    Serial.println("Shifted into fourth gear");
}