#include <SoftwareSerial.h>
#define startFrame 0x02
#define endFrame 0x03
#define PIN_RX 8
#define PIN_TX 9

SoftwareSerial* cptSerial;

void setup()
{
  Serial.begin(115200);
  cptSerial = new SoftwareSerial(PIN_RX, PIN_TX);
  cptSerial->begin(1200);
  Serial.println(F("setup complete"));
}

void loop()
{
  char charIn = 0;

  while (charIn != startFrame) {
    charIn = cptSerial->read() & 0x7F;
  }
  
  while (charIn != endFrame) {
    if (cptSerial->available()) {
      charIn = cptSerial->read() & 0x7F;
      Serial.print(charIn);
    }
  }
  Serial.println("");
}
