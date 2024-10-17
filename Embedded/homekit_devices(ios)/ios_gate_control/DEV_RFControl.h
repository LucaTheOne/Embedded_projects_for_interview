#include "rf433_data.h"
#include <HomeSpan.h>
//////////////////////////////////////
// DEVICE-SPECIFIC RC_Gate SERVICES //
//////////////////////////////////////

/* 
* Here's the devices is built!
* in this case, switch is chosen between possibilities to 
* represent this device in home kit
*/

/*
* First of all,a derived class from the HomeSpan Switch Service is made up
*/
struct DEV_RFControl : Service::Switch 
{ 

  SpanCharacteristic *power; // creating a generic pointer to a SpanCharacteristic named "power" that we will use below

  /*
  * Next we define the constructor for DEV_RFControl.  
  * Note that it takes no argument, because they are all defined
  * in the file rf433_data.h
  */
  DEV_RFControl() : Service::Switch()
  {
    power=new Characteristic::On(); // this is where is created the On Characterstic we had defined in setup() and Save this in the pointer created above, for use below.
  } // end constructor

  /* 
  * It's necessary now to over-ride the default update() method 
  * with instructions that actually command the gate: open_gate from rf433_data.ino 
  * Note that update() returns type boolean.
  */

  boolean update()
  {    
    if(power->getNewVal()) //when the state of the button change the gate is opended
    {
      open_gate();  
    }
    return true;  // return true to indicate the operation was successful, then the gate received command to open else false;
  } // update overriding

  void loop()
  {
    if(power->getVal() && power->timeVal()>500)
    {   // check that power is true, and that time since last modification is greater than half second
      power->setVal(false);// set power to false
      /*
      * this slice of code make the switch acting like a button, resetting is state
      * to false after 0,5 sec
      */
    }      
  }
};