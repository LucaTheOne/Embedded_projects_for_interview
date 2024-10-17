////////////////////////////////////////////////////////////
//                                                        //
//    HomeSpan: A HomeKit implementation for the ESP32    //
//    ------------------------------------------------    //
//                                                        //
//            Smart Table Lamb Control device             //
//            Productor: Bolelli Luca                     //
//                                                        //
////////////////////////////////////////////////////////////


#include <HomeSpan.h>
#include "DEV_smart_table_lamp_esp.h"
#define DEVICE_NAME "Luce Scrivania" //example name, name to change in base to the necessity.
#define HOST_NAME "HS_smart_table_lamp_control_device" //example host name
#define MODEL "TLC-1.0-HK-LB"
#define WORKING_PIN 17 //pin connected to the relay copntrol of the board.
#define SSID "net ssid"
#define PSW "net password"
void setup()
{
  Serial.begin(115200);
  homeSpan.setApSSID(SSID);
  homeSpan.setApPassword(PSW);
  homeSpan.begin(Category::Lighting,DEVICE_NAME,HOST_NAME,MODEL); //initializing homespan services
  new SpanAccessory(); 
    new Service::AccessoryInformation(); 
      new Characteristic::Identify();
  new Smart_table_lamp(WORKING_PIN);
}

void loop()
{
  homeSpan.poll();
}