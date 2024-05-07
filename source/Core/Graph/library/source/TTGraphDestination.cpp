/** @file
 *
 * @ingroup graphLibrary
 *
 * @brief Jamoma Asynchronous Object Graph Layer
 *
 * @details Creates a wrapper for TTObjectBases that can be used to build a control graph for asynchronous message passing
 *
 * @authors Timothy Place
 *
 * @copyright Copyright © 2010 by Timothy Place @n
 * This code is licensed under the terms of the "New BSD License" @n
 * http://creativecommons.org/licenses/BSD/
 */


#include "TTGraphOutlet.h"
#include "TTCallback.h"
#include "TTGraphDestination.h"
#include "TTGraphObjectBase.h"


// C Callback from any Graph Destination objects we are observing
void TTGraphDestinationObserverCallback(TTGraphDestinationPtr self, TTValue& arg)
{
	// at the moment we only receive one callback, which is for the object being deleted
	self->mDestinationObject = NULL;
	self->mInletNumber = 0;
	self->mOwner->drop(*self);
}


// Implementation for Graph Destination class

TTGraphDestination::TTGraphDestination() :
	mDestinationObject(NULL),
	mInletNumber(0),
	mCallbackHandler("callback"),
	mOwner(NULL)
{
	create();
}


TTGraphDestination::~TTGraphDestination()
{
	if (mDestinationObject)
		mDestinationObject->unregisterObserverForNotifications(mCallbackHandler);
	
	mDestinationObject = NULL;
	mInletNumber = 0;
}


void TTGraphDestination::create()
{
	mCallbackHandler.set(TT("owner"), TT("TTGraphDestination"));
	mCallbackHandler.set(TT("function"), TTPtr(&TTGraphDestinationObserverCallback));
	mCallbackHandler.set(TT("baton"), TTPtr(this));
}


void TTGraphDestination::connect(TTGraphObjectBasePtr anObject, TTUInt16 fromOutletNumber)
{
	mDestinationObject = anObject;
	mInletNumber = fromOutletNumber;

	// dynamically add a message to the callback object so that it can handle the 'objectFreeing' notification
	mCallbackHandler.instance()->registerMessage(TT("objectFreeing"), (TTMethod)&TTCallback::notify, kTTMessagePassValue);
	
	// tell the source that is passed in that we want to watch it
	mDestinationObject->registerObserverForNotifications(mCallbackHandler);
}


/*************************************************************/


TTErr TTGraphDestination::push(const TTDictionary& aDictionary)
{
	// TODO: We need to append the inlet number into the dictionary before sending it
	return mDestinationObject->push(aDictionary);
}



