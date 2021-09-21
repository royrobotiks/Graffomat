void drawFile(String fileName) { // read the opened file and draw it
  tft.background(0, 0, 0);

  tft.fill(0, 0, 0);
  tft.stroke(255, 255, 255);
  tft.rect(0, 0, 113, 103);

  tft.setTextSize(1);
  tft.setCursor(1, 106);
  tft.print("FILE: " + fileName);

  tft.fill(255, 255, 0);
  tft.stroke(255, 255, 0);
  tft.rect(0, 116, 37, 11);

  tft.stroke(0, 0, 0);
  tft.setCursor(7, 118);
  tft.print("NEXT");

  Serial.println("..........");
  Serial.println("Opening " + fileName);
  File myFile = SD.open(fileName);

  if (myFile) {
    // read each Byte from the file until there's nothing else in it:
    int value     = 0;
    byte pCommand = 0;
    byte command  = 0;
    byte xPos     = 0;
    byte yPos     = 0;
    byte pxPos    = 0;
    byte pyPos    = 0;
    byte servo    = 0;
    byte index    = 0;
    int lineCount = 0;
    boolean moveToNext = false;
    while (myFile.available() && !moveToNext) { // parse file
      if (!digitalRead(BUTTON_Y)) { // yellow button has been pushed: move to next file
        moveToNext = true;
      }

      byte inByte = myFile.read(); // read one character
      if (inByte >= 48 && inByte <= 57) {
        value = value * 10 + inByte - 48;
      }
      if (inByte == 44) {/*,*/
        if (index == 0) {
          pCommand = command;
          command = byte(value);
        }
        if (index == 1) {
          pxPos = xPos;
          xPos = byte(value);
        }
        if (index == 2) {
          pyPos = yPos;
          yPos = byte(value);
        }
        index++;
        value = 0;
      }
      if (inByte == 13) {
        /*NEW LINE*/
        servo = byte(value);

        command = (constrain(command, 1, 220));
        xPos = (constrain(xPos, 1, 220));
        yPos = (constrain(yPos, 1, 200));
        servo = (constrain(servo, 1, 255));

        index = 0;
        value = 0;
        lineCount++;
        if (lineCount > 1) {
          if (command == 1) { // draw line
            tft.stroke(servo, servo, servo);
            if (servo > 1) {
              tft.line(pxPos / 2 + 1, pyPos / 2 + 1, xPos / 2 + 1, yPos / 2 + 1);
            }
          }
          if (command == 2) { // draw point
            tft.stroke(servo, servo, servo);
            if (servo > 1) {
              tft.line(xPos / 2 + 1, yPos / 2 + 1,xPos / 2 + 2, yPos / 2 + 1);
              tft.line(xPos / 2 + 1, yPos / 2 + 2,xPos / 2 + 2, yPos / 2 + 2);
            }
          }
        }
        /*  Serial.print(xPos);
          Serial.print("-");
          Serial.print(yPos);
          Serial.print("-");
          Serial.print(servo);
          Serial.println();*/
      }
    }
    myFile.close();
    if (!moveToNext) {
      tft.fill(0, 255, 0);
      tft.stroke(0, 255, 0);
      tft.rect(76, 116, 37, 11);
      tft.setCursor(84, 118);
      tft.stroke(0, 0, 0);
      tft.print("PLOT");
    }
  } else {
    Serial.println("error opening file");
    tft.background(0, 0, 0);
    tft.setTextSize(1);
    tft.setCursor(1, 106);
    tft.print("ERROR OPENING FILE");
  }
}
