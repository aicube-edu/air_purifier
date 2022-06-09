#include <ESP8266WiFi.h>
#include <Adafruit_NeoPixel.h>
#include "FirebaseESP8266.h"

//============== parameters for finedust ==============
const char* ssid = "YangJipsa";
const char* password = "15151515";

const char* host = "apis.data.go.kr";
const int httpsPort = 80;
String yourkey = "hlCMHtutt%2FjyibxYtlhLU0DigT9nehzdxLz8WvH3kvoVpHAWjK%2BDH1v4a87T9%2FPOZrmAL76VWu%2BKoWpDMvKMZw%3D%3D";
String station_id = "봉담읍";

const int fineDustClean = 30; // 0~30
const int fineDustGood = 100;  // 31~100

//============== parameters for neopixel ==============
const int PIN = D3; // On Trinket or Gemma, suggest changing this to 1
const int NUMPIXELS = 1; // Popular NeoPixel ring size
Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

//============== parameters for Fan ==============
const int fan = D2;

//============== parameters for firebase ==============
#define FIREBASE_HOST "파이어베이스 실시간데이터베이스 주소" 
#define FIREBASE_AUTH "파이어베이스 실시간데이터베이스 비밀번호"
FirebaseData firebaseData;
FirebaseJson json;


String utf8(String input);

void setup() 
{
  Serial.begin(9600);

  Serial.println();
  Serial.print("connecting to ");

  Serial.println(ssid);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  
  Serial.print("connecting to ");
  Serial.println(host);

  pixels.begin(); // INITIALIZE NeoPixel strip object (REQUIRED)
  pinMode(fan, OUTPUT);

  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.reconnectWiFi(true);
 
  firebaseData.setBSSLBufferSize(1024, 1024);
  firebaseData.setResponseSize(1024);
  Firebase.setReadTimeout(firebaseData, 1000 * 60);
  Firebase.setwriteSizeLimit(firebaseData, "tiny");
}

void loop() 
{
  String strPM10;
  int intPM10 = 0;
  WiFiClient client;

  if (!client.connect(host, httpsPort)) 
  {
    Serial.println("connection failed");
    return;
  }

   String url = "/B552584/ArpltnInforInqireSvc/getMsrstnAcctoRltmMesureDnsty?serviceKey="+yourkey+"&returnType=xmal&numOfRows=1&stationName="+utf8(station_id)+"&dataTerm=DAILY";
   //String url = "https://apis.data.go.kr/B552584/ArpltnInforInqireSvc/getMsrstnAcctoRltmMesureDnsty?serviceKey=hlCMHtutt%2FjyibxYtlhLU0DigT9nehzdxLz8WvH3kvoVpHAWjK%2BDH1v4a87T9%2FPOZrmAL76VWu%2BKoWpDMvKMZw%3D%3D&returnType=xml&numOfRows=1&pageNo=1&stationName=봉담읍&dataTerm=DAILY&ver=1.0";
  client.print(String("GET ") + url + " HTTP/1.1\r\n" + "Host: " + host + "\r\n" + "Connection: close\r\n\r\n");

  unsigned long t = millis(); //생존시간
  while(1)
  {
    if(client.available()) break;
    if(millis() - t > 10000) break;
  }

 // Serial.println("request sent");

  while(client.available())
  {
    String data = client.readStringUntil('\n'); //readline();
    //Serial.print("data:");Serial.println(data);

    String temp = "<pm10Value>";
    
    int s = data.indexOf(temp) + temp.length(); // 길이 = 6
    int e = data.indexOf("</pm10Value>");
    if(s != -1 && e != -1) { strPM10 = data.substring(s,e); }
  }
  char buf[10]={0};
  strPM10.toCharArray(buf,strPM10.length()+1);
  intPM10 = atoi(buf);
  //intPM10 = 80;
  
  Serial.print("Finedust(pm10) : ");
  Serial.print(intPM10);
  Serial.println(" ppm");
  
  Firebase.setInt(firebaseData, "finedust", intPM10);

  delay(1000);
}

String utf8(String input)
{
  String output;
  for(int i = 0;i<input.length();i++)
  {
    output += "%" + String(input[i],HEX);
  }
  return output;
}
