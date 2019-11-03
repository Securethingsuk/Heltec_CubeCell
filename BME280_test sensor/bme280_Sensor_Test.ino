/*Modified By S Collins Securethings.uk to work as a sensor only test and not operational 0nLoRaWan for CubeCell ASR65001/2
 * 
 * File name: BME280_Sensor_Test           Version: v1.0           Date:03/11/19
 * 
 * Original Example sketch for bme280 Copyright (c) 2016 seeed technology inc.Website    : www.seeedstudio.com
 * Author     : Lambor
 * 
 * The MIT License (MIT)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */
#include "Arduino.h"
#include "Wire.h"
#include "BME280.h"
BME280 bme280;

void setup()
{
  Serial.begin(115200);
  pinMode(Vext,OUTPUT);
  digitalWrite(Vext,LOW);//set vext output
  delay(100); //delay to let power line settle
  
  if(!bme280.init()){
    Serial.println("Device error!");
  }
}

void loop()
{
  float pressure;
  
  //get and print temperatures
  Serial.print("Temp: ");
  Serial.print(bme280.getTemperature());
  Serial.println("C");//The unit for  Celsius because original arduino don't support speical symbols
  
  //get and print atmospheric pressure data
  Serial.print("Pressure: ");
  Serial.print(pressure = bme280.getPressure()/100);
  Serial.println("hPa");
  

  //get and print altitude data
  Serial.print(" Approx Altitude: ");
  Serial.print(bme280.calcAltitude(pressure*100));
  Serial.println("m");

  //get and print humidity data
  Serial.print("Humidity: ");
  Serial.print(bme280.getHumidity());
  Serial.println("%");

  delay(5000);
}
