/** @file
 *
 * @ingroup dspFilterLib
 *
 * @brief #TTMirror5 is a 5th-order lowpass/highpass filter built up from a 2-path allpass structure
 *
 * @details
 *
 * @authors Timothy Place, Trond Lossius
 *
 * @copyright Copyright © 2010, Timothy Place @n
 * This code is licensed under the terms of the "New BSD License" @n
 * http://creativecommons.org/licenses/BSD/
 */


#ifndef __TT_MIRROR5_H__
#define __TT_MIRROR5_H__

#include "TTDSP.h"
#include "TTAllpass1a.h"
#include "TTAllpass2a.h"


/**	A 5th-order lowpass/highpass filter with arbitrary Fc built-up from allpass building blocks.
	Based on Multirate Signal Processing for Communication Systems, Chapter 10, Page 31.
 */
class TTMirror5 : TTAudioObjectBase {
	TTCLASS_SETUP(TTMirror5)

protected:

	TTAudioObject	mF0;		///< filter0 (in the upper path)
	TTAudioObject	mF1;		///< filter1 (in the lower path, first block)
	TTAudioObject	mF2;		///< filter2 (in the lower path, second block)
	TTFloat64		mFrequency;	///< attribute: in hertz
	TTSymbol		mMode;		///< attribute: lowpass or highpass
		
	// Notifications
	TTErr updateMaxNumChannels(const TTValue& oldMaxNumChannels, TTValue&);

	// Zero filter history
	TTErr clear();
	
	// attribute accessor
	TTErr setMode(const TTValue& newValue);
	TTErr setFrequency(const TTValue& newValue);

	inline void filterKernel(const TTFloat64& input, TTFloat64& outputPath0, TTFloat64& outputPath1, TTPtrSizedInt channel);

	// Do the processing
	TTErr processLowpass(TTAudioSignalArrayPtr inputs, TTAudioSignalArrayPtr outputs);
	TTErr processHighpass(TTAudioSignalArrayPtr inputs, TTAudioSignalArrayPtr outputs);
	
	/**	Unit Tests
	 @param	returnedTestInfo		Used to return test information
	 @return						#TTErr error code if the method fails to execute, else #kTTErrNone.
	 */
	virtual TTErr test(TTValue& returnedTestInfo);

public:
	TTErr calculateLowpass(const TTFloat64& x, TTFloat64& y, TTPtrSizedInt channel=0);
	TTErr calculateHighpass(const TTFloat64& x, TTFloat64& y, TTPtrSizedInt channel=0);
};


#endif // __TT_MIRROR5_H__
