void checkVoltage(){
  Serial.print("  Voltage: ");
  input_voltage = 5.0 * 11 * analogRead(VOLTAGE_DIVIDER)  / 1023;
  Serial.println(input_voltage);
}
void checkXencoder(){
  Serial.print(analogRead(XENCODER1));
  Serial.print(",");
  Serial.print(analogRead(XENCODER2));
  Serial.println(",0,1023");
}
void checkYencoder(){
  Serial.print(analogRead(YENCODER1));
  Serial.print(",");
  Serial.print(analogRead(YENCODER2));
  Serial.println(",0,1023");
}
void info() {
  Serial.print("Speed poti: ");
  Serial.print(analogRead(SPEED_POTI));
  Serial.print("  Push poti: ");
  Serial.print(analogRead(PUSH_POTI));
  Serial.print("  Test: ");
  Serial.print(digitalRead(TEST_BUTTON));
  Serial.print("  Pause: ");
  Serial.print(digitalRead(PAUSE_SWITCH));
  Serial.print("  End switch top: ");
  Serial.print(digitalRead(YENDSWITCH));
  Serial.print("  End switch left: ");
  Serial.print(digitalRead(XENDSWITCH));

  Serial.print("  Xenc: ");
  Serial.print(analogRead(XENCODER1) > 200);
  Serial.print(analogRead(XENCODER2) > 200);

  Serial.print("  Yenc: ");
  Serial.print(analogRead(YENCODER1) > 300);
  Serial.print(analogRead(YENCODER2) > 300);
  Serial.println();
}
