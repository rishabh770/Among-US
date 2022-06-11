/*
 *  Created by TheCircuit
*/

#define SS_PIN 4  //D2
#define RST_PIN 5 //D1

#include <SPI.h>
#include <MFRC522.h>

#define c1 "53 24 7F 06"
#define c2 "D3 29 E6 05"
#define c3 "E3 7E DA 06"
#define c4 "73 8F 02 04"
#define c5 "F3 47 FA 03"
#define c6 "6A C8 B2 1A"
#define c7 "63 76 96 06" 
#define c8 "DA 11 A0 1A" //imposter
#define c9  "13 23 E6 05" //imposter
#define c10 "73 48 00 04"
char *crew[8];


MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance.
int statuss = 0;
int out = 0;
void setup() 
{
  Serial.begin(9600);   // Initiate a serial communication
  SPI.begin();      // Initiate  SPI bus
  mfrc522.PCD_Init();   // Initiate MFRC522
  pinMode(D3,OUTPUT);
  pinMode(D4,OUTPUT);
  crew[0] = c1;
  crew[1] = c2;
  crew[2] = c3;
  crew[3] = c4;
  crew[4] = c5;
  crew[5] = c6;
  crew[6] = c7;
  crew[7] = c10;
  
}
void loop() 
{
  // Look for new cards
  if ( ! mfrc522.PICC_IsNewCardPresent()) 
  {
    return;
  }
  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial()) 
  {
    return;
  }
  //Show UID on serial monitor
  Serial.println();
  Serial.print(" UID tag :");
  String content= "";
  byte letter;
  for (byte i = 0; i < mfrc522.uid.size; i++) 
  {
     Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
     Serial.print(mfrc522.uid.uidByte[i], HEX);
     content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
     content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  content.toUpperCase();
  Serial.println();
  digitalWrite(D3, LOW);
  digitalWrite(D4, LOW);
  int i;
  for(i = 0;i<8;i++)
  {
    if (content.substring(1) == crew[i]) //change UID of the card that you want to give access
    {
      Serial.println(" Access Granted ");
      Serial.print(" Welcome Mr.Crewmate-");
      Serial.println(i);
      //delay(1000);
      Serial.println(" Have FUN ");
      Serial.println();
      digitalWrite(D4, HIGH);
      statuss = 1;
    }
  }
  if(content.substring(1) == "DA 11 A0 1A" ||content.substring(1) == "13 23 E6 05" )  
  {
    Serial.println(" Welcome Mr.Imposter");
    //delay(3000);
    digitalWrite(D3q, HIGH);
  }
} 
