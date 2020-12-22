
#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <PubSubClient.h>

char auth[] = "";  //Blynk Authentication Token
char ssid[] = "";           //WIFI Name
char pass[] = "";           //WIFI Password

const char *MQTT_server = "broker.mqtt-dashboard.com";

int minRange = 312;
int maxRange = 712;

WiFiClient espClient;
PubSubClient client(espClient);

void setup() 
{
    Serial.begin(115200);
    delay(10);
    
    Serial.print("Connecting to ");
    Serial.println(ssid);

    WiFi.begin(ssid,pass);

    while(WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }
    Serial.println("WiFi connected !");
    Blynk.begin(auth, ssid, pass);
    client.setServer(MQTT_server,1883);
    client.setCallback(callback);

}

void reconnect()
{
    while(!client.connected())
    {
        String clientId = "ESP8266Client-";
        clientId += String(random(0xffff),HEX);

        if(client.connect(clientId.c_str()))
        {
            Serial.println("Connected");
            client.subscribe("Tilt_team4");
            client.subscribe("Distance1_team4");
        }
        else
        {
            Serial.println("failed");
            delay(6000);
        }
    }
}

void callback(char* topic, byte* payload, unsigned int length) 
{

//for tilt 1, stable 0
  if(strcmp(topic,"Tilt_team4") == 0)
  {
    int p =(char)payload[0]-'0';
    Blynk.virtualWrite(V2,p); 
  }
  
  if(strcmp(topic,"Distance1_team4") == 0)
  {
    String distance = String((char*)payload); 
    Blynk.virtualWrite(V0,distance);
  } 
} 

void moveControl(int x, int y)
{

//Move Forward
    if(y >= maxRange && x >= minRange && x<= maxRange)
    {
        client.publish("Movement_team4","1");
    }


//Move Right
    else if((x >= maxRange && y >= maxRange) || (y <= minRange && x <= minRange))
    {
        client.publish("Movement_team4","3");
    }

//Move Left
    else if((x <= minRange && y >= maxRange) || (y <= minRange && x >= maxRange))
    {
        client.publish("Movement_team4","4");
    }
//Move Backward
    else if(y <= minRange && x >= minRange && x <= maxRange)
    {
        client.publish("Movement_team4","2");
    }
    
    delay(5000);
}

void loop()
{
    Blynk.run();
    if(!client.connected())
    {
        reconnect();
    }

    client.loop();

}

BLYNK_WRITE(V1)
{
    int x = param[0].asInt();
    int y = param[1].asInt();
    moveControl(x,y);
}
