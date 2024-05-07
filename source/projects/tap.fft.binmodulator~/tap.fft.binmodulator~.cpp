/* 
 *	External object for Max/MSP
 *	Copyright © 2005 by Timothy Place
 * 
 *	License: This code is licensed under the terms of the "New BSD License"
 *	http://creativecommons.org/licenses/BSD/
 */

#include "TTClassWrapperMax.h"	
#include "../ttblue/tt_audio_base.h"
#include "../ttblue/tt_audio_signal.h"
#include "../ttblue/tt_lfo.h"

#define MAX_NUM_LFOS 512


typedef struct _binmod				// Data Structure for this object
{
	t_pxobject 		x_obj;						// This object - must be first
	void			*dumpout;					// Pointer to the dumpout outlet
	tt_lfo			*lfos[MAX_NUM_LFOS];		// Pointer to member LFO objects
	tt_audio_signal *tempsig;					// Pointer to an audio signal object
	long			vs;							// cached vector size
	long			krate;						// cached sr/vs
	long			attr_bypass;				// Attribute Values...
	float			attr_freq[MAX_NUM_LFOS];
	long			attr_freq_len;
	float			attr_depth[MAX_NUM_LFOS];
	long			attr_depth_len;
	float			attr_phase[MAX_NUM_LFOS];
	long			attr_phase_len;
	t_symbol		*attr_shape[MAX_NUM_LFOS];
	long			attr_shape_len;	
} t_binmod;

// Class Globals
static t_class		*binmod_class;														// Required. Global pointing to this class
static t_symbol	*ps_sine, *ps_ramp, *ps_sawtooth, *ps_triangle, *ps_square;

// Prototypes for methods: need a method for each incoming message type
void *binmod_new(t_symbol *msg, short argc, t_atom *argv);						// New Object Creation Method
void binmod_free(t_binmod *x);
void binmod_assist(t_binmod *x, void *b, long m, long a, char *s);				// Assistance Method
t_int *binmod_perform(t_int *w);												// An MSP Perform (signal) Method
void binmod_dsp(t_binmod *x, t_signal **sp, short *count);						// ../../../../Jamoma/Core/DSP/library/build/JamomaDSP.dylib Method
t_max_err binmod_freq_set(t_binmod *x, void *attr, long argc, t_atom *argv);
t_max_err binmod_depth_set(t_binmod *x, void *attr, long argc, t_atom *argv);
t_max_err binmod_phase_set(t_binmod *x, void *attr, long argc, t_atom *argv);
t_max_err binmod_shape_set(t_binmod *x, void *attr, long argc, t_atom *argv);
void binmod_zero_frequency(t_binmod *x);
void binmod_zero_phase(t_binmod *x);
void binmod_zero_shape(t_binmod *x);
void binmod_zero_depth(t_binmod *x);
void binmod_resync(t_binmod *x);
void binmod_get_frequency_n(t_binmod *x, long value);
void binmod_get_depth_n(t_binmod *x, long value);
void binmod_get_phase_n(t_binmod *x, long value);
void binmod_get_shape_n(t_binmod *x, long value);


/************************************************************************************/
// Main() Function

