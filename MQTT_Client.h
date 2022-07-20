#include <WiFi.h>
#include <PubSubClient.h>

#define WIFISSID "RedeIFAL"
#define PASSWORD "redeifal"
#define DEVICE_ID "623cd9285ba7b70a807e4985"
#define TOKEN "BBFF-uwvbpnHk3Z8oN8fC2N26askdvC3E4v"

#define VARIABLE_LABEL_TEMPERATURE "mediaTemperatura"
#define VARIABLE_LABEL_TENSION "mediaTensao"
#define VARIABLE_LABEL_CURRENT "mediaCorrente"
#define VARIABLE_LABEL_HUMIDITY "mediaUmidade"
#define VARIABLE_LABEL_AIRTEMPERATURE "mediaTemperaturaAr"

#define SERVER "things.ubidots.com"
#define PORT 1883
#define TOPIC "/v1.6/devices/iot_esp32"

WiFiClient ubidots;
PubSubClient client(ubidots);

bool mqttInit(){
  
	WiFi.begin(WIFISSID, PASSWORD);
	
	while(WiFi.status() != WL_CONNECTED)
  {
    delay(1000);
    Serial.println("Estabilizando Conexão WiFi...");
    Serial.println(WiFi.status());
  }
	
	Serial.println("Conexão à Rede Concluída");
	
	client.setServer(SERVER, PORT);
	
	while(!client.connect(DEVICE_ID, TOKEN, "")){
		Serial.println("MQTT - Erro de Conexão");
		return false;
	}
	
	Serial.println("MQTT - Conectado com Sucesso");
	return true;
}

void reconnect(){
	while(!client.connected()){
		Serial.println("Tentando Conexão MQTT...");
		
		if(client.connect(DEVICE_ID, TOKEN,""))
			Serial.println("connected");
		else{
			Serial.print("conexão falhou");
			Serial.print(client.state());
			Serial.println("Tente Novamente em 2 Segundos");
			delay(2000);
		}
	}
	Serial.println("Reconectado com Sucesso");
}

bool sendValues1(float mediaTemperatura, float mediaTensao, float mediaCorrente){
	char json[250];
	
	sprintf(json, "{\"%s\":{\"value\":%02.02f, \"context\":{\"mediaTemperatura\":%02.02f,\"mediaTensao\":%02.02f,\"mediaCorrente\":%02.02f}}}", VARIABLE_LABEL_TEMPERATURE, mediaTemperatura, mediaTemperatura, mediaTensao, mediaCorrente);
	
	if(!client.publish(TOPIC, json))
		return false;
	
	sprintf(json, "{\"%s\":{\"value\":%02.02f, \"context\":{\"mediaTemperatura\":%02.02f,\"mediaTensao\":%02.02f,\"mediaCorrente\":%02.02f}}}", VARIABLE_LABEL_TENSION, mediaTensao, mediaTemperatura, mediaTensao, mediaCorrente);
	
	if(!client.publish(TOPIC, json))
		return false;
	
	sprintf(json, "{\"%s\":{\"value\":%02.02f, \"context\":{\"mediaTemperatura\":%02.02f,\"mediaTensao\":%02.02f,\"mediaCorrente\":%02.02f}}}", VARIABLE_LABEL_CURRENT, mediaCorrente, mediaTemperatura, mediaTensao, mediaCorrente);
	
	if(!client.publish(TOPIC, json))
		return false;
	
	return true;
}

bool sendValues2(float mediaUmidade, float mediaTemperaturaAr){
  char json[250];
  
  sprintf(json, "{\"%s\":{\"value\":%02.02f, \"context\":{\"mediaUmidade\":%02.02f,\"mediaTemperaturaAr\":%02.02f}}}", VARIABLE_LABEL_HUMIDITY, mediaUmidade, mediaUmidade, mediaTemperaturaAr);
  
  if(!client.publish(TOPIC, json))
    return false;
  
  sprintf(json, "{\"%s\":{\"value\":%02.02f, \"context\":{\"mediaUmidade\":%02.02f,\"mediaTemperaturaAr\":%02.02f}}}", VARIABLE_LABEL_AIRTEMPERATURE, mediaTemperaturaAr, mediaUmidade, mediaTemperaturaAr);
  
  if(!client.publish(TOPIC, json))
    return false;
  
  return true;
}
