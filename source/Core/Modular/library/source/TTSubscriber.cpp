/** @file
 *
 * @ingroup modularLibrary
 *
 * @brief A contextual subscriber to register TTObject as TTNode in a TTNodeDirectory
 *
 * @details TODO: Add more info
 *
 * @authors Théo de la Hogue
 *
 * @copyright © 2010, Théo de la Hogue @n
 * This code is licensed under the terms of the "New BSD License" @n
 * http://creativecommons.org/licenses/BSD/
 */


#include "TTSubscriber.h"

#define thisTTClass			TTSubscriber
#define thisTTClassName		"Subscriber"
#define thisTTClassTags		"node, subscriber"

TT_MODULAR_CONSTRUCTOR,
mRelativeAddress(kTTAdrsEmpty),
mNodeAddress(kTTAdrsEmpty),
mContextAddress(kTTAdrsEmpty),
mNewInstanceCreated(NO),
mNewContextInstanceCreated(NO),
mSubscribed(NO),
mExposedMessages(NULL),
mExposedAttributes(NULL)
{
	mObject = arguments[0];
	mRelativeAddress = arguments[1];
	
	addAttribute(RelativeAddress, kTypeSymbol);
	addAttribute(NodeAddress, kTypeSymbol);
	addAttribute(ContextAddress, kTypeSymbol);
	addAttribute(NewInstanceCreated, kTypeBoolean);
    addAttribute(NewContextInstanceCreated, kTypeBoolean);
    addAttribute(Subscribed, kTypeBoolean);
	
	addAttributeProperty(RelativeAddress, readOnly, YES);
	addAttributeProperty(NodeAddress, readOnly, YES);
	addAttributeProperty(ContextAddress, readOnly, YES);
	addAttributeProperty(NewInstanceCreated, readOnly, YES);
    addAttributeProperty(Subscribed, readOnly, YES);
    
    addMessageWithArguments(Subscribe);
    addMessage(Unsubscribe);
	
	mExposedMessages = new TTHash();
	mExposedAttributes = new TTHash();
}

TTSubscriber::~TTSubscriber()
{
    if (mSubscribed)
        Unsubscribe();
    
    delete mExposedMessages;
    delete mExposedAttributes;
}

TTErr TTSubscriber::Subscribe(const TTValue& inputValue, TTValue& outputValue)
{
    TTListPtr           aContextList = NULL;
	TTNodeDirectoryPtr	aDirectory = accessApplicationLocalDirectory;		// only subscribe into local directory
	TTAddress			contextAddress, absoluteAddress;
	TTValue				aTempValue, args;
	TTPtr				ourContext, hisContext;
	TTList				aNodeList;
	TTNodePtr			aNode, aContextNode;
	TTObject            hisObject;
	TTErr				err;
    
    mNewInstanceCreated = NO;
    mNewContextInstanceCreated = NO;
    
    aContextList = TTListPtr((TTPtr)inputValue[0]);
	TT_ASSERT("ContextList passed to TTSubscriber::Subscribe is not NULL", aContextList);
    
    if (aContextList) {
        
        // if there is no subcription, the local directory exists and the address is relative
        if (!mSubscribed && aDirectory && mRelativeAddress.getType() == kAddressRelative) {
            
            // register each Context of the list as TTNode in the tree structure (if they don't exist yet)
            aContextNode = this->registerContextList(aContextList);
            
            // Build the node at /contextAddress/parent/node
            if (aContextNode) {
                
                // Get our Context
                ourContext = aContextNode->getContext();
                
                // Make absolute address
                aContextNode->getAddress(contextAddress);
                absoluteAddress = contextAddress.appendAddress(this->mRelativeAddress);
                
                // Check if the node exists
                err = aDirectory->Lookup(absoluteAddress, aNodeList, &aNode);
                
                // if the node doesn't exist, create it
                if (err) {
                    if (mObject.valid())
                        aDirectory->TTNodeCreate(absoluteAddress, mObject, ourContext,  &aNode, &this->mNewInstanceCreated);
                    else
                        return kTTErrGeneric;
                }
                // else the node already exists
                else if (mObject.valid()) {
                    
                    // Get his refered object
                    hisObject = aNode->getObject();
                    
                    // if there is no refered object
                    if (!hisObject.valid()) {
                        
                        // set our object instead
                        aNode->setObject(mObject);
                        
                        // get his context
                        hisContext = aNode->getContext();
                        
                        // if there is no context
                        if (!hisContext) {
                            
                            // set our context instead
                            aTempValue.clear();
                            aTempValue.append((TTPtr)ourContext);
                            aNode->setContext(ourContext);
                        }
                        
                        // notify for the creation of the address when replacing the Object and Context
                        // !!! Maybe this could introduce confusion for namespace observer !!!
                        // introduce a new flag (kAddressObjectChanged) ?
                        aDirectory->notifyObservers(absoluteAddress, aNode, kAddressCreated);
                    }
                    // else there is already an object
                    else {
                        
                        // if it is the ContextNode, do nothing (our object can't be refered)
                        // else create another instance to refer our object
                        if (aNode != aContextNode)
                            aDirectory->TTNodeCreate(absoluteAddress, mObject, ourContext,  &aNode, &this->mNewInstanceCreated);
                    }
                }
                
                aNode->getAddress(this->mNodeAddress);
                aContextNode->getAddress(this->mContextAddress);
                aNode->getAddress(this->mRelativeAddress, this->mContextAddress);
                
                mSubscribed = YES;
                
                // return the node and the context node to work on them
                outputValue = TTValue((TTPtr)aNode);
                outputValue.append((TTPtr)aContextNode);
                
                return kTTErrNone;
            }
        }
    }
    
    return kTTErrGeneric;
}

