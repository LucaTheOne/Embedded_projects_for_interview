#include "DEV_RFControl.h"
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#include <HomeSpan.h>
#include "rc.h"

// Telegram BOT Token (Get from Botfather)
#define BOT_TOKEN "6846614392:AAG3IkOWxR92wHgJR9WbJzZ10kNWs8SlWsE"
#define PIN_GATE 4

//system const
const unsigned long BOT_MTBS = 500; // mean time between scan messages
const String authorized[] = {"1068719667","1942077915","6917270140"};//lista utenti authorizzati
const int user_authorized_number = 3;//numero di utenti autorizzati

//variables
unsigned long bot_lasttime; // last time messages' scan has been done

//Objects
WiFiClientSecure secured_client;
UniversalTelegramBot bot(BOT_TOKEN, secured_client);



//ritorna true se l' id utente passato come argomento è nella lista degli utenti autorizzati
bool is_user_authorized(String user_id);

//attiva l' apertura del cancellone.
void open_tg(String chat_id);

void handleNewMessages(int numNewMessages);

void setup()
{
  Serial.begin(115200);
  pinMode(PIN_GATE,OUTPUT);

  /*
  Serial.println(F("Initializing Arduino cloud..."));
  init_arduino_cloud();
  ArduinoCloud.begin(ArduinoIoTPreferredConnection);
  setDebugMessageLevel(2);
  ArduinoCloud.printDebugInfo();
  */

  // Add root certificate for api.telegram.org
  Serial.println(F("Adding root certificate for api.telegram.org"));
  secured_client.setCACert(TELEGRAM_CERTIFICATE_ROOT); 

  Serial.println(F("Adding ios services..."));
  homeSpan.begin(Category::Locks,"Apertura cancello");
  
  new SpanAccessory(); 
    new Service::AccessoryInformation(); 
      new Characteristic::Identify();
    new DEV_RFControl(4);  
    
}

void loop()
{

  homeSpan.poll();

  if (millis() - bot_lasttime > BOT_MTBS)
  {
    int numNewMessages = bot.getUpdates(bot.last_message_received + 1);

    while (numNewMessages)
    {
      Serial.println(F("got response"));
      handleNewMessages(numNewMessages);
      numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    }

    bot_lasttime = millis();
  }
}

//ritorna true se l' id utente passato come argomento è nella lista degli utenti autorizzati
bool is_user_authorized(String user_id)
{
  //check user authorization
  for(int i = 0; i< user_authorized_number;i++){
    if (authorized[i] == user_id) return true;
  } 
  return false;
}

//attiva l' apertura del cancellone.
void open_tg(String chat_id)
{
  open_gate();
  String message = "ciao "+ bot.messages[0].from_name + " ti sto aprendo il cancellone...";
  bot.sendMessage(chat_id, message, "");
}

void handleNewMessages(int numNewMessages)
{
  Serial.print("handleNewMessages: ");
  Serial.println(numNewMessages);

  for (int i = 0; i < numNewMessages; i++){
    String chat_id = bot.messages[i].chat_id;
    String text = bot.messages[i].text;

    if(is_user_authorized(chat_id))
    {   
      if(text == "/apri_cancello"){
        open_tg(chat_id);
      } else if (text.equalsIgnoreCase("/start")||text.equalsIgnoreCase("info"))
      {
        String from_name = bot.messages[i].from_name;
        if (from_name == "")
          from_name = "Guest";
        String welcome = "Benvenuto,io sono il bot a gestione della tua casa, " + from_name + "<3.\n";
        welcome += "Comandi disponibili:\n -> /apri_cancello per aprire il cancellone!\n";
        bot.sendMessage(chat_id, welcome);
      } else {bot.sendMessage(chat_id, "Operazione non prevista!");}
    } else {bot.sendMessage(chat_id, "Utente non autorizzato! - User isn't allowed to operate with this bot!", "Markdown");}
  }
}