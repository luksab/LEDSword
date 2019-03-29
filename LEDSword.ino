
#include <MPU6050_tockn.h>
#include <Wire.h>
#include <FastLED.h>

#define NUM_LEDS 144
CRGBArray<NUM_LEDS> leds;

MPU6050 mpu6050(Wire, 0.1, 0.9);

void setup() {
  Serial.begin(9600);
  Wire.begin(22,21);
  mpu6050.begin();
  //mpu6050.calcGyroOffsets(true);
  mpu6050.setGyroOffsets(-0.2,1.2,-1.2);

  FastLED.addLeds<WS2812B, 13, GRB>(leds, NUM_LEDS);
  FastLED.setBrightness(50);
  FastLED.setDither(255);
  FastLED.setMaxPowerInMilliWatts(1000);
}
float i = 0;

float mapfloat(float x, float in_min, float in_max, float out_min, float out_max)
{
 return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

float gradiant(float x){
  if(x < 0) return 0;
  if(x > 3) return 0;
  if(x < 1) return x;
  if(x > 2) return 3-x;
  return 1;
}

bool startAnim = false;
bool continueAnim = false;

void loop() {
  mpu6050.update();
  /*Serial.print("angleX : ");
  Serial.print(mpu6050.getAngleX());
  Serial.print("\tangleY : ");
  Serial.print(mpu6050.getAngleY());
  Serial.print("\tangleZ : ");
  Serial.println(mpu6050.getAngleZ());*/
  int val;
  if(continueAnim && mpu6050.getAngleY() > 0){
    for(int I=0;I<NUM_LEDS;I++){
      val = 255;
      leds[I][0] = val;
      leds[I][1] = val;
      leds[I][2] = val;
    }
    startAnim = false;
    continueAnim = false;
    FastLED.delay(10);
  }
  if(startAnim && mpu6050.getGyroY() > 360) continueAnim = true;
  else if(mpu6050.getGyroY() > 360) startAnim = true;
  else{startAnim = false;continueAnim = false;}

  i = mapfloat(mpu6050.getAngleY(),-180,180,1,NUM_LEDS-2);
  
  for(int I=0;I<NUM_LEDS;I++){
    val = gradiant(i-I)*255;
    leds[I][0] = val;
    leds[I][1] = val;
    leds[I][2] = val;
  }
  FastLED.show();
}
