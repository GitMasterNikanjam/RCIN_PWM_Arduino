#ifndef RCIN_PWM_H
#define RCIN_PWM_H

// ##################################################################
// Library information:
/*
RCIN - a small RC hobby receiver library for Arduino.
Developed by: Mohammad Nikanjam

This library used for 8 channel RC reciever pwm measurement.
For more information read README.md file.
*/
// ###################################################################
// Include libraaries:

#include <Arduino.h>

// ####################################################################
// Define macros:

#define RCIN_PWM_VERSION  "v1.0" // software version of this library

// ###################################################################################
//  General function declarations:

// ##################################################################################3
// RCIN class

// RCIN class
class RCIN_PWM
{
  public:

    // // Get the single instance of the class (Singleton pattern)
    // static RCIN_PWM_CLASS& getInstance();

    // // Delete the copy constructor and assignment operator to prevent copying
    // RCIN_PWM_CLASS(const RCIN_PWM_CLASS&) = delete;
    // RCIN_PWM_CLASS& operator=(const RCIN_PWM_CLASS&) = delete;

    // Last error accured for object.
    String errorMessage;

    // Parameters struct
    struct ParametersStruct
    {
      // Min pwm value for all channels maped value. [us].
      uint16_t MAP_MIN;

      // Max pwm value for all channels maped value. [us].
      uint16_t MAP_MAX;

      // Dead zone value [us] of channels from RAW_MID value point that ignored.
      uint16_t DEADZONE;

      // [Hz]. Low pass filter frequency(Cutoff filter frequency). **Hint: 0 value means disable it.
      float FILTER_FRQ;

      // Update method frequency. This value insure that RC maped/filtered values just update in certain frequency. **Hint: 0 value means disable it.
      float UPDATE_FRQ;

      // Digital pin number of arduino that used for input pwm signal.
      // -1 value means no pin.
      int8_t PIN_NUM;	

      uint8_t CHANNEL_NUM;											

      // [us]. Min domain raw pwm value for channels. default is 1000 us. After calibration its value changed.
      uint16_t RAW_MIN;	
  
      // [us]. Mid domain raw pwm value for chanels. default is 1500 us. After calibration its value changed.
      uint16_t RAW_MID;	
      
      // [us]. Max domain raw pwm value for channels. default is 2000 us. After calibration its value changed.
      uint16_t RAW_MAX;	
    }parameters;

    // Values struct.
    struct VariablesStruct
    {
      // [us]. Raw input pwm signal measurement values. (For 8 channel).
      volatile uint16_t raw;

      // [us]. Maped input pwm signal values. (For 8 channel).
      float maped;
      
      // [us]. Filtered maped input pwm signal values. (For 8 channel).
      float filtered;		
    }value;

    /**
    * Constructor. Init default value of variables and parameters.
    */
    RCIN_PWM();

    // Destructor
    ~RCIN_PWM();

    /**
     * Initialize RCIN_PWM object. Check parameters validation.
     * @return true if successed.
     */ 
    bool init(void);

    /**
     * Attach a digital pin to RCIN_PWM channels. 
     * Static function to create or get the instance for a specific channel
     * Creates an instance of the class for a specific channel and pin. If an object for the same channel exists, it is replaced.
     */ 
    bool attach(uint8_t channel_number, uint8_t pin_number);
	
    // Static function to detach the object for a specific channel
    // Removes an object for a specific channel.
    bool detach(void);

    /**
     * Set Map domain. [us]
     * @return true if successed.
     */
    bool setMapDomain(uint16_t min, uint16_t max);
    
    /**
     * Set dead zone of raw pwm value [us] for channels.
     * @return true if successed.
     */
    void setDeadzone(uint16_t value);
    
    /**
     * Set raw domain of pwm raw value [us].
     * @return true if successed.
     */
    bool setRawDomain(uint16_t min, uint16_t max);
    
    /**
     * Set filter frequency [Hz] for low pass filter. **Hint: 0 value means it is disable.
     * @return true if successed.
     */
    bool setFilterFrequency(float value);
    
    /**
     * Set update method frequency [Hz]. **Hint: 0 value means it is disable.
     * @return true if successed.
     */
    bool setUpdateFrequency(float value);
    
    /**
     * Change calibration flag to true.
     */
    static void startCalibration(void);
    
    /**
     * Change calibration flag to false.
     */
    static void stopCalibration(void);

    /**
     * update and calculate maped/filtered pwm value.
     */
    void update(void);

  private:

    // Flag for active/inactive calibration process.
    static bool _calibrationFlag;

    // Static array to store instances per channel
    // Array to hold one object per channel (1-8)
    static RCIN_PWM* _instances[8];

    // Define function pointer type
    typedef void (*FunctionPtr)();

    // FunctionPtr object for RCIN PWM interrupts handler.
    FunctionPtr _funPointer;

    // Flag for store state of channeles that attached(true) or not_attached(false)
    bool _attachedFlag;

    // Start timer value for each pwm channel.
    volatile unsigned long _startPeriod;						
    
    // Gain that used at low pass filter calculations.
    // _alpha = 1.0 / (1.0 + _2PI * parameters.FILTER_FRQ / UPDATE_FRQ)
    float _alpha;
    
    // [us]. Time at update() method.
    volatile unsigned long _T;

    /**
    *
    */
    uint16_t _map(void);

    /** 
    * Check parameters validation.
    * @return true if successed.
    */
    bool _checkParameters(void);

    // Interrupt handler function for RCIN channel 1.
    friend void _calcInput_CH1(void);

    // Interrupt handler function for RCIN channel 2.
    friend void _calcInput_CH2(void);

    // Interrupt handler function for RCIN channel 3.
    friend void _calcInput_CH3(void);

    // Interrupt handler function for RCIN channel 4.
    friend void _calcInput_CH4(void);

    // Interrupt handler function for RCIN channel 5.
    friend void _calcInput_CH5(void);

    // Interrupt handler function for RCIN channel 6.
    friend void _calcInput_CH6(void);

    // Interrupt handler function for RCIN channel 7.
    friend void _calcInput_CH7(void);

    // Interrupt handler function for RCIN channel 8.
    friend void _calcInput_CH8(void);
    
};


#endif
