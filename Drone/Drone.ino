#define pinESC1 8
#define pinESC2 9
#define pinESC3 10
#define pinESC4 11
#include <Servo.h>
#include <SoftwareSerial.h>

Servo esc1;
Servo esc2;
Servo esc3;
Servo esc4;

SoftwareSerial AT09(0, 1);

int minSpeed = 1000;
int maxSpeed = 2000;
int speed = minSpeed;
int firstSpeed = minSpeed;
int secondSpeed = minSpeed;
int thirdSpeed = minSpeed;
int fourthSpeed = minSpeed;
bool started = false;
int specialSpeed = minSpeed;
char speedsArray[] = "1000,1000,1000,1000";

void setup() {
  Serial.begin(250000);
  AT09.begin(9600);
  initializeESC();
}

void loop() {
  readBluetooth();
  if (started)
  {
    updateEscSpeed();
  }
}

void readBluetooth()
{
  if (AT09.available() > 0)
  {
    String data = AT09.readString();
    Serial.print(data);

    char buf[sizeof(speedsArray)];
    data.toCharArray(buf, sizeof(buf));
    char *p = buf;
    char *str;
    int i = 1;
    while((str = strtok_r(p, ",", &p)) != NULL)
    {
      if (i == 1)
        firstSpeed = atoi(str);

      if (i == 2)
        secondSpeed = atoi(str);

      if (i == 3)
        thirdSpeed = atoi(str);

      if (i == 4)
        fourthSpeed = atoi(str);
      i++;
    }

    // Serial.println(firstSpeed);
    // Serial.println(secondSpeed);
    // Serial.println(thirdSpeed);
    // Serial.println(fourthSpeed);
  }
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
  started = true;
}


void updateEscSpeed()
{
  NormalizeSpeeds();
  esc1.writeMicroseconds(firstSpeed);
  esc2.writeMicroseconds(secondSpeed);
  esc3.writeMicroseconds(thirdSpeed);
  esc4.writeMicroseconds(fourthSpeed);
}

void NormalizeSpeeds()
{
  if (firstSpeed < minSpeed)
    firstSpeed = minSpeed;
  if (firstSpeed > maxSpeed)
    firstSpeed = maxSpeed;

  if (secondSpeed < minSpeed)
    secondSpeed = minSpeed;
  if (secondSpeed > maxSpeed)
    secondSpeed = maxSpeed;

  if (thirdSpeed < minSpeed)
    thirdSpeed = minSpeed;
  if (thirdSpeed > maxSpeed)
    thirdSpeed = maxSpeed;

  if (fourthSpeed < minSpeed)
    fourthSpeed = minSpeed;
  if (fourthSpeed > maxSpeed)
    fourthSpeed = maxSpeed;
}
