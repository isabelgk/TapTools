/** @file
 *
 * @ingroup dspFunctionLib
 *
 * @brief #TTFunction is a generalized function wrapper for Jamoma DSP
 *
 * @details All functions will map incoming values in the normalised range [0,1] to output values in the normalised range [0,1].
 *
 * @authors Tim Place, Trond Lossius
 *
 * @copyright Copyright © 2010 by Tim Place @n
 * This code is licensed under the terms of the "New BSD License" @n
 * http://creativecommons.org/licenses/BSD/
 */


#ifndef __FUNCTION_H__
#define __FUNCTION_H__

#include "TTDSP.h"


/**	Generalized Function Wrapper
 */
class TTFunction : TTAudioObjectBase {
	TTCLASS_SETUP(TTFunction)

protected:

	TTSymbol			mFunction;			///< Name of the window function to use
	TTAudioObjectBasePtr	mFunctionObject;	///< The actual window function object for mFunction
	TTUInt32			mNumPoints;			///< Number of Points in the lookup table
	TTSampleVector		mLookupTable;		///< Cached table of size mNumPoints
	TTSymbol			mMode;				///< Options: 'generate', 'lookup', 'apply'
	TTUInt32			mPadding;			///< for generating the lookup table - (e.g. padded welch window padded by 16 zeros on both sides)
											// further padding references: http://www.dsprelated.com/dspbooks/sasp/Sliding_FFT_Maximum_Overlap.html

	/**	choose the window function */
	TTErr setFunction(const TTValue& function);
	
	/**	set lookup table size */
	TTErr setNumPoints(const TTValue& numPoints);
	TTErr doSetNumPoints(const TTUInt32 numPoints);

	/**	set process method */
	TTErr setMode(const TTValue& mode);
	
	/** zero-padding applied to lookup table */
	TTErr setPadding(const TTValue& padding);
	
	// internal method for filling the lookup table
	TTErr fill();
	
	/**	return a list of all the available window shapes	*/
	TTErr getFunctions(const TTValue&, TTValue& listOfWindowTypesToReturn);
	
	/** y = f(x) for a single value */
	inline TTErr calculateValue(const TTFloat64& x, TTFloat64& y, TTPtrSizedInt data);
	inline TTErr lookupValue(const TTFloat64& x, TTFloat64& y, TTPtrSizedInt data);
	
	/**	A standard audio processing method as used by Jamoma DSP objects.*/
	TTErr processGenerate(TTAudioSignalArrayPtr inputs, TTAudioSignalArrayPtr outputs);
	TTErr processLookup(TTAudioSignalArrayPtr inputs, TTAudioSignalArrayPtr outputs);
	TTErr processApply(TTAudioSignalArrayPtr inputs, TTAudioSignalArrayPtr outputs);

};


#endif // __FUNCTION_H__
