#include <SPI.h>
#include <MFRC522.h>

constexpr uint8_t RST_PIN = 9;     // Configurable, see typical pin layout above
constexpr uint8_t SS_PIN = 10;     // Configurable, see typical pin layout above

MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance
char c;

void setup() {
  Serial.begin(9600);        // Initialize serial communications with the PC
  SPI.begin();               // Init SPI bus
  mfrc522.PCD_Init();        // Init MFRC522 card
}

void loop()
{
  while(!Serial.available()){}
  c=Serial.read();
  switch(c)
  {
    case '1':
      Escribir();
    break;
    case '2':
      Leer();
    break;
  }
}
void Escribir()
{
  // Prepare key - all keys are set to FFFFFFFFFFFFh at chip delivery from the factory.
  MFRC522::MIFARE_Key key;
  byte buffer[34];
  byte block;
  MFRC522::StatusCode status;
  byte len;
  Serial.println("1");
  Serial.setTimeout(20000L) ;     // wait until 20 seconds for input from serial
  // Ask personal data: Family name
  len = Serial.readBytesUntil('#', (char *) buffer, 30) ; // read family name from serial
  for (byte i = len; i < 30; i++) buffer[i] = ' ';     // pad with spaces
  for (byte i = 0; i < 6; i++) key.keyByte[i] = 0xFF;
  Serial.println("1");
  // Look for new cards
  while ( ! mfrc522.PICC_IsNewCardPresent()) {
    //return;
  }

  // Select one of the cards
  while ( ! mfrc522.PICC_ReadCardSerial()) {
    //return;
  }
  for (byte i = 0; i < mfrc522.uid.size; i++) {
  }
  MFRC522::PICC_Type piccType = mfrc522.PICC_GetType(mfrc522.uid.sak);

  

  block = 1;
  //Serial.println(F("Authenticating using key A..."));
  status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, block, &key, &(mfrc522.uid));
  if (status != MFRC522::STATUS_OK) {
    return;
  }

  // Write block
  status = mfrc522.MIFARE_Write(block, buffer, 16);
  if (status != MFRC522::STATUS_OK) {
    return;
  }
  Serial.println("1");
  mfrc522.PICC_HaltA(); // Halt PICC
  mfrc522.PCD_StopCrypto1();  // Stop encryption on PCD
}
void Leer()
{
  MFRC522::MIFARE_Key key;
  for (byte i = 0; i < 6; i++) key.keyByte[i] = 0xFF;

  //some variables we need
  byte block;
  byte len;
  MFRC522::StatusCode status;

  //-------------------------------------------

  // Look for new cards
  while ( ! mfrc522.PICC_IsNewCardPresent()) {
    //return;
  }

  // Select one of the cards
  while ( ! mfrc522.PICC_ReadCardSerial()) {
    //return;
  }

  //mfrc522.PICC_DumpDetailsToSerial(&(mfrc522.uid)); //dump some details about the card

  //mfrc522.PICC_DumpToSerial(&(mfrc522.uid));      //uncomment this to see all blocks in hex
  len = 18;
  //---------------------------------------- GET LAST NAME

  byte buffer2[18];
  block = 1;

  status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, 1, &key, &(mfrc522.uid)); //line 834
  if (status != MFRC522::STATUS_OK) {
    return;
  }

  status = mfrc522.MIFARE_Read(block, buffer2, &len);
  if (status != MFRC522::STATUS_OK) {
    return;
  }
  String data="";
  //PRINT LAST NAME
  for (uint8_t i = 0; i < 16; i++) {
    data+=(char)buffer2[i];
  }
  Serial.println(data);

  delay(1000); //change value if you want to read cards faster

  mfrc522.PICC_HaltA();
  mfrc522.PCD_StopCrypto1();
}

