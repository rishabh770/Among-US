#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <SPI.h>
#include <MFRC522.h>

#define RST_PIN 5 //D1
#define SS_PIN 4  //D2
const char* ssid = "Rishabh C9pro";
const char* password = "rishabh206";
const char* mqtt_server = "broker.emqx.io";  // Local IP address of Raspberry Pi

const char* username = "";
const char* pass = "";


char str_hum_data[10];
char str_temp_data[10];



MFRC522 mfrc522(SS_PIN, RST_PIN); 
WiFiClient espClient;
PubSubClient client(espClient);
unsigned long lastMsg = 0;
#define MSG_BUFFER_SIZE  (50)
char msg[MSG_BUFFER_SIZE];
int value = 0;

void setup_wifi() {

  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  randomSeed(micros());

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");

  
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str(), username, pass) ) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      // ... and resubscribe
      
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}


int check_uid(){
   // Look for new cards
  if ( ! mfrc522.PICC_IsNewCardPresent()) 
  {
    return 0;
  }
   //Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial()) 
  {
    return 0;
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
 
  if (content.substring(1) == "6A C8 B2 1A") //change UID of the card that you want to give access
  {
    Serial.println(" Access");
    char player_id[2] = "2"; 
    client.publish("task1",player_id);
    Serial.println("grtd");
    return 1;
    
  }
  else if(content.substring(1) == "63 76 96 06")
    {
    Serial.println(" Access");
    char player_id[2] = "1"; 
    client.publish("task1",player_id);
    Serial.println("grtd");
    return 1;
    
  }
  else   {
    Serial.println(" Access Denied ");
    //delay(3000);
    return 0;    
  }
}

void setup() {
 
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
  SPI.begin();      // Initiate  SPI bus
  mfrc522.PCD_Init();   // Initiate MFRC522
}

void loop() {

  if (!client.connected()) {
    reconnect();
  }
  client.loop();  
  delay(2000);
  int x;
  x = check_uid();
  Serial.println(x);
  
}
