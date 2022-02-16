const int pulsePin = 12; // Input signal connected to Pin 8 of Arduino
const int intPin = 0;

int pulseHigh; // Integer variable to capture High time of the incoming pulse
int pulseLow; // Integer variable to capture Low time of the incoming pulse
float pulseTotal; // Float variable to capture Total time of the incoming pulse
int frequency; // Calculated Frequency

void setup() {
  Serial.begin(2000000);
   pinMode(pulsePin,INPUT);
   attachInterrupt(0, high, RISING);
   attachInterrupt(1, low, FALLING);
   //delay(5000);
}

void high() {
  //pulseHigh = pulseIn(pulsePin,HIGH);
  //Serial.println("high");
  frequency++;
}

void low() {
  //pulseLow = pulseIn(pulsePin,LOW);
  //Serial.println("low");
  frequency--;
}

void loop() {
    //pulseHigh = pulseIn(pulsePin,HIGH);
    //Serial.println(pulseHigh);
    //pulseLow = pulseIn(pulsePin,LOW);
    //Serial.println(pulseLow);
    
    //pulseTotal = pulseHigh + pulseLow; // Time period of the pulse in microseconds
    //frequency=1000000/pulseTotal; // Frequency in Hertz (Hz)
    
    Serial.println(frequency);
    //Serial.println("Hz");
    //delay(1000);
}