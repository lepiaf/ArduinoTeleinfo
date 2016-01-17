#include <SoftwareSerial.h>

#define startFrame 0x02
#define endFrame 0x03
#define startLine 0x0A
#define endLine 0x0D
#define maxFrameLen 280

SoftwareSerial* cptSerial;

char bufferTeleinfo[21] = "";
int bufferLen = 0;
int checkSum;

void setup()
{
  Serial.begin(115200);
  cptSerial = new SoftwareSerial(8, 9);
  cptSerial->begin(1200);
  Serial.println(F("setup complete"));
}

void loop()
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
          Serial.println(F("Checksum error ..."));
        }

        strncpy(&bufferTeleinfo[0], &bufferTeleinfo[1], bufferLen - 3);
        bufferTeleinfo[bufferLen - 3] =  0x00;

        Serial.println(getLabel(bufferTeleinfo));

        Serial.println(handleBuffer(bufferTeleinfo));
      } else {
        bufferLen++;
      }
    }
  }

  // après chaque fin de trame, on provoque un retour chariot
  Serial.println("");
}


String getLabel(char *bufferTeleinfo)
{
  char label[10] = "";
  int i;
  for (i = 0; i < 22; i++) {
    if (bufferTeleinfo[i] != 0x20) {
       label[i] = bufferTeleinfo[i];
    }

    break;
  }

  return String(label);
  
}

//Lit le buffer pour trouver le label complet et sa valeur
String handleBuffer(char *bufferTeleinfo)
{
  char* resultString = strchr(bufferTeleinfo, ' ') + 1;
  String label;
  char firstCharBufferTeleinfo = bufferTeleinfo[0];

  if (firstCharBufferTeleinfo == 'A') { //ADC0
    label = "Compteur numero:";
    return label + resultString;
  }

  if (firstCharBufferTeleinfo == 'O') { //OPTARIF
    label = "Option tarifaire:";
    return label + resultString;
  }

  if (firstCharBufferTeleinfo == 'I' && bufferTeleinfo[1] == 'S') { //ISOUSC
    label = "Intensité souscrite:";
    return label + resultString;
  }

  if (firstCharBufferTeleinfo == 'H' && bufferTeleinfo[3] == 'C') { //HCHC
    label = "Index option heure creuse - heure creuse:";
    return label + resultString;
  }

  if (firstCharBufferTeleinfo == 'H' && bufferTeleinfo[3] == 'P') { //HCHP
    label = "Index option heure creuse - heure pleine:";
    return label + resultString;
  }

  if (firstCharBufferTeleinfo == 'P' && bufferTeleinfo[1] == 'T') { //PTEC
    label = "Période tarifaire en cours:";
    return label + resultString;
  }

  if (firstCharBufferTeleinfo == 'I' && bufferTeleinfo[1] == 'I') { //IINST
    label = "Intensité Instantanée:";
    return label + resultString;
  }

  if (firstCharBufferTeleinfo == 'I' && bufferTeleinfo[1] == 'M') { //IMAX
    label = "Intensité maximale:";
    return label + resultString;
  }

  if (firstCharBufferTeleinfo == 'P' && bufferTeleinfo[1] == 'A') { //PAPP
    label = "PAPP:";
    return label + resultString;
  }

  if (firstCharBufferTeleinfo == 'H' && bufferTeleinfo[3] == 'H') { //HHPHC
    label = "Horaire Heures Pleines Heures Creuses:";
    return label + resultString;
  }

  label = "Valeur inconnue:";

  return label + String(bufferTeleinfo);
}

char calculateChecksum(char *buff, uint8_t len)
{
  int i;
  char sum = 0;
  for (i = 1; i < (len - 2); i++) {
    sum = sum + buff[i];
  }
  sum = (sum & 0x3F) + 0x20;

  return (sum);
}
