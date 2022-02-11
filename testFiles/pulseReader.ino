const int pulsePin = 8; // Input signal connected to Pin 8 of Arduino

int pulseHigh; // Integer variable to capture High time of the incoming pulse
int pulseLow; // Integer variable to capture Low time of the incoming pulse
float pulseTotal; // Float variable to capture Total time of the incoming pulse
float frequency; // Calculated Frequency

void setup() {
  Serial.begin(9600);
   pinMode(pulsePin,INPUT);
   //attachInterrupt(0,high,RISING);
   //attachInterrupt(0,low,FALLING);
   //delay(5000);
}

void high() {
  pulseHigh = pulseIn(pulsePin,HIGH);
  Serial.println("high");
}

void low() {
  pulseLow = pulseIn(pulsePin,LOW);
  Serial.println("low");
}

void loop() {
    pulseHigh = pulseIn(pulsePin,HIGH);
    //Serial.println(pulseHigh);
    pulseLow = pulseIn(pulsePin,LOW);
    //Serial.println(pulseLow);
    
    pulseTotal = pulseHigh + pulseLow; // Time period of the pulse in microseconds
    frequency=1000000/pulseTotal; // Frequency in Hertz (Hz)
    
    Serial.print(frequency);
    Serial.println("Hz");
    //delay(5000);
}