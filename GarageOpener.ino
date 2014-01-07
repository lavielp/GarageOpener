#include <SPI.h>
#include <Ethernet.h>
#include <PubSubClient.h>

// Values to setup the ethernet connection
byte mac[] = {0xDE, 0xAD, 0xEF, 0xBE, 0xFE, 0xED};
byte mqttbroker[] = {192,168,0,101};
IPAddress ip(192,168,0,85);


// Values to setup the button
int lastSwitchState = 0;
int switchState = 0;
const int onOffSwitchPin = 8;
const int outputPin = 2;

// Values for the LED
const int ledPin = 5;
boolean LEDState = 0;
boolean lastLEDState = 0;

// byte string "pressed"
//byte pressed[] = {0x70,0x72,0x65,0x73,0x73,0x65,0x64};
//String strPayload;

void mycallback(char* topic, byte* payload, unsigned int length) {
  // Add a null character to make the byte* a C style string.
  // Then cast it as a string to read.
  payload[length] = '\0';
  //strPayload = String((char*)payload);
  
  //if(strPayload == "pressed") {
  if(String((char*)payload) == "pressed") {
    toggleLED();
  }
}

// Stuff for MQTT
EthernetClient ethClient;
PubSubClient client(mqttbroker, 1883, mycallback, ethClient);

// Topic this device publishes too
//"/devices/arduino/led/status";

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
  
  while(! client.connected()) {
      client.connect("arduinoClient");
  }
  
  if(client.connected()) {
    client.publish("/devices/led/mqtt", "connected");
    sendLEDStatus();
    client.subscribe("/devices/pages/light/button/event");
  }
  
  /* if(client.connect("arduinoClient")) {
    client.subscribe("/devices/pages/light/button/event");
  }*/
}

void loop() {
  switchState = digitalRead(onOffSwitchPin);
  delay(1);
  if (buttonWasPressed()) {
    //client.publish("test", "button pressed");
    toggleLED();
    //sendLEDStatus();
  }
  lastSwitchState = switchState;
  
  
  client.loop();
  
  if (LEDState != lastLEDState) {
    sendLEDStatus();
  }
  lastLEDState = LEDState;
}

boolean buttonWasPressed(){ 
  if (!lastSwitchState && switchState) {
    return true;
  } else {
    return false;
  }
}

void toggleLED() {
  // Turn the LED on or off
  if (digitalRead(ledPin)) {
    digitalWrite(ledPin, LOW);
    LEDState = 0;
  } else {
    digitalWrite(ledPin, HIGH);
    LEDState = 1;
  }
}

void sendLEDStatus() {
  if (LEDState) {
    //client.publish("/devices/led/light/status", "on",length);
    client.publish("/devices/led/light/status", (uint8_t*)"on",2,1);
  } else {
    client.publish("/devices/led/light/status", (uint8_t*)"off",3,1);
  }
}
