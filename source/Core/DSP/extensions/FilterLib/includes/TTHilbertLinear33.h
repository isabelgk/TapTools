/** @file
 *
 * @ingroup dspFilterLib
 *
 * @brief #TTHilbertLinear33 is a 33rd-order Hilbert Transform filter built up from a 2-path allpass structure with linear phase relationship to the input signal.
 *
 * @details See notes on TTHilbert9 for information about the structure of this filter. @n
 *  @n
 *  See notes on TTHalfband33 for information regarding coefficient calculations, remembering that we need to flip signs to perform the Hilbert Transform. Note however, that we flip signs for the Z^(-2) terms because i^2 = -1. The caveat is that in this filter we also have Z^(-4) terms in the second-order allpass filters, and i^4 = 1, meaning we don't flip those signs. This may seem obvious after reading this, however it is easy to miss, especially as the general rule is stated as "flip all of the signs".
 *
 * @authors Timothy Place, Trond Lossius
 *
 * @copyright Copyright © 2010, Timothy Place @n
 * This code is licensed under the terms of the "New BSD License" @n
 * http://creativecommons.org/licenses/BSD/
 */


#ifndef __TT_HILBERTLINEAR33_H__
#define __TT_HILBERTLINEAR33_H__

#include "TTDSP.h"
#include "TTAllpass1a.h"
#include "TTAllpass1b.h"
#include "TTAllpass2b.h"
#include "TTDelay.h"


/**	A 33rd-order Hilber Transform filter built-up from allpass building blocks.
 
	See notes on TTHilbert9 for information about the structure of this filter.

	See notes on TTHalfband33 for information regarding coefficient calculations,
	remembering that we need to flip signs to perform the Hilbert Transform.
	Note however, that we flip signs for the Z^(-2) terms because i^2 = -1.
	The caveat is that in this filter we also have Z^(-4) terms in the second-order
	allpass filters, and i^4 = 1, meaning we don't flip those signs.
	This may seem obvious after reading this, however it is easy to miss, especially
	as the general rule is stated as "flip all of the signs".
*/
class TTHilbertLinear33 : TTAudioObjectBase {
	TTCLASS_SETUP(TTHilbertLinear33)

protected:

	TTAudioObject	mP0Delay;	///< path0, pure delay, 16 samples
	
	TTAudioObject	mP1Delay;	///< path1
	TTAudioObject	mF0;		///< path1
	TTAudioObject	mF1;		///< path1
	TTAudioObject	mF2;		///< path1
	TTAudioObject	mF3;		///< path1
	TTAudioObject	mF4;		///< path1
	
	// Notifications
	TTErr updateMaxNumChannels(const TTValue& oldMaxNumChannels, TTValue&);

	// Zero filter history
	TTErr clear();
	
	inline void filterKernel(const TTFloat64& input, TTFloat64& outputPath0, TTFloat64& outputPath1, TTPtrSizedInt channel);

	TTErr processAudio(TTAudioSignalArrayPtr inputs, TTAudioSignalArrayPtr outputs);
	
	
	/**	Unit Tests
	 @param	returnedTestInfo		Used to return test information
	 @return						#TTErr error code if the method fails to execute, else #kTTErrNone.
	 */
	virtual TTErr test(TTValue& returnedTestInfo);
};


#endif // __TT_HILBERTLINEAR33_H__
