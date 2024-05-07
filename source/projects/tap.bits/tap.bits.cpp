/* 
 *	External object for Max/MSP
 *	Copyright © 2003 by Timothy Place
 * 
 *	License: This code is licensed under the terms of the "New BSD License"
 *	http://creativecommons.org/licenses/BSD/
 */

#include "TTClassWrapperMax.h"


typedef struct _bits 				// Data Structure for this object
{
	t_object			ob;				// REQUIRED: Our object
	void 				*my_outlet[2];	// pointers to our outlets
	t_symbol			*mode; 			// ATTRIBUTE: mode (stored as a symbol)
	long				matrix_width;	// ATTRIBUTE: matrix_width (stored as long) - used by matrixctrl modes
	t_atom				bitlist[32];	// storage for output list
	long				matrixlist[32];	// storage for matrixctrl mode
} t_bits;


// Prototypes for our methods:
void *bits_new(t_symbol *s, long argc, t_atom *argv);
void bits_assist(t_bits *x, void *b, long msg, long arg, char *dst);
void bits_list(t_bits *x, t_symbol *msg, short argc, t_atom *argv);
void bits_int(t_bits *x, long value);

// Globals
static t_class *bits_class;					// Required: Global pointer for our class
static t_symbol *ps_bits2ints;
static t_symbol *ps_ints2bits;
static t_symbol *ps_matrixctrl2ints;
static t_symbol *ps_ints2matrixctrl;


/************************************************************************************/
// Main() Function

extern "C" int C74_EXPORT main(void)
{	
	t_class *c;
	
	c = class_new("tap.bits",(method)bits_new, (method)0L, sizeof(t_bits), (method)0L, A_GIMME, 0);

	common_symbols_init();
	class_addmethod(c, (method)bits_int,		"int", A_LONG, 0L);
	class_addmethod(c, (method)bits_list,		"list", A_GIMME, 0L);	
	class_addmethod(c, (method)bits_assist,		"assist", A_CANT, 0L); 
	class_addmethod(c, (method)stdinletinfo,	"inletinfo",	A_CANT, 0);

	CLASS_ATTR_SYM(c,		"mode",			0,	t_bits, mode);
	CLASS_ATTR_ENUM(c,		"mode",			0, "bits2ints ints2bits matrixctrl2ints ints2matrixctrl");
	
	CLASS_ATTR_LONG(c,		"matrix_width",	0,	t_bits, matrix_width);

	class_register(_sym_box, c);
	bits_class = c;

	// Initialize Globals
	ps_bits2ints = gensym("bits2ints");	// Initialize these globals (so we don't have to constantly run a gensym for them in our methods)
	ps_ints2bits = gensym("ints2bits");
	ps_matrixctrl2ints = gensym("matrixctrl2ints");
	ps_ints2matrixctrl = gensym("ints2matrixctrl");
	
	return 0;
}


/************************************************************************************/
// Object Creation Method

void *bits_new(t_symbol *s, long argc, t_atom *argv)
{
	t_bits *x;

	x = (t_bits *)object_alloc(bits_class);
	if(x){
		x->my_outlet[1] = outlet_new(x,0L);			// create an outlet (this is my outlet) and store the pointer in the struct
		x->my_outlet[0] = outlet_new(x,0L);			// create an outlet (this is my outlet) and store the pointer in the struct
    	object_obex_store((void *)x, _sym_dumpout, (object *)x->my_outlet[1]);	// dumpout	
		
		x->mode = ps_bits2ints;						// Set the default before I go loading in values from the attributes
		x->matrix_width = 8;

		//no normal args, no matrices
		attr_args_process(x,argc,argv);				//handle attribute args			
		
		// Check the mode attributes to make sure they are valid (i.e. the user didn't make a typo)
		if((x->mode != ps_bits2ints) && (x->mode != ps_ints2bits) && (x->mode != ps_matrixctrl2ints)&& (x->mode != ps_ints2matrixctrl))
		x->matrix_width = TTClip(x->matrix_width, 2L, 31L);
	}
	return (x);										// return the pointer to our new instantiation
}


/************************************************************************************/
// Methods bound to input/inlets

// Method for Assistance Messages
void bits_assist(t_bits *x, void *b, long msg, long arg, char *dst)
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


// Method for list input (converts a list of bits into a long)
void bits_list(t_bits *x, t_symbol *msg, short argc, t_atom *argv)
{
	int i, j;			// counters
	long val = 0;		// Our bit-constructed number
	long temp;
	t_atom templist[3];	// used in matrixctrl mode
	
	if(x->mode==ps_bits2ints){						// *** BIT-LIST TO INTEGER(LIST) MODE ***
		for(i=(argc - 1); i>=0; i--){
			temp = argv[i].a_w.w_long;
			val |= temp<<(argc-(i+1));						// bit shift, then or it with the val
		}	
		outlet_int(x->my_outlet[0], val);					// spit it out
	}
	else if(x->mode==ps_matrixctrl2ints){			// *** MATRIX-CTRL TO INTEGER-LIST MODE ***
		object_post((t_object *)x, "tap.bits: This mode is not yet implemented");
	}
	else if(x->mode==ps_ints2matrixctrl){			// *** INTEGER-LIST TO MATRIX-CTRL MODE ***
		for(j=0; j<argc; j++){
			atom_setlong(templist+1, j);						// Store the row in the output list
			temp = argv[j].a_w.w_long;					// Get the value in the input list (for the moment we assume it is an int - should also handle floats)
			for(i=0; i < x->matrix_width; i++){
				atom_setlong(templist+0, i);						// Store the column in the output list
				atom_setlong(templist+2, 1 & temp);				// Store the switch value in the output list
				temp = temp>>1;								// Bit shift to the next one
				
				outlet_list(x->my_outlet[0], 0L, 3, templist);	// output the result	
			}
		}
	}
	else			// use the Jitter attribute dumpout outlet to report an error
		outlet_anything(x->my_outlet[1], gensym("error - wrong mode for list"), 0, 0L);

}


// Method for int input (converts a long into a list of bits)
void bits_int(t_bits *x, long value)
{
	int i;
	
	if(x->mode==ps_ints2bits){
		for(i=0; i < 32; i++){
			atom_setlong(x->bitlist+(31-i), 1 & value);
			value = value>>1;
		}
		outlet_list(x->my_outlet[0], 0L, i, x->bitlist);		// output the result	
	}
	else			// use the Jitter attribute dumpout outlet to report an error
		outlet_anything(x->my_outlet[1], gensym("error - wrong mode for int"), 0, 0L);
}
