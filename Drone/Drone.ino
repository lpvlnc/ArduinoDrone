#include <SoftwareSerial.h>
#include <Servo.h>

#define pinESC1 8
#define pinESC2 9
#define pinESC3 10
#define pinESC4 11

SoftwareSerial HC05(0, 1);

Servo esc1;
Servo esc2;
Servo esc3;
Servo esc4;

int minSpeed = 1000;
int maxSpeed = 2000;
int speed = minSpeed;

void setup() {
  Serial.begin(9600);
  HC05.begin(9600);
  initializeESC();
}

void loop() {
  readBluetooth();
  setSpeed();
}

void readBluetooth()
{
  if (HC05.available() > 0)
  {
    int message = HC05.readString().toInt();
    messageHandler(message);
  }
}

void messageHandler(int message)
{
  speed = message;
  if (speed < minSpeed)
    speed = minSpeed;

  if (speed > maxSpeed)
    speed = maxSpeed;
}

void initializeESC()
{
  // attach
  esc1.attach(pinESC1);
  esc2.attach(pinESC2);
  esc3.attach(pinESC3);
  esc4.attach(pinESC4);

  // max
  esc1.writeMicroseconds(maxSpeed);
  esc2.writeMicroseconds(maxSpeed);
  esc3.writeMicroseconds(maxSpeed);
  esc4.writeMicroseconds(maxSpeed);

  delay(5000);

  // min
  esc1.writeMicroseconds(minSpeed);
  esc2.writeMicroseconds(minSpeed);
  esc3.writeMicroseconds(minSpeed);
  esc4.writeMicroseconds(minSpeed);

  delay(5000);
}


void setSpeed()
{
  esc1.writeMicroseconds(speed);
  esc2.writeMicroseconds(speed);
  esc3.writeMicroseconds(speed);
  esc4.writeMicroseconds(speed);
}
