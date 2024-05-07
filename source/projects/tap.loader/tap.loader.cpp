/* 
 *	External object for Max/MSP
 *	Copyright © 2009 by Timothy Place
 * 
 *	License: This code is licensed under the terms of the "New BSD License"
 *	http://creativecommons.org/licenses/BSD/
 */

#include "TTClassWrapperMax.h"


// Data Structure for this object
typedef struct _loader {
	t_object ob;	
} t_loader;

// Prototypes
void* loader_new(t_symbol *name, long argc, t_atom *argv);

// Globals
static t_class *s_loader_class;


/************************************************************************************/
// Main() Function

extern "C" int C74_EXPORT main(void)
{
	s_loader_class = class_new((char*)"tap.loader",(method)loader_new, (method)NULL, sizeof(t_loader), (method)NULL, A_GIMME, 0);
	class_register(CLASS_NOBOX, s_loader_class);
	post("TapTools 4.1 | 74objects.com");
}


/************************************************************************************/
// Object Life

void *loader_new(t_symbol *name, long argc, t_atom *argv)
{
	return object_alloc(s_loader_class);
}
