/*
  Copyright (c) 2017 zmstudio
*/

#include <SPI.h>
#include <Ethernet.h>
#include <PubSubClient.h>

// Update these with values suitable for your network.
byte mac[]    = {  0xDE, 0xED, 0xBA, 0xFE, 0xFE, 0xED };
IPAddress ip(192, 168, 2, 80);
IPAddress server(121, 42, 230, 237);

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i=0;i<length;i++) {
    Serial.print((char)payload[i]);
  }
  if((char)payload[0]=='1'){
    digitalWrite(9, HIGH);
  }else{
    digitalWrite(9, LOW);
  }
  Serial.println();
}

EthernetClient ethClient;
PubSubClient client(ethClient);
int sensorValue = 0;

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("f3f119","f3f119","b799c3")) {
      Serial.println("connected");
      client.subscribe("device/f3f119/to/light");
      Serial.println("subscribe successfully");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void setup()
{
  Serial.begin(57600);
  pinMode(9, OUTPUT);
  digitalWrite(9, LOW);

  client.setServer(server, 1883);
  client.setCallback(callback);

  Ethernet.begin(mac, ip);
  // Allow the hardware to sort itself out
  delay(1500);
}

void loop()
{
  sensorValue = analogRead(A5);
  if (!client.connected()) {
    reconnect();
  }else{
    client.publish("device/f3f119/property/temp",(String((5.0 * sensorValue * 100.0) / 1024,4)).c_str());
  }
  client.loop();
  delay(2000);
}
