#include <SoftwareSerial.h>
#include <Gpsneo.h>

const int tx = D1;
const int rx = D2;

Gpsneo gps(rx,tx,9600);

char link[70];

void  setup()
{
Serial.begin(9600);
Serial.println("start-------------------------------");
}

void loop()
{  
  gps.Google(link);
  
}
