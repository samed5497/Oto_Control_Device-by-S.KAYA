#include <Arduino.h>

#define UYGULAMA "Oto_Control_Device"
#define SURUM "v1.0"
#define DEVELOPER "github.com/samed5497"

#define led 13
#define buton 12
#define sarj 7
#define role 9
#define batt_in 14 // A0
#define ir_led 10

int role_tetik_suresi = 500;
int kontrol_dakika = 1;
int kontrol_suresi = 0;
bool kontrol_durum, ilk_sefer, enerji_durumu = false;
unsigned long son_kontrol_zaman = 0;

unsigned long lastTime1, son_acma_zaman = 0;
int gecensure_sn, gecensure_dk, gecensure_saat, gecensure_gun, gecensure_hafta = 0;

void seriport_rapor(int seriport_sure) // sure = ms
{
  if (millis() - lastTime1 > seriport_sure)
  {
    gecensure_sn = lastTime1 / 1000;
    gecensure_dk = lastTime1 / 60000;
    gecensure_saat = lastTime1 / 3600000;
    gecensure_gun = lastTime1 / 86400000;
    gecensure_hafta = lastTime1 / 604800000;

    Serial.println();
    Serial.print("Sistem Çalışma Süresi: ");

    if (gecensure_hafta > 0)
    {
      Serial.print(gecensure_hafta);
      Serial.print(" hafta ");
    }
    if (gecensure_gun > 0)
    {
      if (gecensure_gun >= 7)
      {
        Serial.print(gecensure_gun - (gecensure_hafta * 7));
      }
      else
      {
        Serial.print(gecensure_saat);
      }
      Serial.print(" gun ");
    }
    if (gecensure_saat > 0)
    {
      if (gecensure_saat >= 24)
      {
        Serial.print(gecensure_saat - (gecensure_gun * 24));
      }
      else
      {
        Serial.print(gecensure_saat);
      }
      Serial.print(" saat ");
    }
    if (gecensure_dk > 0)
    {
      if (gecensure_dk >= 60)
      {
        Serial.print(gecensure_dk - (gecensure_saat * 60));
      }
      else
      {
        Serial.print(gecensure_dk);
      }
      Serial.print(" dk ");
    }
    if (gecensure_sn >= 60)
    {
      Serial.print(gecensure_sn - (gecensure_dk * 60));
    }
    else
    {
      Serial.print(gecensure_sn);
    }
    Serial.println(" sn");
    Serial.println();

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
}
void setup()
{
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

  pinMode(buton, INPUT_PULLUP);
  pinMode(sarj, INPUT);
  pinMode(role, OUTPUT);
  pinMode(led, OUTPUT);
  pinMode(ir_led, OUTPUT);

  digitalWrite(ir_led, HIGH);
  delay(500);
  digitalWrite(ir_led, LOW);
  delay(250);

  digitalWrite(role, LOW);
  digitalWrite(led, LOW);
}

void loop()
{
  seriport_rapor(1000);

  //    SARJ TAKILI MI DEGİL Mİ KONTROL EDİYORUZ.

  if (digitalRead(sarj) == HIGH)
  {
    enerji_durumu = true;
    digitalWrite(role, LOW);
    digitalWrite(ir_led, LOW);
    son_kontrol_zaman = millis();
  }
  else
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