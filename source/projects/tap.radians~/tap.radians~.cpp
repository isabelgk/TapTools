/* 
 *	External object for Max/MSP
 *	Copyright © 2003 by Timothy Place
 * 
 *	License: This code is licensed under the terms of the "New BSD License"
 *	http://creativecommons.org/licenses/BSD/
 */

#include "TTClassWrapperMax.h"	
#include "../ttblue/tt_audio_base.h"
#include "../ttblue/tt_audio_signal.h"

static t_class *radians_class;		// Required. Global pointing to this class

// Data Structure for this object
typedef struct _radians{
	t_pxobject		x_obj;
	void 			*radians_out;	// Floating-point outlet
	tt_audio_base	*tt;			// taptools object that will perform the conversions
	long 			radians_mode;	// Current mode
} t_radians;


// Prototypes for methods: need a method for each incoming message type:
void radians_perform64(t_radians *x, t_object *dsp64, double **ins, long numins, double **outs, long numouts, long sampleframes, long flags, void *userparam);										// An MSP Perform (signal) Method
void radians_dsp64(t_radians *x, t_object *dsp64, short *count, double samplerate, long maxvectorsize, long flags);			// ../../../../Jamoma/Core/DSP/library/build/JamomaDSP.dylib Method
void radians_assist(t_radians *x, void *b, long m, long a, char *s);	// Assistance Method
void *radians_new(t_symbol *msg, short argc, t_atom *argv);				// New Object Creation Method
void radians_float(t_radians *x, double value);							// Float method
void radians_mode(t_radians *x, int value);								// "mode" method
void radians_free(t_radians *x);


/************************************************************************************/
// Main() Function

extern "C" int C74_EXPORT main(void)
{
	t_class *c;

	c = class_new("tap.radians~",(method)radians_new, (method)radians_free, sizeof(t_radians), 
		(method)0L, A_GIMME, 0);

		common_symbols_init();														// Initialize TapTools
	class_addmethod(c, (method)radians_dsp64, "dsp64", A_CANT, 0);
    class_addmethod(c, (method)radians_float, 			"float", A_FLOAT, 0L);	// Input as double
    class_addmethod(c, (method)radians_assist, 			"assist", A_CANT, 0L); 
	class_addmethod(c, (method)stdinletinfo,			"inletinfo",	A_CANT, 0);

	CLASS_ATTR_LONG(c,		"mode",		0,	t_radians, radians_mode);
	CLASS_ATTR_ENUMINDEX(c,	"mode",		0,	"hz-to-radians radians-to-hertz degrees-to-radians radians-to-degrees");

	class_dspinit(c);									// Setup object's class to work with MSP
class_register(_sym_box, c); 	radians_class = c;
}


/************************************************************************************/
// Object Creation Method

void *radians_new(t_symbol *msg, short argc, t_atom *argv)
{
	t_atom_long myArg = 0;
	long attrstart;	
	t_radians *x;
	
	attrstart = attr_args_offset(argc, argv);
	if(attrstart && argv)
		atom_arg_getlong(&myArg, 0, attrstart, argv);	// support a normal int argument for bwc	
	
	x = (t_radians *)object_alloc(radians_class);;
	if(x){
		object_obex_store((void *)x, _sym_dumpout, (object *)outlet_new(x,NULL));	// dumpout
		dsp_setup((t_pxobject *)x,1);
		x->radians_out = floatout(x);					// Create a floating-point Outlet
		outlet_new((t_object *)x, "signal");
		
		x->tt = new tt_audio_base;				// Create object for performing radian conversions
		x->tt->set_sr(sys_getsr());
		
		x->radians_mode = myArg;					// default mode

		attr_args_process(x, argc, argv); 			//handle attribute args			
		
	}
	return (x);
}


/************************************************************************************/
// Methods bound to input/inlets

// Method for Assistance Messages
void radians_assist(t_radians *x, void *b, long msg, long arg, char *dst)
{
	if(msg==1) 	// Inlets
		strcpy(dst, "(signal/float) Input");
	else if(msg==2){ // Outlets
		switch(arg){
			case 0: strcpy(dst, "(signal) Output"); break;
			case 1: strcpy(dst, "(double) Output"); break;
		}
	}
}


// mode method
void radians_mode(t_radians *x, int value)
{
	x->radians_mode = value;
}


// method for double input
void radians_float(t_radians *x, double value)
{	
	switch (x->radians_mode) 
	{
		case 0:
			value = x->tt->hertz_to_radians(value);	// hz to radians
			break;
		case 1:
			value = x->tt->radians_to_hertz(value);
			break;
		case 2:
			value = x->tt->degrees_to_radians(value);
			break;
		case 3:
			value = x->tt->radians_to_degrees(value);
			break;
	}
	outlet_float(x->radians_out, value);		// output the result
}


// Memory Deallocation
void radians_free(t_radians *x)
{
	dsp_free((t_pxobject *)x);
	delete x->tt;
}


void radians_perform64(t_radians *x, t_object *dsp64, double **ins, long numins, double **outs, long numouts, long sampleframes, long flags, void *userparam)
{
	double *in, value, *out;
	int n;

	in = ins[0];
	out = outs[0];
	n = sampleframes;
	
	switch(x->radians_mode){
		case 0:
			while(--n){
				value = *++in;										// Input
				//value = value * (3.141593 / (x->radians_sr * 0.5));	// hz to radians
				value = x->tt->hertz_to_radians(value);
				*++out = value;
			}
			break;
		case 1:
			while(--n){
				value = *++in;										// Input
				//value = (value * x->radians_sr) / 6.283186; 		// radians to hz
				value = x->tt->radians_to_hertz(value);
				*++out = value;
			}
			break;
		case 2:
			while(--n){
				value = *++in;										// Input
				//value = (value * 3.141593) / 180.;
				value = x->tt->degrees_to_radians(value);
				*++out = value;
			}
			break;
		case 3:
			while(--n){
				value = *++in;										// Input
				//value = (value * 180.) / 3.141593;
				value = x->tt->radians_to_degrees(value);
				*++out = value;
			}
			break;
	}
out:
	return;
}


void radians_dsp64(t_radians *x, t_object *dsp64, short *count, double samplerate, long maxvectorsize, long flags)
{
	x->tt->set_sr(samplerate);		// update the sample rate
	if (count[0])					// only add to the chain if a signal is connected
		object_method(dsp64, gensym("dsp_add64"), (t_object*)x, radians_perform64, 0, NULL);
}

