#include<ESP8266WiFi.h>
#include<PubSubClient.h>

char ssid[] = "Sanjay 2.4";           //WIFI Name
char pass[] = "12341234";           //WIFI Password
const char *MQTT_server = "broker.mqtt-dashboard.com";
int reading;
long lastMsg = 0;
long duration; 


#define tilt_pin 12
#define echoPin 2 
#define trigPin 3
#define D0 16
#define D1 5
#define D2 4

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
  client.setServer(MQTT_server,1883);
  client.setCallback(callback);
  pinMode(tilt_pin, INPUT);
  pinMode(trigPin, OUTPUT); 
  pinMode(echoPin, INPUT);
  pinMode(D0, OUTPUT); // D0
  pinMode(D1, OUTPUT);  // D1
  pinMode(D2, OUTPUT);  // D2
  turnoff();
  
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
            client.subscribe("Distance_team4");
            client.subscribe("Movement_team4");
        }
        else
        {
            Serial.println("failed");
            delay(6000);
        }
    }
}

void turnoff(){
  digitalWrite(D2,LOW);
  digitalWrite(D1,LOW);
  digitalWrite(D0,LOW);
}

void moveforward(){   //D0D1D2 ~D2:enable pin //111:for forward
  digitalWrite(D2,HIGH);
  digitalWrite(D1,HIGH);
  digitalWrite(D0,HIGH);
  delay(1000);
  turnoff();
}

void movebackward(){   //D0D1D2 ~D2:enable pin //111:for forward
  digitalWrite(D2,HIGH);
  digitalWrite(D1,LOW);
  digitalWrite(D0,LOW);
  delay(1000);
  turnoff();
}

void moveleft(){   //D0D1D2 ~D2:enable pin //111:for forward
  digitalWrite(D2,HIGH);
  digitalWrite(D1,LOW);
  digitalWrite(D0,HIGH);
  delay(1000);
  turnoff();
}

void moveright(){   //D0D1D2 ~D2:enable pin //111:for forward
  digitalWrite(D2,HIGH);
  digitalWrite(D1,HIGH);
  digitalWrite(D0,LOW);
  delay(1000);
  turnoff();
}

void callback(char* topic, byte* payload, unsigned int length) 
{

  if(strcmp(topic,"Movement_team4") == 0)
  {
    
    int d = (char)payload[0]-'0';
    
    if(d==1)
     { 
      moveforward();
     }
    else if(d==2)
      {
        movebackward();
      }
    else if(d==3)
    {
      moveright();
    }
    else if(d==4)
      {
        moveleft();
   
      }
  }
}

float getDistance()
{
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  // Sets the trigPin HIGH (ACTIVE) for 10 microseconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);
  // Calculating the distance
  float distance = duration * 0.034 / 2;
  return distance;
}
void loop()
{
  if(!client.connected())
    {
        reconnect();
    }

    client.loop();

    long now = millis();

   if (now - lastMsg > 6000) {

      lastMsg = now;
      float distance = getDistance();
      const char* msg= (String (distance)).c_str();
      
     client.publish("Distance_team4", msg);
   }

    
  reading = digitalRead(tilt_pin);
  if(reading == 1)
  {
   client.publish("Tilt_team4","1");
   delay(5000);
  }
  else
  {
   client.publish("Tilt_team4","0");
   delay(5000);
  }
}
