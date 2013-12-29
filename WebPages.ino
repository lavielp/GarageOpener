void writeHTTPHeaders(EthernetClient &CL) {
  CL.println("HTTP/1.1 200 OK");
  CL.println("Content-Type: text/html");
  CL.println("Connection: close");
  CL.println();
}

void writeHTTPIndex(EthernetClient &CL){
  CL.println("<!DOCTYPE html>");
  CL.println("<html>");
  CL.println("<head>");
  CL.println("<title>Arduino Web Page</title>");
  CL.println("</head>");
  CL.println("<body>");
  CL.println("<h1>Hello World!</h1>");
  CL.println("<p>A web page from the arduino server</p>");
  CL.println("<p>Using a seperate for the web page</p>");
  CL.println("</body>");
  CL.println("</html>");
}

void writeHTTPLEDPage(EthernetClient &CL) {
  CL.println("<!DOCTYPE html>");
  CL.println("<html>");
  CL.println("<head>");
  CL.println("<title>Arduino LED Control</title>");
  CL.println("</head>");
  CL.println("<body>");
  CL.println("<h1>LED</h1>");
  CL.println("<p>Click to switch LED on and off.</p>");
  CL.println("<form method=\"get\">");
  processCheckbox(CL);
  //CL.println("<input type=\"checkbox\" name=\"LED2\" value=\"2\" onclick=\"submit();\">LED2");
  CL.println("</form>");
  CL.println("</body>");
  CL.println("</html>");
}

void writeHTTPLEDStatusPage(EthernetClient &CL) {
  CL.println("<!DOCTYPE html>");
  CL.println("<html>");
  CL.println("<head>");
  CL.println("<title>Arduino Read Switch State</title>");
  CL.println("<meta http-equiv=\"refresh\" content=\"1\">");
  CL.println("</head>");
  CL.println("<body>");
  CL.println("<h1>Switch</h1>");
  CL.println("<p>Switch state is: </p>");
  getSwitchState(CL);
  CL.println("</body>");
  CL.println("</html>");
}

void processCheckbox(EthernetClient &CL) {
  if(HTTP_req.indexOf("LED2=2") > -1) {
    if (LED_status) {
      LED_status = 0;
    } else {
      LED_status = 1;
    }
  }
  
  if (LED_status) {
    digitalWrite(ledPin,HIGH);
    CL.println("<input type=\"checkbox\" name=\"LED2\" value=\"2\" onclick=\"submit();\" checked>LED2");
  } else {
    digitalWrite(ledPin,LOW);
    CL.println("<input type=\"checkbox\" name=\"LED2\" value=\"2\" onclick=\"submit();\">LED2");
  }
}

void getSwitchState(EthernetClient &CL) {
  if (digitalRead(onOffSwitchPin)) {
    CL.println("<p>ON</p>");
  } else {
    CL.println("<p>OFF</p>");
  }
}
