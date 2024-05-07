/** @file
 *
 * @ingroup dspEffectsLib
 *
 * @brief #TTThru pass an input signal to the output unchanged.
 *
 * @authors Tim Place, Trond Lossius
 *
 * @copyright Copyright © 2011, Tim Place @n
 * This code is licensed under the terms of the "New BSD License" @n
 * http://creativecommons.org/licenses/BSD/
 */


#include "TTThru.h"

#define thisTTClass			TTThru
#define thisTTClassName		"thru"
#define thisTTClassTags		"dspEffectsLib, audio, processor"


TT_AUDIO_CONSTRUCTOR
{
	setProcessMethod(processAudio);
}


TTThru::~TTThru()
{
	;
}


TTErr TTThru::processAudio(TTAudioSignalArrayPtr inputs, TTAudioSignalArrayPtr outputs)
{
	TTAudioSignal&	in = inputs->getSignal(0);
	TTAudioSignal&	out = outputs->getSignal(0);
	
	// We can't reliably simply reference in the case of jcom.unpack≈ because of crashes
	// not sure what is going wrong with that yet...
	
	//return TTAudioSignal::reference(in, out);
	return TTAudioSignal::copy(in, out);
}
