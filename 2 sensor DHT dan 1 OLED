
#include <Wire.h>
#include <DHT.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// =============================
// PIN LED
// =============================
#define LED_MERAH 18
#define LED_HIJAU 19
#define LED_KUNING 16

// =============================
// DUA SENSOR DHT22
// =============================
#define DHTPIN1 4
#define DHTPIN2 32
#define DHTTYPE DHT22

DHT dht1(DHTPIN1, DHTTYPE);
DHT dht2(DHTPIN2, DHTTYPE);

// =============================
// OLED 0.96 INCH
// =============================
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define OLED_ADDRESS 0x3C

#define OLED_SDA 27
#define OLED_SCL 26

Adafruit_SSD1306 display(
  SCREEN_WIDTH,
  SCREEN_HEIGHT,
  &Wire,
  OLED_RESET
);

// =============================
// BATAS SUHU
// =============================
const float BATAS_DINGIN = 20.0;
const float BATAS_PANAS = 30.0;

// =============================
// INTERVAL SENSOR
// =============================
unsigned long waktuDHT = 0;
const unsigned long intervalDHT = 2000;

// =============================
// VARIABEL SENSOR
// =============================
float suhu1, suhu2;
float kelembapan1, kelembapan2;
float suhuRataRata = 0;

bool sensor1OK = false;
bool sensor2OK = false;

String kondisi = "NORMAL";

// =============================
// TANGGAL OFFLINE
// =============================
// 30 Oktober 2026 = Jumat
// Minggu = 0, Senin = 1, ..., Jumat = 5

int tanggal = 30;
int bulan = 10;
int tahun = 2026;
int hari = 5;

unsigned long waktuTanggal = 0;
const unsigned long satuHari = 86400000UL;

const char* namaHari[] = {
  "Min", "Sen", "Sel", "Rab",
  "Kam", "Jum", "Sab"
};

// =============================
// CEK TAHUN KABISAT
// =============================
bool tahunKabisat(int th) {
  return (th % 400 == 0) ||
         (th % 4 == 0 && th % 100 != 0);
}

// =============================
// JUMLAH HARI DALAM BULAN
// =============================
int jumlahHariBulan(int bln, int th) {
  int jumlah[] = {
    31, 28, 31, 30, 31, 30,
    31, 31, 30, 31, 30, 31
  };

  if (bln == 2 && tahunKabisat(th)) {
    return 29;
  }

  return jumlah[bln - 1];
}

// =============================
// PERBARUI TANGGAL
// =============================
void perbaruiTanggal() {
  unsigned long sekarang = millis();

  while (sekarang - waktuTanggal >= satuHari) {
    waktuTanggal += satuHari;

    tanggal++;
    hari = (hari + 1) % 7;

    if (tanggal > jumlahHariBulan(bulan, tahun)) {
      tanggal = 1;
      bulan++;

      if (bulan > 12) {
        bulan = 1;
        tahun++;
      }
    }
  }
}

// =============================
// MATIKAN SEMUA LED
// =============================
void semuaMati() {
  digitalWrite(LED_MERAH, LOW);
  digitalWrite(LED_HIJAU, LOW);
  digitalWrite(LED_KUNING, LOW);
}

// =============================
// INDIKATOR SUHU
// =============================
void indikatorSuhu(float suhu) {
  semuaMati();

  if (suhu < BATAS_DINGIN) {
    digitalWrite(LED_HIJAU, HIGH);
    kondisi = "DINGIN";
  }
  else if (suhu < BATAS_PANAS) {
    digitalWrite(LED_KUNING, HIGH);
    kondisi = "NORMAL";
  }
  else {
    digitalWrite(LED_MERAH, HIGH);
    kondisi = "PANAS";
  }
}

