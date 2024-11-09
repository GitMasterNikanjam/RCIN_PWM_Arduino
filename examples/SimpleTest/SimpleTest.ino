
// #######################################################################
// Include libraries:

#include <RCIN_PWM.h>
#include <Servo.h>

// #######################################################################
// Define macros:

#define RCIN1_PIN                     45        // RC PWM input signal pin number.
#define RCIN2_PIN                     47        // RC PWM input signal pin number.
#define RCIN3_PIN                     49        // RC PWM input signal pin number.
#define RCIN4_PIN                     51        // RC PWM input signal pin number.
#define RCIN5_PIN                     43        // RC PWM input signal pin number.

#define RCOUT1_PIN                    8         // RC PWM output signal pin number.

// ############################################################################
// Define Global variables and objects:

// Servo object to control a servo or ESC.
Servo RCOUT;  

//RCIN_PWM object.
RCIN_PWM RCIN_CH1;
RCIN_PWM RCIN_CH2;

// ##########################################################################
// Setup:

void setup() 
{
  Serial.begin(115200);

  RCOUT.attach(RCOUT1_PIN);  // attaches the servo on pin 9 to the servo object
  RCOUT.writeMicroseconds(800);

  RCIN_CH1.parameters.CHANNEL_NUM = 1;
  RCIN_CH2.parameters.CHANNEL_NUM = 2;

  RCIN_CH1.parameters.PIN_NUM = RCIN1_PIN;
  RCIN_CH2.parameters.PIN_NUM = RCIN2_PIN;

  RCIN_CH1.parameters.DEADZONE = 50;
  RCIN_CH1.parameters.MAP_MAX = 2000;
  RCIN_CH1.parameters.MAP_MIN = 1000;

  RCIN_PWM::StaticParameters::FILTER_FRQ = 100;
  RCIN_PWM::StaticParameters::UPDATE_FRQ = 100;
  
  if(!RCIN_CH1.init())
  {
    Serial.println(RCIN_CH1.errorMessage);
    while(1);
  }

  if(!RCIN_CH2.init())
  {
    Serial.println(RCIN_CH2.errorMessage);
    while(1);
  }

  delay(1000);

  RCIN_PWM::startCalibration();
}

// #############################################################################
// Loop:

void loop() {

  if( (millis() > 8000) && (RCIN_PWM::StaticParameters::CALIBRATION_FLAG == true) )
  {
    RCIN_PWM::stopCalibration();
  }

  RCIN_PWM::update();

  Serial.print(RCIN_CH1.value.raw);Serial.print(", ");
  Serial.print(RCIN_CH1.value.maped);Serial.print(", ");
  Serial.println(RCIN_CH1.value.filtered);

  delay(10);
}
