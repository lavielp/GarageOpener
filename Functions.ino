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

void pressGarageButton() {
  Serial.println("The button was pressed.");
  digitalWrite(outputPin,HIGH);
  delay(250);
  digitalWrite(outputPin,LOW);
}
