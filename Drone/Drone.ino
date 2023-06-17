#define pinESC1 8
#define pinESC2 9
#define pinESC3 10
#define pinESC4 11
#include <Servo.h>
#include <SoftwareSerial.h>
#include <Wire.h>
#include <MPU6050.h>

Servo esc1;
Servo esc2;
Servo esc3;
Servo esc4;

SoftwareSerial AT09(0, 1);

MPU6050 mpu;

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

class Vector3D {
public:
  float x;
  float y;
  float z;
};

void setup() {
  Serial.begin(250000);
  AT09.begin(9600);
  Wire.begin();
  mpu.initialize();
  initializeESC();
}

void loop() {
  readBluetooth();
  if (started)
  {
    stabilizeFlight();
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
    if (firstSpeed == 0 && secondSpeed == 0 && thirdSpeed == 0 && fourthSpeed == 0)
    {
      Serial.println("Parada de emergência iniciada");
      updateEscSpeed();
      started = false;
      Serial.println("Parada de emergência finalizada");
    }
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

void stabilizeFlight()
{
  Vector3D acceleration;
  acceleration.x = mpu.getAccelerationX() / 16384.0; // Divide by sensitivity scale factor
  acceleration.y = mpu.getAccelerationY() / 16384.0;
  acceleration.z = mpu.getAccelerationZ() / 16384.0;

  Vector3D gyro;
  gyro.x = mpu.getRotationX();
  gyro.y = mpu.getRotationY();
  gyro.z = mpu.getRotationZ();

  float rollAngle = atan2(acceleration.y, acceleration.z) * 180 / PI;
  float pitchAngle = atan2(-acceleration.x, sqrt(acceleration.y * acceleration.y + acceleration.z * acceleration.z)) * 180 / PI;

  float rollRate = gyro.x;
  float pitchRate = gyro.y;

  float rollOutput = map(rollAngle, -90, 90, -500, 500);
  float pitchOutput = map(pitchAngle, -90, 90, -500, 500);

  firstSpeed += pitchOutput - rollOutput;
  secondSpeed += pitchOutput + rollOutput;
  thirdSpeed -= pitchOutput - rollOutput;
  fourthSpeed -= pitchOutput + rollOutput;
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
