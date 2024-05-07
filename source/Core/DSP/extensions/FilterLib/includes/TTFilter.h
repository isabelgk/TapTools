/** @file
 *
 * @ingroup dspFilterLib
 *
 * @brief #TTFilter is a generalized filter wrapper object for Jamoma
 *
 * @details
 *
 * @authors Timothy Place, Trond Lossius
 *
 * @copyright Copyright © 2010, Timothy Place @n
 * This code is licensed under the terms of the "New BSD License" @n
 * http://creativecommons.org/licenses/BSD/
 */


#ifndef __TT_FILTER_H__
#define __TT_FILTER_H__

#include "TTDSP.h"


/**	A Generalized filter wrapper object for Jamoma. */
class TTFilter : TTAudioObjectBase {
	TTCLASS_SETUP(TTFilter)
	
protected:

	TTAudioObjectBasePtr	mActualFilterObject;	///< The actual filter object that this object is currently wrapping
	TTFloat64			mFrequency;				///< The center or cutoff frequency of the filter
	TTFloat64			mQ;						///< The width of the filter
	TTSymbol			mType;					///< The name of the current filter type
	
public:
	
	TTErr setFrequency(const TTValue& newValue)
	{	
		mFrequency = newValue;
		return mActualFilterObject->setAttributeValue(TT("frequency"), mFrequency);
	}
	
	TTErr setQ(const TTValue& newValue)
	{	
		mQ = newValue;
		return mActualFilterObject->setAttributeValue(TT("q"), mQ);
	}
	
	TTErr setType(const TTValue& newValue)
	{	
		TTSymbol	newType = newValue;
		TTErr		err = kTTErrNone;
		
		// if the type didn't change, then don't change the filter
		if (newType == mType)
			return kTTErrNone;
		
		mType = newType;
		err = TTObjectBaseInstantiate(mType, &mActualFilterObject, mMaxNumChannels);			
		if (!err) {
			// Now that we have our new filter, update it with the current state of the wrapper:
			mActualFilterObject->setAttributeValue(TT("frequency"), mFrequency);
			err = mActualFilterObject->setAttributeValue(TT("q"), mQ);
			if (err == kTTErrInvalidAttribute)
				err = mActualFilterObject->setAttributeValue(TT("resonance"), mQ);
			mActualFilterObject->setAttributeValue(TT("bypass"), this->attrBypass);
			mActualFilterObject->setAttributeValue(kTTSym_sampleRate, sr);
		}
		return err;
	}
	
	
	TTErr getTypes(const TTValue&, TTValue& listOfFilterTypesToReturn)
	{
		return TTObject::GetRegisteredClassNamesForTags(listOfFilterTypesToReturn, TT("filter"));
	}
	
	
	TTErr clear()
	{
		return mActualFilterObject->sendMessage(TT("clear"));
	}
	
	
	TTErr mode(const TTValue& newMode, TTValue&)
	{
		if (mActualFilterObject)
			return mActualFilterObject->setAttributeValue(TT("mode"), const_cast<TTValue&>(newMode));
		else
			return kTTErrNone;
	}
	
	
	TTErr updateMaxNumChannels(const TTValue& oldMaxNumChannels, TTValue&)
	{
		if (mActualFilterObject)
			return mActualFilterObject->setAttributeValue(kTTSym_maxNumChannels, mMaxNumChannels);
		else
			return kTTErrNone;
	}
	
	
	TTErr updateSampleRate(const TTValue& oldSampleRate, TTValue&)
	{
		return mActualFilterObject->setAttributeValue(kTTSym_sampleRate, sr);
	}
	
	
	TTErr processAudio(TTAudioSignalArrayPtr inputs, TTAudioSignalArrayPtr outputs)
	{
		return mActualFilterObject->process(inputs, outputs);
	}
	
	
	/**	Unit Tests
	 @param	returnedTestInfo		Used to return test information
	 @return						#TTErr error code if the method fails to execute, else #kTTErrNone.
	 */
	virtual TTErr test(TTValue& returnedTestInfo);
};


#endif // __TT_FILTER_H__
