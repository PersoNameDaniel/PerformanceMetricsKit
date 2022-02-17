const int pulsePin = 12; // Input signal connected to Pin 8 of Arduino
const int intPin = 0;

int pulseHigh; // Integer variable to capture High time of the incoming pulse
int pulseLow; // Integer variable to capture Low time of the incoming pulse
float pulseTotal; // Float variable to capture Total time of the incoming pulse
double rPM; // Calculated RPM
int timeSinceLast;

void setup() {
  Serial.begin(1200);
  pinMode(pulsePin,INPUT);
  attachInterrupt(0, high, RISING);
  attachInterrupt(1, low, FALLING);
}

void high() {
  int time = millis() - timeSinceLast;
  rPM = (time);
  //rPM = (166.66666667 / (10 * timeSinceLast));
  timeSinceLast = millis();
  //Serial.println(timeSinceLast);
  //Serial.println("beep");
}

void low() {
  int time = millis() - timeSinceLast;
  rPM = (time);
  //rPM = (166.666666666 / (10 * timeSinceLast));
  //rPM = (timeSinceLast / 166.66666666666);
  timeSinceLast = millis();
  //Serial.println("boop");
}

void loop() {
Serial.println(rPM);
//Serial.println(millis());
//delay(1000);
}