extern "C" int C74_EXPORT main(void)
{
	t_class *c;
	
	c = class_new("tap.fft.binmodulator~",(method)binmod_new, (method)binmod_free, sizeof(t_binmod), 
		(method)0L, A_GIMME, 0);

		common_symbols_init();												// Initialize TapTools
	class_addmethod(c, (method)binmod_dsp, 				"dsp", A_CANT, 0L);		
    class_addmethod(c, (method)binmod_assist, 			"assist", A_CANT, 0L); 
    
    class_addmethod(c, (method)binmod_resync,			"phase_resync", 0L);
    class_addmethod(c, (method)binmod_zero_frequency,	"zero_frequency", 0L);
    class_addmethod(c, (method)binmod_zero_phase,		"zero_phase", 0L);
    class_addmethod(c, (method)binmod_zero_depth,		"zero_depth", 0L);
    class_addmethod(c, (method)binmod_zero_shape,		"zero_shape", 0L);
    class_addmethod(c, (method)binmod_get_frequency_n,	"getfrequencyn", A_LONG, 0L);
    class_addmethod(c, (method)binmod_get_phase_n,		"getphasen", A_LONG, 0L);
    class_addmethod(c, (method)binmod_get_depth_n,		"getdepthn", A_LONG, 0L);
    class_addmethod(c, (method)binmod_get_shape_n,		"getshapen", A_LONG, 0L);
	class_addmethod(c, (method)stdinletinfo,			"inletinfo",	A_CANT, 0);

	CLASS_ATTR_FLOAT(c,			"bypass",		0,	t_binmod, attr_bypass);
	CLASS_ATTR_STYLE(c,			"bypass",		0,	"onoff");

	CLASS_ATTR_FLOAT_VARSIZE(c,	"frequency",	0,	t_binmod, attr_freq, attr_freq_len, MAX_NUM_LFOS);
	CLASS_ATTR_ACCESSORS(c,		"frequency",	NULL, binmod_freq_set);

	CLASS_ATTR_FLOAT_VARSIZE(c,	"depth",		0,	t_binmod, attr_depth, attr_depth_len, MAX_NUM_LFOS);
	CLASS_ATTR_ACCESSORS(c,		"depth",		NULL, binmod_depth_set);

	CLASS_ATTR_FLOAT_VARSIZE(c,	"phase",		0,	t_binmod, attr_phase, attr_phase_len, MAX_NUM_LFOS);
	CLASS_ATTR_ACCESSORS(c,		"phase",		NULL, binmod_phase_set);

	CLASS_ATTR_SYM_VARSIZE(c,	"shape",		0,	t_binmod, attr_shape, attr_shape_len, MAX_NUM_LFOS);
	CLASS_ATTR_ACCESSORS(c,		"shape",		NULL, binmod_shape_set);

	class_dspinit(c);									// Setup object's class to work with MSP
class_register(_sym_box, c); 	binmod_class = c;

	// init globals
	ps_sine = gensym("sine");
	ps_ramp = gensym("ramp");
	ps_sawtooth = gensym("sawtooth");
	ps_square = gensym("square");
	ps_triangle = gensym("triangle");
}


/************************************************************************************/
// Object/Instance Life

// Constructor
void *binmod_new(t_symbol *msg, short argc, t_atom *argv)
{
//	long attrstart = attr_args_offset(argc, argv);
	int i;
	t_binmod *x = (t_binmod *)object_alloc(binmod_class);;

	if(x){
		object_obex_store((void *)x, _sym_dumpout, (object *)(x->dumpout = outlet_new(x,NULL)));	// dumpout
		dsp_setup((t_pxobject *)x, 3);				// Create Object and 3 Inlets (last argument)
		outlet_new((t_pxobject *)x, "signal");		// Create a signal Outlet
		outlet_new((t_pxobject *)x, "signal");		// Create a signal Outlet
		
		for(i=0;i < MAX_NUM_LFOS; i++)				// Allocate TapTools LFO objects and store pointers to them
			x->lfos[i] = new tt_lfo;
		x->vs = sys_getblksize();					// Cache the global vector size
		x->krate = sys_getsr() / x->vs;
		x->tempsig = new tt_audio_signal(x->vs);	// Allocate TapTools signal object and store a pointer to it
		x->attr_bypass = 0;							// Set defaults...
		x->attr_freq_len = 0;
		x->attr_depth_len = 0;
		x->attr_phase_len = 0;
		x->attr_shape_len = 0;

		binmod_zero_shape(x);
		binmod_zero_frequency(x);
		binmod_zero_phase(x);
		binmod_zero_depth(x);
		
		attr_args_process(x,argc,argv);				// handle attribute args	
	}
	return(x);										// Return the pointer
}

