//----------------------------------------Incluir a biblioteca NodeMCU ESP8266
#include <ESP8266WiFi.h>
//----------------------------------------Inclua a Biblioteca ThingSpeak, Baixe aqui: https://github.com/mathworks/thingspeak-arduino
#include "ThingSpeak.h"

#define ON_Board_LED D4  //  Definindo um LED On Board, usado para indicadores quando o processo de conexão a um roteador wifi
#define LIGHT D1 //  Definindo LIGHT/Relay Port

//----------------------------------------SSID e senha do seu roteador/hotspot WiFi
const char* ssid     = "Cubico Blacks"; // ssid
const char* password = "cristao12"; // password

//----------------------------------------Channel ID do ThingSpeak
unsigned long channel = 1728705;

//----------------------------------------Declaração field1, eu uso field1 para luz/relé, se você usar muitos field, adicione field1, field2 e assim por diante.
unsigned int field1 = 1;

WiFiClient  client;

//----------------------------------------SETUP
void setup() {
  Serial.begin(115200);
  delay(100);
  
  pinMode(ON_Board_LED, OUTPUT);  // LED output
  pinMode(LIGHT, OUTPUT); // LIGHT/Relay port output
  
  digitalWrite(ON_Board_LED, HIGH); // off Led
  digitalWrite(LIGHT, LOW); // off LIGHT LIGHT/Relay

  //----------------------------------------Wait for connection
  WiFi.begin(ssid, password);
  Serial.println("");
  Serial.print("Conectando");
  while (WiFi.status() != WL_CONNECTED) { 
    Serial.print(".");
    //----------------------------------------Faça o LED intermitente a bordo no processo de conexão ao roteador wifi.
    digitalWrite(ON_Board_LED, LOW);
    delay(250);
    digitalWrite(ON_Board_LED, HIGH);
    delay(250);
  }
  digitalWrite(ON_Board_LED, LOW); // Ligar o LED On Board quando estiver conectado ao roteador wifi.
  
  Serial.println("");
  Serial.print("Conectado com sucesso: ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.print("Netmask: ");
  Serial.println(WiFi.subnetMask());
  Serial.print("Gateway: ");
  Serial.println(WiFi.gatewayIP());
  //----------------------------------------ThingSpeak.begin(client)
  ThingSpeak.begin(client);
}

void loop() {
  int statusCode = 0;
  
  int last_light_state = ThingSpeak.readFloatField(channel, field1);  // obtenha os dados mais recentes dos campos no ThingSpeak

  statusCode = ThingSpeak.getLastReadStatus();  // Verifique o status da operação de leitura para ver se foi bem-sucedida
  if(statusCode == 200){
    if(last_light_state == 1){
      digitalWrite(LIGHT, HIGH);
      Serial.println("LUZ está ligada");
    }
    else if(last_light_state == 0){
      digitalWrite(LIGHT, LOW);
      Serial.println("LUZ está desligada");
    }
    Serial.print("Os dados mais recentes do Field1 no ThingSpeak são: ");
    Serial.println(last_light_state);    
  }
  else {
    Serial.println("Problema na leitura do channel. Código de erro HTTP: " + String(statusCode));
  }
  delay(15000);
}
