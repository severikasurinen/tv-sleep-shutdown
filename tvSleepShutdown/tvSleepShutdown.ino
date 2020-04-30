const int buzzerPin = 2, relayPin = 3, motionPin = 4, irReceiverPin = 5, notifyTime = 540, sleepTime = 600; // buzzer, relay, motion sensor, and infrared receiver pins, time (in seconds) of no movement until starting notify beeps, and time (in seconds) of no movement until turning TV off
const float checkInterval = 1.0, notifyInterval = 20.0; // time (in seconds) between checking for movement, and time (in seconds) between notify beeps
const bool useRemote = true; // do you want to use a remote for activating the device?

int checks = 0; // number of checks since last movement detection
bool active = false; // is device active?
unsigned long lastNotify = 0, lastMovement = 0; // time of last notify beep, and movement detection

void setup()
{
  // initialize pins
  pinMode(buzzerPin, OUTPUT);
  pinMode(relayPin, OUTPUT);
  pinMode(motionPin, INPUT);
  
  if(useRemote)
  {
    pinMode(irReceiverPin, INPUT);
    digitalWrite(relayPin, HIGH); // default TV to off
  }
  else
  {
    active = true;
  }
  
  
  Serial.begin(9600);
}

void loop()
{
  if(active) // is device active?
  {
    if(digitalRead(motionPin)) // movement detected?
    {
      if((millis() - lastMovement)/1000 > round(checkInterval)) // check if already printed in serial monitor during last check
      {
        Serial.print((millis() - lastMovement)/1000.0); // print time since last movement
        Serial.println(" - Motion detected.");
      }
      checks = 0; // set checks to 0
      lastMovement = millis(); // mark down time of latest movement detection
    }
    if(checks*checkInterval >= sleepTime) // has enough time passed to turn TV off?
    {
      digitalWrite(relayPin, HIGH); // turn TV off
      active = false; // comment out this line if you want to restart after new movement
    }
    else if(checks*checkInterval >= notifyTime && (millis() - lastNotify >= notifyInterval*1000 || lastNotify == 0)) // has enough time passed for notify beeps?
    {
      // play notify beep
      tone(buzzerPin, 200, 100);
      lastNotify = millis();
    }
    else
    {
      digitalWrite(relayPin, LOW); // turn TV on if movement detected
    }
    
    // wait for check time
    delay(checkInterval*1000);
    checks++;
  }
  else
  {
    if(useRemote)
    {
      if(!digitalRead(irReceiverPin))
      {
        active = true; // activate device
        // reset values
        checks = 0;
        lastNotify = 0;
        lastMovement = millis();
        
        Serial.println("Device activated.");
      }
      else
      {
        delay(25);
      }
    }
    else
    {
      delay(10000); // idle
    }
    
  }
}
