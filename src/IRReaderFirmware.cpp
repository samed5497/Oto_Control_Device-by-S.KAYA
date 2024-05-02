
#include <slave_func.h>

#ifdef IRReaderFirmware

void setup()
{
  pinMode(BUTTON_PIN2, OUTPUT);
  pinMode(BUTTON_PIN, INPUT);
  digitalWrite(BUTTON_PIN2, HIGH);

  Serial.begin(115200);
  Serial.println("-----------------------------------------");
  Serial.println();
  Serial.println();

  Serial.print(UYGULAMA);
  Serial.print(" - ");
  Serial.print(SURUM);
  Serial.println();
  Serial.print("Developer: ");
  Serial.print(DEVELOPER);
  Serial.println();

  Serial.flush();
  IrReceiver.begin(IR_RECEIVE_PIN, ENABLE_LED_FEEDBACK);

  IrSender.begin();     // Start with IR_SEND_PIN -which is defined in PinDefinitionsAndMore.h- as send pin and enable feedback LED at default feedback LED pin
  disableLEDFeedback(); // Disable feedback LED at default feedback LED pin
}

void loop()
{

  if (IrReceiver.decode())
  {
    if (IrReceiver.decodedIRData.protocol == UNKNOWN)
    {
      Serial.println(F("Received noise or an unknown (or not yet enabled) protocol"));
      // We have an unknown protocol here, print extended info
      IrReceiver.printIRResultRawFormatted(&Serial, true);
      IrReceiver.resume(); // Do it here, to preserve raw data for printing with printIRResultRawFormatted()
    }
    else
    {
      IrReceiver.resume(); // Early enable receiving of the next IR frame
      IrReceiver.printIRResultShort(&Serial);
      IrReceiver.printIRSendUsage(&Serial);
    }
    Serial.println();

    // SINYALE GORE KOMUT İŞLEME
    if (IrReceiver.decodedIRData.command == 0x10)
    {
      // do something
    }
    else if (IrReceiver.decodedIRData.command == 0x11)
    {
      // do something else
    }
  }

  // Butona basıldığında
  if (digitalRead(BUTTON_PIN) == HIGH)
  {
    IrSender.sendNEC(0x7F00, 0x15, 1);
    Serial.println("GONDERILDI");
    delay(500); // Sinyalin tamamlanması için biraz bekleyin
  }
}

#endif
