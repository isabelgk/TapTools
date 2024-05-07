/** @file
 *
 * @ingroup dspFilterLib
 *
 * @brief #TTAllpass2b is a second-order allpass filter.
 *
 * @details A second-order Z^2 building-block allpass filter.
 *  This allpass filter uses a fixed delay of 2 samples (M=2). *
 *
 * @authors Timothy Place, Trond Lossius
 *
 * @copyright Copyright © 2010, Timothy Place @n
 * This code is licensed under the terms of the "New BSD License" @n
 * http://creativecommons.org/licenses/BSD/
 */


#include "TTAllpass2b.h"

#define thisTTClass			TTAllpass2b
#define thisTTClassName		"allpass.2b"
#define thisTTClassTags		"dspFilterLib, audio, processor, filter, allpass"

#ifdef TT_PLATFORM_WIN
#include <Algorithm>
#endif

TT_AUDIO_CONSTRUCTOR,
	mC1(0),
	mC2(0)
{
	TTChannelCount initialMaxNumChannels = arguments;

	addAttribute(C1, kTypeFloat64);
	addAttribute(C2, kTypeFloat64);
	
	addMessage(clear);
	addUpdates(MaxNumChannels);

	setAttributeValue(kTTSym_maxNumChannels,	initialMaxNumChannels);
	setProcessMethod(processAudio);
}


TTAllpass2b::~TTAllpass2b()
{
	;
}


TTErr TTAllpass2b::updateMaxNumChannels(const TTValue& oldMaxNumChannels, TTValue&)
{
	mX1.resize(mMaxNumChannels);
	mX2.resize(mMaxNumChannels);
	mX3.resize(mMaxNumChannels);
	mX4.resize(mMaxNumChannels);
	mY1.resize(mMaxNumChannels);
	mY2.resize(mMaxNumChannels);
	mY3.resize(mMaxNumChannels);
	mY4.resize(mMaxNumChannels);
	clear();
	return kTTErrNone;
}


TTErr TTAllpass2b::clear()
{
	mX1.assign(mMaxNumChannels, 0.0);
	mX2.assign(mMaxNumChannels, 0.0);
	mX3.assign(mMaxNumChannels, 0.0);
	mX4.assign(mMaxNumChannels, 0.0);
	mY1.assign(mMaxNumChannels, 0.0);
	mY2.assign(mMaxNumChannels, 0.0);
	mY3.assign(mMaxNumChannels, 0.0);
	mY4.assign(mMaxNumChannels, 0.0);
	return kTTErrNone;
}


TTErr TTAllpass2b::calculateValue(const TTFloat64& x, TTFloat64& y, TTPtrSizedInt channel)
{
	TTFloat64 w1 = mC1 * (mX2[channel] - mY2[channel]);
	TTFloat64 w2 = mC2 * (x - mY4[channel]);
	
	y = w1 + w2 + mX4[channel];

	TTZeroDenormal(y);
	
	mX4[channel] = mX3[channel];
	mY4[channel] = mY3[channel];
	mX3[channel] = mX2[channel];
	mY3[channel] = mY2[channel];
	mX2[channel] = mX1[channel];
	mY2[channel] = mY1[channel];
	mX1[channel] = x;
	mY1[channel] = y;
	return kTTErrNone;
}


TTErr TTAllpass2b::processAudio(TTAudioSignalArrayPtr inputs, TTAudioSignalArrayPtr outputs)
{
	TT_WRAP_CALCULATE_METHOD(calculateValue);
}

