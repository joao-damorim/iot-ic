#include <SoftwareSerial.h>
#include <Gpsneo.h>
//Gpsneo gps;

const int tx = D1;
const int rx = D2;

Gpsneo gps(rx,tx,9600);

char latitud[11];
char latitudHemisphere[3];
char longitud[11];
char longitudMeridiano[3];


void  setup()
{
    Serial.begin(9600);
}
void loop()
{
    //Just simple do getDataGPRMC, and the method solve everything.
    gps.getDataGPRMC(latitud,
                    latitudHemisphere,
                    longitud,
                    longitudMeridiano);

   Serial.println(latitud);
   Serial.println(latitudHemisphere);
   Serial.println(longitud);
   Serial.println(longitudMeridiano);
   
}
