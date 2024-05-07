/** @file
 *
 * @ingroup dspFilterLib
 *
 * @brief #TTAllpass4a is a fourth-order allpass filter.
 *
 * @details Essentially the same as #TTAllpass2a, but with some signs flipped.
 *  Based on Fredric J. Harris (2004): Multirate Signal Processing for Communication Systems, Prentice Hall, Chapter 10, Figure 42.
 *
 *
 * @authors Timothy Place, Trond Lossius
 *
 * @copyright Copyright © 2010, Timothy Place @n
 * This code is licensed under the terms of the "New BSD License" @n
 * http://creativecommons.org/licenses/BSD/
 */


#ifndef __TT_ALLPASS4A_H__
#define __TT_ALLPASS4A_H__

#include "TTDSP.h"


/**	A second-order building-block allpass filter.
	Based on Multirate Signal Processing for Communication Systems, Chapter 10, Figure 42. 
 */
class TTAllpass4a : public TTAudioObjectBase {
	TTCLASS_SETUP(TTAllpass4a)

protected:

	TTFloat64			mD1;	///< first coefficient
	TTFloat64			mD2;	///< second coefficient
	TTFloat64			mD3;	///< third coefficient
	TTFloat64			mD4;	///< fourth coefficient

	TTSampleVector		mX1;	///< previous input sample (n-1) for each channel
	TTSampleVector		mX2;	///< previous input sample (n-2) for each channel
	TTSampleVector		mX3;	///< previous input sample (n-3) for each channel
	TTSampleVector		mX4;	///< previous input sample (n-4) for each channel
	TTSampleVector		mY1;	///< previous output sample (n-1) for each channel
	TTSampleVector		mY2;	///< previous output sample (n-2) for each channel
	TTSampleVector		mY3;	///< previous output sample (n-3) for each channel
	TTSampleVector		mY4;	///< previous output sample (n-4) for each channel
	
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


#endif // __TT_ALLPASS4A_H__
