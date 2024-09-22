#include "RCIN_PWM.h"

#define _2PI  6.2831853

// Initialize static array to store instances for each 8 channels
RCIN_PWM* RCIN_PWM::_instances[8] = {nullptr};

bool RCIN_PWM::calibrationFlag = false;

float RCIN_PWM::ParametersStruct::FILTER_FRQ = 0;

float RCIN_PWM::ParametersStruct::UPDATE_FRQ = 0;

float RCIN_PWM::_alpha = 0;

volatile unsigned long RCIN_PWM::_T = 0;

// ##########################################################################
// General function definitions:

 void _calcInput_CH1(void)
{
  // if the pin is high, its the start of an interrupt
  if(digitalRead(RCIN_PWM::_instances[0]->parameters.PIN_NUM) == HIGH)
  { 
    RCIN_PWM::_instances[0]->_startPeriod = micros();
  }
  else
  {
      RCIN_PWM::_instances[0]->value.raw = (int)(micros() - RCIN_PWM::_instances[0]->_startPeriod);
      RCIN_PWM::_instances[0]->_startPeriod = 0;
  }
}

 void _calcInput_CH2(void)
{
  // if the pin is high, its the start of an interrupt
  if(digitalRead(RCIN_PWM::_instances[1]->parameters.PIN_NUM) == HIGH)
  { 
    RCIN_PWM::_instances[1]->_startPeriod = micros();
  }
  else
  {
      RCIN_PWM::_instances[1]->value.raw = (int)(micros() - RCIN_PWM::_instances[1]->_startPeriod);
      RCIN_PWM::_instances[1]->_startPeriod = 0;
  }
}

 void _calcInput_CH3(void)
{
  // if the pin is high, its the start of an interrupt
  if(digitalRead(RCIN_PWM::_instances[2]->parameters.PIN_NUM) == HIGH)
  { 
    RCIN_PWM::_instances[2]->_startPeriod = micros();
  }
  else
  {
      RCIN_PWM::_instances[2]->value.raw = (int)(micros() - RCIN_PWM::_instances[2]->_startPeriod);
      RCIN_PWM::_instances[2]->_startPeriod = 0;
  }
}

 void _calcInput_CH4(void)
{
  // if the pin is high, its the start of an interrupt
  if(digitalRead(RCIN_PWM::_instances[3]->parameters.PIN_NUM) == HIGH)
  { 
    RCIN_PWM::_instances[3]->_startPeriod = micros();
  }
  else
  {
      RCIN_PWM::_instances[3]->value.raw = (int)(micros() - RCIN_PWM::_instances[3]->_startPeriod);
      RCIN_PWM::_instances[3]->_startPeriod = 0;
  }
}

 void _calcInput_CH5(void)
{
  // if the pin is high, its the start of an interrupt
  if(digitalRead(RCIN_PWM::_instances[4]->parameters.PIN_NUM) == HIGH)
  { 
    RCIN_PWM::_instances[4]->_startPeriod = micros();
  }
  else
  {
      RCIN_PWM::_instances[4]->value.raw = (int)(micros() - RCIN_PWM::_instances[4]->_startPeriod);
      RCIN_PWM::_instances[4]->_startPeriod = 0;
  }
}

 void _calcInput_CH6(void)
{
  // if the pin is high, its the start of an interrupt
  if(digitalRead(RCIN_PWM::_instances[5]->parameters.PIN_NUM) == HIGH)
  { 
    RCIN_PWM::_instances[5]->_startPeriod = micros();
  }
  else
  {
      RCIN_PWM::_instances[5]->value.raw = (int)(micros() - RCIN_PWM::_instances[5]->_startPeriod);
      RCIN_PWM::_instances[5]->_startPeriod = 0;
  }
}

 void _calcInput_CH7(void)
{
  // if the pin is high, its the start of an interrupt
  if(digitalRead(RCIN_PWM::_instances[6]->parameters.PIN_NUM) == HIGH)
  { 
    RCIN_PWM::_instances[6]->_startPeriod = micros();
  }
  else
  {
      RCIN_PWM::_instances[6]->value.raw = (int)(micros() - RCIN_PWM::_instances[6]->_startPeriod);
      RCIN_PWM::_instances[6]->_startPeriod = 0;
  }
}

 void _calcInput_CH8(void)
{
  // if the pin is high, its the start of an interrupt
  if(digitalRead(RCIN_PWM::_instances[7]->parameters.PIN_NUM) == HIGH)
  { 
    RCIN_PWM::_instances[7]->_startPeriod = micros();
  }
  else
  {
      RCIN_PWM::_instances[7]->value.raw = (int)(micros() - RCIN_PWM::_instances[7]->_startPeriod);
      RCIN_PWM::_instances[7]->_startPeriod = 0;
  }
}

