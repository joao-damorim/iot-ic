
#include <PubSubClient.h>
#include <ESP8266WiFi.h>

#define WIFI_AP "jualabs"
#define WIFI_PASSWORD "#jualabsufrpe#"

#define TOKEN "tuO7zcFdeDLRs1trFbPq"

#define LED1  5

// DHT
#define DHTPIN 2
#define DHTTYPE DHT22

char thingsboardServer[] = "demo.thingsboard.io";

WiFiClient wifiClient;

// Initialize DHT sensor.
//DHT dht(DHTPIN, DHTTYPE);

PubSubClient client(wifiClient);

int status = WL_IDLE_STATUS;
unsigned long lastSend;

void setup()
{
  pinMode(LED1, OUTPUT);
  Serial.begin(9600);
  //dht.begin();
  delay(10);
  InitWiFi();
  client.setServer( thingsboardServer, 1883 );
  lastSend = 0;
}

void loop()
{
  if ( !client.connected() ) {
    reconnect();
  }

  if ( millis() - lastSend > 1000 ) { // Update and send only after 1 seconds
    long input = Serial.parseInt();
    Serial.println(input);  
    if(input == 987654321) {
      digitalWrite(LED1, HIGH);  
    } 
    if(input == 123456789) {
      digitalWrite(LED1, LOW);
    }
   
    lastSend = millis();
  }

  client.loop();
}


void InitWiFi()
{
  Serial.println("Connecting to AP ...");
  // attempt to connect to WiFi network

  WiFi.begin(WIFI_AP, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Connected to AP");
}


void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    status = WiFi.status();
    if ( status != WL_CONNECTED) {
      WiFi.begin(WIFI_AP, WIFI_PASSWORD);
      while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
      }
      Serial.println("Connected to AP");
    }
    Serial.print("Connecting to ThingsBoard node ...");
    // Attempt to connect (clientId, username, password)
    if ( client.connect("ESP8266 Device", TOKEN, NULL) ) {
      Serial.println( "[DONE]" );
    } else {
      Serial.print( "[FAILED] [ rc = " );
      Serial.print( client.state() );
      Serial.println( " : retrying in 5 seconds]" );
      // Wait 5 seconds before retrying
      delay( 5000 );
    }
  }
}
