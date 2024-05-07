/* 
 *	External object for Max/MSP
 *	Copyright © 2003 by Timothy Place
 * 
 *	License: This code is licensed under the terms of the "New BSD License"
 *	http://creativecommons.org/licenses/BSD/
 */

#include "TTClassWrapperMax.h"

#define MAX_LIST_LENGTH 100

t_class *mapper_class;				// Required: Global pointer for our class

typedef struct _mapper 				// Data Structure for this object
{
	t_object	ob;								// REQUIRED: Our object
	void		*myproxies[10];					// for proxy inlets
	void 		*my_outlet[2];					// my outlet -- NOTE: the attribute dump outlet (2nd outlet) is INCLUDED
	t_atom		mylist[MAX_LIST_LENGTH];		// storage for output list
	short		mylistlen;						// how many elements in our list
	
	t_atom		mapto[MAX_LIST_LENGTH];			// ATTRIBUTE: message to map the midi input to
	long		maptolen;						// 			  number of list elements in mapto

	t_atom		includes[3];					// ATTRIBUTE: store a list of flags for what to include
	long		includeslen;					// 			  number of list elements in includes

	long 		channel;						// ATTRIBUTE: channel
	t_symbol	*type;				 			// ATTRIBUTE: type (stored as a symbol)
	long 		match1;							// ATTRIBUTE: match1
	long 		match2;							// ATTRIBUTE: match2

	long		inchannel;						// the actual channel of the input
} t_mapper;


// Prototypes for our methods:
void *mapper_new(t_symbol *s, long argc, t_atom *argv);
void mapper_free(t_mapper *x);
void mapper_assist(t_mapper *x, void *b, long msg, long arg, char *dst);
void mapper_symbol(t_mapper *x, t_symbol *msg, short argc, t_atom *argv);
void mapper_int(t_mapper *x, long val);
void mapper_list(t_mapper *x, t_symbol *msg, short argc, t_atom *argv);
void mapper_1item(t_mapper *x, long val);
void mapper_2item(t_mapper *x, long val1, long val2);


// Globals
static t_symbol *ps_any;
static t_symbol *ps_none;
static t_symbol *ps_note;
static t_symbol *ps_polypressure;
static t_symbol *ps_control;
static t_symbol *ps_touch;
static t_symbol *ps_bend;
static t_symbol *ps_program;

/************************************************************************************/
// Main() Function

extern "C" int C74_EXPORT main(void)
{	
	t_class *c;

	c = class_new("tap.midimapper",(method)mapper_new, (method)mapper_free, sizeof(t_mapper), 
		(method)0L, A_GIMME, 0);

		common_symbols_init();
	class_addmethod(c, (method)mapper_int,		"int", A_LONG, 0);
 	class_addmethod(c, (method)mapper_list, 	"list", A_GIMME, 0);		
	class_addmethod(c, (method)mapper_assist, 	"assist", A_CANT, 0); 
//	addmess((method)mapper_createmapping, "createmapping", A_GIMME, 0);	// bind method for symbols
//	addmess((method)mapper_clearmapping, "clearmapping", A_GIMME, 0);	// bind method for symbols
	class_addmethod(c, (method)stdinletinfo,	"inletinfo",	A_CANT, 0);

	CLASS_ATTR_ATOM_VARSIZE(c,	"mapto",		0,	t_mapper, mapto, maptolen, MAX_LIST_LENGTH);
	CLASS_ATTR_ATOM_VARSIZE(c,	"includes",		0,	t_mapper, includes, includeslen, MAX_LIST_LENGTH);
	CLASS_ATTR_LONG(c,			"channel",		0,	t_mapper, channel);
	CLASS_ATTR_SYM(c,			"type",			0,	t_mapper, type);
	CLASS_ATTR_LONG(c,			"match1",		0,	t_mapper, match1);
	CLASS_ATTR_LONG(c,			"match2",		0,	t_mapper, match2);
		
class_register(_sym_box, c); 	mapper_class = c;

	// Init Globals
	ps_any= gensym("any");
	ps_none = gensym("none");;
	ps_note = gensym("note");
	ps_polypressure = gensym("polypressure");
	ps_control = gensym("control");
	ps_touch = gensym("touch");
	ps_bend = gensym("bend");
	ps_program = gensym("program");
}


/************************************************************************************/
// Object Creation Method

