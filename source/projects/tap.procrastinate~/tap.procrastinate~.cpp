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
#include "../ttblue/tt_delay.h"
#include "../ttblue/tt_phasor.h"
#include "../ttblue/tt_buffer_window.h"
#include "../ttblue/tt_gain.h"
#include "../ttblue/tt_offset.h"
#include "../ttblue/tt_onewrap.h"
#include "../ttblue/tt_mixer_mono.h"
#include "../ttblue/tt_buffer.h"
#include "../ttblue/tt_pan.h"
#include "../ttblue/tt_procrastinate.h"

#define NUM_INPUTS 1
#define NUM_OUTPUTS 2

// Data Structure for this object
typedef struct _procrastinate{
    t_pxobject 			x_obj;
   	tt_procrastinate	*procrastinate;
    tt_audio_signal		*signal_in[NUM_INPUTS];
    tt_audio_signal		*signal_out[NUM_OUTPUTS];
   	double				attr_gain_range[8];		// 4 units by 2 params each (low, high)
   	long				attr_gain_len;
   	double				attr_pan_range[8];		// 4 units by 2 params each (low, high)
   	long				attr_pan_len;
   	double				attr_shift_range[8];	// 4 units by 2 params each (low, high)
   	long				attr_shift_len;
   	double				attr_delay_range[8];	// 4 units by 2 params each (low, high)
   	long				attr_delay_len;
   	// t_symbol			*attr_interpolation;
	long				attr_mute;
} t_procrastinate;

// Prototypes for methods: need a method for each incoming message type
void *		procrastinate_new(t_symbol *msg, short argc, t_atom *argv);
void 		procrastinate_perform64(t_procrastinate *x, t_object *dsp64, double **ins, long numins, double **outs, long numouts, long sampleframes, long flags, void *userparam);
void procrastinate_dsp64(t_procrastinate *x, t_object *dsp64, short *count, double samplerate, long maxvectorsize, long flags);
void 		procrastinate_assist(t_procrastinate *x, void *b, long msg, long arg, char *dst);	// Assistance Method
void 		procrastinate_free(t_procrastinate *x);
void 		procrastinate_bang(t_procrastinate *x);
t_max_err 	attr_set_gain_range(t_procrastinate *x, void *attr, long argc, t_atom *argv);
t_max_err 	attr_set_pan_range(t_procrastinate *x, void *attr, long argc, t_atom *argv);
t_max_err 	attr_set_shift_range(t_procrastinate *x, void *attr, long argc, t_atom *argv);
t_max_err 	attr_set_delay_range(t_procrastinate *x, void *attr, long argc, t_atom *argv);
t_max_err 	attr_set_interpolation(t_procrastinate *x, void *attr, long argc, t_atom *argv);
t_max_err 	attr_set_mute(t_procrastinate *x, void *attr, long argc, t_atom *argv);
void 		procrastinate_ratio(t_procrastinate *x, long unit, double value);
void 		procrastinate_window(t_procrastinate *x, long unit, double value);
void 		procrastinate_clear(t_procrastinate *x);
void 		procrastinate_anything(t_procrastinate *x, t_symbol *msg, long argc, t_atom *argv);

// Globals
static t_class	*procrastinate_class;			// Required. Global pointing to this class
static t_symbol	*ps_linear;
static t_symbol	*ps_polynomial;

/************************************************************************************/
// Main() Function

