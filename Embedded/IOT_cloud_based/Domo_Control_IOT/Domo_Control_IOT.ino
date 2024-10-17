
#include "IOT_cloud_WiFi_data.h"
#include "rf433_data.h"
#include <UniversalTelegramBot.h>
#include <WiFiClientSecure.h>

//#include "TG_bot_data.h"
void setup() {
  // Initialize serial and wait for port to open:
  Serial.begin(115200);
  // This delay gives the chance to wait for a Serial Monitor without blocking if none is found
  delay(1500); 

  // Defined in IOT_cloud_WiFi_data.h
  init_arduino_cloud();
  // Connect to Arduino IoT Cloud
  ArduinoCloud.begin(ArduinoIoTPreferredConnection);
  //initialize connection to bot
  secured_client.setCACert(TELEGRAM_CERTIFICATE_ROOT); 

  Serial.println("Connected with telegram bot...");
  
  /*
     The following function allows you to obtain more information
     related to the state of network and IoT Cloud connection and errors
     the higher number the more granular information you’ll get.
     The default is 0 (only errors).
     Maximum is 4
 */
  setDebugMessageLevel(2);
  ArduinoCloud.printDebugInfo();

  //pin to send data to emitter
  pinMode(PIN_GATE,OUTPUT);

  Serial.println("All systems ready!");
}

void loop() {
  ArduinoCloud.update();
  /*
  if(millis()- bot_lasttime > BOT_MTBS)
  {
    int numNewMessages = bot.getUpdates(bot.last_message_received + 1);

    while (numNewMessages)
    {
      Serial.println("got response from telegram bot...");
      handleNewMessages(numNewMessages);
      numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    }

    bot_lasttime = millis();
  }*/
  
}

/*
  Since OpenGateCommand is READ_WRITE variable, onOpenGateCommandChange() is
  executed every time a new value is received from IoT Cloud.
*/
void onOpenGateCommandChange()  {
  Serial.println("IOT variable changed...");
  if(open_gate_command){
    open_gate();
  }
}
