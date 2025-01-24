//PWM pins 13 and 12
#define PWMA 13
#define PWMB 12

//define 22-26
#define AI1 22
#define AI2 23
#define STBY 24
#define BI1 25
#define BI2 26

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

  if (Serial.available() > 0) {
    char command = Serial.read();
    if (command == '1') {
      digitalWrite(STBY, HIGH);
      
    // Set PWM to 50% duty cycle (128 out of 255)
    analogWrite(PWMA, 128); // Half-speed for motor A
    analogWrite(PWMB, 128); // Half-speed for motor B

    // Let the motors run for 5 seconds
    delay(5000);
    Serial.println("A!");
    }
  }


  // Stop the motors
  analogWrite(PWMA, 0);
  analogWrite(PWMB, 0);

  // Wait for 2 seconds before restarting
  delay(2000);
}
