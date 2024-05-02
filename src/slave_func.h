#include <core.h>

#ifdef OtoControlDeviceFirmware

void writeEEPROM(int address, int data)
{
    Wire.beginTransmission(EEPROM_ADDRESS); // 24C32'nin adresi
    Wire.write((byte)(address >> 8));       // Yüksek adres byte
    Wire.write((byte)(address & 0xFF));     // Düşük adres byte
    Wire.write((byte)data);                 // Veri
    Wire.endTransmission();
}

// EEPROM'dan veri okuma fonksiyonu
int readEEPROM(int address)
{
    int data = 0;                           // Okunan veri
    Wire.beginTransmission(EEPROM_ADDRESS); // 24C32'nin adresi
    Wire.write((byte)(address >> 8));       // Yüksek adres byte
    Wire.write((byte)(address & 0xFF));     // Düşük adres byte
    Wire.endTransmission(false);

    Wire.requestFrom(EEPROM_ADDRESS, 1); // 1 byte oku
    if (Wire.available())
    {
        data = Wire.read(); // Okunan veriyi al
    }
    return data;
}

// RTC değerlerini EEPROM'a yazma işlemi
void writeRTCtoEEPROM()
{
    writeEEPROM(10, rtc.hour());
    delay(5); // Yazma işlemi için biraz daha uzun bir gecikme ekleyin
    writeEEPROM(20, rtc.minute());
    delay(5);
    writeEEPROM(30, rtc.second());
    delay(5);
    writeEEPROM(40, rtc.day());
    delay(5);
    writeEEPROM(50, rtc.month());
    delay(5);
    writeEEPROM(60, rtc.year());
    delay(5);
    writeEEPROM(70, rtc.dayOfWeek());
    delay(5);
}

void readEEPROMData()
{
    // Veri okuma örneği
    readDatahour = readEEPROM(10);
    readDataminute = readEEPROM(20);
    readDatasecond = readEEPROM(30);
    readDataday = readEEPROM(40);
    readDatamonth = readEEPROM(50);
    readDatayear = readEEPROM(60);
    readDatadayOfWeek = readEEPROM(70);

    Serial.print("~ Eeprom Zamanı   : ");
    Serial.println(String(readDatahour) + ":" + String(readDataminute) + ":" + String(readDatasecond) + "  -  " + String(readDataday) + "/" + String(readDatamonth) + "/" + String(readDatayear) + " - " + String(readDatadayOfWeek));
}

void ZamanGuncelleYazdir()
{
    rtc.refresh();

    Serial.print("Saat: ");
    Serial.print(rtc.hour());
    Serial.print(":");
    Serial.print(rtc.minute());
    Serial.print(":");
    Serial.print(rtc.second());

    Serial.print(" *** Tarih: ");
    Serial.print(rtc.day());
    Serial.print("/");
    Serial.print(rtc.month());
    Serial.print("/");
    Serial.print(rtc.year());
    Serial.print(" *** ");
    Serial.print(rtc.dayOfWeek());
    Serial.println(".gün");

    if (rtcModel == URTCLIB_MODEL_DS1307 || rtcModel == URTCLIB_MODEL_DS3232)
    {
        // Serial.print("~ SRAM position   : ");
        // Serial.print(position);
        // Serial.print(" / value: ");
        // Serial.println(rtc.ramRead(position), HEX);
        position++;
    }

    writeRTCtoEEPROM();

    Serial.flush();
}

