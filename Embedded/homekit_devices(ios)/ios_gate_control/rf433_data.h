#include <RF433send.h>

//pin of 433rf module emitter
#define PIN_GATE 4
#define N_SENDINGS = 4
#define LONG_PRESSED = nullptr
#define INIT_SEQUENCE = 24332
#define LOW_PREFIX = 0
#define HIGH_PREFIX = 0
#define FIRST_LOW_IGN = 300
#define LOW_SHORT = 300
#define LOW_LONG = 554
#define HIGH_SHORT = 0
#define HIGH_LONG = 0
#define LOW_LASTING = 304
#define SEPARATOR = 24332
#define N_BITS = 24


const byte gate_command_code[] = {0x67,0xe7,0Xc7};

//funcion to build the object needed to use the rf433 mhz module
RfSend *tx_whatever = rfsend_builder(
        RfSendEncoding::TRIBIT_INVERTED,//from rf433 module library
        PIN_GATE,
        RFSEND_DEFAULT_CONVENTION,  // Do we want to invert 0 and 1 bits? No in this scenario.
        N_SENDINGS,       // Number of sendings
        LONG_PRESSED, 
        /*
        * No callback to keep/stop sending (if you want to send
        * SO LONG AS a button is pressed, the function reading the
        * button state is to be put here or in the define section).
        */
        INIT_SEQUENCE,    // initseq
        LOW_PREFIX,       // lo_prefix
        HIGH_PREFIX,       // hi_prefix
        FIRST_LOW_IGN,       // first_lo_ign
        LOW_SHORT,    // lo_short
        LOW_LONG,    // lo_long
        HIGH_SHORT,       // hi_short
        HIGH_LONG,       // hi_long
        LOW_LASTING,       // lo_last
        SEPARATOR,    // sep
        N_BITS       // nb_bits
);

/*function to call to open the gate*/
void open_gate(){
  byte n = tx_whatever->send(sizeof(gate_command_code), gate_command_code);
  //for debugging purpose uncomment line below
  //Serial.println("Gate opening...");
}