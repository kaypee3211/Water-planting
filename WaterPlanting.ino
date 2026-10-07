#include <Button.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <UrlEncode.h>

//use millis instead of del;ay in every pomp reset the milis variable

//25 - captive moisture
//32 - sensor
const int Pomp = 25;
const int CaptureAir = 2544;
const int CaptureWater = 1050;
const int Sensor = 32;
int buttonPressed = 0;
unsigned long timePassed = 0;

String ssid = "your_ssid";
String password = "your_pass";
String phoneNumber = "your_phone_number";
String apiKey = "you_api_from_callmebot";





Button red(27); //red one - 1 // begonia and hypoestes water when below 35-40
Button white(14); //white one - 2 - lillypop, chlorophytum comosum, shefflera moondrop below 20-25
Button brown(12); //brown one - 3 - reset button ( do nothing)
Button green(13); //green one - 4 - reset button ( do nothing)

///red diode - normally open
void pompTime(int time) {
  pinMode(Pomp, OUTPUT);
  digitalWrite(Pomp, LOW);
  delay(time);
  pinMode(Pomp, INPUT);
    
  
}

void sendMessage(String message) {
  if(WiFi.status()== WL_CONNECTED){
    String url = "http://api.callmebot.com/whatsapp.php?phone=" + phoneNumber + "&apikey=" + apiKey + "&text=" + urlEncode(message);
    WiFiClient client; 
    HTTPClient http;
    http.begin(client, url);
    int httpResponseCode = http.GET();
    http.end();



  }
}



void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  pinMode(Pomp, INPUT);
  pinMode(Sensor, OUTPUT);
  //buttons
  red.begin();
  green.begin();
  brown.begin();
  white.begin();

  WiFi.begin(ssid, password);
  Serial.println("Connecting");
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  

}

void loop() {


  // pinMode(Pomp, OUTPUT);
  // digitalWrite(Pomp, LOW);//open
  // delay(2000);
  // pinMode(Pomp, INPUT);
  // delay(2000);

  // put your main code here, to run repeatedly:
 
  


  
  
  //test field
  if (red.pressed()) {
    buttonPressed = 1;
    Serial.println("1");
  }
  else if (white.pressed()) {
    buttonPressed = 2;
    Serial.println("2");
  }
  else if (brown.pressed()) {
    buttonPressed = 3;
    Serial.println("3");
  }
  else if (green.pressed()) {
    buttonPressed = 4;
    Serial.println("4");
  }


  if ((millis() - timePassed) > 1800000) {

    
    pinMode(Sensor, INPUT);
    digitalWrite(Sensor, HIGH);
    float meas = analogRead(Sensor);
    float realMeas = map(meas, CaptureWater, CaptureAir, 100,0);
    realMeas = constrain(realMeas,0,100);


    switch(buttonPressed) {
      case 1:
        if (realMeas <= 40) {
          pompTime(3000);
          Serial.println("1 swtgichj");
          sendMessage("Flower 1 has been watered. Soil moisture:" + String(realMeas) + "\n");
        }
        else {
          sendMessage("Flower 1 has not been watered. Soil moisture:" + String(realMeas) + "\n");
        }
        break;
      case 2:
        if (realMeas <=25) {
          pompTime(4000);
          Serial.println("2 swtgichj");
          sendMessage("Flower 2 has been watered. Soil moisture:" + String(realMeas) + "\n");
        }
        else {
          sendMessage("Flower 2 has not been watered. Soil moisture:" + String(realMeas) + "\n");
        }
        break;
      case 3:
        pompTime(4000);
        buttonPressed = 0;
        Serial.println("3 swtgichj");
        sendMessage("Test btn3 \n");
        break;
      case 4:
        buttonPressed = 0;
        Serial.println("4 swtgichj");
        break;
      default:
        break;


    }
    timePassed = millis();
  }

  

  // delay(600000); //10min
  digitalWrite(Sensor, LOW);

  }









