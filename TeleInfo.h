#include "Arduino.h"
#include <SoftwareSerial.h>

class TeleInfo
{
  public:
    TeleInfo();
    boolean readTeleInfo();

    String ADC0;
    String HHPHC;
    String ISOUSC;             // intensité souscrite
    String IINST;              // intensité instantanée en A
    String IMAX;               // intensité maxi en A
    String PAPP;               // puissance apparente en VA
    String HCHC;  // compteur Heures Creuses en W
    String HCHP;  // compteur Heures Pleines en W
    String PTEC;            // Régime actuel : HPJB, HCJB, HPJW, HCJW, HPJR, HCJR

    String OPTARIF;         // option tarifaire

  private:
    char calculateChecksum(char *buff, uint8_t len);
    boolean handleBuffer(char *bufferTeleinfo);
    String getLabel(char *bufferTeleinfo);

    SoftwareSerial* cptSerial;

};

