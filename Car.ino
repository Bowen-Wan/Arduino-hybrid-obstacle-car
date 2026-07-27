#include <IRremote.hpp>

// DC Motor

// Left Motor
int speedPinA = 11;
int dir1PinA = 12;
int dir2PinA = 13;

// Right Motor
int speedPinB = 10;
int dir1PinB = 9;
int dir2PinB = 8;

int motorSpeed = 255;

// Ultrasonic Sensor
int trigPin = 5;
int echoPin = 6;
int pingTravelTime;
float pingTravelDist;
float distanceToTarget;

int stopDistance = 15; // cm

// IR Remote
int irPin = 4;

#define IR_CODE_TOGGLE   0xBA45FF00  // top-left red button
#define IR_CODE_FORWARD  0xE718FF00  // "2"
#define IR_CODE_BACKWARD 0xAD52FF00  // "8"
#define IR_CODE_LEFT     0xF708FF00  // "4"
#define IR_CODE_RIGHT    0xA55AFF00  // "6"
#define IR_CODE_STOP     0xE31CFF00  // "5"

enum Mode { AUTONOMOUS, MANUAL };
Mode currentMode = AUTONOMOUS;

void setup() {
  pinMode(speedPinA, OUTPUT);
  pinMode(dir1PinA, OUTPUT);
  pinMode(dir2PinA, OUTPUT);

  pinMode(speedPinB, OUTPUT);
  pinMode(dir1PinB, OUTPUT);
  pinMode(dir2PinB, OUTPUT);

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  IrReceiver.begin(irPin, DISABLE_LED_FEEDBACK);

  randomSeed(analogRead(A0));

  Serial.begin(9600);
}

float getDistance() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  pingTravelTime = pulseIn(echoPin, HIGH);
  pingTravelDist = pingTravelTime * 0.034 / 2;
  return pingTravelDist;
}

void moveForward() {
  digitalWrite(dir1PinA, LOW);
  digitalWrite(dir2PinA, HIGH);
  analogWrite(speedPinA, motorSpeed);

  digitalWrite(dir1PinB, LOW);
  digitalWrite(dir2PinB, HIGH);
  analogWrite(speedPinB, motorSpeed);
}

void moveBackward() {
  digitalWrite(dir1PinA, HIGH);
  digitalWrite(dir2PinA, LOW);
  analogWrite(speedPinA, motorSpeed);

  digitalWrite(dir1PinB, HIGH);
  digitalWrite(dir2PinB, LOW);
  analogWrite(speedPinB, motorSpeed);
}

void turnLeft() {
  digitalWrite(dir1PinA, HIGH);
  digitalWrite(dir2PinA, LOW);

  digitalWrite(dir1PinB, LOW);
  digitalWrite(dir2PinB, HIGH);
  analogWrite(speedPinA, motorSpeed);
  analogWrite(speedPinB, motorSpeed);
}

void turnRight() {
  digitalWrite(dir1PinA, LOW);
  digitalWrite(dir2PinA, HIGH);

  digitalWrite(dir1PinB, HIGH);
  digitalWrite(dir2PinB, LOW);
  analogWrite(speedPinA, motorSpeed);
  analogWrite(speedPinB, motorSpeed);
}

void stopMotors() {
  analogWrite(speedPinA, 0);
  analogWrite(speedPinB, 0);
}

void handleIR() {
  if (IrReceiver.decode()) {
    unsigned long code = IrReceiver.decodedIRData.decodedRawData;

    if (code == IR_CODE_TOGGLE) {
      currentMode = (currentMode == AUTONOMOUS) ? MANUAL : AUTONOMOUS;
      stopMotors();
      Serial.println(currentMode == AUTONOMOUS ? "Mode: AUTONOMOUS" : "Mode: MANUAL");
    }
    else if (currentMode == MANUAL) {
      switch (code) {
        case IR_CODE_FORWARD:  moveForward();  break;
        case IR_CODE_BACKWARD: moveBackward(); break;
        case IR_CODE_LEFT:     turnLeft();     break;
        case IR_CODE_RIGHT:    turnRight();    break;
        case IR_CODE_STOP:     stopMotors();   break;
      }
    }

    IrReceiver.resume();
  }
}

void loop() {
  handleIR();

  if (currentMode == MANUAL) {
    return;
  }

  distanceToTarget = getDistance();

  Serial.print("Distance: ");
  Serial.print(distanceToTarget);
  Serial.println(" cm");

  if (distanceToTarget > stopDistance) {
    moveForward();
  } 
  else {
    stopMotors();
    delay(200);

    moveBackward();
    delay(300);
    stopMotors();
    delay(200);

    if (random(2) == 0) {
      turnLeft();
    } 
    else {
      turnRight();
    }
    delay(400);
    stopMotors();
    delay(200);
  }

  delay(50);
}