/** @file
 *
 * @ingroup audioGraphUtilityLib
 *
 * @brief sig≈: generate multichannel signal based on one input value
 *
 * @details
 *
 * @authors Timothy Place
 *
 * @copyright Copyright © 2010 by Timothy Place @n
 * This code is licensed under the terms of the "New BSD License" @n
 * http://creativecommons.org/licenses/BSD/
 */


#ifndef __TT_SIG_H__
#define __TT_SIG_H__

#include "TTDSP.h"


/**	The sig≈ object takes a single input message and converts it into a JAG signal */
class TTAudioGraphSig : public TTAudioObjectBase {
	TTCLASS_SETUP(TTAudioGraphSig)

protected:

	TTFloat64	mValue;
	
	/** A value passed in as a dictionary will set the value attribute
		If an array is passed in for the value in the dictionary, the first item in the array is used. */
	TTErr dictionary(TTValue& input, TTValue&);
	
	TTErr processAudio(TTAudioSignalArrayPtr inputs, TTAudioSignalArrayPtr outputs)
	{
		TTAudioSignal&		out			= outputs->getSignal(0);
		TTSampleValuePtr	outSample;
		TTChannelCount		numChannels = out.getNumChannelsAsInt();
		TTUInt16			vs			= out.getVectorSizeAsInt();
		TTUInt16			n;
		
		for (TTChannelCount channel=0; channel < numChannels; channel++) {
			outSample = out.mSampleVectors[channel];
			n = vs;
			while (n--)
				*outSample++ = mValue;
		}
		return kTTErrNone;
	}
	
};



#endif // __TT_SIG_H__
