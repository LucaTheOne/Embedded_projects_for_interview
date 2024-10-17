////////////////////////////////////////////////////////////
//            RC 433mhz gate's control Device             //
//            for home kit IOS                            //
//            Author: Bolelli Luca                        //
////////////////////////////////////////////////////////////


#include <HomeSpan.h>
#include "DEV_RFControl.h"         
#define SSID ""
#define PSW ""

void setup() 
{
  /* In setup gate control instantiation is made*/

  //with the purpose to debug, uncomment  the below line
  //Serial.begin(115200);
  pinMode(PIN_GATE,OUTPUT); //set the mode for control_pin to be an OUTPUT (standard Arduino function).  
  homeSpan.begin(Category::Locks,"Telecomando cancellone"); //the homespan comunication to homekit start, an image of a lock represent the device
  new SpanAccessory(); //the device location is created
  new Service::AccessoryInformation(); //info are updated
  new Characteristic::Identify(); //this makes possible to find the device
  new DEV_RFControl();//now our device is ready        
} // end of setup()

void loop()
{
  //set the devices listening for updates from home kit
  homeSpan.poll();
} // end of loop()
