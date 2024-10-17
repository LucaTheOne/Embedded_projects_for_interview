#ifndef rc.h
#define rc.h
#include <RF433send.h>
#define PIN_GATE 4

byte gate_command_code[] = {0x67,0xe7,0Xc7};
RfSend *tx_whatever = rfsend_builder(
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
void open_gate(){
  byte n = tx_whatever->send(3, gate_command_code);
}
#endif
