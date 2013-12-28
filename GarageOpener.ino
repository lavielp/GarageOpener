// Adding a comment as a test.

#include <SPI.h>
#include <Ethernet.h>

byte mac[] = {0xDE, 0xAD, 0xEF, 0xBE, 0xFE, 0xED};
IPAddress ip(192,168,0,85);
EthernetServer server(80);

void setup() {
  pinMode(4,OUTPUT);
  digitalWrite(4,HIGH);
  
  Ethernet.begin(mac,ip);
  server.begin();
}

void loop() {
  EthernetClient client = server.available();
  
  if(client) {
    boolean currentLineIsBlank = true;
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        if (c == '\n' && currentLineIsBlank) {
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println("Connection: close");
          client.println();
          client.println("<!DOCTYPE html>");
          client.println("<html>");
          client.println("<head>");
          client.println("<title>Arduino Web Page</title>");
          client.println("</head>");
          client.println("<body>");
          client.println("<h1>Hello World!</h1>");
          client.println("<p>A web page from the arduino server</p>");
          client.println("</body>");
          client.println("</html>");
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
}