// Destructor
void binmod_free(t_binmod *x)
{
	short i;
	
	dsp_free((t_pxobject *)x);		// MSP's free routine - MUST BE FIRST!
	delete x->tempsig;				// free memory for our temporary vector
	for(i=0; i<MAX_NUM_LFOS; i++)	// free memory from the lfo objects
		delete x->lfos[i];
}


/************************************************************************************/
// Methods bound to input/inlets

// Method for Assistance Messages
void binmod_assist(t_binmod *x, void *b, long msg, long arg, char *dst)
{
	if(msg==1){ 	// Inlets
		switch(arg){
			case 0: strcpy(dst, "(signal) Input 1 - to be modulated"); break;
			case 1: strcpy(dst, "(signal) Input 2 - to be modulated"); break;
			case 2: strcpy(dst, "(signal) bin number of the inputs"); break;
		}
	}
	else if(msg==2){ // Outlets
		switch(arg){
			case 0: strcpy(dst, "(signal) Output 1"); break;
			case 1: strcpy(dst, "(signal) Output 2"); break;
		}
	}
	#pragma unused(x)
	#pragma unused(b)
}


// Set Freq
t_max_err binmod_freq_set(t_binmod *x, void *attr, long argc, t_atom *argv)	
{
	float	value;
	long	index;
	short	i;

	switch(argv[0].a_type){
		case A_LONG:						// Index, Value Pair
			index = atom_getlong(argv);
			value = atom_getfloat(argv+1);
			x->lfos[index]->set_attr(tt_lfo::k_frequency, TTClip(value, 0.f, float(x->krate-1)));
			x->attr_freq[index] = value;
			if(index > x->attr_freq_len-1) x->attr_freq_len = index + 1;			
			break;
		case A_FLOAT:						// List of Values
			x->attr_freq_len = argc;
			for(i=0; i < argc; i++){
				x->attr_freq[i] = atom_getfloat(argv+i);
				x->lfos[i]->set_attr(tt_lfo::k_frequency,  TTClip<double>(atom_getfloat(argv+i), 0.f, float(x->krate-1)));				
			}
			break;
	}
	return MAX_ERR_NONE;
}


// Set Depth
t_max_err binmod_depth_set(t_binmod *x, void *attr, long argc, t_atom *argv)	
{
	float	value;
	long	index;
	short	i;

	switch(argv[0].a_type){
		case A_LONG:						// Index, Value Pair
			index = atom_getlong(argv);
			value = atom_getfloat(argv+1);
			x->lfos[index]->set_attr(tt_lfo::k_depth, value);
			x->attr_depth[index] = value;			
			if(index > x->attr_depth_len-1) x->attr_depth_len = index + 1;
			break;
		case A_FLOAT:						// List of Values
			x->attr_depth_len = argc;
			for(i=0; i < argc; i++){
				x->attr_depth[i] = atom_getfloat(argv+i);
				x->lfos[i]->set_attr(tt_lfo::k_depth, atom_getfloat(argv+i));				
			}	
			break;
	}
	return MAX_ERR_NONE;
}

// Set Phase
t_max_err binmod_phase_set(t_binmod *x, void *attr, long argc, t_atom *argv)	
{
	float	value;
	long	index;
	short	i;

	switch(argv[0].a_type){
		case A_LONG:						// Index, Value Pair
			index = atom_getlong(argv);
			value = atom_getfloat(argv+1);
			x->lfos[index]->set_attr(tt_lfo::k_phase, TTClip(value, 0.f, 1.f));
			x->attr_depth[index] = value;			
			if(index > x->attr_depth_len-1) x->attr_depth_len = index + 1;
			break;
		case A_FLOAT:						// List of Values
			x->attr_phase_len = argc;
			for(i=0; i < argc; i++){
				x->attr_phase[i] = atom_getfloat(argv+i);
				x->lfos[i]->set_attr(tt_lfo::k_phase, TTClip<double>(atom_getfloat(argv+i), 0.f, 1.f));				
			}
			break;
	}
	return MAX_ERR_NONE;
}

