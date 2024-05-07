/* 
 *	External object for Max/MSP
 *	Copyright © 2005 by Timothy Place
 * 
 *	License: This code is licensed under the terms of the "New BSD License"
 *	http://creativecommons.org/licenses/BSD/
 */

#include "TTClassWrapperMax.h"

#define MAX_LIST_LENGTH 512

// Data Structure for this object
typedef struct _list_index
{
	t_object	ob;
	t_atom		stored_junk[MAX_LIST_LENGTH];		// storage for output list
	short		stored_junk_len;
	t_symbol	*attr_mode;				// mode 0: list-to-indexed, 1: indexed-to-list
	long		attr_offset;
	long		attr_onebased;			// toggle: 0 = zero-based, 1 = one-based 
	void 		*outlet;				// pointer to outlet
	void		*dumpout;				// pointer to dump outlet
} t_list_index;

// Prototypes for methods: need a method for each incoming message type:
void *tap_list_index_new(t_symbol *msg, short argc, t_atom *argv);
void tap_list_index_assist(t_list_index *x, void *b, long msg, long arg, char *dst);
void tap_list_index_anything(t_list_index *x, t_symbol *msg, short argc, t_atom *argv);

// Class Globals
static t_class *list_index_class;							// Required. Global pointing to this class
static t_symbol *ps_indexed2list, *ps_list2indexed;	


/************************************************************************************/
// Main() Function

extern "C" int C74_EXPORT main(void)
{
	t_class *c;

	c = class_new("tap.list.index",(method)tap_list_index_new, (method)0L, 
		sizeof(t_list_index), (method)0L, A_GIMME, 0);

		common_symbols_init();	// Initialize TapTools
	class_addmethod(c, (method)tap_list_index_anything,		"list",	A_GIMME, 0L);
	class_addmethod(c, (method)tap_list_index_anything,		"anything", A_GIMME, 0L);
    class_addmethod(c, (method)tap_list_index_assist, 		"assist", A_CANT, 0L); 
	class_addmethod(c, (method)stdinletinfo,				"inletinfo",	A_CANT, 0);

	CLASS_ATTR_SYM(c,	"mode",			0,	t_list_index, attr_mode);
	CLASS_ATTR_ENUM(c,	"mode",			0,	"indexed2list list2indexed");

	CLASS_ATTR_LONG(c,	"offset",		0,	t_list_index, attr_offset);

	CLASS_ATTR_LONG(c,	"onebased",		0,	t_list_index, attr_onebased);
	CLASS_ATTR_STYLE(c,	"onebased",		0,	"onoff");

class_register(_sym_box, c); 	list_index_class = c;
	
	// Init Class Globals
	ps_indexed2list = gensym("indexed2list");
	ps_list2indexed = gensym("list2indexed");
}


/************************************************************************************/
// Object Creation Method

void *tap_list_index_new(t_symbol *msg, short argc, t_atom *argv)
{
	t_list_index *x;
	short i;

	x = (t_list_index *)object_alloc(list_index_class);;
	if(x){
    	x->dumpout = outlet_new(x, 0L);
    	x->outlet = listout(x);
		object_obex_store((void *)x, _sym_dumpout, (object *)x->dumpout);	// dumpout

		for(i=0; i<MAX_LIST_LENGTH; i++) 
			atom_setlong(&(x->stored_junk[i]), 0);
		x->stored_junk_len = 0;
		x->attr_mode = ps_list2indexed;
		x->attr_offset = 0;
		x->attr_onebased = 0;
    	attr_args_process(x, argc, argv); //handle attribute args	
    }
 	return (x);
}

/************************************************************************************/
// Methods bound to input/inlets

// Method for Assistance Messages
void tap_list_index_assist(t_list_index *x, void *b, long msg, long arg, char *dst)
{
	if(msg==1) 			// Inlets
		strcpy(dst, "(list)");
	else if(msg==2){ 	// Outlets
		switch(arg){
			case 0: strcpy(dst, "(list)"); break;
			case 1: strcpy(dst, "dumpout"); break;
		}
	}
}


// METHOD: input processor
void tap_list_index_anything(t_list_index *x, t_symbol *msg, short argc, t_atom *argv)
{
	short 	i, temp;
	t_atom	outlist[2];
	
	if(x->attr_mode == ps_indexed2list){				// indexed-to-list
		if(argc > 1){		// if there are at least 2 args
			temp = atom_getfloat(argv);
			temp = TTClip(int(temp - x->attr_offset - x->attr_onebased), 0, MAX_LIST_LENGTH - 1);
//			taptools_max::atom_copy(&(x->stored_junk[temp]), argv+1);
			memcpy(&(x->stored_junk[temp]), argv+1, sizeof(t_atom));
			if(temp > x->stored_junk_len) 
				x->stored_junk_len = temp + 1;
		}
		outlet_list(x->outlet, 0L, x->stored_junk_len, x->stored_junk);
	}
	else{												// list-to-indexed
		for(i=0; i<argc; i++){
			atom_setlong(&outlist[0], i + x->attr_offset + x->attr_onebased);	// index
//			taptools_max::atom_copy(&(outlist[1]), argv+i);						// value
			memcpy(&(outlist[1]), argv+i, sizeof(t_atom));						// value
			outlet_list(x->outlet, 0L, 2, outlist);
		}
		outlet_anything(x->dumpout, gensym("done"), 0, 0L);
	}
}

