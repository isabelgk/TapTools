/* 
 *	External object for Max/MSP
 *	Copyright © 2003 by Timothy Place
 * 
 *	License: This code is licensed under the terms of the "New BSD License"
 *	http://creativecommons.org/licenses/BSD/
 */

#include "TTClassWrapperMax.h"


typedef struct _count_tilde{		// Data Structure for this object
	t_pxobject ob;					// Required by MSP (must be first)
	long		value;				// stored count value
	long		attr_high_bound;	// ATTRIBUTE
	long		attr_low_bound;		// ATTRIBUTE
	long		attr_active;		// ATTRIBUTE
	long		attr_autoreset;		// ATTRIBUTE
	long		attr_loop;			// ATTRIBUTE
} t_count_tilde;


// Prototypes for methods: need a method for each incoming message type:
void count_tilde_dsp64(t_count_tilde *x, t_object *dsp64, short *count, double samplerate, long maxvectorsize, long flags);			// ../../../../Jamoma/Core/DSP/library/build/JamomaDSP.dylib Method
void count_tilde_assist(t_count_tilde *x, void *b, long m, long a, char *s);	// Assistance Method
void *count_tilde_new(t_symbol *s, long argc, t_atom *argv);				// New Object Creation Method
void count_tilde_perform64(t_count_tilde *x, t_object *dsp64, double **ins, long numins, double **outs, long numouts, long sampleframes, long flags, void *userparam);
void count_tilde_free(t_count_tilde *x);
void count_tilde_activate(t_count_tilde *x, short toggle);
void count_tilde_reset(t_count_tilde *x);


static t_class *count_tilde_class;					// Required. Global pointing to this class


/************************************************************************************/
// Main() Function

extern "C" int C74_EXPORT main(void)
{
	t_class *c;
	
	c = class_new("tap.count~",(method)count_tilde_new, (method)count_tilde_free, sizeof(t_count_tilde), (method)0L, A_GIMME, 0);

	common_symbols_init();
	
	class_addmethod(c, (method)count_tilde_reset, 	"reset", 0L);
	class_addmethod(c, (method)count_tilde_dsp64,	"dsp64", A_CANT, 0);
    class_addmethod(c, (method)count_tilde_assist, 	"assist", A_CANT, 0L); 
	class_addmethod(c, (method)stdinletinfo,		"inletinfo",	A_CANT, 0);
 
	CLASS_ATTR_LONG(c,		"high_bound",	0,	t_count_tilde, attr_high_bound);
	CLASS_ATTR_LONG(c,		"low_bound",	0,	t_count_tilde, attr_low_bound);
	
	CLASS_ATTR_LONG(c,		"active",		0,	t_count_tilde, attr_active);
	CLASS_ATTR_STYLE(c,		"active",		0,	"onoff");

	CLASS_ATTR_LONG(c,		"autoreset",	0,	t_count_tilde, attr_autoreset);
	CLASS_ATTR_STYLE(c,		"autoreset",	0,	"onoff");

	CLASS_ATTR_LONG(c,		"loop",			0,	t_count_tilde, attr_loop);
	CLASS_ATTR_STYLE(c,		"loop",			0,	"onoff");
	
	class_dspinit(c);									// Setup object's class to work with MSP
	class_register(_sym_box, c);
	count_tilde_class = c;
	return 0;
}


/************************************************************************************/
// Object Creation Method

void *count_tilde_new(t_symbol *s, long argc, t_atom *argv)
{
	t_count_tilde *x;

	x = (t_count_tilde *)object_alloc(count_tilde_class); 
	if(x){
		object_obex_store((void *)x, _sym_dumpout, (object *)outlet_new(x,NULL));	// dumpout

		dsp_setup((t_pxobject *)x,1);			// Create object with 1 inlet
		outlet_new((t_object *)x, "signal");	// Create signal outlet
		
		x->attr_low_bound = 0;					// Set the defaults before I go loading in values from the attributes
		x->attr_high_bound = 32000;
		x->attr_active = 1;
		x->attr_autoreset = 0;
		x->attr_loop = 1;
		x->value = 0;
		
		attr_args_process(x, argc, argv); 		//handle attribute args			
	}
	return (x);
}


/************************************************************************************/
// Methods bound to input/inlets

// Method for Assistance Messages
void count_tilde_assist(t_count_tilde *x, void *b, long msg, long arg, char *dst)
{
	if(msg==1) strcpy(dst, "(signal) audio to analyze");					// Inlet
	else if(msg==2) strcpy(dst, "(signal) normalized zero-crossing count");	// Outlet		
}


// Reset the counter
void count_tilde_reset(t_count_tilde *x)
{
	x->value = x->attr_low_bound;
}


void count_tilde_perform64(t_count_tilde *x, t_object *dsp64, double **ins, long numins, double **outs, long numouts, long sampleframes, long flags, void *userparam)
{
	double *in = ins[0];
	double *out = outs[0];
	int n = sampleframes;
	double value;
	
	while (n--) {	
		value = *in++;							// load input sample
		
		count_tilde_activate(x, value != 0);

		if (x->attr_active)		// do the counting				
			x->value++;
		
		if (x->value >= x->attr_high_bound) {		// range check
			if (x->attr_loop == 1)
				x->attr_active = 0;
			else 
				x->value = x->attr_high_bound;
		}

		*out++ = x->value;			// Output stored count value					
	}
}


void count_tilde_dsp64(t_count_tilde *x, t_object *dsp64, short *count, double samplerate, long maxvectorsize, long flags)
{
	if (x->attr_autoreset)
		x->value = x->attr_low_bound;
	object_method(dsp64, gensym("dsp_add64"), (t_object*)x, count_tilde_perform64, 0, NULL);
}


/************************************************************************************/
// Additional functions

// destroy the object - free its memory allocation	
void count_tilde_free(t_count_tilde *x)
{
	dsp_free((t_pxobject *)x);		// the standard MSP deallocation routine - THIS MUST BE FIRST	
}


void count_tilde_activate(t_count_tilde *x, short toggle)
{
	if(toggle != 0){					// if this flagged to turn on
		if(x->attr_active == 0){			// if it was previously turned off
			x->value = x->attr_low_bound;
			x->attr_active = 1;
		}
	}
	else{
		x->attr_active = 0;
	}
}
