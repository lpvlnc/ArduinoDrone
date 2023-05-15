#include <SoftwareSerial.h>
#include <Servo.h>

#define pinESC1 7
#define pinESC2 8
#define pinESC3 9
#define pinESC4 10

SoftwareSerial HC05(0, 1);

Servo esc1;
Servo esc2;
Servo esc3;
Servo esc4;

int speed = 1000;

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

  Serial.println("Message:");
  Serial.println(message);

  speed = message;
  if (speed < 1000)
    speed = 1000;

  if (speed > 2000)
    speed = 2000;

  Serial.println("Speed:");
  Serial.println(speed);
  Serial.println("\n");
}

void initializeESC()
{
  // attach
  esc1.attach(pinESC1);
  esc2.attach(pinESC2);
  esc3.attach(pinESC3);
  esc4.attach(pinESC4);

  // max
  esc1.write(2000);
  esc2.write(2000);
  esc3.write(2000);
  esc4.write(2000);

  delay(5000);

  // min
  esc1.write(1000);
  esc2.write(1000);
  esc3.write(1000);
  esc4.write(1000);

  delay(5000);
}


void setSpeed()
{
  esc1.write(speed);
  esc2.write(speed);
  esc3.write(speed);
  esc4.write(speed);
}
