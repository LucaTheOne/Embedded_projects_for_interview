
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#include <RF433send.h>

// Wifi network station credentials
#define WIFI_SSID "Vodafone-A62045736"
#define WIFI_PASSWORD "FamigliaBolelli2020@"
// Telegram BOT Token (Get from Botfather)
#define BOT_TOKEN "6846614392:AAG3IkOWxR92wHgJR9WbJzZ10kNWs8SlWsE"
#define PIN_GATE 4

//system const
const unsigned long BOT_MTBS = 500; // mean time between scan messages
const int led_pin = 16;//pin cui è collegato il led
const int gate_pin = 22;//pin cui è collegato il telecomando del cancellone
const String authorized[] = {"1068719667","1942077915","6917270140"};//lista utenti authorizzati
const int user_authorized_number = 3;//numero di utenti autorizzati
byte gate_command_code[] = {0x67,0xe7,0Xc7};

//Objects
WiFiClientSecure secured_client;
UniversalTelegramBot bot(BOT_TOKEN, secured_client);
RfSend *tx_whatever;

//variables
unsigned long bot_lasttime; // last time messages' scan has been done
int ledStatus = 0;//lo stato del led

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
void open_gate(String chat_id){
  byte n = tx_whatever->send(sizeof(gate_command_code), gate_command_code);
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
        open_gate(chat_id);
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

void setup()
{
  Serial.begin(115200);
  Serial.println();

  pinMode(led_pin, OUTPUT); // initialize digital ledPin as an output.
  pinMode(gate_pin, OUTPUT); // initialize digital gate_pin as an output.
  pinMode(PIN_GATE,OUTPUT);

  delay(10);
  digitalWrite(led_pin, HIGH); // initialize pin as off (active LOW)

  // attempt to connect to Wifi network:
  Serial.print("Connecting to Wifi SSID: ");
  Serial.print(WIFI_SSID);
  Serial.println("...");
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED)
  {}
  Serial.print("\nWiFi connected. IP address: ");
  Serial.println(WiFi.localIP());

  // Add root certificate for api.telegram.org
  Serial.println("Adding root certificate for api.telegram.org");
  secured_client.setCACert(TELEGRAM_CERTIFICATE_ROOT); 


  tx_whatever = rfsend_builder(
        RfSendEncoding::TRIBIT_INVERTED,
        PIN_GATE,
        RFSEND_DEFAULT_CONVENTION,  // Do we want to invert 0 and 1 bits? No.
        4,       // Number of sendings
        nullptr, // No callback to keep/stop sending (if you want to send
                 // SO LONG AS a button is pressed, the function reading the
                 // button state is to be put here).
        24332,    // initseq
        0,       // lo_prefix
        0,       // hi_prefix
        300,       // first_lo_ign
        300,    // lo_short
        554,    // lo_long
        0,       // hi_short
        0,       // hi_long
        304,       // lo_last
        24332,    // sep
        24       // nb_bits
    );

  /*
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(500);
  }*/
  /*
  Serial.print("Retrieving time: ");
  configTime(0, 0, "pool.ntp.org"); // get UTC time via NTP
  time_t now = time(nullptr);
  while (now < 24 * 3600)
  {
    Serial.print(".");
    delay(100);
    now = time(nullptr);
  }
  Serial.println(now);*/
}

void loop()
{
  if (millis() - bot_lasttime > BOT_MTBS)
  {
    int numNewMessages = bot.getUpdates(bot.last_message_received + 1);

    while (numNewMessages)
    {
      Serial.println("got response");
      handleNewMessages(numNewMessages);
      numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    }

    bot_lasttime = millis();
  }
}
