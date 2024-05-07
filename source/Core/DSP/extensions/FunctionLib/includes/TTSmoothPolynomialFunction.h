/** @file
 *
 * @ingroup dspFunctionLib
 *
 * @brief #TTSmoothPolynomialFunction Unit for Jamoms DSP
 *
 * @details This function has the following properties: @n
 *  - It ramps from 0 to 1 @n
 *	- The 1st and 2nd derivate equals 0 at x=0 and x=1, giving a smooth start and end to the ramp @n
 *	- The start and end of the ramp should be more smooth than for cosine in #TTCosineFunction, as the 2nd order derivative of cosine do not equal 0 @n
 *	- The function is symmetrical around the point x=0.5, y=0.5
 *
 * @authors Trond Lossius
 *
 * @copyright Copyright © 2011 by Trond Lossius @n
 * This code is licensed under the terms of the "New BSD License" @n
 * http://creativecommons.org/licenses/BSD/
 */


#ifndef __TTSMOOTHPOLYNOMIALFUNCTION_H__
#define __TTSMOOTHPOLYNOMIALFUNCTION_H__

#include "TTDSP.h"


/**	This implements a function which basically does nothing: 
	y = f(x)
 */
class TTSmoothPolynomialFunction : TTAudioObjectBase {
	TTCLASS_SETUP(TTSmoothPolynomialFunction)

protected:

	/** y = f(x) for a single value */
	inline TTErr calculateValue(const TTFloat64& x, TTFloat64& y, TTPtrSizedInt data);
	
	/**	A standard audio processing method as used by TTBlue objects.*/
	TTErr processAudio(TTAudioSignalArrayPtr inputs, TTAudioSignalArrayPtr outputs);

	/**	Unit Tests	*/
	virtual TTErr test(TTValue& returnedTestInfo);
};


#endif // __SMOOTHPOLYNOMIALFUNCTION_H__
