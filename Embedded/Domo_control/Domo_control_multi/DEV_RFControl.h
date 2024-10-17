#include "rc.h"
#include <HomeSpan.h>
////////////////////////////////////
//   DEVICE-SPECIFIC RC_Gate SERVICES //
////////////////////////////////////

// HERE'S WHERE WE DEFINE OUR NEW SERVICE!
struct DEV_RFControl : Service::Switch {               // First we create a derived class from the HomeSpan LightBulb Service

  int control_pin;
  SpanCharacteristic *power;                        // here we create a generic pointer to a SpanCharacteristic named "power" that we will use below

  // Next we define the constructor for DEV_RFControl.  
  //Note that it takes one argument, control_pin,
  // which specifies the pin to which the LED is attached.
  
  DEV_RFControl(int control_pin /*pin I/O dedicated to the use of the rf emitter*/ ) : Service::Switch()
  {
    power=new Characteristic::On(); // this is where we create the On Characterstic we had defined in setup().
                                    //Save this in the pointer created above, for use below.
    this->control_pin=control_pin; // Storing control pin.
    pinMode(control_pin,OUTPUT); // ...and set the mode for control_pin to be an OUTPUT (standard Arduino function).  
  } // end constructor

  // now ,became necessary to over-ride the default update() method with instructions that actually command the gate; Note update() returns type boolean.

  boolean update()
  {    
    if(power->getNewVal())
    {
      open_gate();  
    }
    return true;  // return true to indicate the operation was successful, then the gate received command to open else false;
  } // update overriding

  void loop()
  {
    if(power->getVal() && power->timeVal()>500)
    {   // check that power is true, and that time since last modification is greater than 3 seconds 
      power->setVal(false);                         // set power to false
    }      
  }
};