#include <ESP8266WiFi.h>
#include <Adafruit_NeoPixel.h>

//============== parameters for finedust ==============
const char* ssid = "와이파이 SSID";
const char* password = "와이파이 비밀번호";

const char* host = "apis.data.go.kr";
const int httpsPort = 80;

String yourkey = "서비스키";
String station_id = "지역";

const int fineDustClean = 30; // 0~30
const int fineDustGood = 100;  // 31~100

//============== parameters for neopixel ==============
const int PIN = D3; // On Trinket or Gemma, suggest changing this to 1
const int NUMPIXELS = 1; // Popular NeoPixel ring size
Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

//============== parameters for Fan ==============
const int fan = D2;

//============== parameters for firebase ==============
// firebase code

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

  // firebase code

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
  client.print(String("GET ") + url + " HTTP/1.1\r\n" + "Host: " + host + "\r\n" + "Connection: close\r\n\r\n");

  unsigned long t = millis(); //생존시간
  while(1)
  {
    if(client.available()) break;
    if(millis() - t > 10000) break;
  }

  while(client.available())
  {
    String data = client.readStringUntil('\n'); //readline();
    Serial.print("data:");Serial.println(data);

    // data parsing code
  }
  
  // data parsing code

  // firebase code
  
 
  /*hardware test code START*/
  pixels.clear();
  pixels.setPixelColor(0, pixels.Color(0, 0, 150)); // BLUE
  pixels.show(); 
  digitalWrite(fan, HIGH);
  /*hardware test code END*/
  
  delay(1000);
}

String utf8(String input)
{
  String output;
  for(int i = 0;i<input.length();i++)  {
    output += "%" + String(input[i],HEX);
  }
  return output;
}
