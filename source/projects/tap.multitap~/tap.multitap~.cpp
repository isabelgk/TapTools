/* 
 *	External object for Max/MSP
 *	Copyright © 2004 by Timothy Place
 * 
 *	License: This code is licensed under the terms of the "New BSD License"
 *	http://creativecommons.org/licenses/BSD/
 */

#include "TTClassWrapperMax.h"
#include "../ttblue/tt_audio_base.h"
#include "../ttblue/tt_audio_signal.h"
#include "../ttblue/tt_multitap.h"

#define MAX_NUM_TAPS 100
#define NUM_INPUTS 1
#define NUM_OUTPUTS 1

static t_class *multitap_class;					// Required. Global pointing to this class

typedef struct _multitap{					// Data Structure for this object
    t_pxobject 		x_obj;
	tt_multitap		*audio_delay;
    tt_audio_signal	*signal_in[NUM_INPUTS];
    tt_audio_signal	*signal_out[NUM_OUTPUTS];
	long			attr_taps;
	double			attr_delay[MAX_NUM_TAPS];
	long			attr_delay_len;
	double			attr_gain[MAX_NUM_TAPS];
	long			attr_gain_len;
	std::vector<double>	input_buffer;
	std::vector<double>	output_buffer;
} t_multitap;


// Prototypes for methods: need a method for each incoming message type
void *multitap_new(t_symbol *msg, short argc, t_atom *argv);				// New Object Creation Method
void multitap_perform64(t_multitap *x, t_object *dsp64, double **ins, long numins, double **outs, long numouts, long sampleframes, long flags, void *userparam);
void multitap_dsp64(t_multitap *x, t_object *dsp64, short *count, double samplerate, long maxvectorsize, long flags);
void multitap_assist(t_multitap *x, void *b, long msg, long arg, char *dst);// Assistance Method
void multitap_free(t_multitap *x);
t_max_err attr_set_taps(t_multitap *x, void *attr, long argc, t_atom *argv);
t_max_err attr_set_delay(t_multitap *x, void *attr, long argc, t_atom *argv);
t_max_err attr_set_gain(t_multitap *x, void *attr, long argc, t_atom *argv);


/************************************************************************************/
// Main() Function

extern "C" int C74_EXPORT main(void)
{
	t_class *c;
	
	c = class_new("tap.multitap~",(method)multitap_new, (method)multitap_free, sizeof(t_multitap), 
		(method)0L, A_GIMME, 0);

		
	common_symbols_init();
 	
	class_addmethod(c, (method)multitap_dsp64,			"dsp64", A_CANT, 0);
	class_addmethod(c, (method)multitap_assist, 		"assist", A_CANT, 0L); 
	class_addmethod(c, (method)stdinletinfo,			"inletinfo",	A_CANT, 0);

	CLASS_ATTR_LONG(c,			"taps",		0,	t_multitap, attr_taps);
	CLASS_ATTR_ACCESSORS(c,		"taps",		NULL, attr_set_taps);

	CLASS_ATTR_DOUBLE_VARSIZE(c,	"delay",	0,	t_multitap, attr_delay, attr_delay_len, MAX_NUM_TAPS);
	CLASS_ATTR_ACCESSORS(c,		"delay",	NULL, attr_set_delay);

	CLASS_ATTR_DOUBLE_VARSIZE(c,	"gain",		0,	t_multitap, attr_gain, attr_gain_len, MAX_NUM_TAPS);
	CLASS_ATTR_ACCESSORS(c,		"gain",		NULL, attr_set_gain);

	class_dspinit(c);									// Setup object's class to work with MSP
class_register(_sym_box, c); 	multitap_class = c;
}


/************************************************************************************/
// Object Creation Method

void *multitap_new(t_symbol *msg, short argc, t_atom *argv)
{
    t_multitap	*x;
    long		attrstart;
    float		value = 0;
    short		i;
    
    attrstart = attr_args_offset(argc, argv);
	if (attrstart && argv)
		atom_arg_getfloat(&value, 0, attrstart, argv);	// support a normal double argument
	if (value == 0) 
		value = 2000.0;						// default is 2 seconds
 
    x = (t_multitap *)object_alloc(multitap_class);;
    if(x){
		object_obex_store((void *)x, _sym_dumpout, (object *)(outlet_new(x,NULL)));	// dumpout
	    dsp_setup((t_pxobject *)x,1);					// Create Object and 1 Inlet (last argument)
	    outlet_new((t_pxobject *)x, "signal");			// Create a signal Outlet

		//taptools_max::set_global_sr(sys_getsr());
		x->audio_delay = new tt_multitap(value);			
		x->audio_delay->set_attr(tt_multitap::k_master_gain, 0);
		for(i=0; i<NUM_INPUTS; i++)
			x->signal_in[i] = new tt_audio_signal;
		for(i=0; i<NUM_OUTPUTS; i++)
			x->signal_out[i] = new tt_audio_signal;
		
		attr_args_process(x,argc,argv);					// handle attribute args			
	}
	return (x);											// Return the pointer
}