// Set Shape
t_max_err binmod_shape_set(t_binmod *x, void *attr, long argc, t_atom *argv)	
{
	t_symbol	*shape;
	long		index;
	short		i;
	
	switch(argv[0].a_type){
		case A_LONG:						// Index, Value Pair
			index = atom_getlong(argv);
			shape = atom_getsym(argv+1);
			x->attr_shape[index] = shape;			
			if(index > x->attr_shape_len-1) x->attr_shape_len = index + 1;
			if(atom_getsym(argv+1) == ps_sine)
				x->lfos[atom_getlong(argv)]->set_attr(tt_lfo::k_mode, tt_lfo::k_mode_sine_mod);
			else if(atom_getsym(argv+1) == ps_ramp)
				x->lfos[atom_getlong(argv)]->set_attr(tt_lfo::k_mode, tt_lfo::k_mode_ramp_mod);
			else if(atom_getsym(argv+1) == ps_square)
				x->lfos[atom_getlong(argv)]->set_attr(tt_lfo::k_mode, tt_lfo::k_mode_square_mod);
			else if(atom_getsym(argv+1) == ps_triangle)
				x->lfos[atom_getlong(argv)]->set_attr(tt_lfo::k_mode, tt_lfo::k_mode_triangle_mod);
			else
				object_error((t_object *)x, "not a valid shape");
			break;
		case A_SYM:							// List of Values
			x->attr_shape_len = argc;
			for(i=0; i < argc; i++){
				x->attr_shape[i] = atom_getsym(argv+i);
				if(atom_getsym(argv+i) == ps_sine)
					x->lfos[atom_getlong(argv)]->set_attr(tt_lfo::k_mode, tt_lfo::k_mode_sine_mod);
				else if(atom_getsym(argv+i) == ps_ramp)
					x->lfos[atom_getlong(argv)]->set_attr(tt_lfo::k_mode, tt_lfo::k_mode_ramp_mod);
				else if(atom_getsym(argv+i) == ps_square)
					x->lfos[atom_getlong(argv)]->set_attr(tt_lfo::k_mode, tt_lfo::k_mode_square_mod);
				else if(atom_getsym(argv+i) == ps_triangle)
					x->lfos[atom_getlong(argv)]->set_attr(tt_lfo::k_mode, tt_lfo::k_mode_triangle_mod);
				else
					object_error((t_object *)x, "not a valid shape");
			}
			break;
	}
	return MAX_ERR_NONE;
}


// ZERO THE ATTRIBUTES
void binmod_zero_frequency(t_binmod *x)
{
	short i;
	for(i=0; i < MAX_NUM_LFOS; i++){
		x->attr_freq[i] = 0.0;
		x->lfos[i]->set_attr(tt_lfo::k_frequency, 0.0);
	}	
}

void binmod_zero_phase(t_binmod *x)
{
	short i;
	for(i=0; i < MAX_NUM_LFOS; i++){
		x->attr_phase[i] = 0.0;
		x->lfos[i]->set_attr(tt_lfo::k_phase, 0.0);
	}	
}

void binmod_zero_shape(t_binmod *x)
{
	short i;
	for(i=0; i < MAX_NUM_LFOS; i++){
		x->attr_shape[i] = ps_sine;
		x->lfos[i]->set_attr(tt_lfo::k_mode, tt_lfo::k_mode_sine_mod);
	}	
}

void binmod_zero_depth(t_binmod *x)
{
	short i;
	for(i=0; i < MAX_NUM_LFOS; i++){
		x->attr_depth[i] = 0.0;
		x->lfos[i]->set_attr(tt_lfo::k_depth, 0.0);
	}	
}