extern "C" int C74_EXPORT main(void)
{
	t_class *c;
	
	c = class_new("tap.procrastinate~",(method)procrastinate_new, (method)procrastinate_free, sizeof(t_procrastinate), 
		(method)0L, A_GIMME, 0);

		common_symbols_init();												// Initialize TapTools
	class_addmethod(c, (method)procrastinate_bang,		"bang", 0L);
	class_addmethod(c, (method)procrastinate_bang,		"generate_parameters", 0L);
	class_addmethod(c, (method)procrastinate_bang,		"/generate_parameters", 0L);
	class_addmethod(c, (method)procrastinate_ratio,		"shift_ratio", A_LONG, A_FLOAT, 0L);	// directly control a specific shifter
	class_addmethod(c, (method)procrastinate_window,	"window", A_LONG, A_FLOAT, 0L);			// directly control a specific delay time
	class_addmethod(c, (method)procrastinate_clear,		"clear", 0L);
	class_addmethod(c, (method)procrastinate_dsp64, "dsp64", A_CANT, 0);
	class_addmethod(c, (method)procrastinate_assist, 	"assist", A_CANT, 0L);
	class_addmethod(c, (method)attr_set_mute,			"/audio/mute",	A_GIMME, 0);
	class_addmethod(c, (method)procrastinate_anything,	"anything",		A_GIMME, 0);
	class_addmethod(c, (method)stdinletinfo,			"inletinfo",	A_CANT, 0);

	CLASS_ATTR_LONG_VARSIZE(c,	"gain_range",		0,	t_procrastinate, attr_gain_range, attr_gain_len, 8);
	CLASS_ATTR_ACCESSORS(c,		"gain_range",		NULL, attr_set_gain_range);

	CLASS_ATTR_LONG_VARSIZE(c,	"pan_range",		0,	t_procrastinate, attr_gain_range, attr_pan_len, 8);
	CLASS_ATTR_ACCESSORS(c,		"pan_range",		NULL, attr_set_pan_range);

	CLASS_ATTR_LONG_VARSIZE(c,	"shift_range",		0,	t_procrastinate, attr_gain_range, attr_shift_len, 8);
	CLASS_ATTR_ACCESSORS(c,		"shift_range",		NULL, attr_set_shift_range);

	CLASS_ATTR_LONG_VARSIZE(c,	"delay_range",		0,	t_procrastinate, attr_gain_range, attr_delay_len, 8);
	CLASS_ATTR_ACCESSORS(c,		"delay_range",		NULL, attr_set_delay_range);

//	attr = attr_offset_new("interpolation", _sym_symbol, attrflags,
//		(method)0L,(method)attr_set_interpolation, calcoffset(t_procrastinate, attr_interpolation));
//	class_addattr(c, attr);

	CLASS_ATTR_LONG(c,			"mute",				0,	t_procrastinate, attr_mute);
	CLASS_ATTR_STYLE(c,			"mute",				0,	"onoff");
	
	class_dspinit(c);									// Setup object's class to work with MSP
class_register(_sym_box, c); 	procrastinate_class = c;
	
	// Init Globals
	ps_linear = gensym("linear");
	ps_polynomial = gensym("polynomial");
}


/************************************************************************************/
// Object Creation Method

void *procrastinate_new(t_symbol *msg, short argc, t_atom *argv)
{
    t_procrastinate *x;
    short i;
    
    x = (t_procrastinate *)object_alloc(procrastinate_class);;
    if(x){
    	object_obex_store((void *)x, _sym_dumpout, (object *)outlet_new(x,NULL));	// dumpout	
	    dsp_setup((t_pxobject *)x, 1);								// Create Object with 1 Inlet
	    x->x_obj.z_misc = Z_NO_INPLACE;
	    outlet_new((t_pxobject *)x, "signal");						// Create a signal Outlet
	    outlet_new((t_pxobject *)x, "signal");						// Create a signal Outlet

		//taptools_max::set_global_sr(sys_getsr());					// Set Tap.Tools global SR...
		//taptools_max:: set_global_vectorsize(sys_getblksize());	// Set Tap.Tools global vector size...
		
		x->procrastinate = new tt_procrastinate;
		for(i=0; i<NUM_INPUTS; i++)
			x->signal_in[i] = new tt_audio_signal;
		for(i=0; i<NUM_OUTPUTS; i++)
			x->signal_out[i] = new tt_audio_signal;

		x->attr_mute = 0;
		attr_args_process(x, argc, argv);							// handle attribute args			
	}
	return (x);														// Return the pointer
}

// Memory Deallocation
void procrastinate_free(t_procrastinate *x)
{
	short i;
	
	dsp_free((t_pxobject *)x);
	delete x->procrastinate;
	for(i=0; i<NUM_INPUTS; i++)
		delete x->signal_in[i];
	for(i=0; i<NUM_OUTPUTS; i++)
		delete x->signal_out[i];
}


/************************************************************************************/
// Methods bound to input/inlets

// Method for Assistance Messages
void procrastinate_assist(t_procrastinate *x, void *b, long msg, long arg, char *dst)
{
	if(msg==1) 	// Inlets
		strcpy(dst, "(signal) input, control messages");		
	else if(msg==2) // Outlets
		strcpy(dst, "(signal) Filtered output");
	#pragma unused(x)
	#pragma unused(b)
	#pragma unused(arg)
}


// BANG ************************
void procrastinate_bang(t_procrastinate *x)
{
	x->procrastinate->randomize_parameters();
}


