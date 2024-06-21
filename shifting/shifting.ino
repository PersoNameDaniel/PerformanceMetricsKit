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
        gear++;
        Serial.print("shift: current gear is ");
        Serial.println(gear);
    } else {
        Serial.print("up shift invalid: current gear is ");
        Serial.println(gear);
    }
    shift -= 1;
}

void downShift() {
    if (gear > 1) {
        gear--;
        Serial.print("shift: current gear is ");
        Serial.println(gear);
    } else {
        Serial.print("down shift invalid: current gear is ");
        Serial.println(gear);
    }
    shift += 1;
}