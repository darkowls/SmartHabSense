// Project HabBox
// Sketch or several sensors, connected to Nano and akting as Sensors box.
// Including hardware libraries
#include "Wire.h"                               //I2C TWI Library - uses pin A4
#include "BMP085.h"                             //Barometer library
#include "DHT.h"                                //DHT Library from Github

// Defining pins and HW types
#define DHTPIN 2                                //Digital pin number for DHT22
#define DHTTYPE DHT22                           //choosing DHT22
DHT dht(DHTPIN, DHTTYPE);                       //setting DHT
BMP085 dps = BMP085();                          //Pressure sensor type
#define pirPin 4                                //Choosing pin for PIR
#define micDpin 6                               //Setting mic digital pin
#define micApin 0                               //Setting mic analog pin
#define mq7Dpin 3                               //Setting MQ7 Digital Pin
#define mq7Apin 3                               //Setting MQ7 Analog Pin
#define mq4Dpin 5                               //Setting MQ4 Digital Pin
#define mq4Apin 7                               //Setting MQ4 Analog Pin
#define mq4LEDpin 7                             //Setting MQ4 LED Pin
#define mq7LEDpin 8                             //Setting MQ7 LED Pin

float h = 0;                                    //Setting variable for humidity from DHT22
float t1 = 0;                                   //Setting variable for temperatire from DHT22
float hic = 0;                                  //Setting variable for heat index
long t2 = 0;                                    //Setting variable for temperatire from BMP085
long p = 0;                                     //Setting variable for pressure from BMP085
float pb = 0;                                   //Setting variable for pressure in Bar
int pirVal = 0;                                 //Setting variable for PIR value
int micDval = 0;                                //Setting variable for MIC digital value
int micAval = 0;                                //Setting variable for MIC analog value
int mq7Dval = 0;                                //Setting variable for MQ7 digital value
int mq7Aval = 0;                                //Setting variable for MQ7 analog value
int mq4Dval = 0;                                //Setting variable for MQ4 digital value
int mq4Aval = 0;                                //Setting variable for MQ4 analog value


void setup() {
Serial.begin(9600);                             //starting serial
Serial.println("HabSense Init");                //testing serial print
dht.begin();                                    //starting dht
delay(1000);
Wire.begin();                                   //starting i2c
delay(1000);
dps.init();                                     //starting digital pressure sensor
pinMode(pirPin, INPUT);                         //setting pin mode for PIR
pinMode(micDpin, INPUT);                        //setting pin mode for mic
//pinmode(micApin, INPUT);                        //setting pin mode for analog mic (unnesessary?)
pinMode(mq7Dpin, INPUT);                        //Setting MQ7 digital pin as input
pinMode(mq4Dpin, INPUT);                        //Setting MQ4 digital pin as input
pinMode(mq4LEDpin, OUTPUT);                     //Setting MQ4 LED Pin Mode
pinMode(mq7LEDpin, OUTPUT);                     //Setting MQ7 LED Pin Mode
}

void loop() {
delay(1000);                                    //read params every 5 seconds)
h = dht.readHumidity();                         //read humidity from DHT22
t1 = dht.readTemperature();                     //read temperature from DHT22
if (isnan(h) || isnan(t1)) {                    //if data read is not a number, go to beginning
  Serial.println("ERR: unable to read DHT22 sensor"); 
  return;
}
dps.getPressure(&p); 
dps.getTemperature(&t2);
//p = dps.getPressure();                        //read pressure from BMP085
//t2 = dps.getTemperature();                    //read temperature from BMP085

if (isnan(p) || isnan(t2)) {                    //if data read is not a number, go to beginning
  Serial.println("ERR: unable to read BMP085 sensor"); 
  return;
}

hic = dht.computeHeatIndex(t1, h, false);       //computing heat index
pb = p/100000;                                  //recalculating pressure in bars
pirVal = digitalRead(pirPin);                   //reading motion value
micDval = digitalRead(micDpin);                 //reading mic digital value
micAval = analogRead(micApin);                  //reading mic analog value

mq7Aval = analogRead(mq7Apin);                  //reading analaog value from MQ7
mq7Dval = digitalRead(mq7Dpin);                 //reading digital value from MQ7
mq4Aval = analogRead(mq4Apin);                  //reading analaog value from MQ4
mq4Dval = digitalRead(mq4Dpin);                 //reading digital value from MQ4

//LED setting
if (mq4Dval == 0){
digitalWrite(mq4LEDpin, HIGH);                    //if limit has been reached, LED turns on as status indicator
}
else{
digitalWrite(mq4LEDpin, LOW);                     //if threshold not reached, LED remains off
}
if (mq7Dval == 0){
digitalWrite(mq7LEDpin, HIGH);                    //if limit has been reached, LED turns on as status indicator
}
else{
digitalWrite(mq7LEDpin, LOW);                     //if threshold not reached, LED remains off
}

// Printing
Serial.print("H,%: ");
Serial.print(h);                                // Humidity
Serial.print("\t T1,*C: ");
Serial.print(t1);                               //Temperature from DHT22
Serial.print("\t T2,*C: ");
Serial.print(t2*0.1);                           //Temperature from BMP085           
Serial.print("\t Ts,*C: ");
Serial.print(hic);                              //Heat Index from DHT22
Serial.print("\t P,Pascal: ");
Serial.print(p);                                //Pressure in Pascals
//Serial.print("\t P,Bar: ");
//Serial.print(pb);                               //Pressure in Bars

Serial.print("\t CO,ppm: ");
Serial.print(mq7Aval);                          //Printing CO value
Serial.print("\t CO,lvl: ");
Serial.print(mq7Dval);                          //Printing CO lvl
Serial.print("\t CH4,ppm: ");
Serial.print(mq4Aval);                          //Printing CO value
Serial.print("\t CH4,lvl: ");
Serial.print(mq4Dval);                          //Printing CO lvl


if(micDval == HIGH){                            //Printing soung value
    Serial.print("\t Loud");
    }
    else
    {
    Serial.print("\t Quiet");
    }
Serial.print("\t vol,dB: ");
Serial.print(micAval);

if(pirVal == HIGH){                              //Printing motion value
    Serial.println("\t Motion detected");
   }
  else
  {
    Serial.println("\t No motion");
  }
}
