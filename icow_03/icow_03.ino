#include <Wire.h>
#include <ESP8266WiFi.h> 
#include <MPU6050.h>
#include <SoftwareSerial.h>
#include <TinyGPS.h>
#include <PubSubClient.h>

#define WIFI_AP "jualabs"
#define WIFI_PASSWORD "#jualabsufrpe#"

#define TOKEN "tuO7zcFdeDLRs1trFbPq"

#define LED1  13

#define BTN 15

char thingsboardServer[] = "demo.thingsboard.io";

WiFiClient wifiClient;

PubSubClient client(wifiClient);

int status = WL_IDLE_STATUS;
unsigned long lastSend;

const int tx = D2; // GPS
const int rx = D1; // GPS

MPU6050 mpu;
int SCL_PIN=D6; // Acel
int SDA_PIN=D5; // Acel

SoftwareSerial serial1(rx, tx); // RX, TX
TinyGPS gps1;

void setup() 
{
  pinMode(LED1, OUTPUT);
  pinMode(BTN, INPUT_PULLUP);
  serial1.begin(9600);
  Serial.begin(9600);
  // WiFi.forceSleepBegin();// turn off ESP8266 RF
  delay(10);
  InitWiFi();
  client.setServer( thingsboardServer, 1883 );
  lastSend = 500;
  
  Serial.println("Initialize System");

  

  // If you want, you can set accelerometer offsets
  // mpu.setAccelOffsetX();
  // mpu.setAccelOffsetY();
  // mpu.setAccelOffsetZ();
  

}


void loop()
{
  long latitude, longitude;
  if ( !client.connected() ) {
    reconnect();
  }
  bool recebido = false;

  while (serial1.available()) {
     char cIn = serial1.read();
     recebido = gps1.encode(cIn);
  }

  if (recebido) {
     Serial.println("----------------------------------------");
     
     //Latitude e Longitude
     //long latitude, longitude;
     unsigned long idadeInfo;
     gps1.get_position(&latitude, &longitude, &idadeInfo);     

     if (latitude != TinyGPS::GPS_INVALID_F_ANGLE) {
        Serial.print("Latitude: ");
        Serial.println(float(latitude) / 100000, 6);
     }

     if (longitude != TinyGPS::GPS_INVALID_F_ANGLE) {
        Serial.print("Longitude: ");
        Serial.println(float(longitude) / 100000, 6);
     }

     if (idadeInfo != TinyGPS::GPS_INVALID_AGE) {
        Serial.print("Idade da Informacao (ms): ");
        Serial.println(idadeInfo);
     }


     //Dia e Hora
     int ano;
     byte mes, dia, hora, minuto, segundo, centesimo;
     gps1.crack_datetime(&ano, &mes, &dia, &hora, &minuto, &segundo, &centesimo, &idadeInfo);

     Serial.print("Data (GMT): ");
     Serial.print(dia);
     Serial.print("/");
     Serial.print(mes);
     Serial.print("/");
     Serial.println(ano);

     Serial.print("Horario (GMT): ");
     Serial.print(hora);
     Serial.print(":");
     Serial.print(minuto);
     Serial.print(":");
     Serial.print(segundo);
     Serial.print(":");
     Serial.println(centesimo);


     //altitude
     float altitudeGPS;
     altitudeGPS = gps1.f_altitude();

     if ((altitudeGPS != TinyGPS::GPS_INVALID_ALTITUDE) && (altitudeGPS != 1000000)) {
        Serial.print("Altitude (cm): ");
        Serial.println(altitudeGPS);
     }


     //velocidade
     float velocidade;
     //velocidade = gps1.speed();        //nós
     velocidade = gps1.f_speed_kmph();   //km/h
     //velocidade = gps1.f_speed_mph();  //milha/h
     //velocidade = gps1.f_speed_mps();  //milha/segundo

     Serial.print("Velocidade (km/h): ");
     Serial.println(velocidade, 2);  //Conversão de Nós para Km/h



     //sentito (em centesima de graus)
     unsigned long sentido;
     sentido = gps1.course();

     Serial.print("Sentido (grau): ");
     Serial.println(float(sentido) / 100, 2);


     //satelites e precisão
     unsigned short satelites;
     unsigned long precisao;
     satelites = gps1.satellites();
     precisao =  gps1.hdop();

     if (satelites != TinyGPS::GPS_INVALID_SATELLITES) {
        Serial.print("Satelites: ");
        Serial.println(satelites);
     }

     if (precisao != TinyGPS::GPS_INVALID_HDOP) {
        Serial.print("Precisao (centesimos de segundo): ");
        Serial.println(precisao);
     }

     Vector rawAccel = mpu.readRawAccel();
     Vector normAccel = mpu.readNormalizeAccel();

     Serial.print(" Xraw = ");
     Serial.print(rawAccel.XAxis);
     Serial.print(" Yraw = ");
     Serial.print(rawAccel.YAxis);
     Serial.print(" Zraw = ");
    
     Serial.println(rawAccel.ZAxis);
     Serial.print(" Xnorm = ");
     Serial.print(normAccel.XAxis);
     Serial.print(" Ynorm = ");
     Serial.print(normAccel.YAxis);
     Serial.print(" Znorm = ");
     Serial.println(normAccel.ZAxis);

     digitalWrite(LED1, HIGH);
      
     delay(10);

     digitalWrite(LED1, LOW);


     //float distancia_entre;
     //distancia_entre = gps1.distance_between(lat1, long1, lat2, long2);

     //float sentido_para;
     //sentido_para = gps1.course_to(lat1, long1, lat2, long2);
     
  }
  if(digitalRead(BTN) != 0) {
    getAndSendSerialData(latitude);
  }
  if ( millis() - lastSend > 10000 ){ // Update and send only after 1 seconds
    //getAndSendSerialData(latitude);
    getAndSendMic();
    lastSend = millis();
  }
  client.loop();
  
}

void getAndSendMic()
{
  String captando;
  //bt=false;
  for(int y=0;y<1;y++)
  {
    
    for(int i=0;i<10;i++)
    {
      //long input = Serial.parseInt();
      Serial.println("Collecting data.");

      //string dado = input
  
      // Check if any reads failed and exit early (to try again).
      //if (isnan(input)) {
      //  Serial.println("Failed!");
      //  return;
      //}
    
      //Serial.print("Mic: ");
      //Serial.print(latitude);
    
      //String mic = String(latitude);
    
      captando += char(analogRead(A0)/4);
          
    }
    // Just debug messages
      Serial.print( "Sending data : [" );
      //Serial.print( mic );
      Serial.print( "]   -> " );
    
      // Prepare a JSON payload string
      String payload = "{";
      payload += "\"dado\":"; payload += captando;
      payload += "}";
      
    // Send payload
      char attributes[1000];
      payload.toCharArray( attributes, 1000 );
      client.publish( "v1/devices/me/telemetry", attributes );
      Serial.println( attributes );
      captando = "";
  }
  
}

void getAndSendSerialData(long latitude)
{
  //long input = Serial.parseInt();
  Serial.println("Collecting data.");

  //string dado = input
  
  // Check if any reads failed and exit early (to try again).
  //if (isnan(input)) {
  //  Serial.println("Failed!");
  //  return;
  //}

  Serial.print("Dado: ");
  Serial.print(latitude);

  String dado = String(latitude);

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
