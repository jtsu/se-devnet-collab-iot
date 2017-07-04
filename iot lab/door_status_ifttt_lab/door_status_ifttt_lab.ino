#include <ESP8266WiFi.h>

const int switchPin = D6;   // D6 Pin Connected to Reed Switch NC
const int ledPin = LED_BUILTIN;  

//Need to define the SSID and PSK
const char* SSID = "AMS-IOT";
const char* PSK = "amsiotday";


WiFiClient client;

void setup() {
  Serial.begin(115200);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  
  Serial.println();
  Serial.println("Preparing Door Monitor...");
      
  pinMode(switchPin, INPUT_PULLUP);
  pinMode(ledPin, OUTPUT);   

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



String doorStatus() {
  //Checks the status of the reed switch
  
  if (digitalRead(switchPin) == 0) {
    digitalWrite(ledPin, LOW);
    String door_status = "closed";
    return door_status;
  }
  else {
    //Need to fill in the else condition
  }

}

void startHttpReq(String door_status) {

  //Need to add your Maker key and event information
  String MakerIFTTT_Key = "";
  String MakerIFTTT_Event = "";

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

  //need to get door status

  //send status to Spark with startHttpReq()

  

}


