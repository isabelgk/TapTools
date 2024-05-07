/** @file
 *
 * @ingroup dspFilterLib
 *
 * @brief #TTLowpassButterworth1 is a first-order Butterworth lowpass filter.
 *
 * @details Butterworth filters have maximum flat frequency response in the pass band.
 *  @n
 *	Filter equations from: @n
 *  @n
 *  Second-order IIR Filters will support cascade implementations @n
 *  By Rusty Allred, Texas Instruments, Dallas @n
 *  July 01, 2003 @n
 *  http://www.planetanalog.com/article/printableArticle.jhtml?articleID=12802683
 *
 * @authors Trond Lossius, Timothy Place,
 *
 * @copyright Copyright © 2008, Trond Lossius @n
 * This code is licensed under the terms of the "New BSD License" @n
 * http://creativecommons.org/licenses/BSD/
 */


#ifndef __TT_LOWPASS_BUTTERWORTH_1_H__
#define __TT_LOWPASS_BUTTERWORTH_1_H__

#include "TTDSP.h"


/**	1rst order Butterworth lowpass filter; Butterworth filters have maximum flat frequency response in the pass band.
 *
 * Filter equations from:
 * 
 * Second-order IIR Filters will support cascade implementations
 * By Rusty Allred, Texas Instruments, Dallas
 * July 01, 2003
 * http://www.planetanalog.com/article/printableArticle.jhtml?articleID=12802683
 *
 */
class TTLowpassButterworth1 : public TTAudioObjectBase {
	TTCLASS_SETUP(TTLowpassButterworth1)
	
protected:	
	
	TTFloat64			mFrequency;					///< filter cutoff frequency
	TTFloat64			mK, mRadians;				///< filter coefficients
	TTFloat64			mA0, mA1, mB1;				///< filter coefficients
	TTSampleVector		mX1;						///< Input sample n-1
	TTSampleVector		mY1;						///< Output sample n-1
	
	/**	Receives notifications when there are changes to the inherited 
	 mMaxNumChannels attribute.  This allocates memory for xm1 and ym1 
	 so that each channel's previous values are remembered.		*/
	TTErr updateMaxNumChannels(const TTValue& oldMaxNumChannels, TTValue&);
	TTErr updateSampleRate(const TTValue& oldSampleRate, TTValue&);
	
	/**	This algorithm uses an IIR filter, meaning that it relies on feedback.  If the filter should
	 *	not be producing any signal (such as turning audio off and then back on in a host) or if the
	 *	feedback has become corrupted (such as might happen if a NaN is fed in) then it may be 
	 *	neccesary to clear the filter by calling this method.
	 *	@return Returns a TTErr error code.												*/
	TTErr clear();
	
	void calculateCoefficients();
	
    /**	Standard single value calculate method as used by DSP objects. */
	inline TTErr calculateValue(const TTFloat64& x, TTFloat64& y, TTPtrSizedInt channel);
	
	/**	Standard audio processing method as used by TTBlue objects. */
	TTErr processAudio(TTAudioSignalArrayPtr inputs, TTAudioSignalArrayPtr outputs);
	
	/**	Setter for the frequency attribute. */
	TTErr setFrequency(const TTValue& value);
	
	
	/**	Unit Tests
	 @param	returnedTestInfo		Used to return test information
	 @return						#TTErr error code if the method fails to execute, else #kTTErrNone.
	 */
	virtual TTErr test(TTValue& returnedTestInfo);
};



#endif // __TT_LOWPASS_BUTTERWORTH_1_H__
