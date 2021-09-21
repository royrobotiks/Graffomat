boolean init_SD() {
  boolean returnValue=true;
  Serial.print(F("SD card init..."));
  if (!SD.begin(PIN_SD_CS)) {
    returnValue=false;
  }
  return returnValue;
}
