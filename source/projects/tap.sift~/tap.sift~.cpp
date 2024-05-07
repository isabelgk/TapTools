/* 
 *	External object for Max/MSP
 *	Copyright © 2000 by Timothy Place
 * 
 *	License: This code is licensed under the terms of the "New BSD License"
 *	http://creativecommons.org/licenses/BSD/
 */

#include "TTClassWrapperMax.h"			

#define QMAX 100							// Number of slots in the queue

static t_class *sift_class;						// Required. Global pointing to this class

// Data Structure for this object
typedef struct _sift{
	t_pxobject 	x_obj;						// Header;  Must always be the first field; used by MSP
	long 		sift_value;					// Value from the typed in argument
   	double 		sift_hold;					// Last value output
   	void 		*sift_out;					// Pointer to outlet. need one for each outlet 
   	long 		sift_mode;					// signal or double mode
   	void		*sift_clock;				// Pointer to the clock object
   	double 		sift_queuea[QMAX];	
  	long 		sift_qhead, sift_qtail;
} t_sift;

// Prototypes for methods: need a method for each incoming message type
void *sift_new(t_symbol *msg, short argc, t_atom *argv);		// New Object Creation Method
void sift_perform264(t_sift *x, t_object *dsp64, double **ins, long numins, double **outs, long numouts, long sampleframes, long flags, void *userparam);
void sift_perform64(t_sift *x, t_object *dsp64, double **ins, long numins, double **outs, long numouts, long sampleframes, long flags, void *userparam);
void sift_dsp64(t_sift *x, t_object *dsp64, short *count, double samplerate, long maxvectorsize, long flags);
void sift_assist(t_sift *x, void *b, long m, long a, char *s);	// Assistance Method
void sift_free(t_sift *x);										// Release Memory
void sift_output (t_sift *x);									// Scheduled floating-point output

/************************************************************************************/
// Main() Function

extern "C" int C74_EXPORT main(void)
{
	t_class *c;

	c = class_new("tap.sift~",(method)sift_new, (method)sift_free, sizeof(t_sift), 
		(method)0L, A_GIMME, 0);

		common_symbols_init();									// Initialize TapTools

	class_addmethod(c, (method)sift_dsp64,		"dsp64", A_CANT, 0);
	class_addmethod(c, (method)sift_assist, 	"assist", A_CANT, 0L); 
	class_addmethod(c, (method)stdinletinfo,	"inletinfo",	A_CANT, 0);

	class_dspinit(c);
class_register(_sym_box, c); 	sift_class = c;
}


/************************************************************************************/
// Object Creation Method

void *sift_new(t_symbol *msg, short argc, t_atom *argv)
{
	double	value = 0;
	long	mode = 0;
	long	attrstart;
	int i;
	t_sift *x;

	attrstart = attr_args_offset(argc, argv);
	if(attrstart && argv){
		value = atom_getlong(argv);	// support a normal int argument for bwc
		if(attrstart-1){
			mode = atom_getlong(argv+1);	// support a normal int argument for bwc
		}
	}

	x = (t_sift *)object_alloc(sift_class);;
	if(x){
    	object_obex_store((void *)x, _sym_dumpout, (object *)outlet_new(x,NULL));	// dumpout	
		dsp_setup((t_pxobject *)x, 1);						// Create Object and 1 Inlet
		if (mode == 0) x->sift_out = floatout(x);				// Create a floating-point Outlet	
	 	else outlet_new((t_pxobject *)x, "signal");				// OR create a signal Outlet   
	 	x->sift_clock = clock_new(x, (method)sift_output);		// Create the queue clock
		
		x->sift_value = value;								// Init the typed in argument
		x->sift_hold = 0.;								// Init hold variable
		x->sift_mode = mode;								// Set the mode flag
		x->sift_qhead = x->sift_qtail = 0;					// init the the head and tail locations
		for(i=0;i<QMAX;i++) x->sift_queuea[i] = 0.;			// init the queue

		attr_args_process(x,argc,argv); 					//handle attribute args		
	}
	return x;										// Return pointer
}


/************************************************************************************/
// Methods bound to input/inlets

// Method for Assistance Messages
void sift_assist(t_sift *x, void *b, long msg, long arg, char *dst)
{
	if(msg==1) 	// Inlets
		strcpy(dst, "(signal) To be sifted away");
	else if(msg==2){ // Outlets
		if(x->sift_mode == 0) strcpy(dst, "(double) remaining sample values");	
		else strcpy(dst, "(signal) remaining sample values");	
	}	
}


// Memory Deallocation
void sift_free(t_sift *x)
{
	dsp_free((t_pxobject *)x);	// regular MSP free routine	 - MUST BE FIRST
	clock_unset(x->sift_clock);	// remove the clock routine from the scheduler
	clock_free((t_object *)x->sift_clock);		// free the clock memory
}


// Float Output
void sift_output (t_sift *x)
{
	long head, tail;
	double q[QMAX];
	int i;
	
	head = x->sift_qhead;
	tail = x->sift_qtail;
	for (i=0;i<QMAX;i++) q[i] = x->sift_queuea[i];
	
	while(tail != head){
		outlet_float(x->sift_out, q[tail]);			// Spit out the sample value
		tail++;
		if (tail >= QMAX) tail = 0;
	}
	x->sift_qtail = tail;	
}


void sift_perform64(t_sift *x, t_object *dsp64, double **ins, long numins, double **outs, long numouts, long sampleframes, long flags, void *userparam)
{
	double *in = ins[0];
   	
	int n = sampleframes;
	double value;										// Variable for temporary storage
 	long qhead = x->sift_qhead;
 	int changeFlag = 0;
 	
	while (--n){
		value = *++in;
		if ((value != x->sift_value) && (value != x->sift_hold)){ 	// If sample is NOT argument && Not the same as the last sample
			x->sift_queuea[qhead] = value;					
			qhead++;
			if(qhead >= QMAX) qhead = 0;
			changeFlag = 1;
		}
		x->sift_hold = value;							// Store the output for future comparisons
	}
	if(changeFlag == 1){
		x->sift_qhead = qhead;
		clock_delay(x->sift_clock, 0);					// Set clock to go off ASAP
	}
}


void sift_perform264(t_sift *x, t_object *dsp64, double **ins, long numins, double **outs, long numouts, long sampleframes, long flags, void *userparam)
{
	double *in = ins[0];
	double *out = outs[0];
   	
	int n = sampleframes;
	double value;						// Variable for temporary storage
 
	while (--n){
		value = *++in;
		if (value != x->sift_value) {		// If sample is NOT argument
			*++out = value;	
			x->sift_hold = value;
		}
		else
			*++out = x->sift_hold;		// Store the output for future comparisons
	}
}


void sift_dsp64(t_sift *x, t_object *dsp64, short *count, double samplerate, long maxvectorsize, long flags)
{
	if (x->sift_mode == 0) 
		object_method(dsp64, gensym("dsp_add64"), (t_object*)x, sift_perform64, 0, NULL);	// Float Output Mode	
	else 
		object_method(dsp64, gensym("dsp_add64"), (t_object*)x, sift_perform264, 0, NULL);
}
