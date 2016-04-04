#include "Arduino.h"
#include <SoftwareSerial.h>

class TeleInfo
{
  public:
    TeleInfo();
    boolean readTeleInfo();

    String ADC0;
    String HHPHC;
    int ISOUSC;             // intensité souscrite
    int IINST;              // intensité instantanée en A
    int IMAX;               // intensité maxi en A
    int PAPP;               // puissance apparente en VA
    unsigned long HCHC;  // compteur Heures Creuses en W
    unsigned long HCHP;  // compteur Heures Pleines en W
    String PTEC;            // Régime actuel : HPJB, HCJB, HPJW, HCJW, HPJR, HCJR
    String OPTARIF;         // option tarifaire

  private:
    char calculateChecksum(char *buff, uint8_t len);
    boolean handleBuffer(char *bufferTeleinfo);

    SoftwareSerial* cptSerial;
};

