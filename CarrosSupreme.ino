// required libraries - Servo for the servo, NewPing for the ultrasound sensor
#include <Servo.h>
#include <NewPing.h>

const int motor1Forward = 9, motor1Backward = 8, motor1Pwm = 3; // setting constant integers for the first motor's pins
const int motor2Forward = 7, motor2Backward = 6, motor2Pwm = 5; // second motor
const int STBY = 2; // Standby pin. when set to HIGH, allows us to move the motors. When LOW, every motor stops.
int speed = 100; // speed to move the motors at

const int distanceTriggerPin = A0, distanceEcoPin = A1, servoPin = A2; // sensor pins

// creating objects eg. distanceSensor will be our ultrasound sensor, distanceSensorServo will be our servo
NewPing distanceSensor(distanceTriggerPin,distanceEcoPin); // NOTE: we have to specify the trigger and echo pins. trigger sends signal for sending ultrasound waves, and echo is used to give back data about the waves.
Servo distanceSensorServo;

int distance, distanceRight, distanceLeft; // distance variables that we will update with values from our ultrasound sensor
int dangerousDistance = 15; // the maximum distance for some obstacle to be dangerous

// void setup - code inside runs once in the starting
void setup() {

  // setting these pins as outputs

  pinMode(motor1Forward, OUTPUT);
  pinMode(motor1Backward, OUTPUT);
  pinMode(motor1Pwm, OUTPUT);

  pinMode(motor2Forward, OUTPUT);
  pinMode(motor2Backward, OUTPUT);
  pinMode(motor2Pwm, OUTPUT);

  pinMode(STBY, OUTPUT);

  distanceSensorServo.attach(servoPin); // attaches our distanceSensorServo to pin 8

}

// the void loop - runs forever after void setup, until the arduino is powered off
void loop() {

  look(distanceSensorServo,'f');

  distance = distanceSensor.ping_cm(); // distance now holds the distance in cm from the distanceSensor

  // a bunch of cases and what to do to avoid obstacles
  if (distance<dangerousDistance) { // if the car is blocked by and obstacle in front of it, then:
    look(distanceSensorServo,'f'); // make the distanceSensor look forward
    stop(); // stop both the motors
    delay(500); // wait half a second (for servo to turn completely)
    distance = distanceSensor.ping_cm(); // update the distance variable
    look(distanceSensorServo,'l'); // make the distanceSensor look left so that it can scan the distance from the car towards the left
    delay(1000); // wait a second (for servo to turn completely)
    distanceLeft = distanceSensor.ping_cm(); // store the distance to the left in distanceLeft
    if (distanceLeft>dangerousDistance) { // if there is enough space towards the left, then:
      while (distance<dangerousDistance) { // while the car's front is still blocked (don't worry, we make it look forward in the next line so it's front distance is correct):
        look(distanceSensorServo,'f'); // make the distanceSensor's servo look forward
        delay(500); // wait for half a second for the distanceSensorServo to turn
        // make the first motor go backward and the second go forward, allowing it to turn left on-the-spot
        move(1,speed,'b'); // move the left motor (motor1) backward (anticlockwise) at the value in our speed variable [SEE: Line 8]
        move(2,speed,'f'); // move the right motor (motor2) forward (clockwise) at the value in our speed variable [SEE: Line 8]
        distance = distanceSensor.ping_cm(); // update the distance from its front
      }
    } else if (distanceLeft<dangerousDistance) { // otherwise, if it is blocked on the left too, (blocked on the front and left), then:
        look(distanceSensorServo,'r'); // make the distanceSensor look right so that it can scan the distance from the car towards the right
        delay(1000); // wait a second (for servo to turn completely)
        distanceRight = distanceSensor.ping_cm(); // store the distance to the right in distanceRight
        if (distanceRight>dangerousDistance) { // if there is enough space towards the right, then:
          while (distance<dangerousDistance) { // while the car's front is still blocked (don't worry, we make it look forward in the next line so that it's front distance measurement is correct):
            look(distanceSensorServo,'f'); // make the distanceSensor look forward
            delay(500); // wait for half a second for the distanceSensorServo to turn
            // make the first motor go forward and the second go backward, allowing it to turn right on-the-spot
            move(1,speed,'f'); // move the left motor (motor1) forward (clockwise) at speed
            move(2,speed,'b'); // move the right motor (motor2) backward (anticlockwise) at speed
            distance = distanceSensor.ping_cm(); // update the distance from its front
          }
        } else if (distanceRight<dangerousDistance) { // otherwise, if it is blocked on the right too, (blocked on front, left and right), then:
          while (distance<dangerousDistance) { // while the car's front is still blocked:
            look(distanceSensorServo,'f'); // make the distanceSensor look forward
            delay(500); // wait for half a second for the distanceSensorServo to turn
            move(1,speed,'b'); // move the left motor (motor1) backward (anticlockwise) at speed
            move(2,speed,'b'); // move the right motor (motor2) backward (anticlockwise) at speed
            distance = distanceSensor.ping_cm(); // update the distance from its front
          }
        }
    }
  }

  // thing to keep on doing unless blocked
  move(1,speed,'f'); // moves motor 1 [left motor] forward at speed
  move(2,speed,'f'); // moves motor 2 [right motor] forward at speed

}