void RTCBaslatma()
{
    URTCLIB_WIRE.begin();
    rtc.set_rtc_address(RTC_ADDRESS);
    rtc.set_model(rtcModel);
    rtc.refresh();

    //
    //
    // Yalnızca bir kez kullanın, ardından devre dışı bırakın
    // rtc.set(14, 55, 15, 3, 30, 4, 24);
    // rtc.set(secnd, minute, hour, dayOfWeek, dayOfMonth, month, year)
    //
    //

    rtc.set_12hour_mode(false);

    if (rtc.enableBattery())
    {
        Serial.println("Pil doğru şekilde etkinleştirildi.");
    }
    else
    {
        Serial.println("Pil etkinleştirilirken HATA.");
    }

    // OSC'nin VBAT kullanacak şekilde ayarlanıp ayarlanmadığını kontrol edin
    if (rtc.getEOSCFlag())
        Serial.println(F("Osilatör, VCC kesildiğinde VBAT'ı kullanmayacaktır. VCC olmadan zaman artmaz!"));
    else
        Serial.println(F("Osilatör, VCC kesildiğinde VBAT'ı kullanacaktır."));

    Serial.print("Kayıp güç durumu:");
    if (rtc.lostPower())
    {
        Serial.print("GÜÇ KESİLDİ. Bayrak temizleniyor...");
        rtc.lostPowerClear();
        Serial.println(" Tamamlandı.");
    }
    else
    {
        Serial.println(" GÜÇ TAMAM");
    }

    Serial.print("Yaşlanma kayıt değeri: ");
    Serial.println(rtc.agingGet());

    Serial.println("Yaşlandırma kayıt değeri değiştirilmiyor. Bunu yapmak için şu komutu çalıştırabilirsiniz: rtc.agingSet(newValue)");

    /*
      Serial.println("SQWG/INT çıktısının test edilmesi:");

      Serial.println("fixed 0:");
      rtc.sqwgSetMode(URTCLIB_SQWG_OFF_0);
      delay(5000);

      Serial.println("fixed 1:");
      rtc.sqwgSetMode(URTCLIB_SQWG_OFF_1);
      delay(5000);

      Serial.println("1 hertz:");
      rtc.sqwgSetMode(URTCLIB_SQWG_1H);
      delay(5000);

      Serial.println("1024 hertz:");
      rtc.sqwgSetMode(URTCLIB_SQWG_1024H);
      delay(5000);

      Serial.println("4096 hertz:");
      rtc.sqwgSetMode(URTCLIB_SQWG_4096H);
      delay(5000);

      Serial.println("8192 hertz:");
      rtc.sqwgSetMode(URTCLIB_SQWG_8192H);
      delay(5000);

      Serial.println("32768 hertz:");
      rtc.sqwgSetMode(URTCLIB_SQWG_32768H);
      delay(5000);
    */
    // SRAM'ı ayarlayın (DS3231'de SRAM yoktur, dolayısıyla hiçbir şey saklamaz ve her zaman 0xff değerini döndürür)
    for (position = 0; position < 255; position++)
    {
        rtc.ramWrite(position, position);
    }
    position = 0;
}

void seriport_rapor(int seriport_sure) // sure = ms
{
    if (millis() - lastTime1 > seriport_sure)
    {
        Serial.println();
        Serial.print("***************** ");
        ZamanGuncelleYazdir();

        Serial.print("~ Mesai           : ");
        // Saat 9:30 ile 22:00 arasında ise kontrol true olacak, değilse false olacak
        if (rtc.hour() >= acilissaat and rtc.hour() < kapanissaat)
        {
            if (rtc.hour() < acilissaat + 1)
            {
                if (rtc.minute() >= acilisdakika)
                {
                    zamanKontrolu = true;
                    Serial.println("Mesai İçi");
                }
                else
                {
                    zamanKontrolu = false;
                    Serial.println("Mesai Dışı (Enerji durumu ters yazacaktır)");
                }
            }
            else
            {
                zamanKontrolu = true;
                Serial.println("Mesai İçi");
            }
        }
        else
        {
            zamanKontrolu = false;
            Serial.println("Mesai Dışı (Enerji durumu ters yazacaktır)");
        }

        Serial.print("~ Son Tetikleme   : ");
        Serial.println(timeString);

        Serial.print("~ Kontrol Sıklığı : ");
        Serial.print(kontrol_dakika);
        Serial.println(" dk");

        if (enerji_durumu == true)
        {
            Serial.println("~ Enerji Durumu   : DC Kaynak");
        }
        else
        {
            Serial.println("~ Enerji Durumu   : Batarya");
        }

        Serial.print("~ Batarya Durumu  : ");
        Serial.print(analogRead(batt_in));
        Serial.println(" volt");
        Serial.flush();

        lastTime1 = millis();
    }
}

void TV_AC_IR()
{
    IrSender.sendNEC(0x7F00, 0x15, 0);
}
#endif

#ifdef IRReaderFirmware

#endif
