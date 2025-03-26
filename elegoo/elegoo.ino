#include <Servo.h>
int servopin1 = 2;    //Define servo interface digital interface 5
int servopin2 = 3;    //Define servo interface digital interface 6
int servopin3 = 4;    //Define servo interface digital interface 4
int servopin4 = 5;    //Define servo interface digital interface 3
int servopin5 = 6;   //Define servo interface digital interface 6

//PWM pins 13 and 12
#define rearRight 13
#define frontLeft 12
#define PIN_ANALOG_IN A0

//define 22-26
#define AI1 22
#define AI2 23
#define STBY 24
#define BI1 25
#define BI2 26
#define HEADLIGHTS 27

Servo servo1, servo2, servo3, servo4, servo5;  //Define servo objects
int pos = 0;
const int buttonPin = 28;
const int trigPin = 29;
const int echoPinNorth = 30;
const int echoPinEast = 31;
float duration, distance;

int buttonState = 0;
int lastButtonState = HIGH;
int pressCount = 0;
unsigned long lastDebounceTime = 0;
const int debounceDelay = 50;
bool START = false;
int TotalPhotoReadings = 0;
int NumPhotoReadings = 0;

int BASE_MOTOR_SPEED = 128;

// .925 is the delay for a 90 degree right turn
void setup() {
  //Initialize Serial Communication for rpi
  Serial.begin(115200);

  pinMode(servopin1,OUTPUT);//Set the servo interface as the output interface
  pinMode(servopin2,OUTPUT);//Set the servo interface as the output interface
  pinMode(servopin3,OUTPUT);//Set the servo interface as the output interface
  pinMode(servopin4,OUTPUT);//Set the servo interface as the output interface
  pinMode(servopin5,OUTPUT);//Set the servo interface as the output interface

  //Attach pins to servos
  /*
  servo1.attach(servopin1);
  servo2.attach(servopin2);
  servo3.attach(servopin3);
  servo4.attach(servopin4);
  servo5.attach(servopin5);
  */

  pinMode(buttonPin, INPUT_PULLUP);
  
  //Set pins for ultrasonics
  pinMode(trigPin, OUTPUT);
  pinMode(echoPinNorth, INPUT);
  pinMode(echoPinEast, INPUT);
  
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
  // monitorStartLED();
  // monitorUltrasonics();
  checkButton();

  if (pressCount > 0 || START){
    String message = Serial.readStringUntil('\n');
    Serial.println(message);
    String command;
    String value;
    int separatorIndex = message.indexOf(':');
    if (separatorIndex != -1){
      command = message.substring(0, separatorIndex);
      value = message.substring(separatorIndex + 1); 
    }
    Serial.println(command);
    Serial.println(value);
    if (command.equals("MOVE FORWARD")) {
        moveMotorsForward();
        delay(1000*value.toFloat());
        stopMotors();
        Serial.println(pressCount);
        pressCount--;
    } else if (command == "TURN RIGHT") {
        turnRight();
        delay(1000*value.toFloat());
        stopMotors();
        restoreRearRight();
    } else if (command == "TURN LEFT") {
        turnLeft();
        delay(1000*value.toFloat());
        stopMotors();
        restoreFrontLeft();
    } else if (command == "MOVE BACKWARD") {
        moveMotorsBackward();
        delay(1000*value.toFloat());
        stopMotors();
        setMotorsForward();
    }
  }
}

String readSerialMessage() {
  String message = "";
  if (Serial.available() > 0) {                  // Check if data is available to read
    message = Serial.readStringUntil('\n');     // Read the incoming message until newline
    Serial.println(message);      
  }
  return message;                                // Return the message
}

/************
 DC Motors
*************/
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
  reverseRearRight();

  // Set PWM to 25% duty cycle (64 out of 255)
  analogWrite(rearRight, (BASE_MOTOR_SPEED)); // quarter-speed for motor A
  analogWrite(frontLeft, (BASE_MOTOR_SPEED)); // quarter-speed for motor B
  Serial.println("TURNED LEFT");
}

void turnRight() {
  reverseFrontLeft();

  // Set PWM to 25% duty cycle (64 out of 255)
  analogWrite(rearRight, (BASE_MOTOR_SPEED)); // quarter-speed for motor A
  analogWrite(frontLeft, (BASE_MOTOR_SPEED)); // quarter-speed for motor B
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
  analogWrite(rearRight, (BASE_MOTOR_SPEED));
  analogWrite(frontLeft, (BASE_MOTOR_SPEED));
}

void setMotorsForward() {
  digitalWrite(AI1, LOW);
  digitalWrite(AI2, HIGH);
  digitalWrite(BI1, LOW);
  digitalWrite(BI2, HIGH);
}