// GAIN
t_max_err attr_set_gain_range(t_procrastinate *x, void *attr, long argc, t_atom *argv)	
{
	double	value_low, value_high;
	long	index;
	short	i;

	switch(argv[0].a_type){
		case A_LONG:						// Index, Value Pair
			index = atom_getlong(argv);
			value_low = atom_getfloat(argv+1);
			value_high = atom_getfloat(argv+2);
			// NEED TO UPDATE THE VALUES STORED IN THE STRUCT HERE!!!!
			x->procrastinate->set_attr(tt_procrastinate::k_gain_range, index, value_low, value_high);
			x->attr_gain_len = 8;
			break;
		case A_FLOAT:						// List of Values
			x->attr_gain_len = argc;
			for(i=0; i < argc; i++)
				x->attr_gain_range[i] = atom_getfloat(argv+i);
			for(i=0; i<4; i=i+2)
				x->procrastinate->set_attr(tt_procrastinate::k_gain_range, i, x->attr_gain_range[i], x->attr_gain_range[i+1]);
			break;
	}
	return MAX_ERR_NONE;
	#pragma unused(attr)
}


// PAN
t_max_err attr_set_pan_range(t_procrastinate *x, void *attr, long argc, t_atom *argv)	
{
	double	value_low, value_high;
	long	index;
	short	i;

	switch(argv[0].a_type){
		case A_LONG:						// Index, Value Pair
			index = atom_getlong(argv);
			value_low = atom_getfloat(argv+1);
			value_high = atom_getfloat(argv+2);
			// NEED TO UPDATE THE VALUES STORED IN THE STRUCT HERE!!!!
			x->procrastinate->set_attr(tt_procrastinate::k_pan_range, index, value_low, value_high);
			x->attr_pan_len = 8;
			break;
		case A_FLOAT:						// List of Values
			x->attr_pan_len = argc;
			for(i=0; i < argc; i++)
				x->attr_pan_range[i] = atom_getfloat(argv+i);
			for(i=0; i<4; i=i+2)
				x->procrastinate->set_attr(tt_procrastinate::k_pan_range, i, x->attr_pan_range[i], x->attr_pan_range[i+1]);
			break;
	}
	return MAX_ERR_NONE;
	#pragma unused(attr)
}


// SHIFT
t_max_err attr_set_shift_range(t_procrastinate *x, void *attr, long argc, t_atom *argv)	
{
	double	value_low, value_high;
	long	index;
	short	i;

	switch(argv[0].a_type){
		case A_LONG:						// Index, Value Pair
			index = atom_getlong(argv);
			value_low = atom_getfloat(argv+1);
			value_high = atom_getfloat(argv+2);
			// NEED TO UPDATE THE VALUES STORED IN THE STRUCT HERE!!!!
			x->procrastinate->set_attr(tt_procrastinate::k_shift_range, index, value_low, value_high);
			x->attr_shift_len = 8;
			break;
		case A_FLOAT:						// List of Values
			x->attr_shift_len = argc;
			for(i=0; i < argc; i++)
				x->attr_shift_range[i] = atom_getfloat(argv+i);
			for(i=0; i<4; i=i+2)
				x->procrastinate->set_attr(tt_procrastinate::k_shift_range, i, x->attr_shift_range[i], x->attr_shift_range[i+1]);
			break;
	}
	return MAX_ERR_NONE;
	#pragma unused(attr)
}


// DELAY
t_max_err attr_set_delay_range(t_procrastinate *x, void *attr, long argc, t_atom *argv)	
{
	double	value_low, value_high;
	long	index;
	short	i;

	switch(argv[0].a_type){
		case A_LONG:						// Index, Value Pair
			index = atom_getlong(argv);
			value_low = atom_getfloat(argv+1);
			value_high = atom_getfloat(argv+2);
			// NEED TO UPDATE THE VALUES STORED IN THE STRUCT HERE!!!!
			x->procrastinate->set_attr(tt_procrastinate::k_delay_range, index, value_low, value_high);
			x->attr_delay_len = 8;
			break;
		case A_FLOAT:						// List of Values
			x->attr_delay_len = argc;
			for(i=0; i < argc; i++)
				x->attr_delay_range[i] = atom_getfloat(argv+i);
			for(i=0; i<4; i=i+2)
				x->procrastinate->set_attr(tt_procrastinate::k_delay_range, i, x->attr_delay_range[i], x->attr_delay_range[i+1]);
			break;
	}
	return MAX_ERR_NONE;
	#pragma unused(attr)
}


