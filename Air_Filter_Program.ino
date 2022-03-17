// Import required libraries
#include <WiFi.h>
#include <HTTPClient.h>
//access point information
const char* ssid = "ESP32-Access-Point";
const char* password = "123456789";

//Your IP address or domain name with URL path
const char* serverNameTemp = "http://192.168.4.1/temperature";
const char* serverNameHumi = "http://192.168.4.1/humidity";
const char* serverNameTempF = "http://192.168.4.1/temperatureF";
const char* serverNameCO2 = "http://192.168.4.1/CO2";
const char* serverNameTVOC = "http://192.168.4.1/TVOC";
const char* serverNameSpeed = "http://192.168.4.1/fanSpeed";

String temperature;
String humidity;
String temperatureF;
String CO2;
String TVOC;
String fanSpeed;

unsigned long previousMillis = 0;
const long interval = 5000; 

#define RELAY_PIN  27  // ESP32 pin GIOP27, which connects to the IN pin of relay
#define RELAY2_PIN 33
#define MODE_OFF 0
#define MODE_LOW 1
#define MODE_HIGH 2

unsigned int counter = 0;

// the code in setup function runs only one time when ESP32 starts
void setup() {
  //connect ESP 32 to another ESP 32
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  Serial.println("Connecting");
  while(WiFi.status() != WL_CONNECTED) { 
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
  
  // initialize digital pin A5 as an output.
  pinMode(RELAY_PIN, OUTPUT);
  pinMode(RELAY2_PIN, OUTPUT);
}

// the code in loop function is executed repeatedly infinitely
void loop() {
  unsigned long currentMillis = millis();
  if(currentMillis - previousMillis >= interval) {
     // Check WiFi connection status
    if(WiFi.status()== WL_CONNECTED ){ 
      temperature = httpGETRequest(serverNameTemp);
      humidity = httpGETRequest(serverNameHumi);
      temperatureF = httpGETRequest(serverNameTempF);
      CO2 = httpGETRequest(serverNameCO2);
      TVOC = httpGETRequest(serverNameTVOC);
      fanSpeed = httpGETRequest(serverNameSpeed);
      Serial.println("Temperature: " + temperature + " *C - Humidity: " + humidity + " % - Temperature: " + temperatureF + " *F" + " CO2: " + CO2 + " TVOC: " + TVOC + "Fan Speed: " + fanSpeed);
      
      
      // save the last HTTP GET Request
      previousMillis = currentMillis;
    }
    else {
      Serial.println("WiFi Disconnected");
    }
  }
}

String httpGETRequest(const char* serverName) {
  WiFiClient client;
  HTTPClient http;
    
  // Your Domain name with URL path or IP address with path
  http.begin(client, serverName);
  
  // Send HTTP POST request
  int httpResponseCode = http.GET();
  
  String payload = "--"; 
  
  if (httpResponseCode>0) {
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
    payload = http.getString();
  }
  else {
    Serial.print("Error code: ");
    Serial.println(httpResponseCode);
  }
  // Free resources
  http.end();

  return payload;

  if (fanSpeed.toInt() == 0)
  {
    digitalWrite(RELAY_PIN, LOW);
    digitalWrite(RELAY2_PIN, LOW);
    printf("set OFF\n");
  }
  if (fanSpeed.toInt() == 1)
  {
    digitalWrite(RELAY_PIN, LOW);
    digitalWrite(RELAY2_PIN, HIGH);
    printf("set LOW\n");
  }
  if (fanSpeed.toInt() == 2)
  {
    digitalWrite(RELAY_PIN, HIGH);
    digitalWrite(RELAY2_PIN, LOW);
    printf("set HIGH\n");
  }
}
