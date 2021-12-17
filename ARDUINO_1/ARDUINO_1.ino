#include <SPI.h>
#include <Ethernet.h>
#include <ArduinoJson.h> //6.17.3
#include <SPI.h>
#include <MFRC522.h>
#include <Servo.h>

// replace the MAC address below by the MAC address printed on a sticker on the Arduino Shield 2
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };

EthernetClient client;

int    HTTP_PORT   = 80;
String HTTP_METHOD = "GET";
char   HOST_NAME[] = "192.168.1.58"; // change to your PC's IP address
String PATH_NAME   = "/proyek_delameta/data-api.php";
String getData;
String jakarta ="Tegal";
String golongan ="golongan_3";

#define SS_PIN 8
#define RST_PIN 9
int sensor1 =A0; 
int sensor2 =A1;
int ir =5;
int buzzer =5;
int outservo =7;
int ledRed =2;
int ledYellow =3;
int ledGreen =4; 
int kunci = 0;

MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance.
Servo myservo;
void setup() {
  Serial.begin(115200);
  //Penyetingan PIN
  pinMode(sensor1, INPUT);
  pinMode(sensor2, INPUT);
  pinMode(ir, INPUT);
  pinMode(buzzer,OUTPUT);
  pinMode(ledRed, OUTPUT);
  pinMode(ledYellow, OUTPUT);
  pinMode(ledGreen, OUTPUT);
  myservo.attach(outservo);
  myservo.write(0);
  
  while(!Serial);
  SPI.begin();      // Initiate  SPI bus
  mfrc522.PCD_Init();   // Initiate MFRC522

  //START IP DHCP
  Serial.println("Konfigurasi DHCP, Silahkan Tunggu!");
  if (Ethernet.begin(mac) == 0) {
    Serial.println("DHCP Gagal!");
    if (Ethernet.hardwareStatus() == EthernetNoHardware) {
      Serial.println("Ethernet Tidak tereteksi :(");
    } else if (Ethernet.linkStatus() == LinkOFF) {
      Serial.println("Hubungkan kabel Ethernet!");
    }
    while (true) {delay(1);}
  }  
  //End DHCP
   
  delay(5000); 
  Serial.print("IP address: ");
  Serial.println(Ethernet.localIP());  
  client.connect(HOST_NAME, HTTP_PORT);
  Serial.println("Siap Digunakan!");
}

