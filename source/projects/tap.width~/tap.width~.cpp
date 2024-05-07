/* 
 *	External object for Max/MSP
 *	Copyright © 2002 by Timothy Place
 * 
 *	License: This code is licensed under the terms of the "New BSD License"
 *	http://creativecommons.org/licenses/BSD/
 */

#include "TTClassWrapperMax.h"				


typedef struct _width {
	t_pxobject		x_obj;
	t_symbol		*attr_mode;
	int				sr;					// current sampling rate
	double			width_width;		// pulse width in samples
	double			width_counter;		// counter storage
} t_width;


// Prototypes
void *width_new(t_symbol *msg, short argc, t_atom *argv);
void width_perform64(t_width *x, t_object *dsp64, double **ins, long numins, double **outs, long numouts, long sampleframes, long flags, void *userparam);
void width_dsp64(t_width *x, t_object *dsp64, short *count, double samplerate, long maxvectorsize, long flags);
void width_assist(t_width *x, void *b, long m, long a, char *s);


// Globals
static t_class *width_class;


/************************************************************************************/
// Main() Function

extern "C" int C74_EXPORT main(void)
{
	t_class *c;
		
	c = class_new("tap.width~",(method)width_new, (method)dsp_free, sizeof(t_width), (method)0L, A_GIMME, 0);
	
	
	common_symbols_init();

	class_addmethod(c, (method)width_dsp64,		"dsp64", A_CANT, 0);
    class_addmethod(c, (method)width_assist,	"assist", A_CANT, 0L); 
  	class_addmethod(c, (method)stdinletinfo,	"inletinfo",	A_CANT, 0);
  
	class_dspinit(c);
class_register(_sym_box, c); 	width_class = c;
}


/************************************************************************************/
// Object Creation Method

void *width_new(t_symbol *msg, short argc, t_atom *argv)
{
	t_width *x;

	if (x = (t_width *)object_alloc(width_class)) {
		object_obex_store((void *)x, _sym_dumpout, (object *)outlet_new(x,NULL));	// dumpout
		dsp_setup((t_pxobject *)x,1);				// Create Object and 1 Inlet (last argument)
		outlet_new((t_pxobject *)x, "signal");		// Create a signal Outlet
		x->width_counter = 0;						// init the counter
		x->width_width = 0;							// Init the width
		x->attr_mode = _sym_nothing;	// init mode
		attr_args_process(x, argc, argv);			// handle attribute args
	}	
	return (x);										// Return the pointer
}


/************************************************************************************/
// Methods bound to input/inlets

// Method for Assistance Messages
void width_assist(t_width *x, void *b, long msg, long arg, char *dst)
{
	if (msg==1) 		// Inlet
		strcpy(dst, "(signal) Input To Be Tested");
	else if (msg==2) 	// Outlet
		strcpy(dst, "(signal) Pulse Width in ms");
}


void width_perform64(t_width *x, t_object *dsp64, double **ins, long numins, double **outs, long numouts, long sampleframes, long flags, void *userparam)
{
	double *in = ins[0];
	double *out = outs[0];
	int n = sampleframes;
	double xx;							// (used to carry input)
	double counter = x->width_counter;		// storage of the sample-count
	double width = x->width_width;			// storage of the width

	while (n--) {
		xx = *in++;								// Input
		if (xx > 0)						
			counter++;							// If the Input is above zero, up the count
		else if (xx <= 0 && counter != 0) {	
			width = (1000 * counter) / x->sr;	// If the input falls below zero, update the width with the high count (in ms)
			counter = 0;						// 	... and reset the counter for when it goes back above zero
		}	
		*out++ = width;							// Output
	}	

	x->width_width = width;				// Store Feedback Sample
	x->width_counter = counter;
}


void width_dsp64(t_width *x, t_object *dsp64, short *count, double samplerate, long maxvectorsize, long flags)
{
	x->sr = samplerate;
	object_method(dsp64, gensym("dsp_add64"), (t_object*)x, width_perform64, 0, NULL);
}
