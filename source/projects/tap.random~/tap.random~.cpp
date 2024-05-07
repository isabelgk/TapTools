/* 
 *	External object for Max/MSP
 *	Copyright © 2003 by Timothy Place
 * 
 *	License: This code is licensed under the terms of the "New BSD License"
 *	http://creativecommons.org/licenses/BSD/
 */

#include "TTClassWrapperMax.h"


typedef struct _rand_tilde{				// Data Structure for this object
	t_pxobject ob;					// Required by MSP (must be first)

	double 		rsize;				// reciprocal of size
	int			size;				// desired analysis size
	int     	rand_tilde;             	// zero crossing counter-upper
	int     	i;					// incrementer that counts to numframes 
	int			sr;					// current sampling rate
	double		final_count;
	
	double 		stored_sample;		// previous input sample	
	double		value;				// stored random value
	double		high_bound;			// ATTRIBUTE
	double		low_bound;			// ATTRIBUTE
} t_rand_tilde;


// Prototypes for methods: need a method for each incoming message type:
void rand_tilde_dsp64(t_rand_tilde *x, t_object *dsp64, short *count, double samplerate, long maxvectorsize, long flags);			// ../../../../Jamoma/Core/DSP/library/build/JamomaDSP.dylib Method
void rand_tilde_assist(t_rand_tilde *x, void *b, long m, long a, char *s);	// Assistance Method
void *rand_tilde_new(t_symbol *s, long argc, t_atom *argv);				// New Object Creation Method
void rand_tilde_perform64(t_rand_tilde *x, t_object *dsp64, double **ins, long numins, double **outs, long numouts, long sampleframes, long flags, void *userparam);
void rand_tilde_free(t_rand_tilde *x);
void rand_tilde_gen(t_rand_tilde *x);


static t_class *rand_tilde_class;					// Required. Global pointing to this class


/************************************************************************************/
// Main() Function

extern "C" int C74_EXPORT main(void)
{
	t_class *c;
	
	c = class_new("tap.random~",(method)rand_tilde_new, (method)rand_tilde_free, sizeof(t_rand_tilde), (method)0L, A_GIMME, 0);

		
	common_symbols_init();
		
	class_addmethod(c, (method)rand_tilde_dsp64,	"dsp64",		A_CANT, 0);
    class_addmethod(c, (method)rand_tilde_assist, 	"assist",		A_CANT, 0L); 
 	class_addmethod(c, (method)stdinletinfo,		"inletinfo",	A_CANT, 0);

 	CLASS_ATTR_DOUBLE(c,		"high_bound",		0,	t_rand_tilde, high_bound);
	CLASS_ATTR_DOUBLE(c,		"low_bound",		0,	t_rand_tilde, low_bound);

	class_dspinit(c);
class_register(_sym_box, c); 	rand_tilde_class = c;
}


/************************************************************************************/
// Object Creation Method

void *rand_tilde_new(t_symbol *s, long argc, t_atom *argv)
{
	t_rand_tilde *x;

	x = (t_rand_tilde *)object_alloc(rand_tilde_class);; 
	if(x){
		object_obex_store((void *)x, _sym_dumpout, (object *)outlet_new(x,NULL));	// dumpout

		dsp_setup((t_pxobject *)x,1);				// Create object with 1 inlet
		outlet_new((t_object *)x, "signal");		// Create signal outlet
		
	//	x->size = 0;								// Set the default before I go loading in values from the attributes
		x->low_bound = -1.0;
		x->high_bound = 1.0;

		attr_args_process(x, argc, argv); 			//handle attribute args			
				  		
		x->stored_sample = 0;
		rand_tilde_gen(x);
	}
	return (x);
}


/************************************************************************************/
// Methods bound to input/inlets

// Method for Assistance Messages
void rand_tilde_assist(t_rand_tilde *x, void *b, long msg, long arg, char *dst)
{
	if (msg==1)
		strcpy(dst, "(signal) audio to analyze");					// Inlet
	else if (msg==2)
		strcpy(dst, "(signal) normalized zero-crossing count");		// Outlet		
}


void rand_tilde_perform64(t_rand_tilde *x, t_object *dsp64, double **ins, long numins, double **outs, long numouts, long sampleframes, long flags, void *userparam)
{
	double *in = ins[0];
	double *out = outs[0];
	int n = sampleframes;
	double value=0;
	short b_last, b_current;
	
	b_last = (x->stored_sample == 0); 			// contains whether or not the last sample was 0
	
	while (n--){	
		value = *in++;							// load input sample
		b_current = (value != 0);
		
		if((b_last) && (b_current))
			rand_tilde_gen(x);

		*out++ = x->value;						// Output stored random value
	}
	x->stored_sample = value;
out:
    return;
}


// ../../../../Jamoma/Core/DSP/library/build/JamomaDSP.dylib Method
void rand_tilde_dsp64(t_rand_tilde *x, t_object *dsp64, short *count, double samplerate, long maxvectorsize, long flags)
{
	x->stored_sample = 0;	// Initialize stored sample for delta analysis algorithm
	x->sr = samplerate;	// store the sampling rate
   
	object_method(dsp64, gensym("dsp_add64"), (t_object*)x, rand_tilde_perform64, 0, NULL);
}


/************************************************************************************/
// Additional functions

// destroy the object - free its memory allocation	
void rand_tilde_free(t_rand_tilde *x)
{
	dsp_free((t_pxobject *)x);		// the standard MSP deallocation routine - THIS MUST BE FIRST	
}


// generate and store a random number
void rand_tilde_gen(t_rand_tilde *x)
{
	int 	i;
	double	toast;
	
	int randx = rand();
	i = ((randx = randx*1103515245 + 12345)>>16) & 077777;
	toast = (double)i/32768.0;
	toast = (toast * (x->high_bound - x->low_bound)) + x->low_bound;
	x->value = toast;
}
