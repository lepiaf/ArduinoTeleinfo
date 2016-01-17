
// Nous utilisons la bibliothèque Software.Serial, ce
// qui permet de configurer le port série sur d'autres
// pins que les habituelles 0 & 1 déjà utilisées par
// le "moniteur série"
#include <SoftwareSerial.h>

#define startFrame 0x02
#define endFrame 0x03
#define startLine 0x0A
#define endLine 0x0D
#define maxFrameLen 280

// On crée une instance de SoftwareSerial
SoftwareSerial* cptSerial;

int comptChar = 0; // variable de comptage des caractères reçus
char bufferTeleinfo[21] = "";
int bufferLen = 0;
int checkSum;
int sequenceNumnber = 0;   // number of information group

// Fonction d'initialisation de la carte Arduino, appelée
// 1 fois à la mise sous-tension ou après un reset
void setup()
{
  // On initialise le port utilisé par le "moniteur série".
  // Attention de régler la meme vitesse dans sa fenêtre
  Serial.begin(115200);

  // On définit les PINs utilisées par SoftwareSerial,
  // 8 en réception, 9 en émission (en fait nous ne
  // ferons pas d'émission)
  cptSerial = new SoftwareSerial(8, 9);
  // On initialise le port avec le compteur EDF à 1200 bauds :
  //  vitesse de la Télé-Information d'après la doc EDF
  cptSerial->begin(1200);
  Serial.println(F("setup complete"));
}

// Boucle principale, appelée en permanence une fois le
// setup() terminé
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
  while (charIn != endFrame)
  {
    // S'il y a des caractères disponibles on les traite
    if (cptSerial->available())
    {
      // on "zappe" le 8ème bit
      charIn = cptSerial->read() & 0x7F;
      // on affiche chaque caractère reçu dans le
      // "moniteur série"

      if (charIn == startLine) {
        bufferLen = 0;
      }

      bufferTeleinfo[bufferLen] = charIn;

      if (charIn == endLine)
      {

        checkSum = bufferTeleinfo[bufferLen - 1];
        if (calculateChecksum(bufferTeleinfo, bufferLen) != checkSum) {
          Serial.println(F("Checksum error ..."));
        }

        strncpy(&bufferTeleinfo[0], &bufferTeleinfo[1], bufferLen - 3);
        bufferTeleinfo[bufferLen - 3] =  0x00;

        Serial.println(handleBuffer(bufferTeleinfo));

      } else {
        bufferLen++;
      }
    }
  }

  // après chaque fin de trame, on provoque un retour chariot
  Serial.println("");
}

/**
   Lit le buffer pour trouver le label complet et sa valeur
*/
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
    label = "HCHC:";
    return label + resultString;
  }

  if (firstCharBufferTeleinfo == 'H' && bufferTeleinfo[3] == 'P') { //HCHP
    label = "HCHP:";
    return label + resultString;
  }

  if (firstCharBufferTeleinfo == 'P' && bufferTeleinfo[1] == 'T') { //PTEC
    label = "PTEC:";
    return label + resultString;
  }

  if (firstCharBufferTeleinfo == 'I' && bufferTeleinfo[1] == 'I') { //IINST
    label = "IINST:";
    return label + resultString;
  }

  if (firstCharBufferTeleinfo == 'I' && bufferTeleinfo[1] == 'M') { //IMAX
    label = "IMAX:";
    return label + resultString;
  }

  if (firstCharBufferTeleinfo == 'P' && bufferTeleinfo[1] == 'A') { //PAPP
    label = "PAPP:";
    return label + resultString;
  }

  if (firstCharBufferTeleinfo == 'H' && bufferTeleinfo[3] == 'H') { //HHPHC
    label = "HHPHC:";
    return label + resultString;
  }

  label = "Valeur inconnue:";

  return label + resultString;
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
