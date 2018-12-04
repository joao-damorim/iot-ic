
#include <PubSubClient.h>
#include <ESP8266WiFi.h>

#define WIFI_AP "jualabs"
#define WIFI_PASSWORD "#jualabsufrpe#"

#define TOKEN "tuO7zcFdeDLRs1trFbPq"

#define LED1  5

char thingsboardServer[] = "demo.thingsboard.io";

WiFiClient wifiClient;

PubSubClient client(wifiClient);

int status = WL_IDLE_STATUS;
unsigned long lastSend;

void setup()
{
  pinMode(LED1, OUTPUT);
  Serial.begin(9600);
  delay(10);
  InitWiFi();
  client.setServer( thingsboardServer, 1883 );
  lastSend = 500;
}

void loop()
{
  if ( !client.connected() ) {
    reconnect();
  }
  long input = Serial.parseInt();
  Serial.println(input);  
  if(input == 987654321) {
      digitalWrite(LED1, HIGH);  
  } 
  if(input == 123456789) {
      digitalWrite(LED1, LOW);
  }
  if ( millis() - lastSend > 1000 ) { // Update and send only after 1 seconds
    getAndSendSerialData();
    lastSend = millis();
  }
  client.loop();
}

void getAndSendSerialData()
{
  long input = Serial.parseInt();
  Serial.println("Collecting data.");

  //string dado = input
  
  // Check if any reads failed and exit early (to try again).
  if (isnan(input)) {
    Serial.println("Failed!");
    return;
  }

  Serial.print("Dado: ");
  Serial.print(input);

  String dado = String(input);

  // Just debug messages
  Serial.print( "Sending data : [" );
  Serial.print( dado );
  Serial.print( "]   -> " );

  // Prepare a JSON payload string
  String payload = "{";
  payload += "\"dado\":"; payload += dado;
  payload += "}";

  // Send payload
  char attributes[100];
  payload.toCharArray( attributes, 100 );
  client.publish( "v1/devices/me/telemetry", attributes );
  Serial.println( attributes );

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
