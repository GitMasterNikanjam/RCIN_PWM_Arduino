#include "RCIN_PWM.h"
#include <Servo.h>

#define RCIN1_PIN                     45         // Throttle PWM input. from RC reciever or servo tester or any source.
#define RCIN2_PIN                     47
#define RCIN3_PIN                     49
#define RCIN4_PIN                     51
#define RCIN5_PIN                     43

#define RCOUT1_PIN                    8         // Throttle PWM output from ECU.

Servo RCOUT;  // create servo object to control a servo

RCIN_PWM RCIN;

void setup() 
{
  RCOUT.attach(RCOUT1_PIN);  // attaches the servo on pin 9 to the servo object
  Serial.begin(115200);
  RCOUT.writeMicroseconds(800);
  if(!RCIN.attach(1, RCIN5_PIN))
  {
    Serial.println("error attach");
    Serial.println(RCIN.errorMessage);
  }
  if(!RCIN.init())
  {
    Serial.println("error init");
    Serial.println(RCIN.errorMessage);
  }
  delay(1000);
  RCIN_PWM::startCalibration();
}

void loop() {

  if(millis() > 10000)
  {
    RCIN_PWM::stopCalibration();
  }

  RCIN.update();
  Serial.print(RCIN.value.maped);Serial.print(", ");
  Serial.println(RCIN.value.filtered);
  delay(100);
}
