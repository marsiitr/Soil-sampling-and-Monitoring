

#define BLYNK_TEMPLATE_ID           "TMPLhswNixQX"
#define BLYNK_TEMPLATE_NAME         "Soil Sampling"
#define BLYNK_AUTH_TOKEN            "XKE1utJtPW8NCIKAkiUq-b-fDOO75i8F"
#define BLYNK_PRINT Serial

#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>

#define Offset 0.00
#define samplingInterval 20
#define printInterval 800
#define ArrayLenth 40  

int pHArray[ArrayLenth]; 
int pHArrayIndex = 0;
int countTrueCommand;
int countTimeCommand;
boolean found = false;  
int pHPin=A0;
int ThermistorPin = D0;
int MoisturePin=D1;
int Vo;
float R1 = 22000;
float logR2, R2, T;
float c1 = 1.009249522e-03, c2 = 2.378405444e-04, c3 = 2.019202697e-07;
const int AirValue = 140;  
const int WaterValue = 180;  
float soilMoistureValue = 0;
float soilmoisturepercent=0;

 

//#include <ESP8266WiFi.h>
//#include <BlynkSimpleEsp8266.h>

 
char ssid[] = "PrathamMobile";
char pass[] = "pratham22";

BlynkTimer timer;


float phVal() {
  static unsigned long samplingTime = millis();
  static unsigned long printTime = millis();
  static float pHValue, voltage;
  if(millis()-samplingTime > samplingInterval) {
    pHArray[pHArrayIndex++] = analogRead(pHPin);
    if(pHArrayIndex == ArrayLenth) {
      pHArrayIndex = 0;
    }
    voltage = (avergearray(pHArray, ArrayLenth)*3.3/1024)+2;
    pHValue = 3.5*voltage+Offset;
    samplingTime = millis();
  }
  if(millis()-printTime > printInterval) {
    Serial.print("Voltage:");
    Serial.print(voltage,2);
    Serial.print("    pH value: ");
    Serial.println(pHValue,2);
    printTime = millis();
  }
  return pHValue;
}

double avergearray(int* arr, int number) {
  int i;
  int max,min;
  double avg;
  long amount = 0;
  if(number <= 0) {
    Serial.println("Error number for the array to avraging!/n");
    return 0;
  }
  if(number < 5) {
    for(i = 0; i < number; i++) {
      amount += arr[i];
    }
    avg = amount/number;
    return avg;
  } 
  else {
    if(arr[0] < arr[1]) {
      min = arr[0];
      max = arr[1];
    }
    else {
      min = arr[1];
      max = arr[0];
    }
    for(i = 2; i < number; i++) {
      if(arr[i] < min) {
        amount += min;    
        min = arr[i];
      } 
      else {
        if(arr[i] > max) {
          amount += max; 
          max = arr[i];
        } 
        else {
          amount += arr[i];
        }
      }
    }
    avg = (double)amount/(number-2);
  }
  return avg;
}

void myTimerEvent()
{
   
  
  //int sensorValueMoisture = analogRead(36);
  int valSensorPH = phVal();

  Vo = analogRead(ThermistorPin);
  R2 = R1 * (1023 / (float)Vo - 1.0);
  logR2 = log(R2);
  T = (1.0 / (c1 + c2*logR2 + c3*logR2*logR2*logR2));
  T = T - 273.15;

  float temperature=T;

soilMoistureValue = analogRead(MoisturePin);
 
Serial.println("SoilMoistureValue :");
Serial.println(soilMoistureValue);

soilmoisturepercent = map(soilMoistureValue, AirValue, WaterValue, 0, 100);
if(soilmoisturepercent >= 100)
{
  Serial.println("SoilMoisturePercent :");
  Serial.println("100 %");
  Blynk.virtualWrite(V0, );

}
else if(soilmoisturepercent <=0)
{
  Serial.println("SoilMoisturePercent :");
  Serial.println("0 %");
  Blynk.virtualWrite(V0, );
}
else if(soilmoisturepercent >0 && soilmoisturepercent < 100)
{
  Serial.println("SoilMoisturePercent :");
  Serial.print(soilmoisturepercent);
  Serial.println("%");
  Blynk.virtualWrite(V0, );
  
  
}


   
  Blynk.virtualWrite(V0, );
  Blynk.virtualWrite(V1, );
  Blynk.virtualWrite(V2, );


 
  

  Serial.println("pH : ");
  Serial.println(valSensorPH);

  Serial.println("temperature : ");
  Serial.println(temperature);


  

}

void setup()
{
  // Debug console
  Serial.begin(9600);

  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
   

  // Setup a function to be called every second
  timer.setInterval(1000L, myTimerEvent);
}

void loop()
{
  Blynk.run();
  timer.run();
   
}