// Method: Resync
void binmod_resync(t_binmod *x)
{
	short i;
	for(i=0; i < MAX_NUM_LFOS; i++)
		x->lfos[i]->phase_reset();	
}

// Get N methods
void binmod_get_frequency_n(t_binmod *x, long value)
{
	outlet_float(x->dumpout, x->attr_freq[value]);
}

void binmod_get_depth_n(t_binmod *x, long value)
{
	outlet_float(x->dumpout, x->attr_depth[value]);
}

void binmod_get_phase_n(t_binmod *x, long value)
{
	outlet_float(x->dumpout, x->attr_phase[value]);
}

void binmod_get_shape_n(t_binmod *x, long value)
{
	outlet_anything(x->dumpout, x->attr_shape[value], 0, /*NIL*/0L);
}


// Perform (signal) Method
t_int *binmod_perform(t_int *w)
{
	t_float *in1 = (t_float *)(w[1]);   		// Input:
	t_float *in2 = (t_float *)(w[2]);   		// Input:
	t_float *in3 = (t_float *)(w[3]);   		// Input: index
	t_float *out1 = (t_float *)(w[4]);			// Output
	t_float *out2 = (t_float *)(w[5]);			// Output
   	t_binmod *x = (t_binmod *)(w[6]);			// Pointer
	int n = (int)(w[7]);	// Vector Size (FFT size in pfft~)
		
	if(x->x_obj.z_disabled) goto out;
	if(x->attr_bypass){
		while(n--){
			*out1++ = *in1++;
			*out2++ = *in2++;
		}
		goto out;
	}
	
	// THIS IS DONE IN THE ../../../../Jamoma/Core/DSP/library/build/JamomaDSP.dylib ROUTINE ALREADY
	// n = x->tempsig->vectorsize = taptools_audio::clip(n, 0, MAX_NUM_LFOS-1);	// insure that the vector isn't too big
	
	while(n--){
		x->lfos[TTClip(int(*in3++), 0, MAX_NUM_LFOS-1)]->dsp_vector_calc(x->tempsig);		
//		x->lfos[0]->dsp_vector_calc(x->tempsig);		
		*out1++ = *in1++ * *(x->tempsig->vector);			// apply the lfo to the first signal
		*out2++ = *in2++ * *(x->tempsig->vector);				// apply the lfo to the second signal
	}	

out:
	return (w + 8);	
}


// ../../../../Jamoma/Core/DSP/library/build/JamomaDSP.dylib Method
void binmod_dsp(t_binmod *x, t_signal **sp, short *count)
{
	int i;
	
	// Double-check the vector size
	x->vs = sp[0]->s_n;					// update the cached vector size

	for(i=0; i<MAX_NUM_LFOS; i++){			
		x->lfos[i]->set_sr(sp[0]->s_sr);			// Set the SR for the lfos
		x->lfos[i]->set_vectorsize(x->vs);
	}
	x->tempsig->alloc(x->vs);			// re-allocate the temp vector if necessary
	x->krate = sp[0]->s_sr / x->vs;
	
	if(x->vs > MAX_NUM_LFOS)
		object_error((t_object *)x, "vector_size: %i is higher than the number of available lfos (%i)", sp[0]->s_n, MAX_NUM_LFOS);	
	else
		// Add the perform method: 3 inputs, 2 outputs
		dsp_add(binmod_perform, 7, sp[0]->s_vec, sp[1]->s_vec, sp[2]->s_vec, 
			sp[3]->s_vec, sp[4]->s_vec, x, sp[0]->s_n);

	// Just a note to the compiler to not show us the warning
//object_post((t_object *)x, "dsp vs: %i, TEMP_VS: %i", x->vs, x->tempsig->vectorsize);		
	#pragma unused(count)
//	x->lfos[0]->dsp_vector_calc(x->tempsig);
}
