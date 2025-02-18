//PWM pins 13 and 12
#define PWMA 13
#define PWMB 12

//define 22-26
#define AI1 22
#define AI2 23
#define STBY 24
#define BI1 25
#define BI2 26

int BASE_MOTOR_SPEED = 64;

void setup() {
  //Initialize Serial Communication for rpi
  Serial.begin(115200);

  while (!Serial) {

  }

  // put your setup code here, to run once:
  pinMode(PWMA, OUTPUT);
  pinMode(PWMB, OUTPUT);
  pinMode(AI1, OUTPUT);
  pinMode(AI2, OUTPUT);
  pinMode(STBY, OUTPUT);
  pinMode(BI1, OUTPUT);
  pinMode(BI2, OUTPUT);

  // Ensure standby is off
  digitalWrite(STBY, HIGH);


  //Set initial directions for both wheels
  digitalWrite(AI1, HIGH);  //Forward for motor A
  digitalWrite(AI2, LOW);
  digitalWrite(BI1, HIGH);  //Forward for motor B
  digitalWrite(BI2, LOW);

}

void loop() { 
  String command = "";
  command = readSerialMessage();
  switch (command) {
      case 'MOVE MOTORS SHORT':
        moveMotorsForward();
        delay(2000);
        stopMotors();
        break;
      case 'MOVE MOTORS MEDIUM':
        moveMotorsForward();
        delay(4000);
        stopMotors;
        break;
      case 'MOVE MOTORS LONG':
        moveMotorsForward();
        delay(6000);
        stopMotors;
        break;
      default:
        break;
  }
  
}

String readSerialMessage() {
  String message = "";
  if (Serial.available() > 0) {                  // Check if data is available to read
    message = Serial.readStringUntil('\n');      // Read the incoming message until newline
  }
  return message;                                // Return the message
}

void stopMotors(){
  // Set PWM to 25% duty cycle (64 out of 255)
  analogWrite(PWMA, 0); // quarter-speed for motor A
  analogWrite(PWMB, 0); // quarter-speed for motor B
  Serial.println("MOTORS STOPPED");
}

void moveMotorsForward() {
  // Set PWM to 25% duty cycle (64 out of 255)
  analogWrite(PWMA, BASE_MOTOR_SPEED); // quarter-speed for motor A
  analogWrite(PWMB, BASE_MOTOR_SPEED); // quarter-speed for motor B
  Serial.println("MOTORS MOVED FORWARD");
}

void moveMotorsBackward() {
  // Set directions for both wheels to move backward
digitalWrite(AI1, LOW);   // Reverse for motor A
digitalWrite(AI2, HIGH);
digitalWrite(BI1, LOW);   // Reverse for motor B
digitalWrite(BI2, HIGH);

// Set the speed for both motors
analogWrite(PWMA, BASE_MOTOR_SPEED);
analogWrite(PWMB, BASE_MOTOR_SPEED);
}


