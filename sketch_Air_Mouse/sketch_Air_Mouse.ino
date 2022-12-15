
#include <Wire.h>
#include <I2Cdev.h>
#include <MPU6050.h>
#include <Mouse.h>

MPU6050 mpu;
int16_t ax, ay, az, gx, gy, gz; // 3 accelerometer and 3 gyroscope values
int vx, vy, vx_prec, vy_prec; // vx and vy are x and y coordinates of the cursor respectively and vx_prec and vy_prec are center of the rectangle
int count=0;

void setup() {
  Serial.begin(9600);
  Wire.begin();
  mpu.initialize();
  while (!mpu.testConnection()) {
    delay (1);
    }
}

void loop() {
  mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);

  vx = (gx+300)/200;  
  vy = -(gz-100)/200; 
  
  Mouse.move(vx, vy);
  
  if ( (vx_prec-5)<=vx && vx<=vx_prec+5 && (vy_prec-5)<=vy && vy<=vy_prec+5) { // for checking the cursor doesn't move too much from its actual position: (in this case a 10 pixel square)
    count++;                                                                  
    if(count == 100){ // the click will happen after 2 seconds the pointer has stopped in the 10px square
      if (!Mouse.isPressed(MOUSE_LEFT)) {
        Mouse.press(MOUSE_LEFT);
        count++;
        //count = 0;
      }
    }
    else if(count ==250 ){ // the doubleclick will happen after 4 seconds the pointer has stopped in the 10px square
      //if (!Mouse.is2Pressed(MOUSE_LEFT)) {
        Mouse.press(MOUSE_LEFT);
        delay(10);
        Mouse.release(MOUSE_LEFT);
        delay(10);
        Mouse.press(MOUSE_LEFT);
        count++;
        //count = 0;
      //}
    }
    else {
      if (Mouse.isPressed(MOUSE_LEFT)) {
        Mouse.release(MOUSE_LEFT);
      }
    }
  }
  else {
    vx_prec = vx; // updating values to check the position of the pointer and allow the click
    vy_prec = vy;
    count = 0;
    }
  
  delay(20);
}