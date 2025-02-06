/* 
  File: Radar-Scanner-Projekt2
  Author: Karim Nagy
  Date: 2025-02-06
  Description: This program is the code for an arduino radar
*/


// Initiating libraries for servo and OLED-screen
#include <Servo.h>
#include <U8glib.h>

// Init constants
const int trigPin = 9;
const int echoPin = 10;
const int startX = 63, startY = 63;


// Init global variables
Servo myservo;
float duration, distance;
int x, y;

// Construct objects
U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NO_ACK);

void setup() {

  // Attach servo and setup sensor pins
  myservo.attach(6);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  // Init communication
  Serial.begin(9600);

  // Init hardware
  u8g.setFont(u8g_font_unifont);
}

/*
* This function sweeps the servo 180 degrees back and forth, calls on the measureAndPrintDistance and calculateLineCoordinates function, and updates the OLED-screen
* Parameters: Void
* Returns: Void
*/

void sweepAndMeasure() {
  for (int pos = 0; pos <= 180; pos += 5) {
    measureAndPrintDistance(pos);
    calculateLineCoordinates(pos);
    u8g.firstPage();
    do {
      u8g.drawLine(startX, startY, x, y); // Draws a line on the OLED-screen using the variables  x and y
      u8g.drawCircle(63, 63, 62); // Draws a circle on the OLED-screen
    } while (u8g.nextPage());
    delay(200);
  }
  for (int pos = 180; pos >= 0; pos -= 5) {
    measureAndPrintDistance(pos);
    calculateLineCoordinates(pos);
    u8g.firstPage();
    do {
      u8g.drawLine(startX, startY, x, y); 
      u8g.drawCircle(63, 63, 62);
    } while (u8g.nextPage());
    delay(200);
  }
}

/*
* This function measures the distance with the ultrasonicsensor and prints it along with the degrees in the serial monitor and also stores distance as a global variable
* Parameters: pos - the current servo position
* Returns: Void
*/

void measureAndPrintDistance(int pos) {

  myservo.write(pos);
  delay(15);

  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);

  distance = (duration * 0.0343) / 2;

  Serial.print("Position: ");
  Serial.print(pos);
  Serial.print("\u00b0 - Distance: ");
  Serial.println(distance);
}

/*
* This function calculates the end coordinates of the line on the servo based on the servo position and the ultrasonicsensors measured distance, and also stores x and y as global variables
* Parameters: pos - the current servo position
* Returns: Void
*/

void calculateLineCoordinates(int pos) {
  const int Max_distance = 62;

  if (distance > Max_distance) {
    x = startX;
    y = startY;
    return;
  }

  int lineLength = map(distance, 0, Max_distance, 0, 62);
  float angleRad = pos * M_PI / 180.0;
 
  x = 63 + lineLength * cos(angleRad);
  y = 63 - lineLength * sin(angleRad);
}


void loop() {
  sweepAndMeasure(); // Continuously runs sweepAndMeasure function
}
