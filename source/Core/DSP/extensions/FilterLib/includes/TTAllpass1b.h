/** @file
 *
 * @ingroup dspFilterLib
 *
 * @brief #TTAllpass1b is a first-order building-block allpass filter.
 *
 * @details This allpass filter uses a fixed delay of 2 samples (M=2).
 *
 * @authors Timothy Place, Trond Lossius
 *
 * @copyright Copyright © 2010, Timothy Place @n
 * This code is licensed under the terms of the "New BSD License" @n
 * http://creativecommons.org/licenses/BSD/
 */


#ifndef __TT_ALLPASS1B_H__
#define __TT_ALLPASS1B_H__

#include "TTDSP.h"


/**	A first-order building-block allpass filter.
	This allpass filter uses a fixed delay of 2 samples (M=2). */
class TTAllpass1b : public TTAudioObjectBase {
	TTCLASS_SETUP(TTAllpass1b)

protected:

	TTFloat64			mAlpha;				///< single coefficient for the first-order allpass
	TTSampleVector		mX1;				///< previous input sample (n-1) for each channel
	TTSampleVector		mX2;				///< previous input sample (n-2) for each channel
	TTSampleVector		mY1;				///< previous output sample (n-1) for each channel
	TTSampleVector		mY2;				///< previous output sample (n-2) for each channel
	
	// Notifications
	TTErr updateMaxNumChannels(const TTValue& oldMaxNumChannels, TTValue&);

	// Zero filter history
	TTErr clear();
	
	// Do the processing
	TTErr processAudio(TTAudioSignalArrayPtr inputs, TTAudioSignalArrayPtr outputs);
	
	/**	Unit Tests
	 @param	returnedTestInfo		Used to return test information
	 @return						#TTErr error code if the method fails to execute, else #kTTErrNone.
	 */
	virtual TTErr test(TTValue& returnedTestInfo);

public:
	TTErr calculateValue(const TTFloat64& x, TTFloat64& y, TTPtrSizedInt channel=0);
};


#endif // __TT_ALLPASS1B_H__
