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
int shiftsRequested = 0;
unsigned long crankCount = 0;
float engineRPM = -1.0;
unsigned long startTime = millis();
unsigned long currentTime = 0;

void setup() {
    Serial.begin(115200);
    pinMode(upShiftPin, INPUT);
    attachInterrupt(digitalPinToInterrupt(upShiftPin), incrementShift, RISING);
    pinMode(downShiftPin, INPUT);
    attachInterrupt(digitalPinToInterrupt(downShiftPin), decrementShift, RISING);
    pinMode(crankSensorPin, INPUT);
    attachInterrupt(digitalPinToInterrupt(crankSensorPin), incrementCrankCount, CHANGE);

    pinMode(shiftSolenoid12, OUTPUT);
    pinMode(shiftSolenoid34, OUTPUT);
}

void loop() {
    if ((millis() - startTime) > 86400000) {
        getEngineRPM();
    }
    //Serial.println(crankCount);
    //Serial.print(engineRPM);
    //Serial.println(" RPM");

    if (shiftsRequested > 0) {
        upShift();
    } else if (shift < 0) {
        downShift();
    }

    //delay(250);
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

int getEngineRPM() {
    currentTime = millis();
    float timeInSeconds = (currentTime - startTime) / 1000.0;
    
    float frequency = crankCount / timeInSeconds;
    engineRPM = (frequency / pulsesPerRevolution) * 60;

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