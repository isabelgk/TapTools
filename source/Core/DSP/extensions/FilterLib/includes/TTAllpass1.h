/** @file
 *
 * @ingroup dspFilterLib
 *
 * @brief #TTAllpass1 is a first order allpass filter
 *
 * @details
 *
 * @authors Timothy Place, Trond Lossius
 *
 * @copyright Copyright © 2003, Timothy Place @n
 * This code is licensed under the terms of the "New BSD License" @n
 * http://creativecommons.org/licenses/BSD/
 */


#ifndef __TT_ALLPASS1_H__
#define __TT_ALLPASS1_H__

#include "TTDSP.h"
#include "TTDelayBuffer.h"


/**	An allpass filter. */
class TTAllpass1 : public TTAudioObjectBase {
	TTCLASS_SETUP(TTAllpass1)

protected:

	TTFloat64			mDelay;				///< Attribute: delay time in milliseconds
	TTFloat64			mDelayMax;			///< Maximum delay time in milliseconds (how long is the buffer?)
	TTUInt32			mDelayInSamples;	///< Delay time in samples (internal)		
	TTUInt32			mDelayMaxInSamples;	///< size of the delay buffers
	TTFloat64			mGain;				///< Attribute: gain in db
	TTFloat64			mLinearGain;		///< Attribute: linear gain coefficient

	TTDelayBufferVector	mFeedforward;		///< ff buffers for each channel
	TTDelayBufferVector	mFeedback;			///< fb buffers for each channel
	
	
	// Notifications
	TTErr updateMaxNumChannels(const TTValue& oldMaxNumChannels, TTValue&);
	TTErr updateSampleRate(const TTValue& oldSampleRate, TTValue&);

	/** Internal initialization of buffers and pointers etc. */
	TTErr init(TTUInt64 newDelayMaxInSamples);
	
	// internal - position the buffer pointers
	void reset();

	
	/** Attribute accessor. */
	TTErr setDelay(const TTValue& newValue);

	/** Attribute Accessor */
	TTErr setDelayInSamples(const TTValue& newValue);
	
	/** Attribute Accessor */
	TTErr setDelayMax(const TTValue& newValue);
	
	/** Attribute Accessor */
	TTErr setDelayMaxInSamples(const TTValue& newValue);
	
	/** Attribute accessor. */
	TTErr setGain(const TTValue& newValue);
	
	/** Attribute accessor. */
	TTErr setLinearGain(const TTValue& newValue);
	
	/**	This algorithm uses an IIR filter, meaning that it relies on feedback.  If the filter should
	 *	not be producing any signal (such as turning audio off and then back on in a host) or if the
	 *	feedback has become corrupted (such as might happen if a NaN is fed in) then it may be 
	 *	neccesary to clear the filter by calling this method.
	 *	@return Returns a TTErr error code.												*/
	TTErr clear();
	

	TTErr calculateValue(const TTFloat64& x, TTFloat64& y, TTDelayBufferPtr* data);
	TTErr processAudio(TTAudioSignalArrayPtr inputs, TTAudioSignalArrayPtr outputs);

	/**	Unit Tests	
	 @param	returnedTestInfo		Used to return test information
	 @return						#TTErr error code if the method fails to execute, else #kTTErrNone.
	 */
	virtual TTErr test(TTValue& returnedTestInfo);	
};


#endif // __TT_ALLPASS1_H__
