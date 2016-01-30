#include <SoftwareSerial.h>
#include <MySensor.h>
#include <SPI.h>
#include "TeleInfo.h"

#define ID 4

TeleInfo* homeTeleInfo;
MySensor gw;
MyMessage msg(ID, V_WATT);

void setup()
{
  homeTeleInfo = new TeleInfo();

  gw.begin(NULL, 0);
  gw.sendSketchInfo("EDF Teleinfo", "1.0");
  gw.present(ID, S_POWER);

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
    String e = homeTeleInfo->PAPP;
    int watt = e.toInt();
    gw.send(msg.set(watt, 1));
  }

  gw.sleep(1000); //sleep a bit
}
