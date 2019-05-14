#include <Servo.h>

enum Speeds {
  fullForward =  90,
  fullBackwards = -90,
  turnSpeed = 20
} speeds;

enum Speeds forward = fullForward;

Servo rightServo;  // create servo object to control a servo
Servo leftServo;

int rightPos = 0;  // variable to store the servo position
int leftLeft = 0;
const int BASE = 90; //starting rotation for servos
boolean movementStarted = false;

int forwardTrigPin = A5;    //Trig - yellow on sensor end and black in sensor shield
int forwardEchoPin = A4;    //Echo - red on sensor end and white in sensor shield

int rightEchoPin = A0;
int rightTrigPin = A1; 

int leftEchoPin = A3;
int leftTrigPin = A2;

void setup() {
  pinMode(forwardTrigPin, OUTPUT);
  pinMode(forwardEchoPin, INPUT);

  pinMode(rightTrigPin, OUTPUT);
  pinMode(rightEchoPin, INPUT);

  pinMode(leftTrigPin, OUTPUT);
  pinMode(leftEchoPin, INPUT);

  rightServo.attach(0);
  leftServo.attach(1);// attaches the servo on pins
  Serial.begin(9600);
}

void logStr(char* msg) {
  Serial.println(msg);
}

void log(int value) {
  Serial.println(value);
}

// Servos use rotational degrees between 0 and 180 degrees:
void runLeftServo(int rotation) {
  // left servo is installed 'upside down', so increment to base
  int correctRotation = BASE + rotation;
  leftServo.write(correctRotation);
}

void runRightServo(int rotation) {
  // decrement from base
  int correctRotation = BASE - rotation;
  rightServo.write(correctRotation);
}

void goForward() {
  enum Speeds s = fullForward;
  runRightServo((int) s);
  runLeftServo((int) s);
}

void turnRight() {
  enum Speeds turn = turnSpeed;
  //  Serial.print("left speed: ");
  //log( (int) turn);
  runLeftServo( (int) turn);

  enum Speeds forward = fullForward;
  //  Serial.print("right speed: ");
  //log( (int) forward);
  runRightServo(-1);
}

void turnLeft() {
  runRightServo(forward);
  runLeftServo(1);
}


unsigned long previousMillis = 0;
const long interval = 500;

void loop() {

   
  int leftDist = getDistance(leftEchoPin, leftTrigPin);
  logStr("left distance: ");
  log(leftDist); 
  int rightDist = getDistance(rightEchoPin, rightTrigPin);
  logStr("Right distance: ");
  log(rightDist); 
  
  int forwardDist = getDistance(forwardEchoPin, forwardTrigPin);
  logStr("forward distance" );
  log(forwardDist); 
  
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;

    if (forwardDist <= 25) {
      logStr("Turning");
      if(leftDist < rightDist) {
        logStr("Turning right");
        turnRight();
      } else {
        logStr("Turning left");
        turnLeft();
      }
    } else {
      logStr("Moving forward");
      goForward();
    }
  }
}


int getDistance(int echoPin, int triggerPin) {

  //logStr("echo");
  //log(echoPin);
  //logStr("trigger");
  //log(triggerPin);
  // The sensor is triggered by a HIGH pulse of 10 or more microseconds.
  // Give a short LOW pulse beforehand to ensure a clean HIGH pulse:
  digitalWrite(triggerPin, LOW);
  delayMicroseconds(5);
  digitalWrite(triggerPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(triggerPin, LOW);

  // Read the signal from the sensor: a HIGH pulse whose
  // duration is the time (in microseconds) from the sending
  // of the ping to the reception of its echo off of an object.
  pinMode(echoPin, INPUT);
  long duration = pulseIn(echoPin, HIGH);

  // convert the time into a distance
  int cm = (duration / 2) / 29.1;
  return cm;
}
