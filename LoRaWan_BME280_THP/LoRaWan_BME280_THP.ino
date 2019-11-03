 /* This sketch has been writen by S Collins  https://www.securethings.uk (c) for the HELTEC CubeCell Products  and includes work and  Libraries From: 
 - Seeed BME280 c,h all rights are attibuted for the Libaries - https://www.seeedstudio.com/ (c)
 - Heltec Automation  for the CubeCell basic Lorawan Library  - https://www.heltec.org/proudct_center/ (c)
 - Cayenne if added -myDevices for the use of the CayenneLPP.h Library and its use in the myDevices Dashboard - https://www.mydevices.com/ (c)

 This version does not support Cayenne in this version   
 File name: LoRaWAN_BME280_THP           Version: v1.0           Date:03/11/19
*/

#include "LoRaWan_APP.h"
#include "Arduino.h"
#include <Wire.h>
#include "BME280.h"



#ifndef ACTIVE_REGION
#define ACTIVE_REGION LORAMAC_REGION_EU868
#endif

#ifndef CLASS_MODE
#define CLASS_MODE CLASS_A
#endif


DeviceClass_t  CLASS=CLASS_MODE;

// You can get a more precise measurement of altitude if you know the current sea level pressure which will  vary with weather. Standard presure at sea level is  101325 Pa
// youcan alter this valve  in BME280.cpp by changing the line "float A = pressure/101325" ennd value to the local current sea level pressure.



/*
 * set LoraWan_RGB to 1,the RGB active in loraWan
 * RGB red means sending;
 * RGB purple means joined done;
 * RGB blue means RxWindow1;
 * RGB yellow means RxWindow2;
 * RGB green means received done;
 */
#ifndef LoraWan_RGB
#define LoraWan_RGB 0
#endif

/*
   set to 1 the enable AT mode
   set to 0 the disable support AT mode saves power
*/
#define  AT_SUPPORT  1

/*!
   When set to true the application uses the Over-the-Air activation procedure
   When set to false the application uses the Personalization activation procedure
*/
bool OVER_THE_AIR_ACTIVATION = true; // true or false

/* LoRaWAN Adaptive Data Rate */
bool LORAWAN_ADR_ON = true;  // true or false


/* Indicates if the node is sending confirmed or unconfirmed messages */
bool IsTxConfirmed = true;  // true or false

/*!
* Number of trials to transmit the frame, if the LoRaMAC layer did not
* receive an acknowledgment. The MAC performs a datarate adaptation,
* according to the LoRaWAN Specification V1.0.2, chapter 18.4, according
* to the following table:
*
* Transmission nb | Data Rate
* ----------------|-----------
* 1 (first)       | DR
* 2               | DR
* 3               | max(DR-1,0)
* 4               | max(DR-1,0)
* 5               | max(DR-2,0)
* 6               | max(DR-2,0)
* 7               | max(DR-3,0)
* 8               | max(DR-3,0)
*
* Note, that if NbTrials is set to 1 or 2, the MAC will not decrease
* the datarate, in case the LoRaMAC layer did not receive an acknowledgment
*/
uint8_t ConfirmedNbTrials = 8;

/* Application port  port decoded for application normally 2 for TTN or 10 for TTN mapper*/
uint8_t AppPort = 2; 

/*The application data transmission duty cycle.  value in [ms].*/
uint32_t APP_TX_DUTYCYCLE = 600000; 

/*  get the BatteryVoltage in mV. */
static uint16_t GetBatteryVoltage(void)
{
  pinMode(ADC_CTL,OUTPUT);
  digitalWrite(ADC_CTL,LOW);
  uint16_t volt=analogRead(ADC)*2;
  digitalWrite(ADC_CTL,HIGH);
  return volt;
}

/* Prepares the payload of the frame */

BME280 bme280;

static void PrepareTxFrame( uint8_t port )
{
  pinMode(Vext, OUTPUT);
  digitalWrite(Vext, LOW);
  delay(500); //delay to let power line settle
  
 if(!bme280.init()){
    Serial.println("Device error!");}
    
    
  //  BME280.begin();
  delay(100); // To let Sensor settle
  float temperature = (bme280.getTemperature()); //In Deg C
  float pressure = (bme280.getPressure()/100); //Presssure in HPa
  float humidity = (bme280.getHumidity()); // in %
  
  
  Wire.end();

  //digitalWrite(Vext, HIGH); // turn off Vext
  uint16_t BatteryVoltage = GetBatteryVoltage();
  
  unsigned char *puc;
  puc = (unsigned char *)(&temperature);
  
  AppDataSize = 14;//AppDataSize max value is 64
  
  AppData[0] = puc[0];
  AppData[1] = puc[1];
  AppData[2] = puc[2];
  AppData[3] = puc[3];
  
  puc = (unsigned char *)(&humidity);
  AppData[4] = puc[0];
  AppData[5] = puc[1];
  AppData[6] = puc[2];
  AppData[7] = puc[3];

  puc = (unsigned char *)(&pressure);
  AppData[8] = puc[0];
  AppData[9] = puc[1];
  AppData[10] = puc[2];
  AppData[11] = puc[3];
  
  AppData[12] = (uint8_t)(BatteryVoltage>>8);
  AppData[13] = (uint8_t)BatteryVoltage;


  Serial.print("Temp: ");
  Serial.print(temperature);
  Serial.print(" C, Humidity: ");
  Serial.print(humidity);
  Serial.print(" %, Pressure: ");
  Serial.print(pressure);
  Serial.print(" hPa ");
  Serial.print(" Battery mV: ");
  Serial.println(BatteryVoltage);
 
}

void setup(){
    BoardInitMcu();
    Serial.begin(115200);
    DeviceState = DEVICE_STATE_INIT;

}
void loop()
{
  switch( DeviceState )
  {
    case DEVICE_STATE_INIT:
    {
      Serial.printf("LoRaWan Class%X test start! \r\n",CLASS+10);   
#if(AT_SUPPORT)
      Enable_AT();
      getDevParam();
#endif
      printDevParam();
      LoRaWAN.Init(CLASS,ACTIVE_REGION);
      DeviceState = DEVICE_STATE_JOIN;
      break;
    }
    case DEVICE_STATE_JOIN:
    {
      LoRaWAN.Join();
      break;
    }
    case DEVICE_STATE_SEND:
    {
      PrepareTxFrame( AppPort );
      LoRaWAN.Send();
      DeviceState = DEVICE_STATE_CYCLE;
      break;
    }
    case DEVICE_STATE_CYCLE:
    {
      // Schedule next packet transmission
      TxDutyCycleTime = APP_TX_DUTYCYCLE + randr( 0, APP_TX_DUTYCYCLE_RND );
      LoRaWAN.Cycle(TxDutyCycleTime);
      DeviceState = DEVICE_STATE_SLEEP;
      break;
    }
    case DEVICE_STATE_SLEEP:
    {
      LoRaWAN.Sleep();
      break;
    }
    default:
    {
      DeviceState = DEVICE_STATE_INIT;
      break;
    }
  }
  
}
