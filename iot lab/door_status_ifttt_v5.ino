#include <ESP8266WiFi.h>

const int switchPin = 12;   // D6 Pin Connected to Reed Switch NC
const int ledPin = LED_BUILTIN;      // D7 Pin Connected to LED

const char* SSID = "AMS-IOT";
const char* PSK = "amsiotday";

const char* door_status = "intializing";
const char* last_door_status = "none";

WiFiClient client;

void setup() {
  Serial.begin(115200);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  
  Serial.println();
  Serial.println("Preparing Door Monitor...");
      
  pinMode(switchPin, INPUT_PULLUP);   // D6 Pin Connected to Reed Switch NC
  pinMode(ledPin, OUTPUT);            // D7 Pin Connected to LED

  Serial.println("Connecting to Wireless Network.");
  WiFi.begin(SSID, PSK);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  Serial.println();
  Serial.println("WiFi connected");
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());  
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}



void doorStatus() {
  //Checks the status of the reed switch
  
  if (digitalRead(switchPin) == 0) {
    digitalWrite(ledPin, LOW);
    door_status = "closed";
  }
  else if (digitalRead(switchPin) == 1) {
    //Door is open
    digitalWrite(ledPin, HIGH);
    door_status = "OPEN";
  }
  else {
    // Switch failure
    Serial.println();
    Serial.println("ERROR: Reed Switch is floating.");

  }
}

void startHttpReq() {

  String MakerIFTTT_Key = "yourToken";
  String MakerIFTTT_Event = "doorstatus";

  String path;
  path += "POST /trigger/";
  path += MakerIFTTT_Event;
  path += "/with/key/";
  path += MakerIFTTT_Key;
  path += " HTTP/1.1";

  String payload;
  payload += "{\"value1\":\"";
  payload += door_status;
  payload += "\"}";


  Serial.println(path);
  Serial.println(payload);
  Serial.println(payload.length());
  Serial.println("Sending SMS Notifiction Alert via IFTTT to Spark!");

  
  if (client.connect("maker.ifttt.com", 80)) {  
    // Make a HTTP request
    Serial.println("connecting to server...");

    client.println(path);
    client.println("Host: maker.ifttt.com");
    client.println("Content-Type: application/json");
    client.print("Content-Length: ");
    client.println(payload.length());
    client.println();
    client.println(payload);
    client.println("Connection: close");
    client.println();
    client.stop();
  }

  while (client.available()) {
    char c = client.read();
    Serial.write(c);
  }

  // if the server's disconnected, stop the client:
  if (!client.connected()) {
    Serial.println();
    Serial.println("Disconnecting from server.");
    client.stop();
  }
}




void loop() {

  last_door_status = door_status;
  
  doorStatus();    

  if (last_door_status == door_status) {
    Serial.println("Door status has not changed.");
  }
  else {
    Serial.println();
    Serial.print("Door was ");
    Serial.print(last_door_status);
    Serial.print(" -- Door now ");
    Serial.println(door_status); 

    startHttpReq();
  }
  delay(500);
}


