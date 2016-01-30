#include <SoftwareSerial.h>
#include <MySensor.h>
#include <SPI.h>
#include "TeleInfo.h"

#define NODE_ID 1

#define ID_PAPP 4
#define ID_IINST 5
#define ID_HCHC 6
#define ID_HCHP 7

TeleInfo* homeTeleInfo;

MySensor gw;
MyMessage msgPAPP(ID_PAPP, V_WATT);
MyMessage msgIINST(ID_IINST, V_CURRENT);
MyMessage msgHCHC(ID_HCHC, V_KWH);
MyMessage msgHCHP(ID_HCHP, V_KWH);

void setup()
{
  homeTeleInfo = new TeleInfo();

  gw.begin(NULL, NODE_ID);
  gw.sendSketchInfo("teleinfo", "1.0");

  gw.present(ID_PAPP, S_POWER);
  gw.present(ID_IINST, S_POWER);
  gw.present(ID_HCHC, S_POWER);
  gw.present(ID_HCHP, S_POWER);

  Serial.println(F("Mysensor Node EDF TeleInfo ready"));
}

void loop()
{
  boolean readCompleted;
  readCompleted = homeTeleInfo->readTeleInfo();

  if (readCompleted) {
    gw.send(msgIINST.set((homeTeleInfo->IINST).toInt(), 1));
    gw.send(msgHCHC.set((homeTeleInfo->HCHC).toInt(), 1));
    gw.send(msgHCHP.set((homeTeleInfo->HCHP).toInt(), 1));
    gw.send(msgPAPP.set((homeTeleInfo->PAPP).toInt(), 1));
  }

  gw.sleep(10000);
}