void *mapper_new(t_symbol *s, long argc, t_atom *argv)
{
	t_mapper	*x;
	short	i;

	x = (t_mapper *)object_alloc(mapper_class);;
	if(x){
		x->my_outlet[1] = outlet_new(x, NULL);
		object_obex_store((void *)x, _sym_dumpout, (object *)x->my_outlet[1]);	// dumpout

		x->my_outlet[0] = outlet_new(x,0L);						// create an outlet (this is my outlet) and store the pointer in the struct

		for (i=6; i >= 1; i--)									// Create Inlets (inlets beyond the default (num 0) inlet
			x->myproxies[i] = proxy_new(x, i, 0L);

		// Set defaults before I go loading in values from the attributes
		// atom_setsym(x->mapto, gensym("No Mapping For This MIDI Message"));
		x->channel = 0;
		x->type = ps_none;
		x->match1 = 0;
		x->match2 = 0;

		// handle attribute args
		attr_args_process(x, argc, argv);				// handle attribute args	

		// If no flags are set in the arg, we will now set them
	//	if(x->includeslen < 3){
	//		for(i=0;i<3;i++)
	//			atom_setlong(x->includes+i, 0);
	//	}
	}
	return (x);											// return the pointer to our new instantiation
}


// destroy the object - free its memory allocation	
void mapper_free(t_mapper *x)
{
	for(int i=6; i >= 1; i--)
		object_free((t_object *)x->myproxies[i]);
}


/************************************************************************************/
// Methods bound to input/inlets

// Method for Assistance Messages
void mapper_assist(t_mapper *x, void *b, long msg, long arg, char *dst)
{
	if(msg==1) 						// Inlet
		strcpy(dst, "Input");
	else if(msg==2){ 				// Outlets
		switch(arg){
			case 0: strcpy(dst, "Output"); break;
			case 1: strcpy(dst, "Attribute Stuff"); break;
 		}
 	}		
}


// INTEGER INPUT
void mapper_int(t_mapper *x, long val)
{
	long inletnum = proxy_getinlet((object *)x);

	switch (inletnum) {
		case 3:			// PROGRAM CHANGE
			if((x->type == ps_program) || (x->type == ps_any))
				mapper_1item(x, val);		
			break;
		case 4:			// AFTERTOUCH
			if((x->type == ps_touch) || (x->type == ps_any))
				mapper_1item(x, val);			
			break;
		case 5:			// PITCHBEND
			if((x->type == ps_bend) || (x->type == ps_any))
				mapper_1item(x, val);		
			break;
		case 6:			// CHANNEL
			x->inchannel = val;
			break;
	}
}


// LIST INPUT
void mapper_list(t_mapper *x, t_symbol *msg, short argc, t_atom *argv)
{
	long inletnum = proxy_getinlet((object *)x);

	switch (inletnum) {
		case 0:			// NOTE
			if((x->type == ps_note) || (x->type == ps_any))
				mapper_2item(x, argv[0].a_w.w_long, argv[1].a_w.w_long);
			break;
		case 1:			// POLYPRESSURE
			if((x->type == ps_polypressure) || (x->type == ps_any))
				mapper_2item(x, argv[0].a_w.w_long, argv[1].a_w.w_long);
			break;
		case 2:			// CONTROL
			if((x->type == ps_control) || (x->type == ps_any))
				mapper_2item(x, argv[0].a_w.w_long, argv[1].a_w.w_long);
			break;		
	}
}


// SYMBOL INPUT
void mapper_symbol(t_mapper *x, t_symbol *msg, short argc, t_atom *argv)
{
	long inletnum = proxy_getinlet((object *)x);

	atom_setsym(x->mylist+(inletnum), msg);

	if (argc > 0)
		outlet_anything(x->my_outlet[1], gensym("error - extra arguments for symbol input"), 0, (t_atom *)NIL);
}


/************************************************************************************/
// Additional functions

// ONE ITEM MAPPER
void mapper_1item(t_mapper *x, long val)
{
	short i;
	
	if((x->channel == x->inchannel) || (x->channel < 0)){
		if((x->match1 == val) || (x->match1 < 0)){
			for(i=0;i<x->maptolen;i++){
				x->mylist[i] = x->mapto[i];
			}
			if(x->includes[0].a_w.w_long){
				atom_setlong(x->mylist+i, x->inchannel);
				i++;
			}
			if(x->includes[2].a_w.w_long){
				atom_setlong(x->mylist+i, val);
				i++;
			}
			outlet_list(x->my_outlet[0], 0L, i, x->mylist);	// output the result
		}
	}
}


// TWO ITEM MAPPER
void mapper_2item(t_mapper *x, long val1, long val2)
{
	short i;
	
	if((x->channel == x->inchannel) || (x->channel < 0)){
		if(((x->match1 == val1) || (x->match1 < 0)) && ((x->match2 == val2) || (x->match2 < 0))){
			for(i=0;i<x->maptolen;i++){
				x->mylist[i] = x->mapto[i];
			}
			if(x->includes[0].a_w.w_long){
				atom_setlong(x->mylist+i, x->inchannel);
				i++;
			}
			if(x->includes[1].a_w.w_long){
				atom_setlong(x->mylist+i, val1);
				i++;
			}
			if(x->includes[2].a_w.w_long){
				atom_setlong(x->mylist+i, val2);
				i++;
			}
			outlet_list(x->my_outlet[0], 0L, i, x->mylist);	// output the result
		}
	}
}
