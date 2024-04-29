
#include <Arduino.h>

#define IR_RECEIVE_PIN      5
#define IR_SEND_PIN         10

#include <IRremote.hpp> // include the library

void setup() {
    pinMode(LED_BUILTIN, OUTPUT);

    Serial.begin(115200);

    IrSender.begin(); // Start with IR_SEND_PIN -which is defined in PinDefinitionsAndMore.h- as send pin and enable feedback LED at default feedback LED pin
    disableLEDFeedback(); // Disable feedback LED at default feedback LED pin
}


void loop() {

    IrSender.sendNEC(0x7F00, 0x15, 1);
    Serial.println("GONDERILDI");


    delay(500);  // delay must be greater than 5 ms (RECORD_GAP_MICROS), otherwise the receiver sees it as one long signal
}
