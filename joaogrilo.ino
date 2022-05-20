#include "MQTT_Client.h"
#include <DHT.h> 

#define pinTMP 35
#define pinTENS 32
#define pinAMP 33
#define pinDHT 12

#define DHTTYPE DHT11

DHT dht(pinDHT, DHTTYPE);

const float Vref = 3.3; 
const float div_tensao = 11.1; 

int tempBits = 0;
float temperatura = 0;

int voltBits = 0; 
float tensao = 0;

int ampBits = 0;
float corrente = 0;

float umidade = 0;

float temperaturaAr = 0;


void setup() {
  
  pinMode(pinTMP, INPUT);
  pinMode(pinTENS, INPUT);
  pinMode(pinAMP, INPUT);
  pinMode(pinDHT, INPUT);

  Serial.begin(115200);

  dht.begin();
  Serial.println("Configurando MQTT...");
 
  if(!mqttInit()){
    delay(3000);
    Serial.println("Falhou");
    ESP.restart();
  }
  else{ 
    Serial.println("Conexão OK");  
  }
}

 /*----------------------------------------------------------------------------------------------------------------------*/

void loop() {
  
  if(!client.connected()){
    reconnect();
    }

  tempBits = analogRead(pinTMP);
  temperatura = (4.74 * tempBits * 100)/4095; 

  voltBits = analogRead(pinTENS);
  tensao = (div_tensao * (Vref * voltBits / 4095) + 1.2); 

  ampBits = analogRead(pinAMP);
  corrente = 10 * ((Vref * ampBits)/(4095) - 2.5);

  float umidade = dht.readHumidity();
  
  float temperaturaAr = dht.readTemperature();

  Serial.print("Temperatura: ");
  Serial.print(temperatura);
  Serial.println(" Celsius");

  Serial.print("Tensao: ");
  Serial.print(tensao);
  Serial.println(" Volts");

  Serial.print("Corrente: ");
  Serial.print(corrente);
  Serial.println(" Amperes");

  Serial.print("Umidade: ");
  Serial.print(umidade);
  Serial.println(" UR");

  Serial.print("Temperatura do Ar: ");
  Serial.print(temperaturaAr);
  Serial.println(" Celsius");

  if (isnan(umidade) || isnan(temperaturaAr)) {
    Serial.println(F("Failed to read from DHT sensor!"));
  }

  Serial.println();
  
  if(sendValues1(temperatura, tensao, corrente)){
    Serial.println("Envio de Dados Concluído - Operação 1");
    Serial.println();
  }
  
  else {
    Serial.println("Envio de Dados Falhou! - Operação 1");
    Serial.println();
  }

  
  if(sendValues2(umidade, temperaturaAr)){
    Serial.println("Envio de Dados Concluído - Operação 2");
    Serial.println();
  }
  
  else {
    Serial.println("Envio de Dados Falhou! - Operação 2");
    Serial.println();
  }
  delay(3000);
}
