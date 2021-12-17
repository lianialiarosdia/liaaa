#include <SPI.h>                                      // Library komunikasi SPI
#include <Ethernet.h>                                 // Library Ethernet
#include <ArduinoJson.h>                              // Library Arduino JSON
#include <MFRC522.h>                                  // Library RFID
#include <Servo.h>                                    // Library Servo

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };

EthernetClient client;          // Membuat instance baru dari library Ethernet 

int    HTTP_PORT   = 80;
String HTTP_METHOD = "GET";
char   HOST_NAME[] = "192.168.1.58";                 // Alamat IP Laptop
String PATH_NAME   = "/proyek_delameta/data-api.php"; // URL website
String getData;

// Variabel yang akan dikirim ke website
String lokasi  = "Tegal";
String golongan = "golongan_1";

// Variabel yang digunakan oleh RFID
#define SS_PIN 8
#define RST_PIN 9

// Variabel masukan (input)
int infrared_back  = A0;    // Mendeteksi bagian belakang mobil
int infrared_front   = A1;  // Mendeteksi bagian depan mobil
int infrared_pass   = A2;   // Mendeteksi apakah kendaraan sudah lewat palang atau belum

// Variabel keluaran (output)
int buzzer    = 5;
int pinServo  = 6;
int ledRed    = 2;
int ledYellow = 3;
int ledGreen  = 4; 

// Variabel millis()
//unsigned long millis_akhir = 0;

// Variabel tambahan
int kunci = 0;

MFRC522 mfrc522(SS_PIN, RST_PIN);  // Membuat instance baru dari library MFRC522 
Servo myservo;                  // Membuat instance baru dari library Servo

void setup() {
  Serial.begin(115200);

  // Mendeklarasikan pin Arduino sebagai input/output
  pinMode(infrared_back, INPUT);
  pinMode(infrared_front, INPUT);
  pinMode(infrared_pass, INPUT);
  pinMode(buzzer,OUTPUT);
  pinMode(ledRed, OUTPUT);
  pinMode(ledYellow, OUTPUT);
  pinMode(ledGreen, OUTPUT);
  myservo.attach(pinServo);           // Mendeklarasikan pin servo sebagai output
  myservo.write(0);                   // Mengatur posisi awal servo 
  
  while(!Serial);
  SPI.begin();          // Initiate  SPI bus
  mfrc522.PCD_Init();   // Initiate MFRC522

  //START IP DHCP
  Serial.println("Konfigurasi DHCP, Silahkan Tunggu!");
  if(Ethernet.begin(mac) == 0){
    Serial.println("DHCP Gagal!");
    if(Ethernet.hardwareStatus() == EthernetNoHardware){
      Serial.println("Ethernet Tidak tereteksi :(");
    } 
    else if(Ethernet.linkStatus() == LinkOFF){
      Serial.println("Hubungkan kabel Ethernet!");
    }
    while (true){delay(1);}
  }  
  //End DHCP
   
  delay(5000); 
  Serial.print("IP address: ");
  Serial.println(Ethernet.localIP());  
  client.connect(HOST_NAME, HTTP_PORT);   // Menghubungkan ke alamat IP
  Serial.println("Siap Digunakan!");
}

