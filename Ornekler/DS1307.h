
naguissa / uRTCLib @^6.9.2

#include "Arduino.h"
#include "uRTCLib.h"

uRTCLib rtc;
byte rtcModel = URTCLIB_MODEL_DS1307;
uint8_t position;

void ZamanGuncelleYazdir()
{
  rtc.refresh();

  Serial.print("Zaman: ");
  Serial.print(' ');
  Serial.print(rtc.hour());
  Serial.print('.');
  Serial.print(rtc.minute());
  Serial.print('.');
  Serial.print(rtc.second());

  Serial.print(" / Gün : ");
  Serial.print(rtc.day());
  Serial.print('/');
  Serial.print(rtc.month());
  Serial.print('/');
  Serial.print(rtc.year());
  Serial.print(" - ");
  Serial.print(rtc.dayOfWeek());
  Serial.println(".gün");
  Serial.flush();
}

void RTCBaslatma()
{
  URTCLIB_WIRE.begin();
  rtc.set_rtc_address(0x68);
  rtc.set_model(rtcModel);
  rtc.refresh();

  //
  //
  // Yalnızca bir kez kullanın, ardından devre dışı bırakın
  // rtc.set(15, 23, 20, 5, 25, 4, 24);
  // rtc.set(second, minute, hour, dayOfWeek, dayOfMonth, month, year)
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

void setup()
{
  delay(1000);
  Serial.begin(115200);
  RTCBaslatma();
}

void loop()
{
  ZamanGuncelleYazdir();

  delay(1000);
}