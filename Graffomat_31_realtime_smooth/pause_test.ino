
void checkPause() {
  if (digitalRead(PAUSE_SWITCH)) {              // if pause switch

    int xBeforeBreak = xPos;
    int yBeforeBreak = yPos;

    sprayServo.write(10);                       // stop spraying
    analogWrite(XPWML, 0);                      // stop motors
    analogWrite(XPWMR, 0);
    analogWrite(YPWMU, 0);
    analogWrite(YPWMD, 0);
    delay(500);
    tone(PIEZO, 3000, 100);
    delay(250);
    while (!moveTo(1, 200)) {                  // move to bottom left without spraying
      readPositions();    
      speedMulti = 1023.0 / 800;
    }     
    delay(250);
    tone(PIEZO, 5000, 100);

    while (digitalRead(PAUSE_SWITCH)) {         // while pause is switched:
      if (!digitalRead(TEST_BUTTON)) {          // test servo
        int servoPos = analogRead(PUSH_POTI) / 12 + 15;
        sprayServo.write(servoPos);
      } else {
        sprayServo.write(10);
      }
    }
    delay(100);
    tone(PIEZO, 3000, 100);
    delay(250);
    while (!moveTo(xBeforeBreak, yBeforeBreak)) {// move back to position where the break started
      readPositions();     
      speedMulti = 1023.0 / 800;
    }      
    delay(500);
    tone(PIEZO, 5000, 100);
    delay(500);
  }
}