/*
// ATTRIBUTE
t_max_err attr_set_interpolation(t_procrastinate *x, void *attr, long argc, t_atom *argv)
{
	x->attr_interpolation = atom_getsym(argv);
	if(x->attr_interpolation == ps_none)
		x->procrastinate->set_attr(tap_procrastinate::k_interpolation, tap_procrastinate::k_interpolation_none);
	else if(x->attr_interpolation == ps_linear)
		x->procrastinate->set_attr(tap_procrastinate::k_interpolation, tap_procrastinate::k_interpolation_linear);
	else if(x->attr_interpolation == ps_polynomial)
		x->procrastinate->set_attr(tap_procrastinate::k_interpolation, tap_procrastinate::k_interpolation_polynomial);
	else
		object_error((t_object *)x, "invalid interpolation type specified");
		
	return MAX_ERR_NONE;
	#pragma unused(attr)
	#pragma unused(argc)
}
*/

// ATTRIBUTE: mute
t_max_err attr_set_mute(t_procrastinate *x, void *attr, long argc, t_atom *argv)
{
	x->attr_mute = atom_getlong(argv);
	return MAX_ERR_NONE;
	#pragma unused(attr)
}


// Set ratio
void procrastinate_ratio(t_procrastinate *x, long unit, double value)	
{
	x->procrastinate->set_attr(tt_procrastinate::k_ratio, unit, value);
}

// delay window size
void procrastinate_window(t_procrastinate *x, long unit, double value)
{
	x->procrastinate->set_attr(tt_procrastinate::k_windowsize, unit, value);
}


// CLEAR
void procrastinate_clear(t_procrastinate *x)
{
	x->procrastinate->clear();
}


