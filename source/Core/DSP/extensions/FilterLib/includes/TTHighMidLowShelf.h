/** @file
 *
 * @ingroup dspFilterLib
 *
 * @brief #TTHighMidLowShelf is a High-Mid-Low shelf filter 
 *
 * @details ported by Nils Peters 2009, from the PD external hml_shelf~ by Thomas Musil. WARNING: Possible license violation! Please refrain from using this until this has been sorted out, as we might have to remove this filter unit.
 *
 * @authors Thomas Musil, Nils Peters, Trond Lossius
 *
 * @copyright NOTE: This might be in violation of the LGPL license of the iemlib as noted in this issue: https://github.com/jamoma/JamomaCore/issues/204. Most likely we have to remove this from the library.
 * Copyright © 2010, Thomas Musil @n
 * This code is licensed under the terms of the "New BSD License" @n
 * http://creativecommons.org/licenses/BSD/
 */


#ifndef _TT_HIMIDLOW_H_
#define _TT_HIMIDLOW_H_

#include "TTDSP.h"


/**	High-Mid-Low shelf filter.	*/
class TTHighMidLowShelf : public TTAudioObjectBase {
	TTCLASS_SETUP(TTHighMidLowShelf)

protected:

	TTFloat64		mFrequencyLm, mFrequencyMh, mGainL, mGainM, mGainH;///< filter parameter
	TTFloat64		mA0, mA1, mA2, mB1, mB2;		///< filter coefficients
	TTSampleVector		mX1;
	TTSampleVector		mX2;
	TTSampleVector		mX0;
	TTFloat64		mFmid;
	

	/**	Receives notifications when there are changes to the inherited 
		mMaxNumChannels attribute.  This allocates memory for xm1, xm2, ym1, and ym2 
		so that each channel's previous values are remembered.		*/
	TTErr updateMaxNumChannels(const TTValue& oldMaxNumChannels, TTValue&);

	/** Receives notifications when there are changes to the inherited 
		sr attribute.						*/
	TTErr updateSampleRate(const TTValue& oldSampleRate, TTValue&);
	TTErr clear();
	TTErr calculateCoefficients();

	/**	Standard audio processing method as used by TTBlue objects. */
	TTErr processAudio(TTAudioSignalArrayPtr inputs, TTAudioSignalArrayPtr outputs);

    /** y = f(x) */
    TTErr calculateValue(const TTFloat64& x, TTFloat64& y, TTPtrSizedInt channel);
	
	/**	Setter for the filter attribute. */
	TTErr setGainL(const TTValue& value);
	TTErr setGainM(const TTValue& value);
	TTErr setGainH(const TTValue& value);
	TTErr setFrequencyLm(const TTValue& value);
	TTErr setFrequencyMh(const TTValue& value);
	
	
	/**	Unit Tests
	 @param	returnedTestInfo		Used to return test information
	 @return						#TTErr error code if the method fails to execute, else #kTTErrNone.
	 */
	virtual TTErr test(TTValue& returnedTestInfo);
	
};

#endif //  _HIMIDLOW_H_
