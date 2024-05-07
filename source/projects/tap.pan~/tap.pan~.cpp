/* 
 *	External object for Max/MSP
 *	Copyright © 2001 by Timothy Place
 * 
 *	License: This code is licensed under the terms of the "New BSD License"
 *	http://creativecommons.org/licenses/BSD/
 */

#include "TTClassWrapperMax.h"

extern "C" TTErr TTLoadJamomaExtension_EffectsLib(void);

extern "C" int C74_EXPORT main(void)
{	
	WrappedClassOptionsPtr	options = new WrappedClassOptions;
	WrappedClassPtr			c = NULL;	
	TTValue					v(2);
	
	TTDSPInit();
	TTLoadJamomaExtension_EffectsLib();
	
	options->append(TT("fixedNumOutputChannels"), v);
	wrapTTClassAsMaxClass(TT("panorama"), "tap.pan~", &c, options);
	CLASS_ATTR_ENUM(c->maxClass, "mode", 0, "calculate lookup");
	CLASS_ATTR_ENUM(c->maxClass, "shape", 0, "equalPower linear squareRoot");
	
	return 0;
}
