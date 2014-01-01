void writeHTTPHeaders(EthernetClient &CL) {
  CL.println("HTTP/1.1 200 OK");
  CL.println("Content-Type: text/html");
  CL.println("Connection: close");
  CL.println();
}



void writeHTTPAJAXwithButton(EthernetClient &CL) {
  CL.println("<!DOCTYPE html>");
  CL.println("<html>");
  CL.println("<head>");
  CL.println("<title>Arduino Web Page</title>");
  CL.println("<script>");
  CL.println("function GetSwitchState() {");
  CL.println("nocache = \"&nocache=\" + Math.random() * 1000000;");
  CL.println("var request = new XMLHttpRequest();");
  CL.println("request.onreadystatechange = function() {");
  CL.println("if (this.readyState == 4) {");
  CL.println("if (this.status == 200) {");
  CL.println("if (this.responseText != null) {");
  CL.println("document.getElementById(\"switch_txt\").innerHTML = this.responseText;");
  CL.println("}}}}");
  CL.println("request.open(\"GET\", \"ajax_switch\" + nocache, true);");
  CL.println("request.send(null);");
  CL.println("setTimeout('GetSwitchState()',500);}");
  CL.println("</script>");
  CL.println("<body onload=\"GetSwitchState()\">");
  CL.println("<h1>Arduino AJAX Switch Status</h1>");
  CL.println("<p id=\"switch_txt\">Switch state: Not requested...</p>");
  //CL.println("<button type=\"button\" onclick=\"GetSwitchState()\">Get Switch State</button>");
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

void getSwitchStateAJAX(EthernetClient &CL) {
  if (digitalRead(onOffSwitchPin)) {
    CL.println("Switch state: ON");
  } else {
    CL.println("Switch state: OFF");
  }
}