/************
 Headlights
*************/
void turnHeadlightsOn() {
  digitalWrite(HEADLIGHTS, HIGH);
}

void turnHeadlightsOff() {
  digitalWrite(HEADLIGHTS, LOW);
}

/************
 Photoresistor
*************/
void monitorStartLED() {
  int adcVal = analogRead(PIN_ANALOG_IN); //read adc
  TotalPhotoReadings += adcVal;
  NumPhotoReadings++;
  if (adcVal - (TotalPhotoReadings / NumPhotoReadings) > 10){
    START = true;
  }
  delay(50);
}

void monitorUltrasonics() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  duration = pulseIn(echoPinNorth, HIGH);
  distance = (duration*.0343)/2;
  Serial.print("Distance: ");
  Serial.println(distance);
  delay(100);
}

/************
 Servo Arm
*************/
void initializeArm() {
  servo4Forward();
  delay(100);
  servo3Upright();
  delay(100);
  servo2Upright();
  delay(100);
  servo1Center();
  delay(100);
}


void servo2Retract(){
  pos = servo2.read();
  
  for(pos=servo2.read(); pos>=0; pos--) {
    servo2.write(pos);
    delay(15);
  }
}

void servo1Center() {
  pos = servo1.read();

  if (pos < 90) {
    for (pos = servo1.read(); pos<=90; pos++){
      servo1.write(pos);
      delay(30);
    }
  } else {
    for (pos = servo1.read(); pos>=90; pos--){
      servo1.write(pos);
      delay(30);
    }
  }
}

void servo1Forward() {
  pos = servo1.read();

  for (pos = servo1.read(); pos<=180; pos++){
    servo1.write(pos);
    delay(15);
  }
}

void servo2Upright() {
  pos = servo2.read();

  if (pos < 90) {
    for (pos = servo2.read(); pos<=90; pos++){
      servo2.write(pos);
      delay(30);
    }
  } else {
    for (pos = servo2.read(); pos>=90; pos--){
      servo2.write(pos);
      delay(30);
    }
  }
}

void servo2Extend() {
  pos = servo2.read();

  for (pos = servo2.read(); pos<=145; pos++){
    servo2.write(pos);
    delay(25);
  }
}

void servo3Upright() {
  pos = servo3.read();

  if (pos < 90) {
    for (pos = servo3.read(); pos<=90; pos++){
      servo3.write(pos);
      delay(30);
    }
  } else {
    for (pos = servo3.read(); pos>=90; pos--){
      servo3.write(pos);
      delay(30);
    }
  }
}

void servo3Down() {
  pos = servo3.read();

  for (pos = servo3.read(); pos>=10; pos--){
    servo3.write(pos);
    delay(25);
  }
}

void servo4Forward() {
  pos = servo4.read();

  for (pos = servo4.read(); pos<=180; pos++){
    servo4.write(pos);
    delay(20);
  }


  // pos = servo4.read();

  // if (pos < 90) {
  //   for (pos = servo4.read(); pos<=90; pos++){
  //     servo4.write(pos);
  //     delay(30);
  //   }
  // } else {
  //   for (pos = servo4.read(); pos>=90; pos--){
  //     servo4.write(pos);
  //     delay(30);
  //   }
  // }
}

void servo4Backward() {
  pos = servo4.read();

  for (pos = servo4.read(); pos>=0; pos--){
    servo4.write(pos);
    delay(20);
  }
}

void openClaw(){
  pos=servo5.read();

  for(pos=servo5.read(); pos<=55; pos++){
    servo5.write(pos);
    delay(25);
  }

  Serial.println("Claw is opened");
}

void closeClaw(){
  pos=servo5.read();

  for(pos=servo5.read(); pos>=10; pos--){
    servo5.write(pos);
    delay(35);
  }
}

void fullyExtendArm() {
  servo2Upright();
  servo1Forward();
  servo2Extend();
}


void homeClaw() {
  servo5.write(10);

  servo1Forward();
  servo2Retract();
}

void dropBeacon() {
  fullyExtendArm();
  openClaw();
  
  delay(1000);

  servo2Upright();

  delay(500);
  closeClaw();
}

void grabMaterial() {
  fullyExtendArm();
  openClaw();

  servo3Down();
  closeClaw();

  servo4Backward();
  servo3Upright();
}


/************
 Start button(for testing)
*************/
void checkButton(){

  buttonState = digitalRead(buttonPin);

  if (buttonState == LOW && lastButtonState == HIGH) {
    Serial.println("1");
    pressCount+=500;
    delay(500);
  }

lastButtonState = buttonState;
}
