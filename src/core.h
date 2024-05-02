#include <Arduino.h>

// #define OtoControlDeviceFirmware
// #define IRReaderFirmware

/*

Protocol=NEC Address=0x7F00 Command=0x15 Raw-Data=0xEA157F00 32 bits LSB first
Send with: IrSender.sendNEC(0x7F00, 0x15, <numberOfRepeats>);

*/

#ifdef OtoControlDeviceFirmware

#include "uRTCLib.h"
#define UYGULAMA "Oto_Control_Device"
#define SURUM "v1.0"
#define DEVELOPER "github.com/samed5497"

/* KALİBRE EDİLEBİLİR DEĞİŞKEN TANIMLAMALARI */

int role_tetik_suresi = 500;
int acilissaat = 10;
int acilisdakika = 30;
int kapanissaat = 22;
// + SAAT ÇEKİRDEK YAZILIMI

/* PİN TANIMLAMALARI */
#define bildirim_ledi 13
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

#endif

#ifdef IRReaderFirmware

#define DECODE_NEC // Includes Apple and Onkyo. To enable all protocols , just comment/disable this line.

#define IR_RECEIVE_PIN 5 // IR alıcının sinyal pini
#define BUTTON_PIN 7     // Buton pin numarası
#define BUTTON_PIN2 6    // Buton pin numarası
#define IR_SEND_PIN 10   // IR LED'in pin numarası

#include <IRremote.hpp> // include the library

#define UYGULAMA "IR_Okuyucu_Device"
#define SURUM "v1.0"
#define DEVELOPER "github.com/samed5497"

#endif
