/** @file
 *
 * @ingroup graphDictionaryLib
 *
 * @brief Object that appends keys/values to a dictionary
 *
 * @details
 *
 * @authors Timothy Place
 *
 * @copyright Copyright © 2010 by Timothy Place @n
 * This code is licensed under the terms of the "New BSD License" @n
 * http://creativecommons.org/licenses/BSD/
 */

#include "TTDictionaryAppend.h"

#define thisTTClass			TTDictionaryAppend
#define thisTTClassName		"dictionary.append"
#define thisTTClassTags		"dictionary, graph"


TT_OBJECT_CONSTRUCTOR,
	mKey(kTTSymEmpty),
	mValue(NULL)
{
	mValue = new TTValue(0);
	
	addAttributeWithGetterAndSetter(Key,	kTypeSymbol);
	addAttributeWithGetterAndSetter(Value,	kTypeSymbol); // TODO: make this a generic TTValue
	addMessageWithArguments(dictionary);
}


// Destructor
TTDictionaryAppend::~TTDictionaryAppend()
{
	;
}


TTErr TTDictionaryAppend::dictionary(const TTValue& input, TTValue& output)
{
	TTDictionary d;
    
	if (input[0].type() == kTypeDictionary) {
        d = input[0];
        d.remove(mKey);
		d.append(mKey, *mValue);
		output[0] = d;
		return kTTErrNone;
	}
	return kTTErrInvalidValue;
}


TTErr TTDictionaryAppend::getKey(TTValue& newValue)
{
	newValue = mKey;
	return kTTErrNone;
}


TTErr TTDictionaryAppend::setKey(const TTValue& newValue)
{
	mKey = newValue;
	return kTTErrNone;
}


TTErr TTDictionaryAppend::getValue(TTValue& newValue)
{
	newValue = (*mValue);
	return kTTErrNone;
}


TTErr TTDictionaryAppend::setValue(const TTValue& newValue)
{
	(*mValue) = newValue;
	return kTTErrNone;
}
