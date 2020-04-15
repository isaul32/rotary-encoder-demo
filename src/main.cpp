#include <Arduino.h>

static int pinA = 2;
static int pinB = 3;

// volatile is needed with isr!
volatile unsigned int encoderPos = 0;
unsigned int lastReportedPos = 1;
static boolean rotating = false;

boolean aSet = false;
boolean bSet = false;

void doEncoderA()
{
  if (rotating)
  {
    delay(2);
  }
  if (digitalRead(pinA) != aSet)
  {
    aSet = !aSet;
    if (aSet && !bSet && encoderPos < 100)
    {
      encoderPos += 1;
    }
    rotating = false;
  }
}

void doEncoderB()
{
  if (rotating)
  {
    delay(2);
  }
  if (digitalRead(pinB) != bSet)
  {
    bSet = !bSet;
    if (bSet && !aSet && encoderPos > 0)
    {
      encoderPos -= 1;
    }
    rotating = false;
  }
}

void setup()
{
  pinMode(pinA, INPUT);
  pinMode(pinB, INPUT);

  attachInterrupt(digitalPinToInterrupt(2), doEncoderA, CHANGE);
  attachInterrupt(digitalPinToInterrupt(3), doEncoderB, CHANGE);

  pinMode(4, INPUT_PULLUP);

  Serial.begin(115200);
}

void loop()
{
  rotating = true;
  if (lastReportedPos != encoderPos)
  {
    Serial.print("Index:");
    Serial.println(encoderPos, DEC);
    lastReportedPos = encoderPos;
  }
}
