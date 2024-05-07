/* 
 *	External object for Max/MSP
 *	Copyright © 2001 by Timothy Place
 * 
 *	License: This code is licensed under the terms of the "New BSD License"
 *	http://creativecommons.org/licenses/BSD/
 */

#include "TTClassWrapperMax.h"			

static t_class *split_class;		// Required. Global pointing to this class

typedef struct _split	// Data Structure for this object
{
    t_pxobject 	x_obj;	// Header;  Must always be the first field; used by MSP
	double 		s_low;		// low value from the typed in argument
	double 		s_high;		// high value from the typed in argument
	short 		s_connect[6];	// array of inlet and outlet connections
} t_split;

// Prototypes for methods: need a method for each incoming message type
void *split_new(t_symbol *msg, short argc, t_atom *argv);			// New Object Creation Method
void split_perform164(t_split *x, t_object *dsp64, double **ins, long numins, double **outs, long numouts, long sampleframes, long flags, void *userparam);									// An MSP Perform (signal) Method
void split_perform364(t_split *x, t_object *dsp64, double **ins, long numins, double **outs, long numouts, long sampleframes, long flags, void *userparam);									// An MSP Perform (signal) Method
void split_dsp64(t_split *x, t_object *dsp64, short *count, double samplerate, long maxvectorsize, long flags);			// ../../../../Jamoma/Core/DSP/library/build/JamomaDSP.dylib Method
void split_assist(t_split *x, void *b, long m, long a, char *s);	// Assistance Method
void split_int(t_split *x, long n);
void split_float(t_split *x, double val);


/************************************************************************************/
// Main() Function

extern "C" int C74_EXPORT main(void)
{
	t_class *c;

	c = class_new("tap.split~",(method)split_new, (method)dsp_free, sizeof(t_split), 
		(method)0L, A_GIMME, 0);

		common_symbols_init();									// Initialize TapTools
	class_addmethod(c, (method)split_int,		"int", A_LONG, 0L);
	class_addmethod(c, (method)split_float,		"float", A_FLOAT, 0L);
	class_addmethod(c, (method)split_dsp64,		"dsp64", A_CANT, 0);
	class_addmethod(c, (method)split_assist, 	"assist", A_CANT, 0L); 
	class_addmethod(c, (method)stdinletinfo,	"inletinfo",	A_CANT, 0);

	class_dspinit(c);
class_register(_sym_box, c); 	split_class = c;
}


/************************************************************************************/
// Object Creation Method

void *split_new(t_symbol *msg, short argc, t_atom *argv)
{
	t_split *x;
	double	low=0;
	double	high=1;
	long	attrstart;
	
	attrstart = attr_args_offset(argc, argv);
	if(attrstart && argv){
		low = atom_getfloat(argv);	// support a normal int argument for bwc
		if(attrstart-1){
			high = atom_getfloat(argv+1);	// support a normal int argument for bwc
		}
	}

	x = (t_split *)object_alloc(split_class);;
	if(x){
    	object_obex_store((void *)x, _sym_dumpout, (object *)outlet_new(x,NULL));	// dumpout	
		dsp_setup((t_pxobject *)x, 3);			// Create Object with 3 Inlets (input, low-test, high-test)
		outlet_new((t_object *)x, "signal");	// Create a signal outlet: values w/in range
	  	outlet_new((t_object *)x, "signal");	// Create a signal outlet: values out-of-range
	 	outlet_new((t_object *)x, "signal");	// Create a signal outlet: true/false
	 	x->s_low = low;							// Init the typed in arguments...
		x->s_high = high;

		attr_args_process(x,argc,argv); 		//handle attribute args		
	}
	return x;									// Return pointer
}


/************************************************************************************/
// Methods bound to input/inlets

// Method for Assistance Messages
void split_assist(t_split *x, void *b, long msg, long arg, char *dst)
{
	if(msg==1){ 	// Inlets
		switch(arg){
			case 0: strcpy(dst, "(signal) Value to be sent to one of the 2 outlets"); break;
			case 1: strcpy(dst, "(signal/float) Set lower limit for left outlet"); break;
			case 2: strcpy(dst, "(signal/float) Set upper limit for left outlet"); break;
		}
	}
	else if(msg==2){ // Outlets
		switch(arg){
			case 0: strcpy(dst, "(signal) Input if within limits"); break;
			case 1: strcpy(dst, "(signal) Input if not within limits"); break;
			case 2: strcpy(dst, "(signal) Comparison result (1 or 0)"); break;
		}
	}
}


// Method for Int input
void split_int(t_split *x, long n)
{
	split_float(x,(double)n);	// cast as double-precision-float and pass off the data
}


// Method for double input
void split_float(t_split *x, double val)
{
	switch (x->x_obj.z_in) 
	{
		case 1:
			x->s_low = val;
			break;
		case 2:
			x->s_high = val;
			break;
	}
}


void split_perform364(t_split *x, t_object *dsp64, double **ins, long numins, double **outs, long numouts, long sampleframes, long flags, void *userparam)
{
	double *in1 = ins[0];		// Input 1
//	double *in2 = ins[1];		// Input 2
//	double *in3 = ins[2];		// Input 3
	double *out1 = outs[0];	// Output 1
	double *out2 = outs[1];	// Output 2
	double *out3 = outs[2];	// Output 3    
   	
	int n = sampleframes;
	double value, low, high;			//  Variable for temporary storage
 
 	low = x->s_connect[1]? *ins[1] : x->s_low;
	high = x->s_connect[2]? *ins[2] : x->s_high;
 
	while (n--){
		value = *in1++;
		if ((value >= low) && (value <= high)){	
			*out1++ = value;
			*out2++ = 0;
			*out3++ = 1;
		}else{
			*out1++ = 0;
			*out2++ = value;
			*out3++ = 0; 	 	
		}											
	}
	return;
}


void split_perform164(t_split *x, t_object *dsp64, double **ins, long numins, double **outs, long numouts, long sampleframes, long flags, void *userparam)
{
	double *in1 = ins[0];		// Input 1
	double *out1 = outs[0];	// Output 1
	double *out2 = outs[1];	// Output 2
	double *out3 = outs[2];	// Output 3    
   	
	int n = sampleframes;
	double value, low, high;			// Variables for temporary storage
 
 	low = x->s_low;
	high = x->s_high;
 
	while (n--){
		value = *in1++;
		if ((value >= low) && (value <= high)){	
			*out1++ = value;
			*out2++ = 0;
			*out3++ = 1;
		}else{
			*out1++ = 0;
			*out2++ = value;
			*out3++ = 0; 	 	
		}											
	}
    return;
}


void split_dsp64(t_split *x, t_object *dsp64, short *count, double samplerate, long maxvectorsize, long flags)
{
	short i;
	
	for(i=0; i<6; i++)
		x->s_connect[i] = count[i];

	if (count[1] || count[2])	// IF the 2nd or 3rd inlet has a signal connected to it...
		object_method(dsp64, gensym("dsp_add64"), (t_object*)x, split_perform364, 0, NULL);
	else	
		object_method(dsp64, gensym("dsp_add64"), (t_object*)x, split_perform164, 0, NULL);
}
