const int buzzerPin = 2, relayPin = 3, motionPin = 4, notifyTime = 240, sleepTime = 300; // buzzer, relay and, motion sensor pins, time (in seconds) of no movement until starting notify beeps, time (in seconds) of no movement until turning off TV
const float checkInterval = 1.0, notifyInterval = 20.0; // time (in seconds) between checking for movement, and time (in seconds) between notify beeps

int checks = 0; // number of checks since last movement detection
bool shutDown = false; // has TV been turned off?
unsigned long lastNotify = 0, lastMovement = 0; // time of last notify beep, and movement detection

void setup()
{
  // initialize pins
  pinMode(buzzerPin, OUTPUT);
  pinMode(relayPin, OUTPUT);
  pinMode(motionPin, INPUT);
  
  Serial.begin(9600);
}

void loop()
{
  if(!shutDown) // has TV been turned off?
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
      // turn TV off
      digitalWrite(relayPin, HIGH);
      shutDown = true; // comment out this line if you want to restart after new movement
    }
    else if(checks*checkInterval >= notifyTime && millis() - lastNotify >= notifyInterval*1000) // has enough time passed for notify beeps?
    {
      // make notify beep
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
    delay(10000); // idle
  }
}
