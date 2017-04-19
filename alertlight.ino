#include <math.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
 
const char* ssid = "******";
const char* password = "******";

const int redPin = 5;
const int greenPin = 4;
const int bluePin = 0;

float duration, distance, OldDistance, AverageVelocity;

int trigPin = 14;
int echoPin = 12;

void setup() 
{
  Serial.begin(9600);
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  
  WiFi.begin(ssid, password);
 
  while (WiFi.status() != WL_CONNECTED) {
 
    delay(1000);
    Serial.print("Connecting..");
 
  }
}

void loop() 
{
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = (duration/2) / 29.1;

  //Kijk of old_distance al is gedefineerd
  if (OldDistance != 0)
  {
      if (distance < 200) 
      {
        //Berekening snelheid.
        AverageVelocity = ((OldDistance - distance) / 0.200) * 3.6;

        if (AverageVelocity > 130 && AverageVelocity < 250)
        {
            int TooHardVelocity = round(AverageVelocity);
              
            Serial.print(TooHardVelocity);
            Serial.println(" km/h");

             digitalWrite(redPin,255);
             digitalWrite(greenPin, 0);
             digitalWrite(bluePin, 0);

            //POST REQUEST   
            if (WiFi.status() == WL_CONNECTED) //Check WiFi connection status
            { 
                HTTPClient http;  //Declare an object of class HTTPClient
               
                http.begin("http://api.alertlight.nl/api/speed/new/1/"+String((TooHardVelocity / 2)));  //Specify request destination
                int httpCode = http.GET();                                                                  //Send the request
               
                if (httpCode > 0) { //Check the returning code
               
                    String payload = http.getString();   //Get the request response payload
                    Serial.println(payload);                     //Print the response payload
               
                }
               
                http.end();   //Close connection
               
             }
                                
         } else
         {
             digitalWrite(redPin, 255);
             digitalWrite(greenPin, 255);
             digitalWrite(bluePin, 255);
         }
      }
   }

  OldDistance = distance;

  delay(200);
}
