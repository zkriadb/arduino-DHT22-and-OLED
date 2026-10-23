
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
// SENSOR DHT22
// =============================
#define DHTPIN 4
#define DHTTYPE DHT22

DHT dht(DHTPIN, DHTTYPE);

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
float suhu = 0;
float kelembapan = 0;

String kondisi = "NORMAL";

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

  // Judul
  display.setTextSize(1);

  display.setCursor(0, 0);
  display.println("SUHU RUANGAN");

  display.drawLine(
    0, 10,
    127, 10,
    SSD1306_WHITE
  );

  // Suhu
  display.setCursor(0, 15);

  display.print("Suhu:");

  display.setTextSize(2);

  display.setCursor(0, 25);

  display.print(suhu, 1);

  display.print(" C");

  // Kelembapan
  display.setTextSize(1);

  display.setCursor(0, 47);

  display.print("Humidity: ");

  display.print(kelembapan, 1);

  display.print("%");

  // Kondisi
  display.setCursor(0, 57);

  display.print("Status: ");

  display.print(kondisi);

  display.display();

}

// =============================
// SETUP
// =============================
void setup() {

  Serial.begin(115200);

  // LED
  pinMode(LED_MERAH, OUTPUT);
  pinMode(LED_HIJAU, OUTPUT);
  pinMode(LED_KUNING, OUTPUT);

  semuaMati();

  // Sensor DHT22
  dht.begin();

  // OLED I2C
  Wire.begin(OLED_SDA, OLED_SCL);

  if (!display.begin(
        SSD1306_SWITCHCAPVCC,
        OLED_ADDRESS)) {

    Serial.println("OLED gagal ditemukan!");

    while (true) {
      delay(100);
    }

  }

  // Tampilan awal OLED
  display.clearDisplay();

  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);

  display.setCursor(0, 0);
  display.println("Sistem Suhu Ruangan");

  display.setCursor(0, 20);
  display.println("DHT22 + OLED");

  display.setCursor(0, 40);
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

  // Baca sensor setiap 2 detik
  if (sekarang - waktuDHT >= intervalDHT) {

    waktuDHT = sekarang;

    suhu = dht.readTemperature();

    kelembapan = dht.readHumidity();

    // Cek sensor
    if (isnan(suhu) || isnan(kelembapan)) {

      Serial.println("Gagal membaca DHT22!");

      semuaMati();

      kondisi = "ERROR";

      display.clearDisplay();

      display.setTextSize(1);

      display.setCursor(0, 0);
      display.println("ERROR DHT22!");

      display.setCursor(0, 20);
      display.println("Cek kabel sensor");

      display.display();

      return;

    }

    // Indikator LED
    indikatorSuhu(suhu);

    // Serial Monitor
    Serial.print("Suhu: ");

    Serial.print(suhu, 1);

    Serial.print(" C | Kelembapan: ");

    Serial.print(kelembapan, 1);

    Serial.print(" %RH | Kondisi: ");

    Serial.println(kondisi);

    // Tampilkan data OLED
    tampilkanOLED();

    Serial.println("--------------------");

  }

}
