/** @file
 *
 * @ingroup dspFilterLib
 *
 * @brief #TTHighpassLinkwitzRiley4 is a fourth-order Linkwitz-Riley highpass filter.
 *
 * @details 4th order Linkwitz-Riley filters are typically used as crossover filters, with the following properties: @n
 *  @n
 *  1. Absolutely flat amplitude response throughout the passband with a 6 dB/octave rolloff rate after the crossover point. @n
 *  2. The acoustic sum of the two driver responses is unity at crossover. (Amplitude response of each is -3 dB at crossover, i.e., there is no peaking in the summed acoustic output.) @n
 *  3. Zero phase difference between drivers at crossover. (Lobing error equals zero, i.e., no tilt to the polar radiation pattern.) In addition, the phase difference of zero degrees through crossover places the lobe of the summed acoustic output on axis at all frequencies. @n
 *  4. The low pass and high pass outputs are everywhere in phase. (This guarantees symmetry of the polar response about the crossover point.) @n
 *  5. All drivers are always wired the same (in phase). @n
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


#ifndef __TT_HIGHPASS_LINKWITZ_RILEY_4_H__
#define __TT_HIGHPASS_LINKWITZ_RILEY_4_H__

#include "TTDSP.h"


/**	4th order Linkwitz Riley Highpass Filter
 *
 * Filter equations from:
 * 
 * Second-order IIR Filters will support cascade implementations
 * By Rusty Allred, Texas Instruments, Dallas
 * July 01, 2003
 * http://www.planetanalog.com/article/printableArticle.jhtml?articleID=12802683
 *
 * 4th order Linkwitz-Riley filters are typically used as crossover filters, with the following properties:
 *
 * 1. Absolutely flat amplitude response throughout the passband with a steep 24 dB/octave rolloff rate after the crossover point. 
 * 2. The acoustic sum of the two driver responses is unity at crossover. (Amplitude response of each is -6 dB at crossover, i.e., there is no peaking in the summed acoustic output.)
 * 3. Zero phase difference between drivers at crossover. (Lobing error equals zero, i.e., no tilt to the polar radiation pattern.) In addition, the phase difference of zero degrees through crossover places the lobe of the summed acoustic output on axis at all frequencies.
 * 4. The low pass and high pass outputs are everywhere in phase. (This guarantees symmetry of the polar response about the crossover point.)
 * 5. All drivers are always wired the same (in phase).
 *
 */
class TTHighpassLinkwitzRiley4 : public TTAudioObjectBase {
	TTCLASS_SETUP(TTHighpassLinkwitzRiley4)

protected:

	TTFloat64			mFrequency;				///< filter cutoff frequency
	TTFloat64			mK, mRadians;						///< filter coefficients
	TTFloat64			mA0, mA1, mA2, mA3, mA4;			///< filter coefficients for input samples
	TTFloat64			mB1, mB2, mB3, mB4;				///< filter coefficients for output samples
	TTSampleVector		mX1;						///< Input sample n-1
	TTSampleVector		mX2;						///< Input sample n-2
	TTSampleVector		mX3;						///< Input sample n-3
	TTSampleVector		mX4;						///< Input sample n-4
	TTSampleVector		mY1;						///< Output sample n-1
	TTSampleVector		mY2;						///< Output sample n-2
	TTSampleVector		mY3;						///< Output sample n-3
	TTSampleVector		mY4;						///< Output sample n-4

	/**	Receives notifications when there are changes to the inherited 
		mMaxNumChannels attribute.  This allocates memory for xm1, xm2, ym1, and ym2 
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


#endif // __TT_HIGHPASS_LINKWITZ_RILEY_4_H__
