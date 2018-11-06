
#include <Arduino.h>
#include "SoftwareSerial.h"
#include "string.h"


SoftwareSerial esp_serial(10,11); // RX , TX

//----------------------------------------------------------------------------------------------
bool echoFind(String keyword){
  byte current_char = 0;
  byte keyword_length = keyword.length();
  long deadline = millis() + 5000;

  while (millis()<deadline) {


    if (esp_serial.available()) {
      char ch = esp_serial.read();
      Serial.write(ch);
      if (ch=keyword[current_char] && ++current_char==keyword_length) {

        Serial.println();
        /* code */
      }
    }
  }
}
bool sendCommand(String command, String acknolage){

  esp_serial.println(command);
  if (!echoFind(acknolage)) {
    return true;
  }
}
//----------------------------------------------------------------------------------------------

void confingWifi(){

  sendCommand("AT+RST", "Ready");
  delay(5000);
  sendCommand("AT+CWMODE=3","OK");
  sendCommand("AT+CIFSR", "OK");
  sendCommand("AT+CIPMUX=1","OK");
  sendCommand("AT+CIPSERVER=1,80","OK");
}

void setup() {
    esp_serial.begin(9600);
    Serial.begin(9600);
    confingWifi();
}
void send_http_response(String content){
  String response = "HTTP/1.1 200 OK\r\n";
  response += "Content-Type: text/html; charset=UTF-8\r\n"; 
  response += "Content-Length: ";
  response += content.length();
  response += "\r\n";
  response +="Connection: close\r\n\r\n";
  response += content;

  String cmd = "AT+CIPSEND=0,";
  cmd+="99";
  sendCommand(cmd,"OK");
  delay(1000);
  esp_serial.println(response);
  delay(1000);
  sendCommand("AT+CIPCLOSE=0","OK");
}

void loop() {
  String IncomingString = "";
  while (esp_serial.available()) {
    IncomingString = esp_serial.readString();
    Serial.println("Received String: " + IncomingString);
    send_http_response("ok");

  }
}
