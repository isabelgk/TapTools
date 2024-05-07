/** @file
 *
 * @ingroup dspFilterLib
 *
 * @brief #TTHighpassButterworth3 is a third-order Butterworth highpass filter.
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


#include "TTHighpassButterworth3.h"

#define thisTTClass			TTHighpassButterworth3
#define thisTTClassName		"highpass.butterworth.3"
#define thisTTClassTags		"dspFilterLib, audio, processor, filter, highpass, butterworth"


TT_AUDIO_CONSTRUCTOR
{
	// register attributes
	addAttributeWithSetter(Frequency,		kTypeFloat64);
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


TTHighpassButterworth3::~TTHighpassButterworth3()
{
	;
}


TTErr TTHighpassButterworth3::updateMaxNumChannels(const TTValue& oldMaxNumChannels, TTValue&)
{
	mX1.resize(mMaxNumChannels);
	mX2.resize(mMaxNumChannels);
	mX3.resize(mMaxNumChannels);
	mY1.resize(mMaxNumChannels);
	mY2.resize(mMaxNumChannels);
	mY3.resize(mMaxNumChannels);
	clear();
	return kTTErrNone;
}


TTErr TTHighpassButterworth3::updateSampleRate(const TTValue& oldSampleRate, TTValue&)
{
	TTValue	v(mFrequency);
	return setFrequency(v);
}


TTErr TTHighpassButterworth3::clear()
{
	mX1.assign(mMaxNumChannels, 0.0);
	mX2.assign(mMaxNumChannels, 0.0);
	mX3.assign(mMaxNumChannels, 0.0);
	mY1.assign(mMaxNumChannels, 0.0);
	mY2.assign(mMaxNumChannels, 0.0);
	mY3.assign(mMaxNumChannels, 0.0);
	return kTTErrNone;
}


TTErr TTHighpassButterworth3::setFrequency(const TTValue& newValue)
{	
	mFrequency = newValue;

	mRadians = kTTTwoPi*mFrequency;
	mRadiansSquared = mRadians * mRadians;
	mRadiansCubic = mRadiansSquared * mRadians;
	
	mK =  mRadians/tan(kTTPi*mFrequency/sr); // kTTTwoPi*frequency/tan(kTTPi*frequency/sr);
	mKSquared = mK * mK;
    mKCubic = mKSquared * mK;
	calculateCoefficients();	
	return kTTErrNone;
}
	
	
void TTHighpassButterworth3::calculateCoefficients() //TODO: with a little bit of thinking, this can be optimized
{
    TTFloat64 temp1;
	temp1 = (mRadiansCubic + mKCubic + 2*mRadiansSquared*mK + 2*mRadians*mKSquared);
	
	mA0 = mKCubic / temp1; 
	mA1 = (-3*mKCubic) / temp1; 
	//mA2 = -mA1; //mA2 = (3*mKCubic)  / temp1; 
	//mA3 = -mA0; //mA3 =  (-mKCubic)  / temp1; 

	mB1 = (3*mRadiansCubic - 3*mKCubic + 2*mRadiansSquared*mK - 2*mRadians*mKSquared) / (temp1); 
	mB2 = (3*mRadiansCubic + 3*mKCubic - 2*mRadiansSquared*mK - 2*mRadians*mKSquared) / (temp1); 
	mB3 = (  mRadiansCubic -   mKCubic - 2*mRadiansSquared*mK + 2*mRadians*mKSquared) / (temp1);
	
	
}

inline TTErr TTHighpassButterworth3::calculateValue(const TTFloat64& x, TTFloat64& y, TTPtrSizedInt channel)
{
	//y = TTAntiDenormal(mA0*x + mA1*mX1[channel] + mA2*mX2[channel] + mA3*mX3[channel] - mB1*mY1[channel] - mB2*mY2[channel] -mB3*mY3[channel]);
	// since mA2 = -mA1, an mA3 = -mA0, we can simplyfy to
	y = mA0*(x- mX3[channel]) + mA1*(mX1[channel] - mX2[channel]) - mB1*mY1[channel] - mB2*mY2[channel] -mB3*mY3[channel];
	TTZeroDenormal(y);
	mX3[channel] = mX2[channel];
	mX2[channel] = mX1[channel];
	mX1[channel] = x;
	mY3[channel] = mY2[channel];
	mY2[channel] = mY1[channel];
	mY1[channel] = y;
	return kTTErrNone;
}


TTErr TTHighpassButterworth3::processAudio(TTAudioSignalArrayPtr inputs, TTAudioSignalArrayPtr outputs)
{
	TT_WRAP_CALCULATE_METHOD(calculateValue);
}
