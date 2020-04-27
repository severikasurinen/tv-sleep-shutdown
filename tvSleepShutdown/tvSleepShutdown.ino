const int buzzerPin = 2, relayPin = 3, motionPin = 4, notifyTime = 240, sleepTime = 300;
const float checkInterval = 1.0, notifyInterval = 20.0;

int checks = 0;
bool shutDown = false;
unsigned long lastNotify = 0, lastMovement = 0;

void setup()
{
  pinMode(buzzerPin, OUTPUT);
  pinMode(relayPin, OUTPUT);
  pinMode(motionPin, INPUT);
  
  Serial.begin(9600);
}

void loop()
{
  if(!shutDown)
  {
    if(digitalRead(motionPin))
    {
      if((millis() - lastMovement)/1000 > round(checkInterval))
      {
        Serial.print((millis() - lastMovement)/1000.0);
        Serial.println(" - Motion detected.");
      }
      checks = 0;
      lastMovement = millis();
    }
    if(checks*checkInterval >= sleepTime)
    {
      digitalWrite(relayPin, HIGH);
      shutDown = true;
    }
    else if(checks*checkInterval >= notifyTime && millis() - lastNotify >= notifyInterval*1000)
    {
      tone(buzzerPin, 200, 100);
      lastNotify = millis();
    }
    else
    {
      digitalWrite(relayPin, LOW);
    }
    
    delay(checkInterval*1000);
    checks++;
  }
  else
  {
    delay(10000);
  }
}
