/** @file
 *
 * @ingroup dspFunctionLib
 *
 * @brief #TTPowerFunction Unit for Jamoms DSP
 *
 * @details
 *
 * @authors Trond Lossius
 *
 * @copyright Copyright © 2007 by Trond Lossius @n
 * This code is licensed under the terms of the "New BSD License" @n
 * http://creativecommons.org/licenses/BSD/
 */


#ifndef __TTPOWERFUNCTION_H__
#define __TTPOWERFUNCTION_H__

#include "TTDSP.h"


/**	A function unit that provides exponential curves,
	along with a number of options to control the behavior of the curve.
 */
class TTPowerFunction : TTAudioObjectBase {
	TTCLASS_SETUP(TTPowerFunction)

protected:

	TTSymbol	mSymmetry;		///< Mode: none, point or axis
	double		mPowerValue;	///< Parameter for function: y pow(x,pow(2,powerValue))	
	double		mK;				///< k = pow(2,powerValue)

	/** Attribute setter. */
	TTErr setSymmetry(const TTValue& newValue);
	
	/** Attribute setter. */
	TTErr setPowerValue(const TTValue& newValue);
	
	/** y = f(x) for a single value */
	inline TTErr calculatePoint(const TTFloat64& x, TTFloat64& y, TTPtrSizedInt data);
	inline TTErr calculateAxis(const TTFloat64& x, TTFloat64& y, TTPtrSizedInt data);
	inline TTErr calculateNoSymmetry(const TTFloat64& x, TTFloat64& y, TTPtrSizedInt data);
	
	/**	A standard audio processing method as used by TTBlue objects.*/
	TTErr processAudio(TTAudioSignalArrayPtr inputs, TTAudioSignalArrayPtr outputs);
	
	/**	Unit Tests	*/
	virtual TTErr test(TTValue& returnedTestInfo);	

};


#endif // __POWERFUNCTION_H__
