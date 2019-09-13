/* Code made by Ikbeneendaan for Instructables, parts based on code made by Hugo Gomes ( ESP32 weather station ) and Michalis Vasilakis ( Hygrometer ) */

#include <ESP32_Servo.h> 
#include <WiFi.h>
#include <WiFiMulti.h>

WiFiMulti WiFiMulti;

const char* ssid     = "HotSpot - UI, Event"; // Your SSID (Name of your WiFi)
const char* password = "digitalcsui2019"; //Your Wifi password

const char* host = "api.thingspeak.com";
String api_key = "25DCGS0F3MK0DSV1"; // Your API Key provied by thingspeak

// https://api.thingspeak.com/update?api_key=25DCGS0F3MK0DSV1&field1=0
float kelembapan;
float suhu;
float cahaya;

//Servo myservo; // atach the servo

const int pompa = 32;
//const int servo = 18;
const int sinar = 33;
Servo myservo;

void setup(){
  
  Serial.begin(115200);
  pinMode(pompa, OUTPUT);
  pinMode(sinar, OUTPUT);
//  myservo.attach(servo);
  /*myservo.attach(26);
   myservo.write(0);*/

     Connect_to_Wifi();
}

void loop(){
  kelembapan = analogRead(36)/40.95;
  suhu = analogRead(39)/4/2.08; 
  cahaya = analogRead(34)/40.95;// Every two seconds the temperature rises by 10.
  Serial.print("Kelembapan: ");
  Serial.print(kelembapan);
  Serial.println("%");
  Serial.print("Suhu: ");
  Serial.print(suhu);
  Serial.println(" C");
  Serial.print("Cahaya : ");
  Serial.print(cahaya);
  Serial.print("%");
  
  if (kelembapan > 80){
  digitalWrite(pompa, LOW);
}
 else {
  digitalWrite(pompa, HIGH);
}

 /* if (suhu < 17){
   myservo.write(0);
   delay(15);
   myservo.write(180);
   delay(15);
   myservo.detach();
     
  }
  else{
   myservo.write(180);
   delay(15);
   myservo.write(0);
   delay(15);
   myservo.detach();
  }
  */
  if (cahaya > 40 ){
  digitalWrite(sinar, LOW);
}
 else {
  digitalWrite(sinar, HIGH);
 }

  Send_Data();
  delay(1000);
 
}

 

void Connect_to_Wifi()
{

  // We start by connecting to a WiFi network
  WiFiMulti.addAP(ssid, password);

  Serial.println();
  Serial.println();
  Serial.print("Wait for WiFi... ");

  while (WiFiMulti.run() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

}

void Send_Data()
{

  // map the moist to 0 and 100% for a nice overview in thingspeak.
  Serial.println("Prepare to send data");

  // Use WiFiClient class to create TCP connections
  WiFiClient client;

  const int httpPort = 80;

  if (!client.connect(host, httpPort)) {
    Serial.println("connection failed");
    return;
  }
  else
  {
    String data_to_send = api_key;
    data_to_send += "&field1=";
    data_to_send += String(kelembapan);
    data_to_send += "&field2=";
    data_to_send += String(suhu);
    data_to_send += "&field3=";
    data_to_send += String(cahaya);
    data_to_send += "\r\n\r\n";

    client.print("POST /update HTTP/1.1\n");
    client.print("Host: api.thingspeak.com\n");
    client.print("Connection: close\n");
    client.print("X-THINGSPEAKAPIKEY: " + api_key + "\n");
    client.print("Content-Type: application/x-www-form-urlencoded\n");
    client.print("Content-Length: ");
    client.print(data_to_send.length());
    client.print("\n\n");
    client.print(data_to_send);

    delay(1000);
  }

  client.stop();


}