// this is a function to move a specific motor at a specific speed in a specific direction
void move(int motor, int speed, char direction){
  /*
  Paramaters:
    motor (integer) - which motor to turn:
      1 will move the first motor (motor1) (left motor).
      2 will move the second motor (motor2) (right motor).
    speed (integer) - the speed, the pwm:
      0 is stop.
      255 is the fastest.
      NOTE: in my case, I needed at least 60 speed (pwm) to turn the motors
    direction (character) - the direction to turn:
      'f' is forward (clockwise)
      'b' is backward (anticlockwise)
      NOTE: if some other character is given for direction, it will default to moving forward.
  NOTE: you can also have one motor going forward and another going backward to turn around.
  */
  boolean motorPinOneState;
  boolean motorPinTwoState;
  digitalWrite(STBY, HIGH);
  switch (direction) { // which pin needs to be HIGH and which one LOW to turn clockwise or anticlockwise
    case 'f':
      motorPinOneState = HIGH;
      motorPinTwoState = LOW;
      break;
    case 'b':
      motorPinOneState = LOW;
      motorPinTwoState = HIGH;
      break;
    default:
      motorPinOneState = HIGH;
      motorPinTwoState = LOW;
    break;
  }
  switch (motor) { // moves the required motor
    case 1:
      digitalWrite(motor1Forward, motorPinOneState);
      digitalWrite(motor1Backward, motorPinTwoState);
      analogWrite(motor1Pwm, speed);
      break;
    case 2:
      digitalWrite(motor2Forward, motorPinOneState);
      digitalWrite(motor2Backward, motorPinTwoState);
      analogWrite(motor2Pwm, speed);
      break;
  }
}

// this is a function to stop all the motors
void stop() {
  digitalWrite(STBY, LOW); 
}

// this is a basic function to turn a specific servo either to 90° left, forward, or right, and it has been used to turn our distanceSensorServo [SEE: Line 46]
void look(Servo servoToMove, char direction) {
  /*
  Paramaters:
    servoToMove (Servo) - which servo to turn:
      Any Servo object
    direction (character) - the direction to turn the servo:
      'l' will turn it to 90° left
      'f' will turn it to 90° forward
      'r' will turn it to 90° right
      NOTE: if some other character is given for direction, it will default to turning forward.
  */
  switch (direction) { // switch-case for turning the servo based on the 'direction' char
    case 'l':
      servoToMove.write(180);
      break;
    case 'f':
      servoToMove.write(90);
      break;
    case 'r':
      servoToMove.write(0);
      break;
    default:
      servoToMove.write(90);
      break;
  }
}