TTErr TTSubscriber::Unsubscribe()
{
    TTNodeDirectoryPtr	aDirectory = accessApplicationLocalDirectory;		// only subscribes into local directory
    TTNodePtr           aNode = NULL;
	TTList				childrenList;
	TTValue				aTempValue;
	TTValue				v, keys;
	TTAddress			objectAddress, nameToAddress;
	TTSymbol			k;
	TTObject            anObject;
	TTUInt32            i;
	TTErr				err;
    
    if (mObject.valid()) {
        
        err = aDirectory->getTTNode(mNodeAddress, &aNode);
        if (!err && aNode) {
            
            anObject = aNode->getObject();
            
            if (anObject == mObject) {
                
                // If node have no more child : destroy the node (except for root)
                aNode->getChildren(S_WILDCARD, S_WILDCARD, childrenList);
                if (childrenList.isEmpty() && aNode != aDirectory->getRoot())
                    aDirectory->TTNodeRemove(mNodeAddress);
                
                // else notify for the unregistration of the object
                // !!! Maybe this could introduce confusion for namespace observer !!!
                // introduce a new flag (kAddressObjectUnregistered) ?
                else {
                    
                    // remove alias for TTContainer object before
                    if (anObject.name() == kTTSym_Container)
                        anObject.send("AliasRemove");
                    
                    // notify
                    aDirectory->notifyObservers(mNodeAddress, aNode, kAddressDestroyed);
                    
                    // set empty object
                    aNode->setObject();
                }
            }
        }
    }
	
	// Clear exposed Messages
	err = mExposedMessages->getKeys(keys);
	if (!err) {
		for (i = 0; i < keys.size(); i++) {
			
			k = keys[i];
			mExposedMessages->lookup(k, v);

			convertUpperCasedNameInAddress(k, nameToAddress);
			objectAddress = mNodeAddress.appendAddress(nameToAddress);
			
			aDirectory->TTNodeRemove(objectAddress);
			
			mExposedMessages->remove(k);
		}
	}
	
	// Clear exposed Attributes
	err = mExposedAttributes->getKeys(keys);
	if (!err) {
		for (i = 0; i < keys.size(); i++) {
			
			k = keys[i];
			mExposedAttributes->lookup(k, v);
			
			convertUpperCasedNameInAddress(k, nameToAddress);
			objectAddress = mNodeAddress.appendAddress(nameToAddress);
			
			aDirectory->TTNodeRemove(objectAddress);
			
			mExposedAttributes->remove(k);
		}
	}
    
    return kTTErrNone;
}

