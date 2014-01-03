#include <SPI.h>
#include <Ethernet.h>
#include <PubSubClient.h>

// Values to setup the ethernet connection
byte mac[] = {0xDE, 0xAD, 0xEF, 0xBE, 0xFE, 0xED};
byte mqttbroker[] = {192,168,0,106};
IPAddress ip(192,168,0,85);


// Values to setup the button
int lastSwitchState = 0;
int switchState = 0;
const int onOffSwitchPin = 8;
const int outputPin = 2;

// Values for the LED
const int ledPin = 5;
boolean LED_status = 0;

// byte string "pressed"
byte pressed[] = {0x70,0x72,0x65,0x73,0x73,0x65,0x64};
String strPayload;

void mycallback(char* topic, byte* payload, unsigned int length) {
  // Replace the new line character with a null character to make
  // the byte* payload a C style string array.  Then cast it as a
  // String so we can take a better look at it.
  // The \n character was coming from the stdinpub client
  // just add the null character
  payload[length] = '\0';
  strPayload = String((char*)payload);
  
  Serial.print("topic: ");
  Serial.print(topic);
  Serial.print("\tpayload: ");
  Serial.print(strPayload);
  //Serial.write(payload,length);
  Serial.print("\tlength: ");
  Serial.println(length);
  if(strPayload == "pressed") {
    Serial.println("True");
    toggleLED();
    sendLEDStatus();
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
  if(client.connect("arduinoClient")) {
    client.publish("test","Client connected");
    client.subscribe("test");
  }
}

void loop() {
  switchState = digitalRead(onOffSwitchPin);
  delay(1);
  
  client.loop();
  
  if (buttonWasPressed()) {
    client.publish("test", "button pressed");
    toggleLED();
    sendLEDStatus();
  }
  
  lastSwitchState = switchState;
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
    LED_status = 0;
  } else {
    digitalWrite(ledPin, HIGH);
    LED_status = 1;
  }
}

void sendLEDStatus() {
  if (LED_status) {
    client.publish("test", "on");
  } else {
    client.publish("test", "off");
  }
}
