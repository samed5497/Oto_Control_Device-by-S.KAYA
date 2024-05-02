#include <slave_func.h>

#ifdef OtoControlDeviceFirmware

void setup()
{
  delay(1000);

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

  pinMode(role, OUTPUT);
  pinMode(bildirim_ledi, OUTPUT);
  pinMode(buton, INPUT_PULLUP);
  pinMode(sarj, INPUT);
  pinMode(batt_in, INPUT);

  Wire.begin();
  RTCBaslatma();

  digitalWrite(role, LOW);
  digitalWrite(bildirim_ledi, LOW);

  IrSender.begin();     // Start with IR_SEND_PIN -which is defined in PinDefinitionsAndMore.h- as send pin and enable feedback LED at default feedback LED pin
  disableLEDFeedback(); // Disable feedback LED at default feedback LED pin
}

void loop()
{
  seriport_rapor(1000);

  if (zamanKontrolu) // Ayarlanan zaman içindeyse kapatmak için kontrol et ve hareket et.
  {
    if (digitalRead(sarj) == HIGH) // Cihaz açıksa yani şarj ediliyorsa sonraki kontrole kadar bir şey yapma.
    {
      enerji_durumu = true;
      digitalWrite(role, LOW);
      son_kontrol_zaman = millis();
    }
    else // Ayarlanan zaman içerisinde cihaz kapanmışsa tekrar açmak için gerekli işlemleri yap.
    {
      enerji_durumu = false;

      if ((millis() - son_kontrol_zaman) >= (kontrol_dakika * 60000) and kontrol_durum == false)
      {
        kontrol_durum = true;
        son_kontrol_zaman = millis();
      }

      if (kontrol_durum == true)
      {
        TV_AC_IR();
        digitalWrite(role, HIGH);

        if (ilk_sefer == false)
        {
          Serial.println();
          Serial.println("---------- ROLE TETİKLENDİ ----------");
          Serial.print("---------- ");
          timeString = String(rtc.hour()) + ":" + String(rtc.minute()) + ":" + String(rtc.second());
          Serial.print(timeString);
          Serial.println(" ----------");

          readEEPROMData();

          son_acma_zaman = millis();
          ilk_sefer = true;
        }

        if (millis() - son_acma_zaman >= role_tetik_suresi)
        {
          digitalWrite(role, LOW);
          kontrol_durum = false;
          ilk_sefer = false;
        }
      }
    }
  }
  else // Ayarlanan zaman dışındaysa kapatmak için kontrol et ve hareket et. YUKARDAKİ DONGUNUN TAM TERSİ VE YER DEGİŞTİRİLMİŞ HALİ.
  {
    if (digitalRead(sarj) == HIGH) // Mesai saati dışında cihaz açıksa yani şarj ediliyorsa tekrar kapatmak için gerekli işlemleri yap.
    {
      enerji_durumu = false;

      if ((millis() - son_kontrol_zaman) >= (kontrol_dakika * 60000) and kontrol_durum == false)
      {
        kontrol_durum = true;
        son_kontrol_zaman = millis();
      }

      if (kontrol_durum == true)
      {
        TV_AC_IR();
        digitalWrite(role, HIGH);

        if (ilk_sefer == false)
        {
          son_acma_zaman = millis();
          ilk_sefer = true;
        }

        if (millis() - son_acma_zaman >= role_tetik_suresi)
        {
          digitalWrite(role, LOW);
          kontrol_durum = false;
          ilk_sefer = false;
        }
      }
    }
    else // Mesai saati dışında cihaz kapanmışsa sonraki kontrole kadar hiç bir şey yapma.
    {
      enerji_durumu = true;
      digitalWrite(role, LOW);
      son_kontrol_zaman = millis();
    }
  }

  if (digitalRead(buton) == LOW)
  {
    while (digitalRead(buton) == HIGH)

      Serial.println("[BILGI]: Butona Basıldı.");

    if (kontrol_dakika == 1)
    {
      kontrol_dakika = 5;
      for (int i = 0; i < kontrol_dakika; i += 5)
      {
        {
          digitalWrite(bildirim_ledi, HIGH);
          delay(250);
          digitalWrite(bildirim_ledi, LOW);
          delay(250);
        }
      }
    }

    else if (kontrol_dakika == 5)
    {
      kontrol_dakika = kontrol_dakika + 5;
      for (int i = 0; i < kontrol_dakika; i += 5)
      {
        {
          digitalWrite(bildirim_ledi, HIGH);
          delay(250);
          digitalWrite(bildirim_ledi, LOW);
          delay(250);
        }
      }
    }

    else if (kontrol_dakika == 10)
    {
      kontrol_dakika = kontrol_dakika + 5;
      for (int i = 0; i < kontrol_dakika; i += 5)
      {
        {
          digitalWrite(bildirim_ledi, HIGH);
          delay(250);
          digitalWrite(bildirim_ledi, LOW);
          delay(250);
        }
      }
    }

    else if (kontrol_dakika == 15)
    {
      kontrol_dakika = kontrol_dakika + 5;
      for (int i = 0; i < kontrol_dakika; i += 5)
      {
        {
          digitalWrite(bildirim_ledi, HIGH);
          delay(250);
          digitalWrite(bildirim_ledi, LOW);
          delay(250);
        }
      }
    }
    else if (kontrol_dakika == 20)
    {
      kontrol_dakika = kontrol_dakika + 5;
      for (int i = 0; i < kontrol_dakika; i += 5)
      {
        {
          digitalWrite(bildirim_ledi, HIGH);
          delay(250);
          digitalWrite(bildirim_ledi, LOW);
          delay(250);
        }
      }
    }

    else if (kontrol_dakika == 25)
    {
      kontrol_dakika = kontrol_dakika + 5;
      for (int i = 0; i < kontrol_dakika; i += 5)
      {
        {
          digitalWrite(bildirim_ledi, HIGH);
          delay(250);
          digitalWrite(bildirim_ledi, LOW);
          delay(250);
        }
      }
    }

    else if (kontrol_dakika == 30)
    {
      kontrol_dakika = 1;
      {
        digitalWrite(bildirim_ledi, HIGH);
        delay(50);
        digitalWrite(bildirim_ledi, LOW);
        delay(50);
      }
    }
    Serial.print("[BILGI]: Kontrol Aralığı: ");
    Serial.print(kontrol_dakika);
    Serial.println(" dk");
  }
}

#endif
