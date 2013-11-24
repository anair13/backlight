int redPin = 9; //Red pin 9 has a PWM
int greenPin = 10; //Green pin 10 has a PWM
int bluePin = 11; //Blue pin 11 has a PWM

int buffer[3] = {0, 0, 0};
int numByte = -1;

void setup() {  
  Serial.begin(9600);
}

void loop() {
  
}

void serialEvent() {
  if (numByte >= 0) {
     buffer[numByte] = Serial.read();
     numByte++;
  }
  else if (Serial.read() == 0xff) {
    numByte = 0;
  }
  
  if (numByte > 2) {
    // control led brightness through pulse-width modulation
    analogWrite (redPin, buffer[0]);
    analogWrite (greenPin, buffer[1]);
    analogWrite (bluePin, buffer[2]);
    numByte = -1;
  }
}
