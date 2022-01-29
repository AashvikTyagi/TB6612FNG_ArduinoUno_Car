const int motor1Forward = 9, motor1Backward = 8, motor1Pwm = 3; // setting constant integers for the first motor's pins
const int motor2Forward = 7, motor2Backward = 6, motor2Pwm = 5; // second motor
const int STBY = 2; // Standby pin. when set to HIGH, allows us to move the motors. When LOW, every motor stops.
int speed; // speed to move the motors at
char input; // the input we get from the phone over bluetooth

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

  Serial.begin(9600);

}

// the void loop - runs forever after void setup, until the arduino is powered off
void loop() {

  if (Serial.available() > 0 ) {
    input = Serial.read();
  }

  switch (input) {
    case '0':
      speed = 0;
      break;
    case '1':
      speed = map(10,10,100,50,255);
      break;
    case '2':
      speed = map(20,10,100,50,255);
      break;
    case '3':
      speed = map(30,10,100,50,255);
      break;
    case '4':
      speed = map(40,10,100,50,255);
      break;
    case '5':
      speed = map(50,10,100,50,255);
      break;
    case '6':
      speed = map(60,10,100,50,255);
      break;
    case '7':
      speed = map(70,10,100,50,255);
      break;
    case '8':
      speed = map(80,10,100,50,255);
      break;
    case '9':
      speed = map(90,10,100,50,255);
      break;
    case 'q':
      speed = 255;
      break;
    case 'F':
      move(1,speed,'f');
      move(2,speed,'f');
      break;
    case 'B':
      move(1,speed,'b');
      move(2,speed,'b');
      break;
    case 'L':
      move(1,speed,'b');
      move(2,speed,'f');
      break;
    case 'R':
      move(1,speed,'f');
      move(2,speed,'b');
      break;
    case 'G':
      move(1,(int)speed/2,'f');
      move(2,speed,'f');
      break;
    case 'I':
      move(1,speed,'f');
      move(2,(int)speed/2,'f');
      break;
    case 'H':
      move(1,(int)speed/2,'b');
      move(2,speed,'b');
      break;
    case 'J':
      move(1,speed,'b');
      move(2,(int)speed/2,'b');
      break;
    case 'S':
      stop();
      break;
    case 'D':
      stop();
      break;    
  }

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
