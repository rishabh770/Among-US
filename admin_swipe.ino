/*
 *  Created by TheCircuit
*/

#define SS_PIN 4  //D2
#define RST_PIN 5 //D1
#define time_x 10000
#include <SPI.h>
#include <MFRC522.h>
int x;
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
  pinMode(D8,OUTPUT);
  int check_uid();
  int check_time(unsigned long st_time);
}
void loop() 
{
  
  int var = check_uid();
  //int var = 1;
  digitalWrite(D8,LOW);
  digitalWrite(D3,LOW);
  digitalWrite(D4,LOW);
  
  if(var == 1)
  {
    digitalWrite(D8,HIGH);
    unsigned long time_t = millis();
    int output = check_time(time_t);
    if (output == 1)
      {
        digitalWrite(D4,HIGH);  
        digitalWrite(D8,LOW);
        Serial.println("Task Completed!!");
      }
    else 
      {
        digitalWrite(D3,HIGH);
        digitalWrite(D8,LOW);
        Serial.println("Oops!!");
      }
      delay(5000);
   }
  
  //Serial.print("Value of the card is :");
  //Serial.println(var);
 
} 

int check_uid(){
   // Look for new cards
  if ( ! mfrc522.PICC_IsNewCardPresent()) 
  {
    //Serial.println("Fuck");
    return 0;
  }
   //Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial()) 
  {
    //Serial.println("Fuck");
    return 0;
  }
  //Show UID on serial monitor
  //Serial.println();
  //Serial.print(" UID tag :");
  String content= "";
  byte letter;
  for (byte i = 0; i < mfrc522.uid.size; i++) 
  {
     //Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
     //Serial.print(mfrc522.uid.uidByte[i], HEX);
     content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
     content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  content.toUpperCase();
  //Serial.println();
  digitalWrite(D3, LOW);
  digitalWrite(D4, LOW);
  if (content.substring(1) == "6A C8 B2 1A") //change UID of the card that you want to give access
  {
//    Serial.println(" Access Granted ");
//    Serial.println(" Welcome Mr.Circuit ");
//    //delay(1000);
//    Serial.println(" Have FUN ");
//    Serial.println(); //star
    return 1;
    digitalWrite(D3, HIGH);
    statuss = 1;
  }
  
  else   {
    //Serial.println(" Access Denied ");
    //delay(3000);
    return 0;
    digitalWrite(D4, HIGH);
  }
}
int check_time(unsigned long st_time)
{ 
  unsigned long now = millis();
  //int x;
  x = check_uid();
  x = check_uid();
  Serial.print("x = ");
  Serial.println(x);
  while(x == 1){
     now = millis();
     x = check_uid();
     x = check_uid();
     Serial.print("x = ");
     Serial.println(x);
     Serial.println(now);
     
  }
  Serial.print("Start time: ");
  Serial.println(st_time);
  Serial.print("End time: ");
  Serial.println(now);
  Serial.println(now-st_time);
  
  if(now-st_time < time_x+500 && now-st_time > time_x-500)
    return 1;
  else 
    return 0;
}
