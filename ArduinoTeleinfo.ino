#include <SoftwareSerial.h>
#include <MySensor.h>
#include <SPI.h>
#include "TeleInfo.h"

#define NODE_ID 1

#define ID_PAPP 4
#define ID_IINST 5
#define ID_HCHC 6
#define ID_HCHP 7
#define ID_OPTARIF 8

TeleInfo* homeTeleInfo;

MySensor gw;

MyMessage msgPAPP(ID_PAPP, V_WATT);
MyMessage msgIINST(ID_IINST, V_CURRENT);
MyMessage msgHCHC(ID_HCHC, V_KWH);
MyMessage msgHCHP(ID_HCHP, V_KWH);
MyMessage msgOPTARIF(ID_OPTARIF, V_VAR1);

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
char optarif[4];
void loop()
{
  char optarif[4] = "";
  boolean readCompleted;
  readCompleted = homeTeleInfo->readTeleInfo();

  if (readCompleted) {
    gw.send(msgIINST.set(homeTeleInfo->IINST, 0));
    delay(100);
    gw.send(msgHCHC.set(homeTeleInfo->HCHC, 0));
    delay(100);
    gw.send(msgHCHP.set(homeTeleInfo->HCHP, 0));
    delay(100);
    gw.send(msgPAPP.set(homeTeleInfo->PAPP, 0));
    delay(100);
    
    (homeTeleInfo->OPTARIF).toCharArray(optarif, 4);
    gw.send(msgOPTARIF.set(optarif, 0));
  }

  delay(10000);
}