// when used as the algorithm for a module, we use this to suppress errors for unhandles messages
void procrastinate_anything(t_procrastinate *x, t_symbol *msg, long argc, t_atom *argv)
{
	char	name[256];
	char	*temp = name;
	double	value = 0;
	
	if(argc != 1)
		return;
	value = atom_getfloat(argv);
		
	strcpy(name, msg->s_name);
	if(*temp == '/')
		temp++;
	
	if(*temp == '1'){
		temp++;
		if(!strcmp("/delay/min", temp))
			x->attr_delay_range[0] = value;
		else if(!strcmp("/delay/max", temp))
			x->attr_delay_range[1] = value;
		else if(!strcmp("/gain/min", temp))
			x->attr_gain_range[0] = value;
		else if(!strcmp("/gain/max", temp))
			x->attr_gain_range[1] = value;
		else if(!strcmp("/shift/min", temp))
			x->attr_shift_range[0] = value;
		else if(!strcmp("/shift/max", temp))
			x->attr_shift_range[1] = value;
		else if(!strcmp("/pan/min", temp))
			x->attr_pan_range[0] = value;
		else if(!strcmp("/pan/max", temp))
			x->attr_pan_range[1] = value;

		x->procrastinate->set_attr(tt_procrastinate::k_delay_range, 0, x->attr_delay_range[0], x->attr_delay_range[1]);
		x->procrastinate->set_attr(tt_procrastinate::k_gain_range, 0, x->attr_gain_range[0], x->attr_gain_range[1]);
		x->procrastinate->set_attr(tt_procrastinate::k_shift_range, 0, x->attr_shift_range[0], x->attr_shift_range[1]);
		x->procrastinate->set_attr(tt_procrastinate::k_pan_range, 0, x->attr_pan_range[0], x->attr_pan_range[1]);
	}
	else if(*temp == '2'){
		temp++;
		if(!strcmp("/delay/min", temp))
			x->attr_delay_range[2] = value;
		else if(!strcmp("/delay/max", temp))
			x->attr_delay_range[3] = value;
		else if(!strcmp("/gain/min", temp))
			x->attr_gain_range[2] = value;
		else if(!strcmp("/gain/max", temp))
			x->attr_gain_range[3] = value;
		else if(!strcmp("/shift/min", temp))
			x->attr_shift_range[2] = value;
		else if(!strcmp("/shift/max", temp))
			x->attr_shift_range[3] = value;
		else if(!strcmp("/pan/min", temp))
			x->attr_pan_range[2] = value;
		else if(!strcmp("/pan/max", temp))
			x->attr_pan_range[3] = value;

		x->procrastinate->set_attr(tt_procrastinate::k_delay_range, 1, x->attr_delay_range[2], x->attr_delay_range[3]);
		x->procrastinate->set_attr(tt_procrastinate::k_gain_range, 1, x->attr_gain_range[2], x->attr_gain_range[3]);
		x->procrastinate->set_attr(tt_procrastinate::k_shift_range, 1, x->attr_shift_range[2], x->attr_shift_range[3]);
		x->procrastinate->set_attr(tt_procrastinate::k_pan_range, 1, x->attr_pan_range[2], x->attr_pan_range[3]);
	}
	else if(*temp == '3'){
		temp++;
		if(!strcmp("/delay/min", temp))
			x->attr_delay_range[4] = value;
		else if(!strcmp("/delay/max", temp))
			x->attr_delay_range[5] = value;
		else if(!strcmp("/gain/min", temp))
			x->attr_gain_range[4] = value;
		else if(!strcmp("/gain/max", temp))
			x->attr_gain_range[5] = value;
		else if(!strcmp("/shift/min", temp))
			x->attr_shift_range[4] = value;
		else if(!strcmp("/shift/max", temp))
			x->attr_shift_range[5] = value;
		else if(!strcmp("/pan/min", temp))
			x->attr_pan_range[4] = value;
		else if(!strcmp("/pan/max", temp))
			x->attr_pan_range[5] = value;

		x->procrastinate->set_attr(tt_procrastinate::k_delay_range, 2, x->attr_delay_range[4], x->attr_delay_range[5]);
		x->procrastinate->set_attr(tt_procrastinate::k_gain_range, 2, x->attr_gain_range[4], x->attr_gain_range[5]);
		x->procrastinate->set_attr(tt_procrastinate::k_shift_range, 2, x->attr_shift_range[4], x->attr_shift_range[5]);
		x->procrastinate->set_attr(tt_procrastinate::k_pan_range, 2, x->attr_pan_range[4], x->attr_pan_range[5]);
	}
	else if(*temp == '4'){
		temp++;
		if(!strcmp("/delay/min", temp))
			x->attr_delay_range[6] = value;
		else if(!strcmp("/delay/max", temp))
			x->attr_delay_range[7] = value;
		else if(!strcmp("/gain/min", temp))
			x->attr_gain_range[6] = value;
		else if(!strcmp("/gain/max", temp))
			x->attr_gain_range[7] = value;
		else if(!strcmp("/shift/min", temp))
			x->attr_shift_range[6] = value;
		else if(!strcmp("/shift/max", temp))
			x->attr_shift_range[7] = value;
		else if(!strcmp("/pan/min", temp))
			x->attr_pan_range[6] = value;
		else if(!strcmp("/pan/max", temp))
			x->attr_pan_range[7] = value;
			
		x->procrastinate->set_attr(tt_procrastinate::k_delay_range, 3, x->attr_delay_range[6], x->attr_delay_range[7]);
		x->procrastinate->set_attr(tt_procrastinate::k_gain_range, 3, x->attr_gain_range[6], x->attr_gain_range[7]);
		x->procrastinate->set_attr(tt_procrastinate::k_shift_range, 3, x->attr_shift_range[6], x->attr_shift_range[7]);
		x->procrastinate->set_attr(tt_procrastinate::k_pan_range, 3, x->attr_pan_range[6], x->attr_pan_range[7]);
	}
	else{
	//	object_post((t_object *)x, "anything: %s", msg->s_name);
		return;
	}
}


void procrastinate_perform64(t_procrastinate *x, t_object *dsp64, double **ins, long numins, double **outs, long numouts, long sampleframes, long flags, void *userparam)
{
    x->signal_in[0]->set_vector(ins[0]);
    x->signal_out[0]->set_vector(outs[0]);
    x->signal_out[1]->set_vector(outs[1]);
	x->signal_in[0]->vectorsize = sampleframes;
		
	x->procrastinate->dsp_vector_calc(x->signal_in[0], x->signal_out[0], x->signal_out[1]);	
}


void procrastinate_dsp64(t_procrastinate *x, t_object *dsp64, short *count, double samplerate, long maxvectorsize, long flags)
{
	procrastinate_clear(x);
	x->procrastinate->set_vectorsize(maxvectorsize);
	x->procrastinate->set_sr(samplerate);

	object_method(dsp64, gensym("dsp_add64"), (t_object*)x, procrastinate_perform64, 0, NULL);
}

