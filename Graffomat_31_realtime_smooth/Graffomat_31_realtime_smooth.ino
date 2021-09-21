
/*

  Graffomat motor driver - reads serial commands, moves spray can and pushes the nozzle
  =========================================================================================
  Hardware:

  - The motors for both axis are cheap 18V cordless drill motors, each driven by a
  BTS7960B 43A High-power DC Motor Driver
  - Nozzle is activated by a hobby servo (a good one with metal gears)
  - Power comes from a 12V motorbike battery (lead acid)
  - 5V power is supplied by a XL4016 Step Down Converter (DC-DC 5-40V -> 1,2-35V)
  - optical position encoders are DIY (BPW42 & LED // schematic: https://www.niklasroy.com/diary/3/Niklas_Roy_Diary_391.jpg )

  Protocol:

  BYTE 0: Start   (0)      /  start signal
  BYTE 1: Command (1-255)
  BYTE 2: X-POS   (1-200)  /  X-SPEED (10...60...110)
  BYTE 3: Y-POS   (1-220)  /  Y-SPEED (10...60...110)
  BYTE 4: SERVO   (1-255)  /  if command == 4: 1-100
  BYTE 5: CHKSM   (B0+B1+B2+B3+B4)%250+1 // CHKSM for command 4: (B0+B1+B2+B3+B4)%99+1 (workaround - as JS resolves int->char for values >= 128 as 2 bytes)

  Commands:

  1: move to coordinates, while pushing the servo to position indicated by byte 4
  2: move with max speed to coordinates, once arrived push the servo to max for as long as indicated in byte 4
  3: calibrate
  4: realtime movement - move with speed indicated by byte 2&3, while pushing the servo as indicated in byte 4



  Programmed with Arduino IDE 1.8.13

  ----------------------------------------------------------------------------
  "THE TSCHUNK-WARE LICENSE" (Revision 23):
  < nikl@s - roy.de > wrote this file. As long as you retain this notice you
  can do whatever you want with this stuff. If we meet some day, and you think
  this stuff is worth it, you can buy me a tschunk in return. Niklas
  ----------------------------------------------------------------------------
*/



#define X_NORMAL_SPEED 75
#define Y_NORMAL_SPEED 150

#define XPWML 5
#define XPWMR 3
#define YPWMU 11
#define YPWMD 6

#define XENCODER1 A7
#define XENCODER2 A6
#define XENDSWITCH 4
#define YENCODER1 A5
#define YENCODER2 A4
#define YENDSWITCH 8
#define PIEZO 10
#define SPEED_POTI A3
#define PUSH_POTI A2
#define TEST_BUTTON 7
#define PAUSE_SWITCH A1
#define VOLTAGE_DIVIDER A0
#define SERVO 2

#include <Servo.h>

Servo sprayServo;

float input_voltage;
float speedMulti;         // general speed multiplier as read from poti
//                           pwm fine tuning multipliers
float rightMulti = .97;
float downMulti = .93;
float leftMulti = 1.0;
float upMulti   = 1.1;

int xPos = 0;
int yPos = 0;

int xSpeedSmooth = 0;
int ySpeedSmooth = 0;

byte    receive[6];
byte    receiveIndex = 0;
boolean newMessageFlag = 0;
byte    command, xPosIn, yPosIn, servoIn, pServoIn;
int     chksm;




///////////////////////////////////////////////////////////// SETUP
void setup() {
  pinMode( XPWML           , OUTPUT);
  pinMode( XPWMR           , OUTPUT);
  pinMode( XENCODER1       , INPUT);
  pinMode( XENCODER2       , INPUT);
  pinMode( XENDSWITCH      , INPUT_PULLUP);
  pinMode( YPWMU           , OUTPUT);
  pinMode( YPWMD           , OUTPUT);
  pinMode( YENCODER1       , INPUT);
  pinMode( YENCODER2       , INPUT);
  pinMode( YENDSWITCH      , INPUT_PULLUP);
  pinMode( PIEZO           , OUTPUT);
  pinMode( SPEED_POTI      , INPUT);
  pinMode( PUSH_POTI       , INPUT);
  pinMode( TEST_BUTTON     , INPUT_PULLUP);
  pinMode( PAUSE_SWITCH    , INPUT_PULLUP);
  pinMode( VOLTAGE_DIVIDER , INPUT);
  pinMode( SERVO           , OUTPUT);
  analogWrite(XPWML, 0);
  analogWrite(XPWMR, 0);
  analogWrite(YPWMU, 0);
  analogWrite(YPWMD, 0);
  Serial.begin(38400);

  // Serial.println("GRAFFOMAT");
  tone(PIEZO, 3000, 100);

  calibPosition();

  sprayServo.attach(SERVO);
  sprayServo.write(10);

  checkVoltage();
  pServoIn = 1;

}

