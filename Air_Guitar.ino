#include <Wire.h>
#include <SoftwareSerial.h>
#include <DFRobotDFPlayerMini.h>

#define MPU 0x68

SoftwareSerial mp3Serial(10,11);
DFRobotDFPlayerMini player;

int16_t AcX,AcY,AcZ;
int16_t prevX,prevY,prevZ;

int sensitivity = 8000;
unsigned long lastPlay = 0;
int cooldown = 300;

void setup()
{
  Serial.begin(9600);
  mp3Serial.begin(9600);
  Wire.begin();

  // Wake MPU6050
  Wire.beginTransmission(MPU);
  Wire.write(0x6B);
  Wire.write(0);
  Wire.endTransmission(true);

  if(!player.begin(mp3Serial))
  {
    Serial.println("DFPlayer Error");
    while(true);
  }

  player.volume(30);

  Serial.println("MPU Guitar Ready");
}

void readMPU()
{
  Wire.beginTransmission(MPU);
  Wire.write(0x3B);
  Wire.endTransmission(false);
  Wire.requestFrom(MPU,6,true);

  AcX = Wire.read()<<8 | Wire.read();
  AcY = Wire.read()<<8 | Wire.read();
  AcZ = Wire.read()<<8 | Wire.read();
}

void loop()
{
  readMPU();

  int diffX = AcX - prevX;
  int diffY = AcY - prevY;
  int diffZ = AcZ - prevZ;

  Serial.print("DX:");
  Serial.print(diffX);
  Serial.print(" DY:");
  Serial.print(diffY);
  Serial.print(" DZ:");
  Serial.println(diffZ);

  if(millis() - lastPlay > cooldown)
  {
    if(diffY > sensitivity)
    {
      player.play(1);
      lastPlay = millis();
    }

    else if(diffY < -sensitivity)
    {
      player.play(2);
      lastPlay = millis();
    }

    else if(diffX > sensitivity)
    {
      player.play(3);
      lastPlay = millis();
    }

    else if(diffX < -sensitivity)
    {
      player.play(4);
      lastPlay = millis();
    }

    else if(diffZ > sensitivity)
    {
      player.play(5);
      lastPlay = millis();
    }

    else if(diffZ < -sensitivity)
    {
      player.play(6);
      lastPlay = millis();
    }
  }

  prevX = AcX;
  prevY = AcY;
  prevZ = AcZ;

  delay(20);
}