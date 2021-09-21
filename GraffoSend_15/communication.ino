

void  sendFile(String fileName) {
  tft.fill(255, 0, 0);
  tft.stroke(255, 0, 0);
  tft.rect(0, 116, 113, 11);
  tft.setCursor(7, 118);
  tft.stroke(0, 0, 0);
  tft.print("PLOTTING");

  Serial.println("..........");
  Serial.println("Sending " + fileName);
  File myFile = SD.open(fileName);

  if (myFile) {
    // read each Byte from the file until there's nothing else in it:
    int  value  = 0;
    byte pCommand = 0;
    byte command  = 0;
    byte xPos  = 0;
    byte yPos  = 0;
    byte pxPos = 0;
    byte pyPos = 0;
    byte servo = 0;
    byte index = 0;
    int lineCount = 0;

    while (myFile.available()) {    // parse file

      byte inByte = myFile.read(); // read one character
      if (inByte >= 48 && inByte <= 57) { // if ASCII is a decimal digit
        value = value * 10 + inByte - 48; // multiply exisiting value by 10 and add new value obtained from ASCII
      }
      if (inByte == 44) {// if a comma "," is detected, put the value in the according variable
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
      if (inByte == 13) { /*NEW LINE*/
        servo = byte(value);
        index = 0;
        value = 0;
        lineCount++;
        command = (constrain(command, 1, 220));
        xPos    = (constrain(xPos, 1, 220));
        yPos    = (constrain(yPos, 1, 200));
        servo   = (constrain(servo, 1, 255));
        sendMessage(byte(command), byte(xPos), byte(yPos), byte(servo)); // send: command, xpos, ypos, servo
        if (lineCount > 1) { // draw red lines where plotter is moving
          tft.stroke(servo/2+127, 0, 0);
          if (command == 1) {
            tft.line(pxPos / 2 + 1, pyPos / 2 + 1, xPos / 2 + 1, yPos / 2 + 1);
          }
          if (command == 2) {
            tft.line(xPos / 2 + 1, yPos / 2 + 1, xPos / 2 + 2, yPos / 2 + 1);
            tft.line(xPos / 2 + 1, yPos / 2 + 2, xPos / 2 + 2, yPos / 2 + 2);
          }
        }
      }
    }
    myFile.close();
    sendMessage(1, byte(xPos), byte(yPos), 1); // send: command, xpos, ypos, servo
    delay(1000);
    sendMessage(2, 1, 200, 1); // send: command, xpos, ypos, servo

    tft.fill(0, 0, 0);
    tft.stroke(0, 0, 0);
    tft.rect(0, 116, 160, 11);

    tft.fill(255, 255, 0);
    tft.stroke(255, 255, 0);
    tft.rect(0, 116, 37, 11);

    tft.stroke(0, 0, 0);
    tft.setCursor(7, 118);
    tft.print("NEXT");

    tft.fill(0, 255, 0);
    tft.stroke(0, 255, 0);
    tft.rect(123, 116, 37, 11);
    tft.setCursor(131, 118);
    tft.stroke(0, 0, 0);
    tft.print("PLOT");


  } else {
    Serial.println("error opening file");
    tft.background(0, 0, 0);
    tft.setTextSize(1);
    tft.setCursor(1, 106);
    tft.print("ERROR OPENING FILE");
  }
}

///////////////////////////////////////////////////////////


void sendMessage(  byte command, byte xPos, byte yPos, byte servo) { // send message until ACK is responded
  boolean ack = false;
  while (!ack) {
    int  ichksm = ((command + xPos + yPos + servo) % 250) + 1; //calculate checksum
    byte chksm = byte(ichksm);
    //send message
    mySerial.write(byte(0));
    mySerial.write(command);
    mySerial.write(xPos);
    mySerial.write(yPos);
    mySerial.write(servo);
    mySerial.write(chksm);
    Serial.print("msg:'" + String(command) + "-" + String(xPos) + "-" + String(yPos) + "-" + String(servo) + "' sent..");
    while (!mySerial.available()) {
    }//wait until byte is available
    int inByte = mySerial.read();
    if (inByte == 6) {
      ack = true;
      Serial.println("ok");
      while (mySerial.available()) {
        int inByte = mySerial.read(); //flush buffer
      }
    }
  }
}


/////////////////////////////////////////////////////////////////////////////////////////////
