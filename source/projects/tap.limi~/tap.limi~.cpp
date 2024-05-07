/* 
 *	External object for Max/MSP
 *	Copyright © 2005 by Timothy Place
 * 
 *	License: This code is licensed under the terms of the "New BSD License"
 *	http://creativecommons.org/licenses/BSD/
 */

#include "TTClassWrapperMax.h"

extern "C" TTErr TTLoadJamomaExtension_EffectsLib(void);
extern "C" TTErr TTLoadJamomaExtension_FilterLib(void);

int C74_EXPORT main(void)
{
	WrappedClassPtr	w;
	
	TTDSPInit();
	TTLoadJamomaExtension_EffectsLib();
	TTLoadJamomaExtension_FilterLib();
	
	return wrapTTClassAsMaxClass(TT("limiter"), "tap.limi~", &w);
}
