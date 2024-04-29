#include <Arduino.h>

//#define OtoControlDeviceFirmware
// #define IROkuyucuFirmware

#ifdef OtoControlDeviceFirmware
#include "uRTCLib.h"
#define UYGULAMA "Oto_Control_Device"
#define SURUM "v1.0"
#define DEVELOPER "github.com/samed5497"

/* KALİBRE EDİLEBİLİR DEĞİŞKEN TANIMLAMALARI */

int role_tetik_suresi = 500;
int acilissaat = 10;
int acilisdakika = 30;
int kapanissaat = 23;
// + SAAT ÇEKİRDEK YAZILIMI

/* PİN TANIMLAMALARI */
#define led 13
#define buton 12
#define sarj 7
#define role 9
#define batt_in 14 // A0
#define IR_SEND_PIN 10
#include <IRremote.hpp> // include the library

#define RTC_ADDRESS 0x68    // DS1307 RTC adresi
#define EEPROM_ADDRESS 0x50 // 24c32 adresi

/* GENEL DEĞİŞKEN TANIMLAMALARI */
int kontrol_dakika = 1;
int kontrol_suresi = 0;
bool kontrol_durum, ilk_sefer, enerji_durumu = false;
unsigned long son_kontrol_zaman = 0;

unsigned long lastTime1, son_acma_zaman = 0;
int gecensure_sn, gecensure_dk, gecensure_saat, gecensure_gun, gecensure_hafta = 0;

bool zamanKontrolu;
String timeString = "Yok";

/* ZAMAN DEĞİŞKEN TANIMLAMALARI */
uRTCLib rtc;
byte rtcModel = URTCLIB_MODEL_DS1307;
uint8_t position;

int readDatahour, readDataminute, readDatasecond, readDataday, readDatamonth, readDatayear, readDatadayOfWeek;
/* FONKSİYONLAR */
// EEPROM'a veri yazma fonksiyonu
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
  // rtc.set(14, 6, 13, 6, 26, 4, 24);
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
  IrSender.sendNEC(0x7F00, 0x15, 1);
}

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
  pinMode(led, OUTPUT);
  pinMode(buton, INPUT_PULLUP);
  pinMode(sarj, INPUT);
  pinMode(batt_in, INPUT);

  Wire.begin();
  RTCBaslatma();


  digitalWrite(role, LOW);
  digitalWrite(led, LOW);

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

      if ((millis() - son_kontrol_zaman) >= (kontrol_dakika * 6000) and kontrol_durum == false)
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

      if ((millis() - son_kontrol_zaman) >= (kontrol_dakika * 6000) and kontrol_durum == false)
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
          digitalWrite(led, HIGH);
          delay(250);
          digitalWrite(led, LOW);
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
          digitalWrite(led, HIGH);
          delay(250);
          digitalWrite(led, LOW);
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
          digitalWrite(led, HIGH);
          delay(250);
          digitalWrite(led, LOW);
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
          digitalWrite(led, HIGH);
          delay(250);
          digitalWrite(led, LOW);
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
          digitalWrite(led, HIGH);
          delay(250);
          digitalWrite(led, LOW);
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
          digitalWrite(led, HIGH);
          delay(250);
          digitalWrite(led, LOW);
          delay(250);
        }
      }
    }

    else if (kontrol_dakika == 30)
    {
      kontrol_dakika = 1;
      {
        digitalWrite(led, HIGH);
        delay(50);
        digitalWrite(led, LOW);
        delay(50);
      }
    }
    Serial.print("[BILGI]: Kontrol Aralığı: ");
    Serial.print(kontrol_dakika);
    Serial.println(" dk");
  }
}

#endif

// BIN = 11101010000101010111111100000000
// HEX = 0xEA157F00
// DEC = 3927277312
/*

Protocol=NEC Address=0x7F00 Command=0x15 Raw-Data=0xEA157F00 32 bits LSB first
Send with: IrSender.sendNEC(0x7F00, 0x15, <numberOfRepeats>);

*/

#ifdef IROkuyucuFirmware

#define DECODE_NEC // Includes Apple and Onkyo. To enable all protocols , just comment/disable this line.

#define IR_RECEIVE_PIN 5 // IR alıcının sinyal pini
#define BUTTON_PIN 7     // Buton pin numarası
#define BUTTON_PIN2 6    // Buton pin numarası
#define IR_SEND_PIN 10   // IR LED'in pin numarası

#include <IRremote.hpp> // include the library

#define UYGULAMA "IR_Okuyucu_Device"
#define SURUM "v1.0"
#define DEVELOPER "github.com/samed5497"

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
    delay(1000); // Sinyalin tamamlanması için biraz bekleyin
  }
}

#endif
