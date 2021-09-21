
boolean receiveMessage() {         // receives message packet, returns true if package is ok
  newMessageFlag = 0;
  while (Serial.available()) {
    int inByte = Serial.read();
    if (inByte == 0) {             // start of new message
      receiveIndex = 0;
      receive[0] = 0;
    } else {
      receiveIndex++;
      if (receiveIndex < 6) {      // new byte for the message
        receive[receiveIndex] = inByte;
        if (receiveIndex == 5) {   //complete message received
          command  = receive[1];
          xPosIn   = receive[2];
          yPosIn   = receive[3];
          servoIn  = receive[4];
          chksm    = receive[5];

          boolean chksmOK = false;
          
  // weird workaround for command '4' here (direct speed control of the plotter, e.g. via webserial):
  // since values >=128 are resolved as 2 bytes by JS (WTF!?), all values which are sent stay below 128.
  // -> checksum for command '4' goes from 1...100!
  // -> checksum for all other commands goes from 1...251!
  
       /*   if (command != 4 && chksm == (((receive[0] + receive[1] + receive[2] + receive[3] + receive[4]) % 250) + 1)) {
            chksmOK = true;
          }
          if (command == 4 && chksm == (((receive[0] + receive[1] + receive[2] + receive[3] + receive[4]) % 99) + 1)) {
            chksmOK = true;
          }*/ chksmOK=true;
          
          // if chksm is correct : new message has arrived!
          if (chksmOK) {
            newMessageFlag = 1;
          } else {
            Serial.write(byte(21)); //NACK // Checksum error
          }
        }
      } else {
        Serial.write(byte(21)); //NACK // Message too long
      }
    }
  }
  return newMessageFlag;
}