void loop() {
  //Baca data
   //Program yang akan dijalankan berulang-ulang
   int bacair = digitalRead(ir);
   int bacag1 = digitalRead(sensor1);
   int bacag2 = digitalRead(sensor2);
   if(kunci ==0){
  if ( ! mfrc522.PICC_IsNewCardPresent()) {return;}
  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial()) {return;}
  
  //Show UID on serial monitor
  Serial.print("UID tag :");
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
  
  //POST TO WEB
    client.connect(HOST_NAME, HTTP_PORT);
    client.println(HTTP_METHOD + " " + PATH_NAME + 
                   "?rfid=" + String(uidString) + 
                   "&lokasi=" + String(jakarta) +
                   "&golongan=" + String(golongan)+
                   " HTTP/1.1");
    client.println("Host: " + String(HOST_NAME));
    client.println("Connection: close");
    client.println(); // end HTTP header
  
    while(client.connected()) {
      if(client.available()){
        char endOfHeaders[] = "\r\n\r\n";
        client.find(endOfHeaders);
        getData = client.readString();
        getData.trim();
        Serial.println(getData);
        
        
        //AMBIL DATA JSON
       const size_t capacity = JSON_OBJECT_SIZE(15) + 500; //cari dulu nilainya pakai Arduino Json 5 Asisten
      DynamicJsonDocument doc(capacity);
      // StaticJsonDocument<192> doc;
      DeserializationError error = deserializeJson(doc, getData);
   // Serial.println(error);
      // tb_rfid
      const char* id_rfid       = doc["id_rfid"]; 
      const char* nama_lengkap  = doc["nama_lengkap"]; 
      const char* alamat        = doc["alamat"]; 
      const char* telepon       = doc["telepon"]; 
      const char* saldo         = doc["saldo"]; 
      const char* nomor_rfid    = doc["nomor_rfid"];
      const char* nama_tol      = doc["nama_tol"]; 
      // tb_transaksi_tol
      const char* id_pembayaran = doc["id_pembayaran"]; 
      const char* no_transaksi  = doc["no_transaksi"];
      const char* status_tol    = doc["status"];
      const char* keterangan    = doc["keterangan"];
      const char* tanggal_transaksi = doc["tanggal_transaksi"];
      // tb_golongan
      const char* id_golongan  = doc["id_golongan"]; 
      const char* id_tol       = doc["id_tol"]; 
      const char* nama_golongan = doc["nama_golongan"]; 
      const char* tarif       = doc["tarif"]; 
      Serial.println(nama_lengkap);
      
      //LOGIKA
      if(String(id_rfid)!= "0" && String(keterangan) == "Transaksi berhasil" ){
        kunci = 1;
        buzzer_berhasil();
        Serial.println("SALDO CUKUP");
        //POST TO SERIAL
        Serial.print("Nama gerbang tol  : "); Serial.println(nama_tol);
        Serial.print("Tanggal transaksi : "); Serial.println(tanggal_transaksi);
        Serial.print("Nomor transaksi   : "); Serial.println(no_transaksi);
        Serial.print("Golongan          : "); Serial.println(nama_golongan);
        Serial.print("Tarif tol         : ");Serial.println(tarif);
        Serial.print("Nomor RFID        : ");Serial.println(nomor_rfid);
        Serial.print("Sisa Saldo        : ");Serial.println(saldo);
        myservo.write(90);
        digitalWrite(ledGreen, HIGH);
        digitalWrite(ledYellow, LOW);
        digitalWrite(ledRed, LOW);
      }
      else if(String(id_rfid)!="0" && String(keterangan) == "Saldo tidak cukup"){
        buzzer_gagal();
        Serial.println("SALDO TIDAK CUKUP");
        //POST TO SERIAL
        Serial.print("Nomor RFID        : ");Serial.println(nomor_rfid);
        Serial.print("Sisa Saldo        : ");Serial.println(saldo);
        myservo.write(0);
        digitalWrite(ledYellow, HIGH);
        digitalWrite(ledGreen, LOW);
        digitalWrite(ledRed, LOW);
      }
      else if(String(keterangan) == "Tol tidak terdaftar"){   
        buzzer_gagal();
        Serial.println("Tol Tidak terdaftar!");
        digitalWrite(ledRed, HIGH);
        digitalWrite(ledGreen, LOW);
        digitalWrite(ledYellow, LOW);
      }
      else if(String(keterangan) == "RFID tidak terdaftar"){   
        buzzer_gagal();
        Serial.println("RFID Tidak terdaftar!");
        digitalWrite(ledRed, HIGH);
        digitalWrite(ledGreen, LOW);
        digitalWrite(ledYellow, LOW);
      }   // End Else if
      } 
    //delay(1000);
}
}

//  if(kunci == 1 and bacair==0){
//    kunci = 0;
//    buzzer_berhasil();
//    myservo.write(0);
//    //Serial.println("PALANG TERTUTUP");
//    digitalWrite(green, HIGH);
//    delay(1000);
//    digitalWrite(green, LOW);
//    delay(1000);
//    digitalWrite(green, HIGH);
//    delay(1000);
//    digitalWrite(green, LOW);
//    delay(1000);
//    digitalWrite(yellow, LOW);
//    digitalWrite(red, LOW);
//y  }
//  if(bacag1==0 and bacag2==0){
//    golongan="golongan2";
//    Serial.println(golongan);
//  }
//  else if(bacag1==1 and bacag2==0){
//    golongan="golongan1";
//    Serial.println(golongan);
//  }
  
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
