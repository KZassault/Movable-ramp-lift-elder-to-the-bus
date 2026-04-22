#include <Servo.h>
char serialData;
Servo rampRight, rampLeft, busRight, busLeft;
const int STOP_SPEED = 92;
const int CCW_SPEED = 97;
const int CW_SPEED = 87;
const float TARGET_CIRCLES = 8.0;
const float RPM = 90.0; //RPM = TARGETCIRCLES / MEASURED TIME * 60


void setup() {
  
  Serial.begin(9600);

  rampRight.attach(9);
  rampLeft.attach(10);
  busRight.attach(5);
  busLeft.attach(6);

  stopServoB(rampRight);
  stopServoB(rampLeft);

  busRight.write(60);
  busLeft.write(60);

}

void loop() {

  //read command
  if (Serial.available() > 0) {
    serialData = Serial.read();
    //operate command
    switch (serialData) {

    //ramp going up
      case 'u':
      rotateCircles(rampRight, rampLeft, CCW_SPEED, TARGET_CIRCLES, RPM);
      break;

      //ramp going down
      case 'd':
      rotateCircles(rampRight, rampLeft, CW_SPEED, TARGET_CIRCLES, RPM);
      break;
    
      //track rotating up
      case '1':
      busRight.write(160);
      busLeft.write(160);
      Serial.println("Rail up!");
      delay(2000);
      
      break;

      //track rotating down
      case '0':     
      busRight.write(60);
      busLeft.write(60);
      Serial.println("Rail down!");
      delay(2000);    
      break;

      //nothing if no command
      default:
      break;

    }
  }
}

void rotateCircles(Servo &servoName1, Servo &servoName2, int speedSignal, float circles, float rpm) {
  //check status
  if (rpm <= 0 || speedSignal == 92) {
    Serial.println("Error, rpm = 0 or no signal.");
    return;
  }
  
  //calculate duration time in ms
  unsigned long durationMs = (unsigned long)((circles / rpm) * 60000.0);

  //report rotating information
  Serial.print("Ramp Direction: ");
  if (speedSignal > 92) {
    Serial.println("up");
  }
  else {
    Serial.println("down");
  }
  Serial.print("Speed Signal: ");
  Serial.println(speedSignal);
  Serial.print("Estimated RPM: ");
  Serial.println(rpm);
  Serial.print("Target Circles: ");
  Serial.println(circles);
  Serial.print("Estimated delay: ");
  Serial.println(durationMs);

  //action process
  servoName1.write(speedSignal);
  servoName2.write(speedSignal);
  delay(durationMs);
  stopServoB(servoName1);
  stopServoB(servoName2);
  Serial.println("Rotation accomplished!");
  Serial.print("\n");

}

void stopServoB(Servo servopos) {
  servopos.write(STOP_SPEED);
}