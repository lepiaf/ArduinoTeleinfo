#include <SoftwareSerial.h>
#include "TeleInfo.h"

#define startFrame 0x02
#define endFrame 0x03
#define startLine 0x0A
#define endLine 0x0D

char bufferTeleinfo[21] = "";
int bufferLen = 0;
int checkSum;

TeleInfo::TeleInfo()
{
  // utilisation d'une entrée série logiciel : Arduino UNO
  // Dans le cas d'un Arduino MEGA, il est possible d'utiliser Serial1, Serial2
  cptSerial = new SoftwareSerial(8, 7);
  cptSerial->begin(1200);
}

// lecture de la teleinfo, quand c'est fini, ça retourne true
boolean TeleInfo::readTeleInfo()
{
  // Variable de stockage des caractères reçus
  char charIn = 0;
  // Boucle d'attente du caractère de début de trame
  while (charIn != startFrame)
  {
    // on "zappe" le 8ème bit, car d'après la doc EDF
    // la tramission se fait en 7 bits
    charIn = cptSerial->read() & 0x7F;
  }

  // Boucle d'attente d'affichage des caractères reçus,
  // jusqu'à réception du caractère de fin de trame
  while (charIn != endFrame) {
    // S'il y a des caractères disponibles on les traite
    if (cptSerial->available()) {
      // on "zappe" le 8ème bit
      charIn = cptSerial->read() & 0x7F;

      // début de la ligne, remise à zéro du bufferLen
      if (charIn == startLine) {
        bufferLen = 0;
      }

      // populate bufferTeleinfo
      bufferTeleinfo[bufferLen] = charIn;
      // fin de ligne teleinfo, début de la lecture de la ligne complète
      if (charIn == endLine) {
        // obtenir le dernier caratère du tampon qui contient le checksum
        checkSum = bufferTeleinfo[bufferLen - 1];
        if (calculateChecksum(bufferTeleinfo, bufferLen) != checkSum) {
#ifdef DEBUG
          Serial.println(F("Checksum error ..."));
#endif
          return false;
        }

        strncpy(&bufferTeleinfo[0], &bufferTeleinfo[1], bufferLen - 3);
        bufferTeleinfo[bufferLen - 3] =  0x00;
        handleBuffer(bufferTeleinfo);

      } else {
        bufferLen++;
      }
    }
  }

  return true;

}

char TeleInfo::calculateChecksum(char *buff, uint8_t len)
{
  int i;
  char sum = 0;
  for (i = 1; i < (len - 2); i++) {
    sum = sum + buff[i];
  }
  sum = (sum & 0x3F) + 0x20;

  return (sum);
}


//Lit le buffer pour trouver le label complet et sa valeur
boolean TeleInfo::handleBuffer(char *bufferTeleinfo)
{
  char* resultString = strchr(bufferTeleinfo, ' ') + 1;
  String label;
  char firstCharBufferTeleinfo = bufferTeleinfo[0];

  if (getLabel(bufferTeleinfo) == "ADCO") { //ADC0
    ADC0 = String(resultString);

    return true;
  }

  if (getLabel(bufferTeleinfo) == "OPTARIF") { //OPTARIF
    OPTARIF = String(resultString);
    return true;
  }

  if (firstCharBufferTeleinfo == 'I' && bufferTeleinfo[1] == 'S') { //ISOUSC
    ISOUSC = String(resultString);
    return true;
  }

  if (firstCharBufferTeleinfo == 'H' && bufferTeleinfo[3] == 'C') { //HCHC
    HCHC = String(resultString);
    return true;
  }

  if (firstCharBufferTeleinfo == 'H' && bufferTeleinfo[3] == 'P') { //HCHP
    HCHP = String(resultString);
    return true;
  }

  if (firstCharBufferTeleinfo == 'P' && bufferTeleinfo[1] == 'T') { //PTEC
    PTEC = String(resultString);
    return true;
  }

  if (firstCharBufferTeleinfo == 'I' && bufferTeleinfo[1] == 'I') { //IINST
    IINST = String(resultString);
    return true;
  }

  if (firstCharBufferTeleinfo == 'I' && bufferTeleinfo[1] == 'M') { //IMAX
    IMAX = String(resultString);
    return true;
  }

  if (firstCharBufferTeleinfo == 'P' && bufferTeleinfo[1] == 'A') { //PAPP
    PAPP = String(resultString);
    return true;
  }

  if (firstCharBufferTeleinfo == 'H' && bufferTeleinfo[3] == 'H') { //HHPHC
    HHPHC = String(resultString);
    return true;
  }

  //  label = "Valeur inconnue:";
  //  return label + String(bufferTeleinfo);

  return false;
}

String TeleInfo::getLabel(char *bufferTeleinfo)
{
  char label[10] = "";
  int i;
  for (i = 0; i < 22; i++) {
    if (bufferTeleinfo[i] == 0x20) {
      break;
    }
    label[i] = bufferTeleinfo[i];
  }

  return String(label);
}