///////////////////////////////////////////////////////////// LOOP
void loop() {
  if (command != 4) {
    sprayServo.write(10); // reset servo position except if in realtime control mode
  }
  while (!receiveMessage()) {                  // wait for new message
    checkPause();                              // check if pause switch is switched
    readPositions();                           // read encoders, update xPos and yPos
  }
  Serial.write(byte(6));                       // valid message recieved: respond with ACK

  /*command 1: /shaded vector/
     move to target position with poti controlled speed while servo is in 'servo' position
  */
  if (command == 1) {
    line(xPosIn, yPosIn, servoIn);
  }
  /*command 2: /pointilism/
     move to target position with maximum speed without spraying
     when arrived at target spray for 'servo'*10ms*speed multiplier poti
  */
  if (command == 2) {
    fastLine(xPosIn, yPosIn);
    if (servoIn > 1 && digitalRead(TEST_BUTTON)) {
      float sprayDurationMulti = (1023.0 - analogRead(SPEED_POTI)) / 600;         // read speed poti
      int servoMax = analogRead(PUSH_POTI) / 10 + 15;            // check "PUSH" - Poti
      sprayServo.write(servoMax); // set servo position
      delay(int(sprayDurationMulti * servoIn * 40));
      sprayServo.write(10);
      delay(300);
    }
  }
  /*command 3: calibrate position*/
  if (command == 3) {
    calibPosition();
  }

  /*command 4: realtime movement - move with speed indicated by byte 2&3, while pushing the servo as indicated in byte 4 */
  if (command == 4) {

    //got this via serial -> (xPosIn, yPosIn, servoIn)

    readPositions();                                          // read encoders, update xPos and yPos

    speedMulti = 1023.0 - analogRead(SPEED_POTI);             // read speed poti
    speedMulti = speedMulti / 500;
    int servoMax = analogRead(PUSH_POTI) / 12 + 15;            // check "PUSH" - Poti
    int servoMin = 10;
    sprayServo.write(map(servoIn, 1, 100, servoMin, servoMax)); // set servo position

    readPositions();                     // read encoders, update xPos and yPos

    /////////////////////////////////////// X-SPEED
    int xSpeed = xPosIn - 60; //right
    xSpeedSmooth=(xSpeedSmooth*5+xSpeed)/6;

    float xPWM = X_NORMAL_SPEED * speedMulti * .9 * abs(xSpeedSmooth) / 60;

    if (xPos < 215 && xSpeedSmooth > 0) { // ........................................ move right
      digitalWrite(XPWML, 0);
      xPWM = xPWM * rightMulti;
      analogWrite(XPWMR, int(xPWM));
    } else if (xPos > 5 && xSpeedSmooth < 0) { //   ........................................ move left
      digitalWrite(XPWMR, 0);
      xPWM = xPWM * leftMulti;
      analogWrite(XPWML, int(xPWM));
    } else { //        ........................................ stop left / right
      analogWrite(XPWML, 0);
      analogWrite(XPWMR, 0);
      xSpeedSmooth = 0;
    }

    readPositions();                     // read encoders, update xPos and yPos

    /////////////////////////////////////// Y-SPEED
    int ySpeed = yPosIn - 60;
    ySpeedSmooth = (ySpeedSmooth * 5 + ySpeed) / 6;

    float yPWM = Y_NORMAL_SPEED * speedMulti * abs(ySpeedSmooth) / 60;

    if (yPos < 195 && ySpeedSmooth > 0) { //  ........................................ move down
      digitalWrite(YPWMU, 0);
      yPWM = yPWM * downMulti;
      analogWrite(YPWMD, int(yPWM));
    } else if (yPos > 5 && ySpeedSmooth < 0) {  //   ........................................ move up
      digitalWrite(YPWMD, 0);
      yPWM = yPWM * upMulti;
      analogWrite(YPWMU, int(yPWM));
    } else { //        ........................................ stop up/down
      analogWrite(YPWMU, 0);
      analogWrite(YPWMD, 0);
      ySpeedSmooth = 0;
    }
  }
}

void calibPosition() {
  float     speedMulti = 1023.0 / 800;
  // ..................................... calibrate left

  //Serial.print("Calib X - ");
  while (digitalRead(XENDSWITCH)) {
    analogWrite(XPWML, X_NORMAL_SPEED * speedMulti);
  }
  analogWrite(XPWML, 0);
  //Serial.println("OK");
  delay(500);
  tone(PIEZO, 3000, 100);

  // ..................................... calibrate up
  //Serial.print("Calib Y - ");
  while (digitalRead(YENDSWITCH)) {
    analogWrite(YPWMU, Y_NORMAL_SPEED * speedMulti);
  }
  analogWrite(YPWMU, 0);
  //Serial.println("OK");
  delay(500);
  tone(PIEZO, 5000, 100);
  //Serial.println("Calib complete!");}

  xPos = 0;
  yPos = 0;
}