// ##########################################################################

// RCIN_PWM RCIN_PWM;

RCIN_PWM::RCIN_PWM()
{
		// Set default value at construction function:
		
		parameters.MAP_MIN = 1000.0;
    parameters.MAP_MAX = 2000.0;					
    parameters.RAW_MIN = 1000;
    parameters.RAW_MAX = 2000;
    parameters.RAW_MID = 1500;
    parameters.DEADZONE = 0;	
    parameters.PIN_NUM = -1;	
    parameters.CHANNEL_NUM = 0;	

    value.raw = 0;
    value.maped = 0;
    value.filtered = 0;

    _T = 0;
    _attachedFlag = false;
}

// Destructor
RCIN_PWM::~RCIN_PWM() 
{
    // Detach interrupt when the object is destroyed
    detachInterrupt(digitalPinToInterrupt(parameters.PIN_NUM));
}

bool RCIN_PWM::attach(uint8_t channel_number, uint8_t pin_number)
{	
  if( (channel_number > 8) || (channel_number == 0) )
  {
    errorMessage = "Error RCIN_PWM: channel number is not correct.";
    return false;
  }

  if (_instances[channel_number - 1] != nullptr) 
  {
      // Detach any existing object for this channel
      _instances[channel_number - 1]->detach();
      delete _instances[channel_number - 1];
  }

  // Create a new object for this channel
  _instances[channel_number - 1] = this;

  parameters.CHANNEL_NUM = channel_number;
  parameters.PIN_NUM = pin_number;
  _attachedFlag = true;
  
  return true;
}

// Static function to detach a channel and remove the object
bool RCIN_PWM::detach(void) 
{
  // Detach the interrupt and delete the object
  detachInterrupt(digitalPinToInterrupt(parameters.PIN_NUM));
  _instances[parameters.CHANNEL_NUM - 1] = nullptr;
  _attachedFlag = false;
  return true;
}


uint16_t RCIN_PWM::_map(void)
{
  if(abs(value.raw - parameters.RAW_MID) > parameters.DEADZONE )
  {
    value.maped = map(value.raw,parameters.RAW_MIN,parameters.RAW_MAX,parameters.MAP_MIN,parameters.MAP_MAX);
  }
  else
  {
    value.maped = map(parameters.RAW_MID,parameters.RAW_MIN,parameters.RAW_MAX,parameters.MAP_MIN,parameters.MAP_MAX);
  }

  if(value.maped > parameters.MAP_MAX)
  {
    value.maped = parameters.MAP_MAX;
  }

  if(value.maped < parameters.MAP_MIN)
  {
    value.maped = parameters.MAP_MIN;
  }

  return value.maped;
}
	
bool RCIN_PWM::setMapDomain(uint16_t min, uint16_t max)
{
  if(min > max)
  {
    errorMessage = "Error RCIN_PWM: Max value of mapping can not be lower than Min value of mapping.";
    return false;
  }
	parameters.MAP_MIN = min;
	parameters.MAP_MAX = max;

  return true;
}

void RCIN_PWM::setDeadzone(uint16_t value)
{
  parameters.DEADZONE = value;
}

bool RCIN_PWM::setRawDomain(uint16_t min, uint16_t max)
{
  if(min > max)
  {
    errorMessage = "Error RCIN_PWM: Max value of raw domain can not be lower than Min value of raw domain.";
    return false;
  }

  parameters.RAW_MIN = min;
  parameters.RAW_MAX = max;
  parameters.RAW_MID = (min+max/2.0);
		
  return true;
}

bool RCIN_PWM::setFilterFrequency(float frq)
{
  if(frq < 0)
  {
    errorMessage = "Error RCIN_PWM: filter frequency can not be negative.";
    return false;
  }
	RCIN_PWM::parameters.FILTER_FRQ = frq;
}

bool RCIN_PWM::setUpdateFrequency(float frq)
{
  if(frq < 0)
  {
    errorMessage = "Error RCIN_PWM: Update frequency can not be negative.";
    return false;
  }

	RCIN_PWM::parameters.UPDATE_FRQ = frq;
}

