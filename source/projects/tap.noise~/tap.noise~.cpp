/* 
 *	External object for Max/MSP
 *	Copyright © 2004 by Timothy Place
 * 
 *	License: This code is licensed under the terms of the "New BSD License"
 *	http://creativecommons.org/licenses/BSD/
 */

#include "TTClassWrapperMax.h"

extern "C" TTErr TTLoadJamomaExtension_GeneratorLib(void);

extern "C" int C74_EXPORT main(void)
{	
	WrappedClassOptionsPtr	options = new WrappedClassOptions;
	WrappedClassPtr			c = NULL;	
	TTValue					v(1);
	
	TTDSPInit();
	TTLoadJamomaExtension_GeneratorLib();
	
	options->append(TT("fixedNumOutputChannels"), v);
	wrapTTClassAsMaxClass(TT("noise"), "tap.noise~", &c, options);
	CLASS_ATTR_ENUM(c->maxClass, "mode", 0, "white pink brown blue gauss");
	
	return 0;
}

