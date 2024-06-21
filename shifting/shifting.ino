const int upShiftPin = 2;
const int downShiftPin = 3;
const int crankSensorPin = 18;

const int maxGear = 4;
int gear = 1;
int shift = 0;
int crankCount = 0;
unsigned long startTime = millis();

void setup() {
    Serial.begin(9600);
    pinMode(upShiftPin, INPUT);
    attachInterrupt(digitalPinToInterrupt(upShiftPin), incrementShift, RISING);
    pinMode(downShiftPin, INPUT);
    attachInterrupt(digitalPinToInterrupt(downShiftPin), decrementShift, RISING);
    pinMode(crankSensorPin, INPUT);
    attachInterrupt(digitalPinToInterrupt(crankSensorPin), incrementCrankCount, RISING);
}

void loop() {
    //Serial.println(crankCount);
    Serial.print(getEngineRPM());
    Serial.println(" RPM");

    if (shift > 0) {
        upShift();
    } else if (shift < 0) {
        downShift();
    }
    delay(500);
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
    int engineRPM = -1;
    
    float frequency = crankCount / timeInSeconds; // calculate frequency in Hz
    engineRPM = (frequency / 3) * 60; // convert Hz to RPM

    Serial.println(crankCount);
    Serial.println(timeInSeconds);

    crankCount = 0;
    startTime = currentTime;

    return engineRPM;
}


void upShift() {
    if (gear < maxGear) {
        switch (gear){
            case 1:
                shiftToSecond();
                gear++;
            break;
            case 2:
                shiftToThird();
                gear++;
            break;
            case 3:
                shiftToFourth();
                gear++;
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
                shiftToFirst();
                gear--;
            break;
            case 3:
                shiftToSecond();
                gear--;
            break;
            case 4:
                shiftToThird();
                gear--;
            break;
        }
    } else {
        Serial.print("down shift invalid: current gear is ");
        Serial.println(gear);
    }
    shift += 1;
}

void shiftToFirst() {
    Serial.println("Shifting to first gear");
}

void shiftToSecond() {
    Serial.println("Shifting to second gear");
}

void shiftToThird() {
    Serial.println("Shifting to third gear");
}

void shiftToFourth() {
    Serial.println("Shifting to fourth gear");
}