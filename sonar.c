#include <Servo.h>

#define echoPin 10
#define trigPin 9
#define infraredPin A0

Servo servo;
int distance;
long duration;
float infra_distance;

void setup() {
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an OUTPUT
  pinMode(echoPin, INPUT); // Sets the echoPin as an INPUT
  servo.attach(11);
  Serial.begin(9600);
}

void getDistanceSonic()
{
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(12);
    digitalWrite(trigPin, LOW);
    delayMicroseconds(25);
    duration = pulseIn(echoPin, HIGH);
    distance = duration * 0.034 / 2;
}

void getDistanceInfrared()
{
  float volts = analogRead(infraredPin);
  infra_distance = 270000 * pow(volts , -1.16);
  delay(10);
}

void loop() {
  for(int i=0;i<180;i++){
    servo.write(i);
    getDistanceSonic();
    getDistanceInfrared();
    Serial.println(String(distance) + " " + String(infra_distance) + " " + String(i));
    delay(50);
  }
  servo.write(0);
  delay(750);
}