// Memory Deallocation
void multitap_free(t_multitap *x)
{
	short i;
	
	dsp_free((t_pxobject *)x);
	delete x->audio_delay;
	for(i=0; i<NUM_INPUTS; i++)
		delete x->signal_in[i];
	for(i=0; i<NUM_OUTPUTS; i++)
		delete x->signal_out[i];
}


/************************************************************************************/
// Methods bound to input/inlets

// Method for Assistance Messages
void multitap_assist(t_multitap *x, void *b, long msg, long arg, char *dst)
{
	if(msg==1) 	// Inlets
		strcpy(dst, "(signal) input, control messages");		
	else if(msg==2) // Outlets
		strcpy(dst, "(signal) Filtered output");
}


// Set number of taps
t_max_err attr_set_taps(t_multitap *x, void *attr, long argc, t_atom *argv)
{
	x->attr_taps = atom_getlong(argv);
	x->audio_delay->set_attr(tt_multitap::k_num_taps, x->attr_taps);
	
	return MAX_ERR_NONE;
	#pragma unused(attr)
}


// Set delay
t_max_err attr_set_delay(t_multitap *x, void *attr, long argc, t_atom *argv)	
{
	double	value;
	long	index;
	short	i;

	switch(argv[0].a_type){
		case A_LONG:						// Index, Value Pair
			index = atom_getlong(argv);
			value = atom_getfloat(argv+1);
			x->audio_delay->set_attr(tt_multitap::k_delay_ms, index, value);
			x->attr_delay[index] = value;
			if(index > x->attr_delay_len-1) x->attr_delay_len = index + 1;			
			break;
		case A_FLOAT:						// List of Values
			x->attr_delay_len = argc;
			for(i=0; i < argc; i++){
				x->attr_delay[i] = atom_getfloat(argv+i);
				x->audio_delay->set_attr(tt_multitap::k_delay_ms, i, atom_getfloat(argv+i));				
			}
			break;
	}
	return MAX_ERR_NONE;
}


// set gain
t_max_err attr_set_gain(t_multitap *x, void *attr, long argc, t_atom *argv)	
{
	double	value;
	long	index;
	short	i;

	switch(argv[0].a_type){
		case A_LONG:						// Index, Value Pair
			index = atom_getlong(argv);
			value = atom_getfloat(argv+1);
			x->audio_delay->set_attr(tt_multitap::k_gain, index, value);
			x->attr_gain[index] = value;
			if(index > x->attr_gain_len-1) x->attr_gain_len = index + 1;			
			break;
		case A_FLOAT:						// List of Values
			x->attr_gain_len = argc;
			for(i=0; i < argc; i++){
				x->attr_gain[i] = atom_getfloat(argv+i);
				x->audio_delay->set_attr(tt_multitap::k_gain, i, atom_getfloat(argv+i));				
			}
			break;
	}
	return MAX_ERR_NONE;
}


void multitap_perform64(t_multitap *x, t_object *dsp64, double **ins, long numins, double **outs, long numouts, long sampleframes, long flags, void *userparam)
{
	for (int i=0; i<sampleframes; i++)
   		x->input_buffer[i] = ins[0][i];
		
    x->signal_in[0]->set_vector(&x->input_buffer[0]); 	// Input
    x->signal_out[0]->set_vector(&x->output_buffer[0]);	// Output
	x->signal_in[0]->vectorsize = sampleframes;			// Vector Size
	
	x->audio_delay->dsp_vector_calc(x->signal_in[0], x->signal_out[0]);	
	
	for (int i=0; i<sampleframes; i++)
   		outs[0][i] = x->output_buffer[i];
}


void multitap_dsp64(t_multitap *x, t_object *dsp64, short *count, double samplerate, long maxvectorsize, long flags)
{
	x->audio_delay->set_sr(samplerate);
	x->input_buffer.resize(maxvectorsize);
	x->output_buffer.resize(maxvectorsize);
	object_method(dsp64, gensym("dsp_add64"), (t_object*)x, multitap_perform64, 0, NULL);
}
