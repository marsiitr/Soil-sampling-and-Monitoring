float soilMoistureValue = 0;
float soilmoisturepercent=0;

void myTimerEvent()
{
   
soilMoistureValue = analogRead(D1);
 
Serial.println("SoilMoistureValue :");
Serial.println(soilMoistureValue);

soilmoisturepercent = map(soilMoistureValue, AirValue, WaterValue, 0, 100);
if(soilmoisturepercent >= 100)
{
  Serial.println("SoilMoisturePercent :");
  Serial.println("100 %");
  //Blynk.virtualWrite(V0,soilmoisturepercent);

}
else if(soilmoisturepercent <=0)
{
  Serial.println("SoilMoisturePercent :");
  Serial.println("0 %");
 // Blynk.virtualWrite(V0,soilmoisturepercent);
}
else if(soilmoisturepercent >0 && soilmoisturepercent < 100)
{
  Serial.println("SoilMoisturePercent :");
  Serial.print(soilmoisturepercent);
  Serial.println("%");
  //Blynk.virtualWrite(V0,soilmoisturepercent);

}
}


