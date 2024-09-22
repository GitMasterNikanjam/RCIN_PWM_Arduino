
// #######################################################################
// Include libraries:

#include "RCIN_PWM.h"
#include <Servo.h>

// #######################################################################
// Define macros:


#define RCIN1_PIN                     45        // RC PWM input signal pin number.
#define RCIN2_PIN                     47        // RC PWM input signal pin number.
#define RCIN3_PIN                     49        // RC PWM input signal pin number.
#define RCIN4_PIN                     51        // RC PWM input signal pin number.
#define RCIN5_PIN                     43        // RC PWM input signal pin number.

#define RCOUT1_PIN                    8         // RC PWM output signal pin number.
#define RCOUT2_PIN                    9         // RC PWM output signal pin number.
#define RCOUT3_PIN                    10        // RC PWM output signal pin number.
#define RCOUT4_PIN                    11        // RC PWM output signal pin number.

// ############################################################################
// Define Global variables and objects:

// Servo object to control a servo or ESC.
Servo RCOUT;  

//RCIN_PWM object.
RCIN_PWM RCIN;

// ##########################################################################
// Setup:

void setup() 
{
  Serial.begin(115200);

  RCOUT.attach(RCOUT1_PIN);  // attaches the servo on pin 9 to the servo object
  RCOUT.writeMicroseconds(800);

  if(!RCIN.attach(8, RCIN5_PIN))
  {
    Serial.println(RCIN.errorMessage);
    while(1);
  }

  RCIN_PWM::ParametersStruct::FILTER_FRQ = 20;
  RCIN_PWM::ParametersStruct::UPDATE_FRQ = 100;
  RCIN.parameters.DEADZONE = 50;
  
  if(!RCIN.init())
  {
    Serial.println(RCIN.errorMessage);
    while(1);
  }

  delay(1000);

  RCIN_PWM::startCalibration();
}

// #############################################################################
// Loop:

void loop() {

  if( (millis() > 8000) && (RCIN_PWM::calibrationFlag == true) )
  {
    RCIN_PWM::stopCalibration();
  }

  RCIN_PWM::update();

  Serial.print(RCIN.value.raw);Serial.print(", ");
  Serial.print(RCIN.value.maped);Serial.print(", ");
  Serial.println(RCIN.value.filtered);

  delay(10);
}
