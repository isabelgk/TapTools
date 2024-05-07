/* 
 *	External object for Max/MSP
 *	Copyright © 2003 by Timothy Place
 * 
 *	License: This code is licensed under the terms of the "New BSD License"
 *	http://creativecommons.org/licenses/BSD/
 */

#include "TTClassWrapperMax.h"


typedef struct _counter{			// Data Structure for this object
	t_pxobject	ob;					// Required by MSP (must be first)
	long 		low_bound;			// ATTRIBUTE: low_bound 
	long		high_bound;			// ATTRIBUTE: high_bound
	t_symbol	*direction;			// ATTRIBUTE: direction
	long		init_value;			// ATTRIBUTE: init_value
	long 		stored_value;		// storage for the current count
	double		last_sample;		// storage for the last input sample (we're looking for transitions)
} t_counter;

// Prototypes for methods: need a method for each incoming message type:
void counter_dsp(t_counter *x, t_signal **sp, short *count);
void counter_dsp64(t_counter *x, t_object *dsp64, short *count, double samplerate, long maxvectorsize, long flags);			// ../../../../Jamoma/Core/DSP/library/build/JamomaDSP.dylib Method
void counter_assist(t_counter *x, void *b, long m, long a, char *s);	// Assistance Method
void *counter_new(t_symbol *s, long argc, t_atom *argv);				// New Object Creation Method
void counter_reset(t_counter *x);
void counter_set(t_counter *x, long value);
t_int *counter_perform(t_int *w);
void counter_perform64(t_counter *x, t_object *dsp64, double **ins, long numins, double **outs, long numouts, long sampleframes, long flags, void *userparam);
void counter_free(t_counter *x);

// Class Globals
static t_class *counter_class;				// Required. Global pointing to this class
static t_symbol *ps_up;
static t_symbol *ps_down;


/************************************************************************************/
// Main() Function

extern "C" int C74_EXPORT main(void)
{
	t_class *c;

	c = class_new("tap.counter~",(method)counter_new, (method)dsp_free, sizeof(t_counter), 
		(method)0L, A_GIMME, 0);

	common_symbols_init();
	class_addmethod(c, (method)counter_set,				"set", A_LONG, 0L);
	class_addmethod(c, (method)counter_reset,			"reset", 0L);
 	class_addmethod(c, (method)counter_dsp, 			"dsp", A_CANT, 0L);		
	class_addmethod(c, (method)counter_dsp64, "dsp64", A_CANT, 0);
	class_addmethod(c, (method)counter_assist, 			"assist", A_CANT, 0L); 
    class_addmethod(c, (method)object_obex_dumpout, 	"dumpout", A_CANT,0);      
	class_addmethod(c, (method)stdinletinfo,			"inletinfo",	A_CANT, 0);

	CLASS_ATTR_LONG(c,		"low_bound",	0,	t_counter, low_bound);
	CLASS_ATTR_LONG(c,		"high_bound",	0,	t_counter, high_bound);
	
	CLASS_ATTR_SYM(c,		"direction",	0,	t_counter, direction);
	CLASS_ATTR_ENUM(c,		"direction",	0,	"up down");

	CLASS_ATTR_LONG(c,		"init_value",	0,	t_counter, init_value);

	class_dspinit(c);
	class_register(_sym_box, c);
	counter_class = c;

	ps_up = gensym("up");
	ps_down = gensym("down");
	return 0;
}


/************************************************************************************/
// Object Creation Method

void *counter_new(t_symbol *s, long argc, t_atom *argv)
{
	t_counter *x;

	x = (t_counter *)object_alloc(counter_class);
	if(x){
    	object_obex_store((void *)x, _sym_dumpout, (object *)outlet_new(x,NULL));	// dumpout	
		dsp_setup((t_pxobject *)x,1);							// Create object with 1 inlet
		outlet_new((t_object *)x, "signal");					// Create signal outlet
		
		x->init_value = 0;										// Set the default before I go loading in values from the attributes
		x->direction = ps_up;
		x->last_sample = 0.0;
		x->stored_value = 0;
		x->low_bound = 0;
		x->high_bound = 2000000;

		attr_args_process(x,argc,argv);					//handle attribute args							  		
		x->stored_value = x->init_value;
	}
	return (x);
}


/************************************************************************************/
// Methods bound to input/inlets

// Method for Assistance Messages
void counter_assist(t_counter *x, void *b, long msg, long arg, char *dst)
{
	if(msg==1) strcpy(dst, "(signal) audio to analyze");					// Inlet
	else if(msg==2) strcpy(dst, "(signal) normalized zero-crossing count");	// Outlet		
}

// Reset the counter
void counter_reset(t_counter *x)
{
	x->stored_value = x->init_value;
}

// Set the counter
void counter_set(t_counter *x, long value)
{
	x->stored_value = value;
}


// Perform (signal) Method
t_int *counter_perform(t_int *w)
{
	t_counter *x = (t_counter *)(w[1]);		// Pointer	
	t_float *in = (t_float *)(w[2]);		// Inlet
	t_float *out = (t_float *)(w[3]);		// Outlet
	int n = (int)(w[4]);					// Vector Size
	double value;
//	short b_last, b_current;
	long temp;
	
	if (x->ob.z_disabled) goto out;		
	
	while (n--){	
		value = *in++;						// load input sample
		if((value != 0.0) && (x->last_sample == 0.0)){
			if(x->direction == ps_up){
				x->stored_value = x->stored_value + 1;
				temp = x->stored_value;
				if(temp > (x->high_bound))
					temp = x->low_bound;			
				x->stored_value = temp;
			}
			else if(x->direction == ps_down){
				x->stored_value = x->stored_value - 1;
				temp = x->stored_value;
				if(temp < (x->low_bound))
					temp = x->high_bound;	
				x->stored_value = temp;
			}
		}
		x->last_sample = value;				// store the sample as the last sample (for the next time)
		*out++ = (double)(x->stored_value);	// Output
	}

out:
    return (w+5);
}


void counter_perform64(t_counter *x, t_object *dsp64, double **ins, long numins, double **outs, long numouts, long sampleframes, long flags, void *userparam)
{
	
	double *in = ins[0];
	double *out = outs[0];
	int n = sampleframes;
	double value;
//	short b_last, b_current;
	long temp;
	
	if (x->ob.z_disabled) goto out;		
	
	while (n--){	
		value = *in++;						// load input sample
		if((value != 0.0) && (x->last_sample == 0.0)){
			if(x->direction == ps_up){
				x->stored_value = x->stored_value + 1;
				temp = x->stored_value;
				if(temp > (x->high_bound))
					temp = x->low_bound;			
				x->stored_value = temp;
			}
			else if(x->direction == ps_down){
				x->stored_value = x->stored_value - 1;
				temp = x->stored_value;
				if(temp < (x->low_bound))
					temp = x->high_bound;	
				x->stored_value = temp;
			}
		}
		x->last_sample = value;				// store the sample as the last sample (for the next time)
		*out++ = (double)(x->stored_value);	// Output
	}

out:
    return;
}



// ../../../../Jamoma/Core/DSP/library/build/JamomaDSP.dylib Method
void counter_dsp(t_counter *x, t_signal **sp, short *count)
{
	dsp_add(counter_perform, 4, x, sp[0]->s_vec, sp[1]->s_vec, sp[0]->s_n);
}


void counter_dsp64(t_counter *x, t_object *dsp64, short *count, double samplerate, long maxvectorsize, long flags)
{
	object_method(dsp64, gensym("dsp_add64"), (t_object*)x, counter_perform64, 0, NULL);
}

