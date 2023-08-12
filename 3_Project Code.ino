#include <NewPing.h>
#include <Servo.h>
#include <AFMotor.h>

//hc-sr04 sensor
#define TRIGGER_PIN A2
#define ECHO_PIN A3
#define max_distance 50

//ir sensor
#define irLeft A0
#define irRight A1

//motor
#define MAX_SPEED 200
#define MAX_SPEED_OFFSET 20

Servo servo;

NewPing sonar(TRIGGER_PIN, ECHO_PIN, max_distance);

AF_DCMotor motor1(1, MOTOR12_1KHZ);
AF_DCMotor motor2(2, MOTOR12_1KHZ);
AF_DCMotor motor3(3, MOTOR34_1KHZ);
AF_DCMotor motor4(4, MOTOR34_1KHZ);


int distance = 0;
int leftDistance;
int rightDistance;
boolean object;
int general_speed = 95;
int turning_speed = 255;
int lr_speed = 135;

void setup() {
  //Serial.begin(9600);
  pinMode(irLeft, INPUT);
  pinMode(irRight, INPUT);
  servo.attach(10);
  servo.write(90);

  motor1.setSpeed(general_speed);
  motor2.setSpeed(general_speed);
  motor3.setSpeed(general_speed);
  motor4.setSpeed(general_speed);
}

void loop() {
  if (digitalRead(irLeft) == 0  && digitalRead(irRight) == 0 ) {
    objectAvoid();
    //forword
  }
  else if (digitalRead(irLeft) == 0 && digitalRead(irRight) == 1 ) {
    objectAvoid();
    //Serial.println("TL");
    //leftturn
    moveLeft();
  }
  else if (digitalRead(irLeft) == 1 && digitalRead(irRight) == 0 ) {
    objectAvoid();
    //Serial.println("TR");
    //rightturn
    moveRight();
  }
  else if (digitalRead(irLeft) == 1 && digitalRead(irRight) == 1 ) {
    Stop();
    delay(3000);
    stopturn();
    delay(700);

  }
}

void objectAvoid() {
  distance = getDistance();
  if (distance <= 12) {
    //stop 
    Stop();
    //Serial.println("Stop");

    lookLeft();
    lookRight();
    delay(50);
    if (rightDistance <= leftDistance) {
      //left
      object = true;
      turn();
      //Serial.println("moveLeft");
    } else {
      //right
      object = false;
      turn();
      //Serial.println("moveRight");
    }
    delay(100);
  }
  else {
    //forword
    //Serial.println("moveforword");
    moveForward();
  }
}

int getDistance() {
  delay(50);
  int cm = sonar.ping_cm();
  if (cm == 0) {
    cm = 100;
  }
  return cm;
}

int lookLeft () {
  //lock left
  servo.write(150);
  delay(500);
  leftDistance = getDistance();
  delay(100);
  servo.write(90);
  //Serial.print("Left:");
  Serial.print(leftDistance);
  return leftDistance;
  delay(100);
}

int lookRight() {
  //lock right
  servo.write(60);
  delay(500);
  rightDistance = getDistance();
  delay(100);
  servo.write(90);
  //Serial.print("   ");
  //Serial.print("Right:");
  //Serial.println(rightDistance);
  return rightDistance;
  delay(100);
}
void Stop() {
  motor1.run(RELEASE);
  motor2.run(RELEASE);
  motor3.run(RELEASE);
  motor4.run(RELEASE);
}
void moveForward() {
  motor1.setSpeed(general_speed);
  motor1.run(FORWARD);
  motor2.setSpeed(general_speed);
  motor2.run(FORWARD);
  motor3.setSpeed(general_speed);
  motor3.run(FORWARD);
  motor4.setSpeed(general_speed);
  motor4.run(FORWARD);
}
void moveBackward() {
  motor1.setSpeed(general_speed);
  motor1.run(BACKWARD);
  motor2.setSpeed(general_speed);
  motor2.run(BACKWARD);
  motor3.setSpeed(general_speed);
  motor3.run(BACKWARD);
  motor4.setSpeed(general_speed);
  motor4.run(BACKWARD);

}
void turn() {
  if (object == false) {
   //Serial.println("turn Right");
    moveLeft();
    delay(500);
    moveForward();
    delay(1000);
    moveRight();
    delay(800);
    if (digitalRead(irRight) == 1) {
      loop();
    } else {
      moveForward();
    }
  }
  else {
    moveRight();
    delay(500);
    moveForward();
    delay(1000);
    moveLeft();
    delay(800);
    if (digitalRead(irLeft) == 1) {
      loop();
    } else {
      moveForward();
    }
  }
}
void moveRight() {
    motor1.setSpeed(lr_speed);
    motor1.run(BACKWARD);
    motor2.setSpeed(lr_speed);
    motor2.run(BACKWARD);
    motor3.setSpeed(lr_speed);
    motor3.run(FORWARD);
    motor4.setSpeed(lr_speed);
    motor4.run(FORWARD);
}
void moveLeft() {
  motor1.setSpeed(lr_speed);
    motor1.run(FORWARD);
    motor2.setSpeed(lr_speed);
    motor2.run(FORWARD);
    motor3.setSpeed(lr_speed);
    motor3.run(BACKWARD);
    motor4.setSpeed(lr_speed);
    motor4.run(BACKWARD);
}
void stopturn() {
    motor1.setSpeed(turning_speed);
    motor1.run(FORWARD);
    motor2.setSpeed(turning_speed);
    motor2.run(FORWARD);
    motor3.setSpeed(turning_speed);
    motor3.run(BACKWARD);
    motor4.setSpeed(turning_speed);
    motor4.run(BACKWARD);
}