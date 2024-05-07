/** @file
 *
 * @ingroup audioGraphUtilityLib
 *
 * @brief join≈: merge multichannel signals
 *
 * @details
 *
 * @authors Timothy Place
 *
 * @copyright Copyright © 2008 by Timothy Place @n
 * This code is licensed under the terms of the "New BSD License" @n
 * http://creativecommons.org/licenses/BSD/
 */


#ifndef __TT_JOIN_H__
#define __TT_JOIN_H__

#include "TTDSP.h"


/**	The join≈ object takes N input signals and combines them
 into a single signal with all of the channels present.
 */
class TTAudioGraphJoin : public TTAudioObjectBase {
	TTCLASS_SETUP(TTAudioGraphJoin)
	
protected:
	
	TTErr processAudio(TTAudioSignalArrayPtr inputs, TTAudioSignalArrayPtr outputs)
	{
		TTChannelCount	numSignals = inputs->numAudioSignals;
		TTChannelCount	numAccumulatedChannels = 0;
		TTAudioSignal&	out = outputs->getSignal(0);
		
		// 1. figure out our total number of channels
		for (TTChannelCount i=0; i<numSignals; i++) {
			TTAudioSignal&	in = inputs->getSignal(i);
			numAccumulatedChannels += in.getNumChannelsAsInt(); 
		}
		
		// 2. setup our output buffer for the correct number of channels
		// TODO: check if a resize is necessary
		out.setMaxNumChannels(numAccumulatedChannels);
		out.setNumChannels(numAccumulatedChannels);
				
		// 3. copy the data to the output buffer
		numAccumulatedChannels = 0;
		for (TTChannelCount i=0; i<numSignals; i++) {
			TTAudioSignal&	in = inputs->getSignal(i);
			TTAudioSignal::copyDirty(in, out, numAccumulatedChannels);
			numAccumulatedChannels += in.getNumChannelsAsInt();
		}
		return kTTErrNone;
	}
	
};




#endif // __TT_JOIN_H__