void loop() {
  // Memanggil millis sebagai pengganti delay()
  // unsigned long millis_awal = millis();
  
  // Baca data Infrared
  int bacaInfraredBack   = digitalRead(infrared_back);
  int bacaInfraredFront  = digitalRead(infrared_front);
  int bacaInfraredPass   = digitalRead(infrared_pass);
  
  if(kunci == 0){                      // Start if kunci
    if(!mfrc522.PICC_IsNewCardPresent()){return;}
    if(!mfrc522.PICC_ReadCardSerial()){return;}
  
  // Membaca dan menampilkan UID tag pada Serial Monitor
  Serial.println("UID tag :");
  // variabel RFID
  String uidString;
  byte letter;
  for (byte i = 0; i < mfrc522.uid.size; i++) 
  {
     uidString.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? "" : ""));
     uidString.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  Serial.print("Message : ");
  uidString.toUpperCase();
  Serial.println(uidString);
  digitalWrite(buzzer,HIGH);
  delay(100);
  digitalWrite(buzzer,LOW);
//  Serial.print("Nama tol :" + String(lokasi));
//  Serial.print("Nomor golongan :" + String(golongan));
  
  // Mem-POST-kan data ke website
  client.connect(HOST_NAME, HTTP_PORT);
  client.println(HTTP_METHOD + " " + PATH_NAME + 
                 "?rfid=" + String(uidString) + 
                 "&lokasi=" + String(lokasi) +
                 "&golongan=" + String(golongan)+
                 " HTTP/1.1");
  client.println("Host: " + String(HOST_NAME));
  client.println("Connection: close");
  client.println(); 
  // end HTTP header
  
  while(client.connected()) {         // Start client.connected()
    if(client.available()){           // Start client.avaliable()
      char endOfHeaders[] = "\r\n\r\n";
      client.find(endOfHeaders);
      getData = client.readString();
      getData.trim();
      Serial.println(getData);
        
        
      // meng-GET-kan data dari JSON
     const size_t capacity = JSON_OBJECT_SIZE(8) + 160;//cari dulu nilainya pakai Arduino Json 5 Asisten
      DynamicJsonDocument doc(capacity);
      // StaticJsonDocument<192> doc;
      DeserializationError error = deserializeJson(doc, getData);
//const size_t capacity = JSON_OBJECT_SIZE(18) + 410;

// TB RFID
//const char* id_rfid = doc["id_rfid"]; // "2"
//const char* nama_lengkap = doc["nama_lengkap"]; // "Benedetto"
//const char* alamat = doc["alamat"]; // "Jl. Joglo Raya No. 12"
//const char* telepon = doc["telepon"]; // "081947588234"
//const char* foto_ktp = doc["foto_ktp"]; // ""
//const char* saldo = doc["saldo"]; // "5033000"
//const char* nomor_rfid = doc["nomor_rfid"]; // "1ACE980"
//const char* nama_tol = doc["nama_tol"]; // "Tegal"
//Serial.print("id rfid     : ");Serial.println(nama_lengkap);

// TB Transaksi Tol
//const char* id_pembayaran = doc["id_pembayaran"]; // "4986"
//const char* id_tol = doc["id_tol"]; // "812"
//const char* id_rfid = doc["id_rfid"]; // "2"
//const char* tanggal_transaksi = doc["tanggal_transaksi"]; // "2021-12-17 12:24:37"
//const char* status = doc["status"]; // "Berhasil"
//const char* keterangan = doc["keterangan"]; // "Transaksi berhasil"
//const char* saldo_akhir = doc["saldo_akhir"]; // "4925000"
//const char* no_transaksi = doc["no_transaksi"]; // "INVTOL 2 812 122437"
//Serial.print("id rfid     : ");Serial.println(status);


//TB Golongan
//const char* id_golongan = doc["id_golongan"]; // "10"
//const char* id_tol = doc["id_tol"]; // "812"
//const char* tarif = doc["tarif"]; // "12000"
//const char* nama_golongan = doc["nama_golongan"]; // "golongan_1"
//Serial.print("id rfid     : ");Serial.println(tarif);

// tb rfid dan tb transakasi
const char* id_rfid = doc["id_rfid"]; // "2"
const char* nama_lengkap = doc["nama_lengkap"]; // "Benedetto"
const char* alamat = doc["alamat"]; // "Jl. Joglo Raya No. 12"
const char* telepon = doc["telepon"]; // "081947588234"
const char* foto_ktp = doc["foto_ktp"]; // ""
const char* saldo = doc["saldo"]; // "4541000"
const char* nomor_rfid = doc["nomor_rfid"]; // "1ACE980"
const char* nama_tol = doc["nama_tol"]; // "Tegal"

Serial.print("id rfid     : ");Serial.println(id_rfid);
//      const char* id_rfid     = doc["id_rfid"]; // "2"
//      const char* saldo       = doc["saldo"]; // "245000"
//      const char* nomor_rfid  = doc["nomor_rfid"]; // "1ACE980"
//      const char* nama_tol    = doc["nama_tol"]; // "Tegal"
//      const char* id_pembayaran = doc["id_pembayaran"]; // "4880"
//      const char* no_transaksi  = doc["no_transaksi"]; // "INVTOL2812181130"
//      const char* status        = doc["status"]; // "Berhasil"
//      const char* keterangan    = doc["keterangan"]; // "Transaksi berhasil"
//      const char* tanggal_transaksi = doc["tanggal_transaksi"]; // "2021-12-16 18:11:30"
//      const char* nama_golongan     = doc["nama_golongan"]; // "golongan_3"
//      const char* tarif             = doc["tarif"]; // "12000"
//      Serial.print("id rfid     : ");Serial.println(nama_lengkap);
//      Serial.print("saldo       : ");Serial.println(saldo);
//      Serial.print("Nomor rfid  : ");Serial.println(nomor_rfid);
//      Serial.print("nama tol    : ");Serial.println(nama_tol);
//      Serial.print("id pembayaran : ");Serial.println(id_pembayaran);
//      Serial.print("No transaksi  : ");Serial.println(no_transaksi);
//      Serial.print("Status        : ");Serial.println(status);
//      Serial.print("ket           : ");Serial.println(keterangan);
//      Serial.print("tanggal       : ");Serial.println(tanggal_transaksi);
//      
      //LOGIKA
//      if(String(id_rfid)!= "0" && String(keterangan) == "Transaksi berhasil" ){
//        kunci = 1;
//        buzzer_berhasil();
//        Serial.println("SALDO CUKUP");
//        //POST TO SERIAL
//        Serial.print("Nama gerbang tol  : "); Serial.println(nama_tol);
//        Serial.print("Tanggal transaksi : "); Serial.println(tanggal_transaksi);
//        Serial.print("Nomor transaksi   : "); Serial.println(no_transaksi);
//        Serial.print("Golongan          : "); Serial.println(nama_golongan);
//        Serial.print("Tarif tol         : ");Serial.println(tarif);
//        Serial.print("Nomor RFID        : ");Serial.println(nomor_rfid);
//        Serial.print("Sisa Saldo        : ");Serial.println(saldo);
//        myservo.write(90);
//        digitalWrite(ledGreen, HIGH);
//        digitalWrite(ledYellow, LOW);
//        digitalWrite(ledRed, LOW);
//      }
//      else if(String(id_rfid)!="0" && String(keterangan) == "Saldo tidak cukup"){
//        buzzer_gagal();
//        Serial.println("SALDO TIDAK CUKUP");
//        //POST TO SERIAL
//        Serial.print("Nomor RFID        : ");Serial.println(nomor_rfid);
//        Serial.print("Sisa Saldo        : ");Serial.println(saldo);
//        myservo.write(0);
//        digitalWrite(ledYellow, HIGH);
//        digitalWrite(ledGreen, LOW);
//        digitalWrite(ledRed, LOW);
//      }
//      else if(String(keterangan) == "Tol tidak terdaftar"){   
//        buzzer_gagal();
//        Serial.println("Tol Tidak terdaftar!");
//        digitalWrite(ledRed, HIGH);
//        digitalWrite(ledGreen, LOW);
//        digitalWrite(ledYellow, LOW);
//      }
//      else if(String(keterangan) == "RFID tidak terdaftar"){   
//        buzzer_gagal();
//        Serial.println("RFID Tidak terdaftar!");
//        digitalWrite(ledRed, HIGH);
//        digitalWrite(ledGreen, LOW);
//        digitalWrite(ledYellow, LOW);
//      }   // End Else if
    }     // End client.avaliable() 
  }       // End client.connected()
}         // End if kunci

