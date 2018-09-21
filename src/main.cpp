#include <Arduino.h>

#include <ESP8266WiFi.h>          // https://github.com/esp8266/Arduino
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <PubSubClient.h>


// Wifi config
const char* ssid = "MySSID";
const char* password = "MyPassword";

// Static IP address
IPAddress ip(192, 168, 1, 61);
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 255, 0);
IPAddress dns(8, 8, 8, 8);

// MQTT config
const char* mqtt_server = "1.2.3.4";
const unsigned int mqtt_port = 1883;
const char* mqtt_topicNotification = "verisure_alarmStateChange/STATE";
const char* mqtt_topicGeneral = "general/STATE";


WiFiClient espClient;   
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
long int value = 0;



void setup_wifi() {  
  delay(100);

  // Connecting to wifi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  // Static IP
  WiFi.config(ip, gateway, subnet, dns);
  delay(100);

  WiFi.begin(ssid, password);

  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("Connection Failed! Rebooting...");
    delay(500);
    ESP.restart();
  }

  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println();
    Serial.println("Fail connecting");
    delay(5000);
    ESP.restart();
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}


void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");

    // Attempt to connect
    if (client.connect("verisure_armstate", mqtt_topicNotification, 0, false, "ALARM_ON")) {
      Serial.println("connected");
      
      // Once connected, publish an announcement...
      client.publish(mqtt_topicGeneral, "Verisure alarm state to MQTT bridge signing in", false);

    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");

      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}


void setup() {
  Serial.begin(115200);
  Serial.println();

  setup_wifi();

  client.setServer(mqtt_server, mqtt_port);

 if (!client.connected()) {
    reconnect();
  }

  client.publish(mqtt_topicNotification, "ALARM_OFF", false);
}


void loop() {

  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  long now = millis();
  if (now - lastMsg > 5000) {
    lastMsg = now;
    Serial.println("Ping...");
  }
}