// =============================
// TAMPILKAN OLED
// =============================
void tampilkanOLED() {
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(1);

  display.setCursor(0, 0);
  display.println("MONITOR SUHU RUANGAN");

  display.drawLine(0, 10, 127, 10, SSD1306_WHITE);

  display.setCursor(0, 13);
  display.print("DHT1: ");

  if (sensor1OK) {
    display.print(suhu1, 1);
    display.print("C ");
    display.print(kelembapan1, 0);
    display.print("%");
  } else {
    display.print("ERROR");
  }

  display.setCursor(0, 23);
  display.print("DHT2: ");

  if (sensor2OK) {
    display.print(suhu2, 1);
    display.print("C ");
    display.print(kelembapan2, 0);
    display.print("%");
  } else {
    display.print("ERROR");
  }

  display.drawLine(0, 34, 127, 34, SSD1306_WHITE);

  display.setCursor(0, 37);
  display.print("Rata-rata: ");

  if (sensor1OK || sensor2OK) {
    display.print(suhuRataRata, 1);
    display.print(" C");
  } else {
    display.print("--");
  }

  display.setCursor(0, 47);
  display.print("Status: ");
  display.print(kondisi);

  display.setCursor(0, 57);
  display.print(namaHari[hari]);
  display.print(", ");

  if (tanggal < 10) display.print("0");
  display.print(tanggal);
  display.print("/");

  if (bulan < 10) display.print("0");
  display.print(bulan);

  display.print("/");
  display.print(tahun);

  display.display();
}

// =============================
// SETUP
// =============================
void setup() {
  Serial.begin(115200);

  pinMode(LED_MERAH, OUTPUT);
  pinMode(LED_HIJAU, OUTPUT);
  pinMode(LED_KUNING, OUTPUT);

  semuaMati();

  dht1.begin();
  dht2.begin();

  Wire.begin(OLED_SDA, OLED_SCL);

  if (!display.begin(
        SSD1306_SWITCHCAPVCC,
        OLED_ADDRESS)) {

    Serial.println("OLED gagal ditemukan!");

    while (true) {
      delay(100);
    }
  }

  waktuTanggal = millis();

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);

  display.setCursor(0, 0);
  display.println("Sistem Monitoring");

  display.setCursor(0, 15);
  display.println("2x DHT22 + OLED");

  display.setCursor(0, 30);
  display.println("Memulai...");

  display.display();

  Serial.println("Sistem siap!");

  delay(1000);
}

// =============================
// LOOP
// =============================
void loop() {
  unsigned long sekarang = millis();

  perbaruiTanggal();

  if (sekarang - waktuDHT >= intervalDHT) {
    waktuDHT = sekarang;

    suhu1 = dht1.readTemperature();
    kelembapan1 = dht1.readHumidity();

    suhu2 = dht2.readTemperature();
    kelembapan2 = dht2.readHumidity();

    sensor1OK = !isnan(suhu1) && !isnan(kelembapan1);
    sensor2OK = !isnan(suhu2) && !isnan(kelembapan2);

    // HITUNG RATA-RATA SUHU
    if (sensor1OK && sensor2OK) {
      suhuRataRata = (suhu1 + suhu2) / 2.0;
    }
    else if (sensor1OK) {
      suhuRataRata = suhu1;
    }
    else if (sensor2OK) {
      suhuRataRata = suhu2;
    }

    // CEK KONDISI SENSOR
    if (!sensor1OK && !sensor2OK) {
      semuaMati();
      kondisi = "ERROR";

      Serial.println("Kedua sensor DHT22 gagal dibaca!");
    }
    else {
      indikatorSuhu(suhuRataRata);

      Serial.print("Suhu DHT1: ");
      if (sensor1OK) Serial.print(suhu1);
      else Serial.print("ERROR");

      Serial.print(" C | Suhu DHT2: ");
      if (sensor2OK) Serial.print(suhu2);
      else Serial.print("ERROR");

      Serial.print(" C | Rata-rata: ");
      Serial.print(suhuRataRata);

      Serial.print(" C | Kondisi: ");
      Serial.println(kondisi);
    }

    tampilkanOLED();

    Serial.println("--------------------");
  }
}
