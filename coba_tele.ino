#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#include <ArduinoJson.h>
#include <Wire.h>
#include<Servo.h>
//memanggil library servo
Servo servoku;

//identifikasi sensor yang akan di gunakan
#define gassensor A0
#define servo D1
#define buzzer D2


//menyimpan nama wifi dan password ke dalam variabel di bawah
char ssid[]="xxxxxx";
char password[]="xxxxxxx";

// menyimpan api telegram bot ke dalam variabel di bawah
#define BOTtoken "xxxxxxxxx"

//memnaggil library wifi client 
WiFiClientSecure client;
UniversalTelegramBot bot(BOTtoken,client);

//membuat bot request selama 1 detik
int botRequestDelay = 1000;
unsigned long lastTimeBotRan;

//membuat fungsi untuk tampilan dan fungsi dari bot telegram
void pesanbaru(int pesan){
  Serial.println("terdapat pesan baru dari id "+bot.messages[1].from_name);
//  Serial.println(String(pesan));

  for (int i=0; i<=pesan; i++){
    String chat_id=String (bot.messages[i].chat_id);
    String text=bot.messages[i].text;
//    Serial.println("udah masuk lup");
    String from_name = bot.messages[i].from_name;
    if(from_name=="") from_name = "guest";

    if (text == "/start"){
      String datang = "Helooow~ "+from_name+ ".\n";
      datang += "/jalankanservo : mengerakan servo \n";
      datang += "/kembalikanservo : menutup servo \n";
      datang += "/cekruangan : mengecek kondisi ruangan \n";
      bot.sendMessage(chat_id,datang,"Markdown");
      //Serial.println("udah masuk start");
    }
    else if (text == "/kembalikanservo"){
      servoku.write(0);
      delay(1000);
      bot.sendMessage(chat_id,"helow servo disini","");
       //Serial.println("udah masuk kembali");
    }
    else if(text == "/cekruangan"){
      int gas = analogRead(gassensor);
      Serial.println(gas);
       if (gas > 500)
      {
        tone(buzzer, 1000, 200);
        bot.sendMessage(chat_id,"mas ada asap mengepul nih banyak","");
          Serial.println("terdapat asap");
      }
      else
       {
        bot.sendMessage(chat_id,"aman mas gada asap","");
          Serial.println("tidak ada asap asap");
        noTone(buzzer);
       }
      
     
       Serial.println("udah masuk asap");
    }
    else if(text=="/jalankanservo"){
      servoku.write(90);
      delay(1000);
      bot.sendMessage(chat_id,"servo berjalan 90 derajat","");
//       Serial.println("udah masuk 1");
    }
  }
}

void setup() {
  Serial.begin(115200);
  // put your setup code here, to run once:
  servoku.attach(D1);
  pinMode(gassensor,INPUT);
  pinMode(servo,OUTPUT);

  client.setInsecure();
  
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);

  Serial.print("menyambungkan wifi");
  Serial.println(ssid);
  WiFi.begin(ssid,password);

  while (WiFi.status() != WL_CONNECTED){
    Serial.print(".");
    delay(500);
  }
  Serial.println("");
  Serial.println("WiFi terhubung");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  int gassensor1 = analogRead(gassensor);

 if (gassensor1 > 500)
 {
   tone(buzzer, 1000, 200);
 }
 else
 {
   noTone(buzzer);
 }
  // put your main code here, to run repeatedly:
  if (millis() > lastTimeBotRan +botRequestDelay){
    int pesan=bot.getUpdates(bot.last_message_received+1);
    while(pesan){
      Serial.println("telegram merespon");
      pesanbaru(pesan);
      pesan=bot.getUpdates(bot.last_message_received + 1);
    }
    lastTimeBotRan = millis();
  }
}
