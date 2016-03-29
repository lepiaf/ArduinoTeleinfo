#include <SoftwareSerial.h>
#include <MySensor.h>
#include <MyTransportNRF24.h>
#include <MyHwATMega328.h>
#include <SPI.h>
#include "TeleInfo.h"

#define NODE_ID 1

#define ID_PAPP 4
#define ID_IINST 5
#define ID_HCHC 6
#define ID_HCHP 7

TeleInfo* homeTeleInfo;

MyTransportNRF24 radio(RF24_CE_PIN, RF24_CS_PIN, RF24_PA_LEVEL_GW);  
MyHwATMega328 hw;
MySensor gw(radio, hw);

MyMessage msgPAPP(ID_PAPP, V_WATT);
MyMessage msgIINST(ID_IINST, V_CURRENT);
MyMessage msgHCHC(ID_HCHC, V_KWH);
MyMessage msgHCHP(ID_HCHP, V_KWH);

int count;

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

  count = 0;
}

void loop()
{
  boolean readCompleted;
  readCompleted = homeTeleInfo->readTeleInfo();

  if (readCompleted) {
    gw.send(msgIINST.set(homeTeleInfo->IINST, 0));
    delay(100);
    gw.send(msgHCHC.set((homeTeleInfo->HCHC).toInt(), 0));
    delay(100);
    gw.send(msgHCHP.set((homeTeleInfo->HCHP).toInt(), 0));
    delay(100);
    gw.send(msgPAPP.set((homeTeleInfo->PAPP).toInt(), 0));
  }

  count++;

  delay(10000);

  if (count == 60) {
    softReset();
    count = 0;
  }

}

void softReset() {
  asm volatile ("  jmp 0");
}
