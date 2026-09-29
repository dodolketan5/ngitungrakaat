// =========================
// Definisi Pin GPIO
// =========================
const int TRIG_PIN = 27;  // Pin TRIG HC-SR04
const int ECHO_PIN = 33;  // Pin ECHO HC-SR04
const int LED_PIN  = 32;  // Pin LED

// =========================
// Pengaturan Deteksi
// =========================
const float BATAS_SUJUD = 5.0;  // cm

// =========================
// Variabel Penghitung
// =========================
int jumlahSujud = 0;
int jumlahRakaat = 0;

bool sedangSujud = false;


// =========================
// SETUP
// =========================
void setup() {
  Serial.begin(115200);

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(LED_PIN, OUTPUT);

  digitalWrite(TRIG_PIN, LOW);
  digitalWrite(LED_PIN, LOW);

  Serial.println("=================================");
  Serial.println("   Counter Sujud & Rakaat Siap   ");
  Serial.println("=================================");
  Serial.println();
}


// =========================
// LOOP
// =========================
void loop() {

  // -------------------------
  // Kirim pulsa trigger
  // -------------------------
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);

  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);

  digitalWrite(TRIG_PIN, LOW);


  // -------------------------
  // Baca waktu pantulan ECHO
  // Timeout 30 ms
  // -------------------------
  long duration = pulseIn(ECHO_PIN, HIGH, 30000);


  // -------------------------
  // Hitung jarak
  // -------------------------
  float distance;

  if (duration == 0) {
    distance = -1;
  } 
  else {
    distance = duration * 0.0343 / 2.0;
  }


  // -------------------------
  // Tampilkan jarak
  // -------------------------
  if (distance > 0) {
    Serial.print("Jarak objek: ");
    Serial.print(distance);
    Serial.println(" cm");
  } 
  else {
    Serial.println("Objek tidak terdeteksi");
  }


  // -------------------------
  // Deteksi sujud
  // -------------------------
  if (distance > 0 && distance <= BATAS_SUJUD) {

    digitalWrite(LED_PIN, HIGH);

    // Supaya satu sujud tidak dihitung berkali-kali
    if (!sedangSujud) {

      sedangSujud = true;
      jumlahSujud++;

      // Setiap 2 sujud = 1 rakaat
      jumlahRakaat = jumlahSujud / 2;

      Serial.println();
      Serial.println("---------------------------------");
      Serial.println(">> SUJUD TERDETEKSI!");

      Serial.print(">> Total Sujud   : ");
      Serial.println(jumlahSujud);

      Serial.print(">> Jumlah Rakaat : ");
      Serial.println(jumlahRakaat);

      Serial.println("---------------------------------");
      Serial.println();
    }

  } 
  
  // -------------------------
  // Kepala menjauh
  // -------------------------
  else {

    digitalWrite(LED_PIN, LOW);

    // Reset agar siap mendeteksi sujud berikutnya
    sedangSujud = false;
  }


  // Jeda antar pembacaan
  delay(1000);
}