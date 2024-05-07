/* 
 *	External object for Max/MSP
 *	Copyright © 2008 by Timothy Place
 * 
 *	License: This code is licensed under the terms of the "New BSD License"
 *	http://creativecommons.org/licenses/BSD/
 */

#include "TTClassWrapperMax.h"

extern "C" TTErr TTLoadJamomaExtension_FilterLib(void);

extern "C" int C74_EXPORT main(void)
{
	WrappedClassPtr	w;
	t_class*		c;
	TTErr			err;
	
	TTLoadJamomaExtension_FilterLib();
	
	err = wrapTTClassAsMaxClass(TT("dcblock"), "tap.dcblock~", &w);
	if(!err){
		c = w->maxClass;
		CLASS_ATTR_STYLE(c,	"bypass",	0,	"onoff");
	}
}
	
