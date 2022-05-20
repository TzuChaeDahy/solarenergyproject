#include <WiFi.h>
#include <PubSubClient.h>

#define WIFISSID "VINY"
#define PASSWORD "65223374"
#define DEVICE_ID "623cd9285ba7b70a807e4985"
#define TOKEN "BBFF-uwvbpnHk3Z8oN8fC2N26askdvC3E4v"

#define VARIABLE_LABEL_TEMPERATURE "temperatura"
#define VARIABLE_LABEL_TENSION "tensao"
#define VARIABLE_LABEL_CURRENT "corrente"
#define VARIABLE_LABEL_HUMIDITY "umidade"
#define VARIABLE_LABEL_AIRTEMPERATURE "temperaturaAr"

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

bool sendValues1(float temperatura, float tensao, float corrente){
	char json[250];
	
	sprintf(json, "{\"%s\":{\"value\":%02.02f, \"context\":{\"temperatura\":%02.02f,\"tensao\":%02.02f,\"corrente\":%02.02f}}}", VARIABLE_LABEL_TEMPERATURE, temperatura, temperatura, tensao, corrente);
	
	if(!client.publish(TOPIC, json))
		return false;
	
	sprintf(json, "{\"%s\":{\"value\":%02.02f, \"context\":{\"temperatura\":%02.02f,\"tensao\":%02.02f,\"corrente\":%02.02f}}}", VARIABLE_LABEL_TENSION, tensao, temperatura, tensao, corrente);
	
	if(!client.publish(TOPIC, json))
		return false;
	
	sprintf(json, "{\"%s\":{\"value\":%02.02f, \"context\":{\"temperatura\":%02.02f,\"tensao\":%02.02f,\"corrente\":%02.02f}}}", VARIABLE_LABEL_CURRENT, corrente, temperatura, tensao, corrente);
	
	if(!client.publish(TOPIC, json))
		return false;
	
	return true;
}

bool sendValues2(float umidade, float temperaturaAr){
  char json[250];
  
  sprintf(json, "{\"%s\":{\"value\":%02.02f, \"context\":{\"umidade\":%02.02f,\"temperaturaAr\":%02.02f}}}", VARIABLE_LABEL_HUMIDITY, umidade, umidade, temperaturaAr);
  
  if(!client.publish(TOPIC, json))
    return false;
  
  sprintf(json, "{\"%s\":{\"value\":%02.02f, \"context\":{\"umidade\":%02.02f,\"temperaturaAr\":%02.02f}}}", VARIABLE_LABEL_AIRTEMPERATURE, temperaturaAr, umidade, temperaturaAr);
  
  if(!client.publish(TOPIC, json))
    return false;
  
  return true;
}
