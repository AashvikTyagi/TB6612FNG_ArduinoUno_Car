const int motor1Forward = 13, motor1Backward = 12, motor1Pwm = 6; // setting constant integers for the first motor's pins

const int motor2Forward = 10, motor2Backward = 9, motor2Pwm = 5; // second motor

const int STBY = 2; // Standby pin. when set to HIGH, allows us to move the motors. When LOW, everything stops.

// void setup - runs once in the starting
void setup() {
  // setting these pins as outputs

  pinMode(motor1Forward, OUTPUT);
  pinMode(motor1Backward, OUTPUT);
  pinMode(motor1Pwm, OUTPUT);

  pinMode(motor2Forward, OUTPUT);
  pinMode(motor2Backward, OUTPUT);
  pinMode(motor2Pwm, OUTPUT);

  pinMode(STBY, OUTPUT);
}

// the void loop - runs forever after void setup
void loop() {
  move(1,100,'f'); // moves motor1, forward, at 100 PWM
  move(2,100,'f');  // moves motor2, forward, at 100 PWM

  delay(1000); // wait (keep on doing that for 1000 miliseconds / one second)

  move(1,100,'b'); // moves motor1, backward, at 100 PWM
  move(2,100,'b');  // moves motor2, backward, at 100 PWM

  delay(1000); // delay for one second

  // NOTE: you can also have one motor going forward and another going backward to turn around.
}

// this is a function to move a specific motor at a specific speed in a specific direction
void move(int motor, int speed, char direction){
  /*
  Paramaters:
    motor - which motor to turn:
      1 will move the first motor (motor1) (left motor).
      2 will move the second motor (motor2) (right motor).
    speed - the speed, the pwm:
      0 is stop.
      255 is the fastest.
      NOTE: in my case, i need at least 60 speed (pwm) to turn the motors
    direction - the direction to turn:
      'f' is forward (clockwise)
      'b' is backward (anticlockwise)
      NOTE: if some other character is given for direction, it will default to moving forward.
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