TTNodePtr TTSubscriber::registerContextList(TTListPtr aContextList)
{
	TTNodeDirectoryPtr	aDirectory = accessApplicationLocalDirectory;		// only subscribes into local directory
	TTValue				args;
	TTSymbol			formatedContextSymbol;
	TTAddress           relativeContextAddress, contextAddress, lowerContextAddress;
	TTList				contextNodeList, attributesAccess;
	TTNodePtr			contextNode, lowerContextNode;
	TTPtr				aContext, lowerContext;
    TTObject            empty;
	TTBoolean			found;
	
	// Build the /topContext/subContext/.../contextName/ structure
	// Check each context instance looking at the patcher.
	
	// start by the root
	contextNode = aDirectory->getRoot();
	
	// if there are contexts in the context list
	if(!aContextList->isEmpty()){
		
		// for each context of the context list
		for (aContextList->begin(); aContextList->end(); aContextList->next()){
			
			// get the context symbol as a relative context address
			formatedContextSymbol = aContextList->current()[0];
			relativeContextAddress = TTAddress(formatedContextSymbol);
			
			// get the context
			aContext = aContextList->current()[1];
			
			// if one is missing stop the registration
			if (relativeContextAddress == kTTAdrsEmpty || !aContext)
				return NULL;
			
			// 1. Look for relativeContextName.* in order to find a child with the same context
			contextNode->getChildren(relativeContextAddress.getName(), S_WILDCARD, contextNodeList);
			
			/*	Former step 1.
				This introduced a bug when a user edits /anAddress.I several times.
				However this is faster than the new implementation because now we
				always look for all children to find the context-like child...
			 
			 if (relativeContextAddress.getInstance() == NO_INSTANCE)
				err = contextNode->getChildren(relativeContextAddress.getName(), S_WILDCARD, contextNodeList);
			 else 
				err = contextNode->getChildren(relativeContextAddress.getName(), relativeContextAddress.getInstance(), contextNodeList);
			 */
			
			// 3. For each node of the contextNodeList, check the context
			// if one matches, keep it else we have to create the node
			found = false;
			lowerContextNode = NULL;
			for (contextNodeList.begin(); contextNodeList.end(); contextNodeList.next()) {
				
				lowerContextNode = TTNodePtr((TTPtr)contextNodeList.current()[0]);
				
				// Check if objects are the same
				lowerContext = lowerContextNode->getContext();
				
				if (aContext == lowerContext) {
					found = true;
					break;
				}
                
                // else if the instance are the same
                else if (relativeContextAddress.getInstance() == lowerContextNode->getInstance()) {
                    
                    // if there is no registered object
                    if (!lowerContextNode->getObject().valid()) {
                        found = true;
                        break;
                    }
                }
			}
			
			// if no node exists : create a new instance for this context
			if (!found) {
				
				contextNode->getAddress(contextAddress);
				
				// don't create another root !
				if (relativeContextAddress != kTTAdrsRoot) {
					
					lowerContextAddress = contextAddress.appendAddress(relativeContextAddress);
					
					// Make a TTNode with no object
					aDirectory->TTNodeCreate(lowerContextAddress, empty, aContext, &contextNode, &this->mNewContextInstanceCreated);

				}
				else {
					contextNode = aDirectory->getRoot();
					
					// if the current context of the root is NULL : set our context
					if (!contextNode->getContext())
						contextNode->setContext(aContext);
				}
			}
			else
				contextNode = lowerContextNode;
		} // end for
	} // end if
	
	return contextNode;
}

TTErr TTSubscriber::exposeMessage(TTObject anObject, TTSymbol messageName, TTObject& returnedData)
{
    TTNodeDirectoryPtr	aDirectory = accessApplicationLocalDirectory;		// only subscribes into local directory
	TTValue             baton, v;
	TTAddress           nameToAddress, dataAddress;
	TTNodePtr           aNode;
	TTBoolean           nodeCreated;
	TTPtr               aContext;
    
    aDirectory->getTTNode(mNodeAddress, &aNode);
    if (aNode) {
        
        // Create TTData
        returnedData = TTObject(kTTSym_Data, kTTSym_message);
        
        baton = TTValue(TTObject(this), messageName);
        returnedData.set(kTTSym_baton, baton);
        returnedData.set(kTTSym_function, TTPtr(&TTSubscriberMessageReturnValueCallback));
        
        // register TTData into the local tree
        convertUpperCasedNameInAddress(messageName, nameToAddress);
        dataAddress = mNodeAddress.appendAddress(nameToAddress);
        aContext = aNode->getContext();
        accessApplicationLocalDirectory->TTNodeCreate(dataAddress, returnedData, aContext, &aNode, &nodeCreated);
        
        // store TTData and given object
        v = TTValue(returnedData);
        v.append(anObject);
        mExposedMessages->append(messageName, v);
        
        return kTTErrNone;
    }
    
    return kTTErrGeneric;
}

TTErr TTSubscriber::exposeAttribute(TTObject anObject, TTSymbol attributeName, TTSymbol service, TTObject& returnedData)
{
    TTNodeDirectoryPtr	aDirectory = accessApplicationLocalDirectory;		// only subscribes into local directory
	TTValue             args, baton, v, none;
	TTObject			observeValueCallback;                               // to set the data when an object attribute changed
	TTAttributePtr      anAttribute = NULL;
	TTAddress           nameToAddress, dataAddress;
	TTNodePtr           aNode;
	TTBoolean           nodeCreated;
	TTPtr               aContext;
	TTErr               err;
	
    aDirectory->getTTNode(mNodeAddress, &aNode);
    if (aNode) {

        if (service == kTTSym_parameter || service == kTTSym_return) {

            // Create TTData
            returnedData = TTObject(kTTSym_Data, service);
            
            baton = TTValue(TTObject(this), attributeName);
            returnedData.set(kTTSym_baton, baton);
            returnedData.set(kTTSym_function, TTPtr(&TTSubscriberMessageReturnValueCallback));
            
            // register TTData into the local tree
            convertUpperCasedNameInAddress(attributeName, nameToAddress);
            dataAddress = mNodeAddress.appendAddress(nameToAddress);
            aContext = aNode->getContext();
            accessApplicationLocalDirectory->TTNodeCreate(dataAddress, returnedData, aContext, &aNode, &nodeCreated);
            
            // observe the attribute of the object
            err = anObject.instance()->findAttribute(attributeName, &anAttribute);
            if (!err) {

                observeValueCallback = TTObject("callback");
                
				baton = TTValue(TTObject(this), attributeName);
                observeValueCallback.set(kTTSym_baton, baton);
                observeValueCallback.set(kTTSym_function, TTPtr(&TTSubscriberAttributeObserveValueCallback));
                
                anAttribute->registerObserverForNotifications(observeValueCallback);
            }
            
            // store TTData and given object
            v = TTValue(returnedData);
            v.append(anObject);
            mExposedAttributes->append(attributeName, v);
            
            return kTTErrNone;
        }
    }
    return kTTErrGeneric;
}

