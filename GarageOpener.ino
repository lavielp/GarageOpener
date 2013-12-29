#include <SPI.h>
#include <Ethernet.h>
#include <WebPages.h>

// Values to setup the ethernet connection
byte mac[] = {0xDE, 0xAD, 0xEF, 0xBE, 0xFE, 0xED};
IPAddress ip(192,168,0,85);
EthernetServer server(80);

// Variables for processing web requests
String HTTP_req;

// Values to setup the button
int lastSwitchState = 0;
int switchState = 0;
const int onOffSwitchPin = 8;
const int outputPin = 2;

// Values for the LED
const int ledPin = 5;
boolean LED_status = 0;

void setup() {
  Serial.begin(9600);
  
  // Set pin 4 high to avoid an issues with SD cards
  pinMode(4,OUTPUT);
  digitalWrite(4,HIGH);
  
  // Set the switch input
  pinMode(onOffSwitchPin,INPUT);
  
  // Set the LED initially off
  pinMode(ledPin,OUTPUT);
  digitalWrite(ledPin,LOW);
  
  // Set the garage door ouput initially off
  pinMode(outputPin,OUTPUT);
  digitalWrite(outputPin,LOW);
  
  Ethernet.begin(mac,ip);
  server.begin();
}

void loop() {
  switchState = digitalRead(onOffSwitchPin);
  delay(1);
  
  EthernetClient client = server.available();
  
  if(client) {
    boolean currentLineIsBlank = true;
    while (client.connected()) {
      if (client.available()) {
        // Read and save the client request
        char c = client.read();
        HTTP_req += c;
        
        if (c == '\n' && currentLineIsBlank) {
          writeHTTPHeaders(client);
          //writeHTTPIndex(client);
          //writeHTTPLEDPage(client);
          writeHTTPLEDStatusPage(client);
          break;
        }
        
        if ( c == '\n') {
          currentLineIsBlank = true;
        }
        else if (c != '\r') {
          currentLineIsBlank = false;
        }
      }
    }
    delay(1);
    client.stop(); 
  }
  if (buttonWasPressed()) {
    pressGarageButton();
    toggleLED();
  }
  
  lastSwitchState = switchState;
}


