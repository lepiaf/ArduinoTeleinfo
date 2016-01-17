#include <SoftwareSerial.h>
#include "TeleInfo.h"

// uncomment for debug
#define DEBUG

TeleInfo* homeTeleInfo;

void setup()
{
  Serial.begin(115200);
  homeTeleInfo = new TeleInfo();
  
  Serial.println(F("Mysensor Node EDF TeleInfo ready"));
}

void loop()
{
  boolean readCompleted;
  readCompleted = homeTeleInfo->readTeleInfo();

  if (readCompleted) {
    Serial.println(homeTeleInfo->ADC0);
    Serial.println(homeTeleInfo->IINST);
    Serial.println(homeTeleInfo->HCHC);
    Serial.println(homeTeleInfo->HCHP);
    Serial.println(homeTeleInfo->PAPP);
  }

  delay(5000);
}