TTErr TTSubscriber::unexposeMessage(TTSymbol messageName)
{
	TTNodeDirectoryPtr	aDirectory = accessApplicationLocalDirectory;		// only subscribes into local directory
	TTValue				storedObject;
	TTAddress           objectAddress, nameToAddress;
	
	if (!mExposedMessages->lookup(messageName, storedObject)) {
		
		convertUpperCasedNameInAddress(messageName, nameToAddress);
		objectAddress = mNodeAddress.appendAddress(nameToAddress);
		
		aDirectory->TTNodeRemove(objectAddress);
		
		mExposedMessages->remove(messageName);
		
		return kTTErrNone;
	}
	
	return kTTErrGeneric;
}

TTErr TTSubscriber::unexposeAttribute(TTSymbol attributeName)
{
	TTNodeDirectoryPtr	aDirectory = accessApplicationLocalDirectory;		// only subscribes into local directory
	TTValue				storedObject;
	TTAddress           objectAddress, nameToAddress;
	
	if (!mExposedAttributes->lookup(attributeName, storedObject)) {
		
		convertUpperCasedNameInAddress(attributeName, nameToAddress);
		objectAddress = mNodeAddress.appendAddress(nameToAddress);
		
		aDirectory->TTNodeRemove(objectAddress);
		
		mExposedAttributes->remove(attributeName);
		
		return kTTErrNone;
	}
	
	return kTTErrGeneric;
}

#if 0
#pragma mark -
#pragma mark Some Methods
#endif

TTErr TTSubscriberMessageReturnValueCallback(const TTValue& baton, const TTValue& data)
{
    TTObject        o;
	TTSubscriberPtr aSubscriber;
	TTSymbol		messageName;
	TTValue			v, none;
	TTErr			err;
	
	// unpack baton (a #TTSubscriber)
    o = baton[0];
	aSubscriber = (TTSubscriberPtr)o.instance();
	messageName = baton[1];
	
	// get the exposed TTObject
	err = aSubscriber->mExposedMessages->lookup(messageName, v);
	
	if (!err) {
		o = v[1];
		
		// protect data
		v = data;
		
		// send data
		o.send(messageName, data);
		
		return kTTErrNone;
	}
	
	return kTTErrGeneric;
}

TTErr TTSubscriberAttributeReturnValueCallback(const TTValue& baton, const TTValue& data)
{
    TTObject        o;
	TTSubscriberPtr aSubscriber;
	TTSymbol		attributeName;
	TTValue			v;
	TTErr			err;
	
	// unpack baton (a #TTSubscriber)
    o = baton[0];
	aSubscriber = (TTSubscriberPtr)o.instance();
	attributeName = baton[1];
	
	// get the exposed TTObject
	err = aSubscriber->mExposedAttributes->lookup(attributeName, v);
	
	if (!err) {
		o = v[1];
		
		// protect data
		v = data;
		
		// send data
		o.set(attributeName, data);
		
		return kTTErrNone;
	}
	
	return kTTErrGeneric;
}

TTErr TTSubscriberAttributeObserveValueCallback(const TTValue& baton, const TTValue& data)
{
    TTObject        o;
	TTSubscriberPtr aSubscriber;
	TTSymbol		attributeName;
	TTValue			v;
	TTErr			err;
	
	// unpack baton (a #TTSubscriber)
    o = baton[0];
	aSubscriber = (TTSubscriberPtr)o.instance();
	attributeName = baton[1];
	
	// get the TTData which expose the attribute
	err = aSubscriber->mExposedAttributes->lookup(attributeName, v);
	
	if (!err) {
		o = v[0];
		
		// protect data
		v = data;
		
		// set data
		o.set(kTTSym_value, data);
		
		return kTTErrNone;
	}
	
	return kTTErrGeneric;
}

