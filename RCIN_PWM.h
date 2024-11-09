#ifndef RCIN_PWM_H
#define RCIN_PWM_H

// ###################################################################
// Include libraries:

#include <Arduino.h>

// ####################################################################
// Define Global macros:


// ###################################################################################
//  General function declarations:

// ##################################################################################3
// RCIN class

/**
  @class RCIN_PWM
  @brief A class for RC hobby pwm input managements.
  @note It can only created 8 object from this class.
*/ 
class RCIN_PWM
{
  public:

    /**
      @brief Last error accured for object.
      @note This is static member public variable.
    */ 
    static String errorMessage;

    /**
      @struct ParametersStruct
      @brief Parameters structure.
    */ 
    struct ParametersStruct
    {
      /// @brief Min pwm value for maped value. [us].
      uint16_t MAP_MIN;

      /// @brief Max pwm value for maped value. [us].
      uint16_t MAP_MAX;

      /// @brief Dead zone value from the RAW_MID value point that is ignored. [us]
      uint16_t DEADZONE;

      /**
        @brief Digital pin number of arduino that used for input pwm signal.
        @note -1 value means no pin.
      */ 
      int8_t PIN_NUM;	

      /// @brief Channel number for this object.
      uint8_t CHANNEL_NUM;											

      /// @brief Min domain raw pwm value for channels. default is 1000 us. After calibration its value changed. [us]
      uint16_t RAW_MIN;	
  
      /// @brief Mid domain raw pwm value for chanels. default is 1500 us. After calibration its value changed. [us]
      uint16_t RAW_MID;	
      
      /// @brief Max domain raw pwm value for channels. default is 2000 us. After calibration its value changed. [us]
      uint16_t RAW_MAX;	
    }parameters;

    /**
      @struct StaticParametersStructure
      @brief Static parameters structure.
    */
    struct StaticParameters
    {
      /**
        @brief brief Flag for active/inactive state calibration process. 
        @note true: Calibration proess is running, false: Calibration process is stop.
      */ 
      static bool CALIBRATION_FLAG;

      /**
        @brief Low pass filter frequency(Cutoff filter frequency). [Hz].
        @note **Hint: A value of 0 means to disable it.
      */  
      static float FILTER_FRQ;

      /**
        @brief Update method frequency. This value insure that RC maped/filtered values just update in certain frequency.
        @note **Hint: A value of 0 means to disable it.
      */
      static float UPDATE_FRQ;
    }staticParameters;

    /**
      @struct ValuesStructure
      @brief Value structure.
    */ 
    struct ValuesStructure
    {
      /// @brief Raw input pwm signal measurement values. [us]
      volatile uint16_t raw;

      /// @brief Maped input pwm signal values. [us]
      float maped;
      
      /// @brief Filtered maped input pwm signal values. [us]
      float filtered;		
    }value;

    /**
    * @brief Default constructor. Init default value of variables and parameters.
    */
    RCIN_PWM();

    /// @brief Destructor
    ~RCIN_PWM();

    /**
     * @brief Initialize RCIN_PWM object. Check parameters validation.
     * @return true if succeeded.
     */ 
    bool init(void);

    /**
     * @brief Set Map domain. [us]
     * @return true if succeeded.
     */
    bool setMapDomain(uint16_t min, uint16_t max);
    
    /**
     * @brief Set dead zone of raw pwm value [us] for channels.
     * @return true if succeeded.
     */
    void setDeadzone(uint16_t value);
    
    /**
     * @brief Set raw domain of pwm raw value [us].
     * @return true if succeeded.
     */
    bool setRawDomain(uint16_t min, uint16_t max);
    
    // -------------------------------------------------------------------------------
    // Static Public Member Functions:

    /**
     * @brief Set filter frequency [Hz] for low pass filter. **Hint: 0 value means it is disable.
     * @return true if succeeded.
     */
    static bool setFilterFrequency(float value);
    
    /**
     * @brief Set update method frequency [Hz]. **Hint: 0 value means it is disable.
     * @return true if succeeded.
     */
    static bool setUpdateFrequency(float value);
    
    /**
      @brief Change calibration flag to true.
      @note This is a static member function.
     */
    static void startCalibration(void);
    
    /**
      @brief Change calibration flag to false.
      @note This is a static member function.
     */
    static void stopCalibration(void);

    /**
      @brief Update and calculate maped/filtered pwm value.
      @note This is a static member function.
     */
    static void update(void);

  private:

    /**
      @brief Static array to store instances per channel.   
      Array to hold one object per channel (1-8).  
    */ 
    static RCIN_PWM* _instances[8];

    /// @brief Define function pointer type
    typedef void (*FunctionPtr)();

    /// @brief FunctionPtr object for RCIN PWM interrupts handler.
    FunctionPtr _funPointer;

    /// @brief Flag for store state of channeles that attached(true) or not_attached(false)
    bool _attachedFlag;

    /// @brief Start timer value for each pwm channel.
    volatile unsigned long _startPeriod;						
    
    /**
      @brief Gain that used at low pass filter calculations.
      @note _alpha = 1.0 / (1.0 + _2PI * _FILTER_FRQ / _UPDATE_FRQ)
    */ 
    static float _alpha;
    
    /// @brief Time at update() method.[us]. 
    static volatile unsigned long _T;

    /**
    * @brief Calculate pwm maped value.
    * @return PWM maped value.
    */
    uint16_t _map(void);

    /** 
    * @brief Check parameters validation.
    * @return true if succeeded.
    */
    bool _checkParameters(void);

    // ----------------------------------------------------------------------
    // Friend functions:

    /// @brief Interrupt handler function for RCIN channel 1.
    friend void _calcInput_CH1(void);

    /// @brief Interrupt handler function for RCIN channel 2.
    friend void _calcInput_CH2(void);

    /// @brief Interrupt handler function for RCIN channel 3.
    friend void _calcInput_CH3(void);

    /// @brief Interrupt handler function for RCIN channel 4.
    friend void _calcInput_CH4(void);

    /// @brief Interrupt handler function for RCIN channel 5.
    friend void _calcInput_CH5(void);

    /// @brief Interrupt handler function for RCIN channel 6.
    friend void _calcInput_CH6(void);

    /// @brief Interrupt handler function for RCIN channel 7.
    friend void _calcInput_CH7(void);

    /// @brief Interrupt handler function for RCIN channel 8.
    friend void _calcInput_CH8(void);
    
};


#endif