if(kunci == 1 and bacaInfraredPass == 0){
    buzzer_berhasil();
    myservo.write(0);
    //Serial.println("Palang Tertutup");
    digitalWrite(ledGreen, HIGH);
    delay(1000);
    digitalWrite(ledGreen, LOW);
    delay(1000);
    digitalWrite(ledGreen, HIGH);
    delay(1000);
    digitalWrite(ledGreen, LOW);
    delay(1000);
    digitalWrite(ledYellow, LOW);
    digitalWrite(ledRed, LOW);
    kunci = 0;
  }
}

// Fungsi untuk mengecek golongan kendaraan
void cek_golongan(int ir_front, int ir_back){
  if(ir_front == 1 and ir_back == 1){
    golongan = "golongan_2";
  }
  else if(ir_front == 1 and ir_back == 0){
    golongan = "golongan_1";
  }  
}

void buzzer_berhasil(){
  digitalWrite(buzzer,HIGH);
  delay(100);
  digitalWrite(buzzer,LOW);
  delay(100);
  digitalWrite(buzzer,HIGH);
  delay(100);
  digitalWrite(buzzer,LOW);
  delay(100);
}

void buzzer_gagal(){
  digitalWrite(buzzer,HIGH);
  delay(1000);
  digitalWrite(buzzer,LOW);
  delay(10);
}
