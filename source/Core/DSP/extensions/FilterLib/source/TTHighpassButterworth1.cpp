/** @file
 *
 * @ingroup dspFilterLib
 *
 * @brief #TTHighpassButterworth1 is a second-order Butterworth highpass filter.
 *
 * @details Butterworth filters have maximum flat frequency response in the pass band.
 * Filter equations from: @n
 * @n
 * Second-order IIR Filters will support cascade implementations @n
 * By Rusty Allred, Texas Instruments, Dallas @n
 * July 01, 2003
 * http://www.planetanalog.com/article/printableArticle.jhtml?articleID=12802683
 .
 *
 * @authors Trond Lossius, Timothy Place,
 *
 * @copyright Copyright © 2008, Trond Lossius @n
 * This code is licensed under the terms of the "New BSD License" @n
 * http://creativecommons.org/licenses/BSD/
 */


#include "TTHighpassButterworth1.h"

#define thisTTClass			TTHighpassButterworth1
#define thisTTClassName		"highpass.butterworth.1"
#define thisTTClassTags		"dspFilterLib, audio, processor, filter, highpass, butterworth"


TT_AUDIO_CONSTRUCTOR{
	// register attributes
	addAttributeWithSetter(Frequency,	kTypeFloat64);
	addAttributeProperty(Frequency,			range,			TTValue(10.0, sr*0.475));
	addAttributeProperty(Frequency,			rangeChecking,	TT("clip"));

	// register for notifications from the parent class so we can allocate memory as required
	addUpdates(MaxNumChannels);
	// register for notifications from the parent class so we can recalculate coefficients as required
	addUpdates(SampleRate);
	// make the clear method available to the outside world
	addMessage(clear);

	// Set Defaults...
	setAttributeValue(kTTSym_maxNumChannels,	arguments);			// This attribute is inherited
	setAttributeValue(TT("frequency"),		1000.0);
	setProcessMethod(processAudio);
	setCalculateMethod(calculateValue);
}


TTHighpassButterworth1::~TTHighpassButterworth1()
{
	;
}


TTErr TTHighpassButterworth1::updateMaxNumChannels(const TTValue& oldMaxNumChannels, TTValue&)
{
	mX1.resize(mMaxNumChannels);
	mY1.resize(mMaxNumChannels);	
	clear();
	return kTTErrNone;
}


TTErr TTHighpassButterworth1::updateSampleRate(const TTValue& oldSampleRate, TTValue&)
{
	TTValue	v(mFrequency);
	return setFrequency(v);
}


TTErr TTHighpassButterworth1::clear()
{
	mX1.assign(mMaxNumChannels, 0.0);
	mY1.assign(mMaxNumChannels, 0.0);
	return kTTErrNone;
}


TTErr TTHighpassButterworth1::setFrequency(const TTValue& newValue)
{	
	mFrequency = newValue;
	mRadians = kTTTwoPi*mFrequency;
	mK = mRadians/tan(kTTPi*mFrequency/sr);
	calculateCoefficients();
	return kTTErrNone;
}

	
	
void TTHighpassButterworth1::calculateCoefficients()
{
	mA0 = mK/(mK+mRadians); 
	mA1 = -mA0;//-k/(k+wc); 
	mB1 = (mRadians-mK)/(mK+mRadians);
}	


inline TTErr TTHighpassButterworth1::calculateValue(const TTFloat64& x, TTFloat64& y, TTPtrSizedInt channel)
{
	//y = TTAntiDenormal(mA0*x + mA1*mX1[channel] - mB1*mY1[channel]);
    //since mA1 = -mA0, we can simplyfiy to
	y = mA0*(x - mX1[channel]) - mB1*mY1[channel];
	TTZeroDenormal(y);
	mX1[channel] = x;
	mY1[channel] = y;
	return kTTErrNone;
}


TTErr TTHighpassButterworth1::processAudio(TTAudioSignalArrayPtr inputs, TTAudioSignalArrayPtr outputs)
{
	TT_WRAP_CALCULATE_METHOD(calculateValue);
}