void RCIN_PWM::startCalibration(void)
{
	RCIN_PWM::calibrationFlag = true;

  for(int i = 1; i <= 8; i++)
  {
    if(RCIN_PWM::_instances[i-1]->_attachedFlag == true)
    {
      RCIN_PWM::_instances[i-1]->parameters.RAW_MAX = _instances[i-1]->value.raw;
      RCIN_PWM::_instances[i-1]->parameters.RAW_MIN = _instances[i-1]->value.raw;
    }
  }	
}

void RCIN_PWM::stopCalibration(void)
{
	RCIN_PWM::calibrationFlag = false;	

  for(int i = 1; i <= 8; i++)
  {
    if(RCIN_PWM::_instances[i-1]->_attachedFlag == true)
    {
      RCIN_PWM::_instances[i-1]->parameters.RAW_MID = (float)(RCIN_PWM::_instances[i-1]->parameters.RAW_MAX + RCIN_PWM::_instances[i-1]->parameters.RAW_MIN) * 0.5;
    }
  }
}
	
void RCIN_PWM::update(void)
{
	unsigned long t = micros();
  unsigned long dt = t - _T;

  if(RCIN_PWM::ParametersStruct::UPDATE_FRQ > 0)
  {
    if(dt < (1000000.0/RCIN_PWM::ParametersStruct::UPDATE_FRQ))
    {
      return ;
    }
  }

  if(RCIN_PWM::ParametersStruct::FILTER_FRQ > 0)
  {
    RCIN_PWM::_alpha = 1.0 / (1.0 + _2PI * RCIN_PWM::ParametersStruct::FILTER_FRQ * dt / 1000000.0);
  }
  else
  {
    RCIN_PWM::_alpha = 0;
  }

  for(int i = 1; i <= 8; i++)
  {
    if(_instances[i-1]->_attachedFlag == true)
    {
      if(calibrationFlag)
      {
        if(_instances[i-1]->value.raw > _instances[i-1]->parameters.RAW_MAX)
        {
          _instances[i-1]->parameters.RAW_MAX = _instances[i-1]->value.raw;
        }
        if(_instances[i-1]->value.raw < _instances[i-1]->parameters.RAW_MIN)
        {
          _instances[i-1]->parameters.RAW_MIN = _instances[i-1]->value.raw;
        }

        _instances[i-1]->value.maped = _instances[i-1]->value.raw;
      }
      else
      {
        _instances[i-1]->_map();
      }
      
      if(_instances[i-1]->parameters.FILTER_FRQ > 0)
      {
        _instances[i-1]->value.filtered = _alpha * _instances[i-1]->value.filtered + (1.0 - _alpha) * _instances[i-1]->value.maped;
      }
      else
      {
        _instances[i-1]->value.filtered = _instances[i-1]->value.maped;
      }
    }
  }	
	
		_T = t;
	
}	

bool RCIN_PWM::init(void)
{
  if(!_checkParameters())
  {
    return false;
  }

  if( (_attachedFlag == true) && (parameters.PIN_NUM >= 0) )
  {
    pinMode(parameters.PIN_NUM,INPUT_PULLUP);
    
    _funPointer = nullptr;
    
    switch(parameters.CHANNEL_NUM)
    {
      case 1:
        _funPointer = _calcInput_CH1;
      break;
      case 2:
        _funPointer = _calcInput_CH2;
      break;
      case 3:
        _funPointer = _calcInput_CH3;
      break;
      case 4:
        _funPointer = _calcInput_CH4;
      break;
      case 5:
        _funPointer = _calcInput_CH5;
      break;
      case 6:
        _funPointer = _calcInput_CH6;
      break;
      case 7:
        _funPointer = _calcInput_CH7;
      break;
      case 8:
        _funPointer = _calcInput_CH8;
      break;		
    }

    attachInterrupt(digitalPinToInterrupt(parameters.PIN_NUM), _funPointer, CHANGE);
  }
  else
  {
    _attachedFlag = false;
  }
  

  return true;
}

bool RCIN_PWM::_checkParameters(void)
{
  bool state = (parameters.FILTER_FRQ >= 0) && (parameters.MAP_MAX > parameters.MAP_MIN) &&
               (parameters.RAW_MAX > parameters.RAW_MIN) && (parameters.RAW_MID > parameters.RAW_MIN) &&
               (parameters.RAW_MID < parameters.RAW_MAX) && (parameters.UPDATE_FRQ >= 0) &&
               (parameters.PIN_NUM >= 0) && (parameters.CHANNEL_NUM >= 1) && (parameters.CHANNEL_NUM <= 8) ;

  if(state == false)
  {
    errorMessage = "Error RCIN_PWM: One or some parameters is not correct.";
    return false;
  }

  return true;
}
