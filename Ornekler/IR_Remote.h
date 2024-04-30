  z3t0/IRremote @ ^4.3.1



#define DECODE_NEC          // Includes Apple and Onkyo. To enable all protocols , just comment/disable this line.
#include <Arduino.h>

#define IR_RECEIVE_PIN      5
#define IR_SEND_PIN         10
#include <IRremote.hpp> 

void setup() {
    Serial.begin(115200);
    
    IrReceiver.begin(IR_RECEIVE_PIN, ENABLE_LED_FEEDBACK);

}

void loop() {
   
    if (IrReceiver.decode()) {

      
        if (IrReceiver.decodedIRData.protocol == UNKNOWN) {
            Serial.println(F("Received noise or an unknown (or not yet enabled) protocol"));
            // We have an unknown protocol here, print extended info
            IrReceiver.printIRResultRawFormatted(&Serial, true);
            IrReceiver.resume(); // Do it here, to preserve raw data for printing with printIRResultRawFormatted()
        } else {
            IrReceiver.resume(); // Early enable receiving of the next IR frame
            IrReceiver.printIRResultShort(&Serial);
            IrReceiver.printIRSendUsage(&Serial);
        }
        Serial.println();

        /*
         * Finally, check the received data and perform actions according to the received command
         */
        if (IrReceiver.decodedIRData.command == 0x10) {
            // do something
        } else if (IrReceiver.decodedIRData.command == 0x11) {
            // do something else
        }
    }
}
