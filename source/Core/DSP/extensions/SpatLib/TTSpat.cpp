/** @file
 *
 * @ingroup dspSpatLib
 *
 * @brief Host a SpatLib object
 *
 * @details
 *
 * @authors Trond Lossius, Nils Peters, Timothy Place
 *
 * @copyright Copyright © 2011 by Trond Lossius, Nils Peters, and Timothy Place @n
 * This code is licensed under the terms of the "New BSD License" @n
 * http://creativecommons.org/licenses/BSD/
 */

#include "TTSpat.h"

#define thisTTClass			TTSpat
#define thisTTClassName		"spat"
#define thisTTClassTags		"audio, spatialization"



TT_AUDIO_CONSTRUCTOR,
mSpatFunctionObject(NULL),
mSourceCount(0),
mSinkCount(0)
{
	//TTValue v;
	
	addAttributeWithSetter(SpatFunction,	kTypeSymbol);
	addAttributeWithSetter(SourceCount,		kTypeUInt16);
	addAttributeWithSetter(SinkCount,		kTypeUInt16);

	addMessageWithArguments(getSourcePosition);
	addMessageWithArguments(setSourcePosition);
	addMessageWithArguments(getSinkPosition);
	addMessageWithArguments(setSinkPosition);
	
	addMessageWithArguments(getSpatFunctions);
	
	//addUpdate(MaxNumChannels);
	
	setAttributeValue("spatFunction", "spat.snap");
	setAttributeValue("sourceCount", 2);
	setAttributeValue("sinkCount", 8);
	setProcessMethod(process);
}


TTSpat::~TTSpat()
{
	delete mSpatFunctionObject;
}


TTErr TTSpat::setSpatFunction(const TTValue& aSpatFunction)
{
	TTErr				err;
	TTSymbol			spatFunctionName;
	TTAudioObjectBasePtr	spatFunction = NULL;
	
	spatFunctionName = aSpatFunction[0];
	
	// if the function didn't change, then don't change the function
	if (spatFunctionName == mSpatFunction)
		return kTTErrNone;	
	
	// TTObjectBaseInstantiate will automatically free the object passed into it
	err = TTObjectBaseInstantiate(spatFunctionName, &spatFunction, kTTValNONE);
	if (!err && spatFunction) {
		// Now set the state of the object to the state we have stored
		spatFunction->setAttributeValue(TT("sourceCount"), mSourceCount);
		spatFunction->setAttributeValue(TT("sinkCount"), mSinkCount);
		
		mSpatFunction = spatFunctionName;
		mSpatFunctionObject = spatFunction;
		
		// FIXME: This is not thread safe if the audio is running
		// We need to queue this switch to occur at a time when it is safe 
		// (when audio is not processed by the old object any longer)
		// Redmine #994
		//
		// ACTUALLY: it should be okay because of the locks in the TTObjectBaseInstantiate spinlocking to wait
		// for any process calls.
		// However, maybe those need some improvements like using volatile or atomic types
	}
	else {
		// some problems have occurred, not yet sure how we should handle this...
	}
	
	
	return err;
}



TTErr TTSpat::getSpatFunctions(const TTValue&, TTValue& listOfSpatFunctionsToReturn)
{
	TTValue v;
	
	v.resize(2);
	v[0] = TT("spatialization");
	v[1] = TT("processing"); // more efficent than append
	return TTObject::GetRegisteredClassNamesForTags(listOfSpatFunctionsToReturn, v);

}		   

/*
TTErr TTSpat::getFunctionParameters(const TTValue&, TTValue& aListOfParameterNamesToReturn)
{
	mSpatFunctionObject->getAttributeNames(aListOfParameterNamesToReturn);
	return kTTErrNone;
}


TTErr TTSpat::getFunctionParameter(const TTValue& aParameterNameIn, TTValue& aValueOut)
{
	TTSymbol parameterName;
	
	aParameterNameIn.get(0, parameterName);
	return mSpatFunctionObject->getAttributeValue(parameterName, aValueOut);
}


TTErr TTSpat::setFunctionParameter(const TTValue& aParameterNameAndValue, TTValue&)
{
	TTSymbol	parameterName;
	TTValue		parameterValue;
	
	aParameterNameAndValue.get(0, parameterName);
	parameterValue.copyFrom(aParameterNameAndValue, 1); //TODO: maybe there are more arguments ? 
	//aParameterNameAndValue.clear(); // only needed so that we don't return a value
	return mSpatFunctionObject->setAttributeValue(parameterName, parameterValue);
}
*/


TTErr TTSpat::setSourceCount(const TTValue& aSourceCount)
{
	mSourceCount = aSourceCount;
	return mSpatFunctionObject->setAttributeValue(TT("sourceCount"), (TTValue&)aSourceCount);
}


TTErr TTSpat::setSinkCount(const TTValue& aSinkCount)
{
	mSinkCount = aSinkCount;
	return mSpatFunctionObject->setAttributeValue(TT("sinkCount"), (TTValue&)aSinkCount);
}


TTErr TTSpat::getSourcePosition(const TTValue& anIndex, TTValue& returnedPosition)
{
//	return mSpatFunctionObject->getSourcePosition(aPosition);
	return mSpatFunctionObject->sendMessage("getSourcePosition", anIndex, returnedPosition);
}


TTErr TTSpat::setSourcePosition(const TTValue& aPosition, TTValue& unused)
{
//	return mSpatFunctionObject->setSourcePosition(aPosition);
	return mSpatFunctionObject->sendMessage("setSourcePosition", aPosition, unused);
}


TTErr TTSpat::getSinkPosition(const TTValue& anIndex, TTValue& returnedPosition)
{
//	return mSpatFunctionObject->getSinkPosition(aPosition);
	return mSpatFunctionObject->sendMessage("getSinkPosition", anIndex, returnedPosition);
}


TTErr TTSpat::setSinkPosition(const TTValue& aPosition, TTValue& unused)
{
//	return mSpatFunctionObject->setSinkPosition(aPosition);
	return mSpatFunctionObject->sendMessage("setSinkPosition", aPosition, unused);
}


#if 0
#pragma mark -
#pragma mark Process Routines
#endif


TTErr TTSpat::process(TTAudioSignalArrayPtr inputs, TTAudioSignalArrayPtr outputs)
{
	return mSpatFunctionObject->process(inputs, outputs);
}

