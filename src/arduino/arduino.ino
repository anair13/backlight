//Developed by Rajarshi Roy
int red, green, blue; //red, green and blue values
int RedPin = 9; //Red pin 9 has a PWM
int GreenPin = 10; //Green pin 10 has a PWM
int BluePin = 11; //Blue pin 11 has a PWM

void setup() {
  Serial.begin(9600);
  //initial values (no significance)
  int red = 255;
  int blue = 255;
  int green = 255;
}

void loop() {
  digitalWrite(13, HIGH);   // turn the LED on (HIGH is the voltage level)
  // delay(500);               // wait for a second
  //protocol expects data in format of 4 bytes
  //(xff) as a marker to ensure proper synchronization always
  //followed by red, green, blue bytes
  if (Serial.available()>=4) {
    if (Serial.read() == 0xff) {
      red = Serial.read();
      green= Serial.read();
      blue = Serial.read();
    }
  }
  //finally control led brightness through pulse-width modulation
  analogWrite (RedPin, red);
  analogWrite (GreenPin, green);
  analogWrite (BluePin, blue);
  
  digitalWrite(13, LOW);    // turn the LED off by making the voltage LOW
  delay(50); //just to be safe  
}
