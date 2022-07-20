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
float mediaTemperatura = 0;

int voltBits = 0; 
float tensao = 0;
float mediaTensao = 0;

int ampBits = 0;
float corrente = 0;
float mediaCorrente;

float umidade = 0;
float mediaUmidade = 0;

float temperaturaAr = 0;
float mediaTemperaturaAr;

int temporizador = 0;

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
  temperatura = (Vref * tempBits * 100)/4095; 
  
  mediaTemperatura = mediaTemperatura + temperatura;

  voltBits = analogRead(pinTENS);
  tensao = (div_tensao * (Vref * voltBits / 4095)); 
  
  mediaTensao = mediaTensao + tensao;

  ampBits = analogRead(pinAMP);
  corrente = 10 * ((Vref * ampBits)/(4095) - 2.5);
  
  mediaCorrente = mediaCorrente + corrente;

  float umidade = dht.readHumidity();
  
  mediaUmidade = mediaUmidade + umidade;
  
  float temperaturaAr = dht.readTemperature();
  
  mediaTemperaturaAr = mediaTemperaturaAr + temperaturaAr;

  
  
  if(temporizador == 10){
    
    Serial.print("Temperatura: ");
    Serial.print(mediaTemperatura);
    Serial.println(" Celsius");
  
    Serial.print("Tensao: ");
    Serial.print(mediaTensao);
    Serial.println(" Volts");

    Serial.print("Corrente: ");
    Serial.print(mediaCorrente);
    Serial.println(" Amperes");

    Serial.print("Umidade: ");
    Serial.print(mediaUmidade);
    Serial.println(" UR");

    Serial.print("Temperatura do Ar: ");
    Serial.print(mediaTemperaturaAr);
    Serial.println(" Celsius");
    Serial.println();
    
   if(sendValues1(mediaTemperatura, mediaTensao, mediaCorrente)){
    Serial.println("Envio de Dados Concluído - Operação 1");
    Serial.println();
  }
  
  else {
    Serial.println("Envio de Dados Falhou! - Operação 1");
    Serial.println();
  }

  
  if(sendValues2(mediaUmidade, mediaTemperaturaAr)){
    Serial.println("Envio de Dados Concluído - Operação 2");
    Serial.println();
  }
  
  else {
    Serial.println("Envio de Dados Falhou! - Operação 2");
    Serial.println();
  }
    
    temporizador = -1;
 }
  
  temporizador = temporizador + 1;
  
  delay(1000);
}
