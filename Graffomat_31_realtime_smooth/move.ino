
int pServo = 1;


void fastLine(int xTargetPos, int yTargetPos) {

  xTargetPos = constrain(xTargetPos, 1, 220);
  yTargetPos = constrain(yTargetPos, 1, 200);

  stopMoving();
  int servoMin = 10;
  sprayServo.write(servoMin);                               // set servo position
  delay(400);
  pServo = 0;

  while (!moveTo(xTargetPos, yTargetPos)) {                 // move x and y motor until target is reached
    speedMulti = 1023.0 / 600;
    checkPause();                                           // check pause switch
  }

}

////////////////////////////////////////////////////////////// use this to move to a target position with servo pushed
//                                                             returns when target is reached

void line(int xTargetPos, int yTargetPos, int servo) {      // command '1'

  //Serial.println("xT:" + String(xTargetPos) + " yT:" + String(yTargetPos));

  xTargetPos = constrain(xTargetPos, 1, 220);
  yTargetPos = constrain(yTargetPos, 1, 200);
  int servoMax = analogRead(PUSH_POTI) / 12 + 15;            // check "PUSH" - Poti
  int servoMin = 10;
  if (digitalRead(TEST_BUTTON)) {
    sprayServo.write(map(servo, 1, 255, servoMin, servoMax)); // if TEST/NO-X is not pushed: set servo position
    if (servo != pServo) {
      delay(400);
    }
  } else {
    sprayServo.write(servoMin);                               // if TEST/NO-X is pushed: don't spray
  }

  pServo = servo;
  while (!moveTo(xTargetPos, yTargetPos)) {                 // move x and y motor until target is reached
    speedMulti = 1023.0 - analogRead(SPEED_POTI);           // read speed poti
    speedMulti = speedMulti / 600;
    if (digitalRead(TEST_BUTTON)) {
      sprayServo.write(map(servo, 1, 255, servoMin, servoMax)); // if TEST/NO-X is not pushed: set servo position
    } else {
      sprayServo.write(servoMin);                               // if TEST/NO-X is pushed: don't spray
    }
    checkPause();                                           // check pause switch
  }
}


////////////////////////////////////////////////////////////// proportional move to a target position.
//                                                             generates the PWM signals for the motors
//                                                             has to be called repeatedly
boolean moveTo(int xTargetPos, int yTargetPos) {


  readPositions();                              // read encoders, update xPos and yPos
  float xDelta = abs(xTargetPos - xPos);
  float yDelta = abs(yTargetPos - yPos);
  float ratio = 1;
  if (yDelta != 0) {
    ratio = xDelta / yDelta; // >1==wide <1==high
  }

  float xMulti = 1;
  float yMulti = 1;

  if (ratio > 1) {
    yMulti = 1 / ratio;
  } else {
    xMulti = ratio;
  }


  boolean xTargetPosReached = false;
  int xError = abs(xTargetPos - xPos);
  if (xError < 1) {
    xTargetPosReached = true;
    digitalWrite(XPWML, 0);
    digitalWrite(XPWMR, 0);
  } else {
    float xPWM = X_NORMAL_SPEED * speedMulti * xMulti;


    if (xTargetPos > xPos) { // ................................ move right
      digitalWrite(XPWML, 0);
      xPWM = xPWM * rightMulti;
      xPWM = ((sqrt(xPWM / X_NORMAL_SPEED) * X_NORMAL_SPEED) * 2 + xPWM) / 3; // apply curve: boost lower pwm
      analogWrite(XPWMR, int(xPWM));
    } else { // ........................................ move left
      digitalWrite(XPWMR, 0);
      xPWM = xPWM * leftMulti;
      xPWM = ((sqrt(xPWM / X_NORMAL_SPEED) * X_NORMAL_SPEED) * 2 + xPWM) / 3; // apply curve: boost lower pwm
      analogWrite(XPWML, int(xPWM));
    }
  }

  /////////////// Y ///////////////

  boolean yTargetPosReached = false;
  int yError = abs(yTargetPos - yPos);
  if (yError < 1) {
    yTargetPosReached = true;
    analogWrite(YPWMU, 0);
    analogWrite(YPWMD, 0);
  } else {
    float yPWM = Y_NORMAL_SPEED * speedMulti * yMulti;

    if (yTargetPos > yPos) { // ................................. move down
      analogWrite(YPWMU, 0);
      analogWrite(YPWMD, int(downMulti * yPWM));
    } else { // ......................................... move up
      analogWrite(YPWMD, 0); // move up
      yPWM = sqrt((upMulti * yPWM) / Y_NORMAL_SPEED) * Y_NORMAL_SPEED; // apply curve
      analogWrite(YPWMU, int(yPWM));
    }
  }
  return xTargetPosReached && yTargetPosReached;
}

void stopMoving() {
  analogWrite(YPWMD, 0);
  analogWrite(YPWMU, 0);
  analogWrite(XPWML, 0);
  analogWrite(XPWMR, 0);
}
