//PWM pins 13 and 12
#define rearRight 13
#define frontLeft 12

//define 22-26
#define AI1 22
#define AI2 23
#define STBY 24
#define BI1 25
#define BI2 26
#define HEADLIGHTS 27

const int buttonPin = 28;
int buttonState = 0;
int lastButtonState = HIGH;
int pressCount = 0;
unsigned long lastDebounceTime = 0;
const int debounceDelay = 50;

int BASE_MOTOR_SPEED = 64;

void setup() {
  //Initialize Serial Communication for rpi
  Serial.begin(115200);

  pinMode(buttonPin, INPUT_PULLUP);

  while (!Serial) {

  }

  // put your setup code here, to run once:
  pinMode(rearRight, OUTPUT);
  pinMode(frontLeft, OUTPUT);
  pinMode(AI1, OUTPUT);
  pinMode(AI2, OUTPUT);
  pinMode(STBY, OUTPUT);
  pinMode(BI1, OUTPUT);
  pinMode(BI2, OUTPUT);
  pinMode(HEADLIGHTS, OUTPUT);

  // Ensure standby is off
  digitalWrite(STBY, HIGH);


  //Set initial directions for both wheels
  digitalWrite(AI1, LOW);  //Forward for motor A
  digitalWrite(AI2, HIGH);
  digitalWrite(BI1, LOW);  //Forward for motor B
  digitalWrite(BI2, HIGH);

}

void loop() { 

  checkButton();

<<<<<<< HEAD:elegoo.ino
  if (pressCount > 0){
    String command = Serial.readStringUntil('\n');

    if (command.equals("MOVE MOTORS SHORT")) {
        moveMotorsForward();
        delay(2600);
        stopMotors();
        Serial.println(pressCount);
        pressCount--;
    } else if (command == "MOVE MOTORS MEDIUM") {
        moveMotorsForward();
        delay(4000);
        stopMotors();
        Serial.println(pressCount);
        // pressCount--;
    } else if (command == "MOVE MOTORS LONG") {
        moveMotorsForward();
        delay(6000);
        stopMotors();
        Serial.println(pressCount);
        // pressCount--;
    } else if (command == "TURN RIGHT") {
        turnRight();
        delay(1500);
        stopMotors();
        restoreRearRight();
    } else if (command == "TURN LEFT") {
        turnLeft();
        delay(1500);
        stopMotors();
        restoreFrontLeft();
    } else if (command == "MOVE BACKWARD") {
        moveMotorsBackward();
        delay(1000);
        stopMotors();
        setMotorsForward();
    }
  }
=======
  lastButtonState = buttonState;
  
  String command = "";
  command = readSerialMessage();
  if (command == "MOVE MOTORS SHORT") {
    moveMotorsForward();
    delay(2000);
    stopMotors();
  } else if (command == "MOVE MOTORS MEDIUM") {
    moveMotorsForward();
    delay(4000);
    stopMotors();
  } else if (command == "MOVE MOTORS LONG") {
    moveMotorsForward();
    delay(6000);
    stopMotors();
  }

  
>>>>>>> 73a9578 (moved elegoo.ino into elegoo directory):elegoo/elegoo.ino
}

String readSerialMessage() {
  String message = "";
  if (Serial.available() > 0) {                  // Check if data is available to read
    message = Serial.readStringUntil('\n');     // Read the incoming message until newline
    Serial.println(message);      
  }
  return message;                                // Return the message
}

void stopMotors(){
  analogWrite(rearRight, 0); // stop motor A
  analogWrite(frontLeft, 0); // stop motor B
  Serial.println("MOTORS STOPPED");
}

void moveMotorsForward() {
  // Set PWM to 25% duty cycle (64 out of 255)
  analogWrite(rearRight, BASE_MOTOR_SPEED); // quarter-speed for motor A
  analogWrite(frontLeft, BASE_MOTOR_SPEED); // quarter-speed for motor B
  Serial.println("MOTORS MOVED FORWARD");
}

void reverseRearRight() {
  digitalWrite(AI1, HIGH);
  digitalWrite(AI2, LOW);
}
void restoreRearRight() {
  digitalWrite(AI1, LOW);
  digitalWrite(AI2, HIGH);
}

void reverseFrontLeft() {
  digitalWrite(BI1, HIGH);
  digitalWrite(BI2, LOW);
}
void restoreFrontLeft() {
  digitalWrite(BI1, LOW);
  digitalWrite(BI2, HIGH);
}

void turnLeft() {
  reverseFrontLeft();

  // Set PWM to 25% duty cycle (64 out of 255)
  analogWrite(rearRight, (BASE_MOTOR_SPEED *2)); // quarter-speed for motor A
  analogWrite(frontLeft, (BASE_MOTOR_SPEED *2)); // quarter-speed for motor B
  Serial.println("TURNED LEFT");
}

void turnRight() {
  reverseRearRight();

  // Set PWM to 25% duty cycle (64 out of 255)
  analogWrite(rearRight, (BASE_MOTOR_SPEED *2)); // quarter-speed for motor A
  analogWrite(frontLeft, (BASE_MOTOR_SPEED*2)); // quarter-speed for motor B
  Serial.println("TURNED LEFT");
}

void reverseMotors(){
  reverseRearRight();
  reverseFrontLeft();
}

void moveMotorsBackward() {
  // Set directions for both wheels to move backward
  reverseMotors();
  // Set the speed for both motors
  analogWrite(rearRight, (BASE_MOTOR_SPEED*2));
  analogWrite(frontLeft, (BASE_MOTOR_SPEED*2));
}

void setMotorsForward() {
  digitalWrite(AI1, LOW);
  digitalWrite(AI2, HIGH);
  digitalWrite(BI1, LOW);
  digitalWrite(BI2, HIGH);
}

void turnHeadlightsOn() {
  digitalWrite(HEADLIGHTS, HIGH);
}

void turnHeadlightsOff() {
  digitalWrite(HEADLIGHTS, LOW);
}
<<<<<<< HEAD:elegoo.ino


void checkButton(){

  buttonState = digitalRead(buttonPin);

  if (buttonState == LOW && lastButtonState == HIGH) {
    pressCount+=500;
    delay(500);
    Serial.println("1");
  }

lastButtonState = buttonState;
}
=======
>>>>>>> 73a9578 (moved elegoo.ino into elegoo directory):elegoo/elegoo.ino
