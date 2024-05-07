/* 
 *	External object for Max/MSP
 *	Copyright © 2000 by Timothy Place
 * 
 *	License: This code is licensed under the terms of the "New BSD License"
 *	http://creativecommons.org/licenses/BSD/
 */

#include "TTClassWrapperMax.h"			

static t_class *fftnorm_class;			// Required. Global pointing to this class

typedef struct _fftnorm			// Data Structure for this object
{
	t_pxobject	x_obj;			// Header;  Must always be the first field; used by MSP
	int 		s_bins;		    // Value from the typed in argument
	int 		s_bins2;
} t_fftnorm;

// Prototypes for methods: need a method for each incoming message type
void *fftnorm_new(long value);
t_int *fftnorm_perform(t_int *w);
void fftnorm_dsp(t_fftnorm *x, t_signal **sp, short *count);
void fftnorm_assist(t_fftnorm *x, void *b, long m, long a, char *s);

/************************************************************************************/
// Main() Function

extern "C" int C74_EXPORT main(void)
{
	t_class *c;

	c = class_new("tap.fft.normalize~",(method)fftnorm_new, (method)dsp_free, sizeof(t_fftnorm), 
		(method)0L, A_LONG, 0);

	common_symbols_init();
 	class_addmethod(c, (method)fftnorm_dsp, 	"dsp", A_CANT, 0L);		
	class_addmethod(c, (method)fftnorm_assist, 	"assist", A_CANT, 0L); 
	class_addmethod(c, (method)stdinletinfo,	"inletinfo",	A_CANT, 0);

	class_dspinit(c);
	class_register(_sym_box, c);
	fftnorm_class = c;
}


/************************************************************************************/
// Object Creation Method

void *fftnorm_new(long value)
{
	t_fftnorm *x = (t_fftnorm *)object_alloc(fftnorm_class);;
	if(x){
    	object_obex_store((void *)x, _sym_dumpout, (object *)outlet_new(x,NULL));	// dumpout		
		dsp_setup((t_pxobject *)x, 3);				// Create Object and 2 Inlets
		outlet_new((t_object *)x, "signal");		// Create a signal outlet 
		outlet_new((t_object *)x, "signal");		// Create a signal outlet 
		x->s_bins = value;							// Number of bins in analysis
		//attr_args_process(x,argc,argv);					//handle attribute args			
	}
	return x;										// Return pointer
}


/************************************************************************************/
// Methods bound to input/inlets


// Method for Assistance Messages
void fftnorm_assist(t_fftnorm *x, void *b, long msg, long arg, char *dst)
{
	if(msg==1){ 	// Inlets
		switch(arg){
			case 0: strcpy(dst, "(signal) Real input from FFT"); break;
			case 1: strcpy(dst, "(signal) Imaginary input from FFT"); break;
			case 2: strcpy(dst, "(signal) Index from FFT"); break;
		}
	}
	else if(msg==2){ // Outlets
		switch(arg){
			case 0: strcpy(dst, "(signal) Normalized real output"); break;
			case 1: strcpy(dst, "(signal) Normalized imaginary output"); break;
		}
	}
}


// Perform (signal) Method
t_int *fftnorm_perform(t_int *w)
{
	t_float *in = (t_float *)(w[1]);			// Real
	t_float *in2 = (t_float *)(w[2]);		// Imaginary
	t_float *in3 = (t_float *)(w[3]);		// Index
	t_float *out = (t_float *)(w[4]);		// Real Out
	t_float *out2 = (t_float *)(w[5]);		// Imaginary Out		
   	t_fftnorm *x = (t_fftnorm *)(w[6]);		//  Object Pointer
	int n = (int)(w[7]);					// "Vector" Size
	double val;
	double val2;
	double index;
 	int bins = x->s_bins - 1;
 
	while (--n)
	{
		val = *++in;
		val2 = *++in2;
		index = *++in3;	
		index = index + 0.49; 
		
		val = val / x->s_bins2;
		val2 = -val2 / x->s_bins2;
		
		if (index == 0 || index == bins)
			val = val * 0.5;

		*++out = val;
		*++out2 = val2;
		
	}
	return (w + 8);						// return pointer to NEXT object in dsp chain
}


// ../../../../Jamoma/Core/DSP/library/build/JamomaDSP.dylib Method
void fftnorm_dsp(t_fftnorm *x, t_signal **sp, short *count)
{
	x->s_bins2 = x->s_bins / 2;
	dsp_add(fftnorm_perform, 7, sp[0]->s_vec-1, sp[1]->s_vec-1, sp[0]->s_vec-1, 
		sp[1]->s_vec-1, sp[1]->s_vec-1, x, sp[0]->s_n+1);	// Add Perform (signal) Method to the ../../../../Jamoma/Core/DSP/library/build/JamomaDSP.dylib Call Chain
}