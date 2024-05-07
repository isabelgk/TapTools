/** @file
 *
 * @ingroup modularLibrary
 *
 * @brief #TTRamp class embedes a scheduling mechanism from #TTDrive and a function from #TTFunction
 *
 * @details
 *
 * @authors Tim Place, Théo de la Hogue
 *
 * @copyright © 2008, Tim Place, Théo de la Hogue @n
 * This code is licensed under the terms of the "New BSD License" @n
 * http://creativecommons.org/licenses/BSD/
 */


#ifndef __TT_RAMP_H__
#define __TT_RAMP_H__

#include "TTModularIncludes.h"
#include "TTClock.h"

typedef void (*TTRampCallback)(void *, TTUInt32, TTFloat64 *);

/****************************************************************************************************/
// Class Specification


// Specification of our base class
class TTMODULAR_EXPORT TTRamp : public TTObjectBase {
    
    TTCLASS_SETUP(TTRamp)
  	
public: ///< It is public in order to be able to extend clock or function unit parameters as attributes of another object
    
    TTObject                mDriveUnit;         ///< The actual clock unit object defined by mDrive // TODO: Jamomacore #294 : Ease the access of the object of a kTypeObject attribute of a TTObject
#ifndef TT_NO_DSP
    TTObject                mFunctionUnit;		///< The actual function unit object defined by mFunction // TODO: Jamomacore #294 : Ease the access of the object of a kTypeObject attribute of a TTObject
#endif
    
private:
    
    TTSymbol                mDrive;             ///< ATTRIBUTE : The name of the clock to drive the ramp
#ifndef TT_NO_DSP
    TTSymbol                mFunction;			///< ATTRIBUTE : The name of the function unit
#endif
    TTFloat64               mRampTime;          ///< ATTRIBUTE : The time of ramp in millisecond
    TTUInt32                mNumValues;
    
    TTRampCallback          mCallback;			///<
    TTPtr                   mBaton;				///<
    
    TTFloat64               *startValue;
    TTFloat64               *targetValue;
    TTFloat64               *currentValue;
    
    /** Attribute getter */
    TTErr   getRunning(TTValue& value);
    
    /** Attribute setter */
    // TODO: Jamomacore #294 : Ease the access of the object of a kTypeObject attribute of a TTObject
    TTErr   setDrive(const TTValue& clockName);
    TTErr   getDriveLibrary(TTValue& value);
    TTErr   getDriveParameters(TTValue& value);
    TTErr   getDriveParameterValue(TTValue& value);
    TTErr   setDriveParameterValue(const TTValue& value);
    
#ifndef TT_NO_DSP
    // TODO: Jamomacore #294 : Ease the access of the object of a kTypeObject attribute of a TTObject
    TTErr   setFunction(const TTValue& functionName);
    TTErr   getFunctionLibrary(TTValue& value);
    TTErr   getFunctionParameters(TTValue& value);
    TTErr   getFunctionParameterValue(TTValue& value);
    TTErr   setFunctionParameterValue(const TTValue& value);
#endif
    TTErr   setNumValues(const TTValue& inputValue);

    /** Message */
    TTErr   Set(const TTValue& inputValue, TTValue& outputValue);
    
    TTErr   Target(const TTValue& inputValue, TTValue& outputValue);
    
    TTErr   Go(const TTValue& inputValue, TTValue& outputValue);
    
    TTErr   Slide(const TTValue& inputValue, TTValue& outputValue);
    
    TTErr   Tick();
    
    TTErr   Stop();

public: ///< It is public in order to be able to extend clock or function unit parameters as attributes of another object
    

    
#ifndef TT_NO_DSP

#endif
    friend void TTMODULAR_EXPORT TTRampDriveCallback(TTPtr object, TTFloat64 position, TTFloat64 date);
};
typedef TTRamp* TTRampPtr;

/**
 @param	baton						..
 @param	data						..
 @return							an error code */
void TTMODULAR_EXPORT TTRampDriveCallback(TTPtr object, TTFloat64 position, TTFloat64 date);

#endif // __TT_RAMP_H__
