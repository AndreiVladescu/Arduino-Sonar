#include <Servo.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// DEBUG Flag
#define DEBUG false

// Servo delay between angle shifting in ms
#define SERVO_SPEED 50
// OLED display width, in pixels
#define SCREEN_WIDTH 128 
// OLED display height, in pixels
#define SCREEN_HEIGHT 64 

// PWM Compatible Pins
#define echoPin 10
#define trigPin 9
// Analog Pin
#define infraredPin A0

// Servo Object
Servo servo;
// Global integer for ultrasonic distance distance measurement
int sonic_distance;
// Global floating-point number for infrared distance measurement
float infra_distance;
// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// Setup function, runs only once when starting execution
void setup() {
  // Sets the trigPin as an OUTPUT
  pinMode(trigPin, OUTPUT);
  // Sets the echoPin as an INPUT
  pinMode(echoPin, INPUT);
  // Attaches the servomotor object to pin 11
  servo.attach(11);
  // Address 0x3D for 128x64 OLED Display
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  // Config for OLED Display
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.clearDisplay();
  // Baudrate for PC comms
  Serial.begin(9600);
}

/*
   Function responsible for measuring the distance to the
   front-facing object using HC-SR04 Ultrasonic Sensor
*/
void getDistanceSonic()
{
  /* https://create.arduino.cc/projecthub/abdularbi17/ultrasonic-sensor-hc-sr04-with-arduino-tutorial-327ff6
    HC-SR04 distance sensor
    - uses ultrasonic pulses (40 KHz) to measure distances
    - 2<->450 cm detection
  */
  // Duration in µs
  long duration;
  // Clearing the trigger pin for the sensor
  digitalWrite(trigPin, LOW);
  // Safety delay for clearing
  delayMicroseconds(2);
  // Pulling the trigger *UP*
  digitalWrite(trigPin, HIGH);
  // Keeping the trigger pulse on
  delayMicroseconds(10);
  // Pulling the trigger *DOWN*
  digitalWrite(trigPin, LOW);
  // Getting the distance as a pulse from the echoPin
  duration = pulseIn(echoPin, HIGH);
  // 0.034 cm/µs | 340 m/s
  // Division by 2, since the pulse has to return from the object
  sonic_distance = duration * 0.034 / 2;
#if DEBUG
  Serial.println("Ultrasonic Distance Sensor:" + String(sonic_distance));
#endif
}

/*
   Function responsible for measuring the distance to the
   front-facing object using Sharp GP-2Y0A02 Infrared Sensor
*/
void getDistanceInfrared()
{
  /*https://www.dcode.fr/function-equation-finder
    parabola/hyperbola using curve fitting

    Equation:
    f(x) = 0.0000213281x^2−0.112176x+170.8

      Manually Measured Samples
    |---------------------------|
    | Dist (cm) | Values (volts)|
    | 20        | 2835          |
    | 22        | 2815          |
    | 25        | 2620          |
    | 30        | 2420          |
    | 35        | 2090          |
    | 40        | 1760          |
    | 45        | 1500          |
    | 50        | 1405          |
    | 60        | 1250          |
    | 70        | 1220          |
    | 80        | 960           |
    | 90        | 880           |
    | 100       | 790           |
    |---------------------------|
  */
  float volts = (float)analogRead(infraredPin);

  infra_distance = 0.0000213281 * volts * volts
                   - 0.112176 * volts
                   + 170.8;
  infra_distance = round(infra_distance);
#if DEBUG
  Serial.println("Infrared Distance Sensor:" + String(infra_distance));
#endif
}

// Simple function for displaying information on the OLED 0.96" Display
void screenUpdate(int angle)
{
  // https://randomnerdtutorials.com/guide-for-oled-display-with-arduino/
  // Code speaks for itself
  display.clearDisplay();
  display.setCursor(0, 10);
  display.println(String(angle));
  display.setCursor(20, 10);
  display.println(" Degrees");
  display.println("---------------------");
  display.println(String(sonic_distance));
  display.setCursor(20, 25);
  display.println(" cm - Ultrasonic");
  display.println("---------------------");
  display.println(String((int)infra_distance));
  display.setCursor(20, 40);
  display.println(" cm - Infrared");
  display.display();
}
// Loop function, runs continously
void loop() {
  // Shifting Servo from 0° to 179°
  for (int servo_angle = 0;
       servo_angle < 180;
       servo_angle++)
  {
    servo.write(servo_angle);
    getDistanceSonic();
    getDistanceInfrared();
#if !DEBUG
    Serial.println(String(sonic_distance) + " "
                   + String(infra_distance) + " "
                   + String(servo_angle));
#endif
    screenUpdate(servo_angle);
    delay(SERVO_SPEED);
  }

  // Shifting Servo from 179° to 0°
  for (int servo_angle = 179;
       servo_angle >= 0;
       servo_angle--)
  {
    servo.write(servo_angle);
    getDistanceSonic();
    getDistanceInfrared();
#if !DEBUG
    Serial.println(String(sonic_distance) + " "
                   + String(infra_distance) + " "
                   + String(servo_angle));
#endif
    screenUpdate(servo_angle);
    delay(SERVO_SPEED);
  }
}