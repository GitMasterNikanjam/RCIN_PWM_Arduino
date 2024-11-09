# <RCIN_PWM.h> Library For Arduino

- This is a small RC hobby receiver library for Arduino.      
- This library can be used to measure up to 8 PWM channels of a standard RC hobby receiver.           
- This library uses only hardware interrupts and cannot use software interrupts.         
- This library can be used with boards: UNO, NANO, MEGA2560, DUE and ESP32.    

## Public Member Functions

```c++

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

```

## Public Member Varaibles

```c++

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

```
---------------------------------------------------------------------------------

## A Short Guide to Types of Interrupts  

  There are two types of interrupts:

  - **Hardware Interrupt:**     
    It happens when an external event occurs like an external interrupt pin changes its state from 
    LOW to HIGH or HIGH to LOW.
    These interrupt are interpreted by hardware and are very fast. These interrupts can be set to trigger on the event 
    of RISING or FALLING or LOW levels.

  - **Software Interrupt:**  
    It happens according to the instruction from the software. For example Timer interrupts are software interrupt.
    Arduinos can have more interrupt pins enabled by using pin change interrupts.
    The library for use software interrupts is PinChangeInterrupt Library 1.2.9. That it can install from Manage Libraries Tab in Arduino IDE Tab.

  ### Arduino Board External Interrupt pins:(Hardware interrupt pins)

  - UNO , NANO    
    2,3   

  - Mega    
    2,3,18,19,20,21(pins 20 & 21 are not available to use for interrupts while they are used for I2C communication)   

  - Nano 33 IoT    
    2, 3, 9, 10, 11, 13, A1, A5, A7  

  - Due     
    all digital pins   

  - ESP32   
    All GPIO pins in an ESP32 board can be configured to act as interrupt request inputs.   
-------------------------------------------------------------------------------------------------


