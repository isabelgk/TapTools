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
#include "../ttblue/tt_lfo.h"
#include "../ttblue/tt_svf.h"
#include "../ttblue/tt_ramp.h"

#define NUM_INPUTS 2
#define NUM_OUTPUTS 2
#define NUM_FILTERS 2
#define LEFT 0
#define RIGHT 1
#define NUM_AUDIO_SIGNALS 1

typedef struct _svf			// Data Structure for this object
{
    t_pxobject 			x_obj;
	tt_svf				*filter[NUM_FILTERS];
	tt_lfo				*lfo;
	tt_ramp				*portamento;
	tt_audio_signal		*audio_sig[NUM_AUDIO_SIGNALS];
    tt_audio_signal		*signal_in[NUM_INPUTS];
    tt_audio_signal		*signal_out[NUM_OUTPUTS];
	double				attr_portamento_time;
	t_symbol			*attr_type;
	double				attr_frequency;
	double				attr_resonance;
	double				attr_lfo_freq_depth;
	double				attr_lfo_freq_speed;
	t_symbol			*attr_lfo_freq_shape;
	long				attr_lfo_freq_toggle;
	
	tt_attribute_value	freq;	// current frequency as stored by the ramp object
//	tt_attribute_value	depth;
} t_svf;


// Prototypes for methods: need a method for each incoming message type
void *svf_new(t_symbol *msg, short argc, t_atom *argv);
void svf_perform_mono64(t_svf *x, t_object *dsp64, double **ins, long numins, double **outs, long numouts, long sampleframes, long flags, void *userparam);
void svf_perform64(t_svf *x, t_object *dsp64, double **ins, long numins, double **outs, long numouts, long sampleframes, long flags, void *userparam);
void svf_dsp64(t_svf *x, t_object *dsp64, short *count, double samplerate, long maxvectorsize, long flags);
void svf_assist(t_svf *x, void *b, long msg, long arg, char *dst);	// Assistance Method
void svf_clear(t_svf *x);
void svf_freqlfo_depth(t_svf *x, double value);
void svf_freqlfo_speed(t_svf *x, double value);
void svf_freqlfo_shape(t_svf *x, long value);
void svf_freqlfo_toggle(t_svf *x, long value);
void svf_portamento(t_svf *x, double value);
t_max_err attr_set_frequency(t_svf *x, void *attr, long argc, t_atom *argv);
t_max_err attr_set_resonance(t_svf *x, void *attr, long argc, t_atom *argv);
t_max_err attr_set_lfo_freq_depth(t_svf *x, void *attr, long argc, t_atom *argv);
t_max_err attr_set_lfo_freq_speed(t_svf *x, void *attr, long argc, t_atom *argv);
t_max_err attr_set_lfo_freq_shape(t_svf *x, void *attr, long argc, t_atom *argv);
t_max_err attr_set_type(t_svf *x, void *attr, long argc, t_atom *argv);
void svf_free(t_svf *x);


// Globals
static t_class		*svf_class;			// Required. Global pointing to this class
static t_symbol 	*ps_lowpass;
static t_symbol 	*ps_highpass; 
static t_symbol 	*ps_bandpass;
static t_symbol 	*ps_notch;
static t_symbol 	*ps_peak;


/************************************************************************************/
// Main() Function

extern "C" int C74_EXPORT main(void)
{
	t_class *c;
	
	c = class_new("tap.svf~", (method)svf_new, (method)svf_free, sizeof(t_svf), 
		(method)0L, A_GIMME, 0);

		common_symbols_init();									// Initialize TapTools
	class_addmethod(c, (method)svf_clear, 	"clear", 0L);		
	class_addmethod(c, (method)svf_dsp64, "dsp64", A_CANT, 0);
    class_addmethod(c, (method)svf_assist, 	"assist", A_CANT, 0L);	
	class_addmethod(c, (method)stdinletinfo,"inletinfo",	A_CANT, 0);

	CLASS_ATTR_SYM(c,		"type",				0,	t_svf, attr_type);
	CLASS_ATTR_ACCESSORS(c,	"type",				NULL, attr_set_type);
	CLASS_ATTR_ENUM(c,		"type",				0,	"lowpass highpass bandpass notch peak");

	CLASS_ATTR_DOUBLE(c,		"frequency",		0,	t_svf, attr_frequency);
	CLASS_ATTR_ACCESSORS(c,	"frequency",		NULL, attr_set_frequency);

	CLASS_ATTR_DOUBLE(c,		"resonance",		0,	t_svf, attr_resonance);
	CLASS_ATTR_ACCESSORS(c,	"resonance",		NULL, attr_set_resonance);

	CLASS_ATTR_DOUBLE(c,		"lfo.freq.depth",	0,	t_svf, attr_lfo_freq_depth);
	CLASS_ATTR_ACCESSORS(c,	"lfo.freq.depth",	NULL, attr_set_lfo_freq_depth);

	CLASS_ATTR_DOUBLE(c,		"lfo.freq.speed",	0,	t_svf, attr_lfo_freq_speed);
	CLASS_ATTR_ACCESSORS(c,	"lfo.freq.speed",	NULL, attr_set_lfo_freq_speed);

	CLASS_ATTR_SYM(c,		"lfo.freq.shape",	0,	t_svf, attr_lfo_freq_shape);
	CLASS_ATTR_ACCESSORS(c,	"lfo.freq.shape",	NULL, attr_set_lfo_freq_shape);
	CLASS_ATTR_ENUM(c,		"lfo.freq.shape",	0,	"sine triangle square ramp");

	CLASS_ATTR_LONG(c,		"lfo.freq.toggle",	0,	t_svf, attr_lfo_freq_toggle);
	CLASS_ATTR_STYLE(c,		"lfo.freq.toggle",	0,	"onoff");

	CLASS_ATTR_DOUBLE(c,		"portamento_time",	0,	t_svf, attr_portamento_time);	

	class_dspinit(c);								// Setup object's class to work with MSP
class_register(_sym_box, c); 	svf_class = c;
	
	// Init Globals
	ps_lowpass = gensym("lowpass");
	ps_highpass = gensym("highpass"); 
	ps_bandpass = gensym("bandpass");
	ps_notch = gensym("notch");
	ps_peak = gensym("peak");
}


/************************************************************************************/
// Object Creation Method

void *svf_new(t_symbol *msg, short argc, t_atom *argv)
{
	short i;
    t_svf *x; 
    
    x = (t_svf *)object_alloc(svf_class);;
    if(x){
   		object_obex_store((void *)x, _sym_dumpout, (object *)outlet_new(x,NULL));	// dumpout 
	    dsp_setup((t_pxobject *)x, 2);				// Create Object and 2 Inlets
	    x->x_obj.z_misc = Z_NO_INPLACE;  			// THIS OBJECT FARTS LIKE MAD WITHOUT THIS    
	    outlet_new((t_pxobject *)x, "signal");		// Create a signal Outlet
	    outlet_new((t_pxobject *)x, "signal");		// Create a signal Outlet

		x->attr_lfo_freq_shape = gensym("sine");

		// Set the SR & Vector Size
		//taptools_max::set_global_sr(sys_getsr());
		//taptools_max:: set_global_vectorsize(sys_getblksize());

		// Create the TapTools Objects
		x->attr_type = ps_bandpass;
		for(i=0; i<NUM_FILTERS; i++){
			x->filter[i] = new tt_svf;
			x->filter[i]->set_attr(tt_svf::k_mode, tt_svf::k_mode_bandpass);
		}
		x->lfo = new tt_lfo;	
		x->portamento = new tt_ramp;	// create a ramper - defaults to 0.0 and vector accuracy
		//x->portamento->set_attr(tt_ramp::k_mode, tt_ramp::k_mode_sample_accurate);
		x->attr_portamento_time = 5.0;
		
		// Create the Audio Signals ("patch cords")
		for(i=0; i<NUM_AUDIO_SIGNALS; i++){
			x->audio_sig[i] = new tt_audio_signal(sys_getblksize());
		}
		for(i=0; i<NUM_INPUTS; i++)
			x->signal_in[i] = new tt_audio_signal;
		for(i=0; i<NUM_OUTPUTS; i++)
			x->signal_out[i] = new tt_audio_signal;
		
		// default values
		x->attr_lfo_freq_depth = 1.0;
		x->attr_lfo_freq_speed = 0.0;
		
		
		attr_args_process(x,argc,argv); //handle attribute args			
	}
	return (x);									// Return the pointer
	#pragma unused(msg)
}

// Memory Deallocation
void svf_free(t_svf *x)
{
	short i;
	dsp_free((t_pxobject *)x);
	
	for(i=0; i<NUM_FILTERS; i++)
		delete x->filter[i];
	delete x->lfo;
	delete x->portamento;
	for(i=0; i<NUM_AUDIO_SIGNALS; i++)
		delete x->audio_sig[i];
	for(i=0; i<NUM_INPUTS; i++)
		delete x->signal_in[i];
	for(i=0; i<NUM_OUTPUTS; i++)
		delete x->signal_out[i];
}


/************************************************************************************/
// Methods bound to input/inlets

// Method for Assistance Messages
void svf_assist(t_svf *x, void *b, long msg, long arg, char *dst)
{
	if(msg==1){ 	// Inlets
		switch(arg){
			case 0: strcpy(dst, "(signal) input, control messages"); break;
			case 1: strcpy(dst, "(signal) input"); break;
		}
	}
	else if(msg==2){ // Outlets
		switch(arg){
			case 0: strcpy(dst, "(signal) filtered output"); break;
			case 1: strcpy(dst, "(signal) filtered output"); break;		
			case 2: strcpy(dst, "dumpout"); break;
		}	
	}
	#pragma unused(x)
	#pragma unused(b)
	#pragma unused(arg)
}


void svf_clear(t_svf *x)
{
	x->filter[LEFT]->clear();
	x->filter[RIGHT]->clear();
}


// ATTRIBUTE: type of filter to use
t_max_err attr_set_type(t_svf *x, void *attr, long argc, t_atom *argv)
{
	if(argc && argv){
		x->attr_type = atom_getsym(argv);
		if(x->attr_type == ps_lowpass){
			x->filter[LEFT]->set_attr(tt_svf::k_mode, tt_svf::k_mode_lowpass);
			x->filter[RIGHT]->set_attr(tt_svf::k_mode, tt_svf::k_mode_lowpass);
		}
		else if(x->attr_type == ps_highpass){ 
			x->filter[LEFT]->set_attr(tt_svf::k_mode, tt_svf::k_mode_highpass);
			x->filter[RIGHT]->set_attr(tt_svf::k_mode, tt_svf::k_mode_highpass);
		}
		else if(x->attr_type == ps_bandpass){
			x->filter[LEFT]->set_attr(tt_svf::k_mode, tt_svf::k_mode_bandpass);
			x->filter[RIGHT]->set_attr(tt_svf::k_mode, tt_svf::k_mode_bandpass);
		}
		else if(x->attr_type == ps_notch){
			x->filter[LEFT]->set_attr(tt_svf::k_mode, tt_svf::k_mode_notch);
			x->filter[RIGHT]->set_attr(tt_svf::k_mode, tt_svf::k_mode_notch);
		}
		else if(x->attr_type == ps_peak){
			x->filter[LEFT]->set_attr(tt_svf::k_mode, tt_svf::k_mode_peak);
			x->filter[RIGHT]->set_attr(tt_svf::k_mode, tt_svf::k_mode_peak);
		}
		else
			object_error((t_object *)x, "invalid type specified");
	}
	return MAX_ERR_NONE;
	#pragma unused(attr)
}


// ATTRIBUTE: frequency
t_max_err attr_set_frequency(t_svf *x, void *attr, long argc, t_atom *argv)
{
	x->attr_frequency = atom_getfloat(argv);
	if(x->attr_lfo_freq_depth > (x->attr_frequency - 1))
		x->attr_lfo_freq_depth = x->attr_frequency - 1;
	TTClip(x->attr_frequency, 0.0, 500.0);

	x->portamento->set_attr(tt_ramp::k_destination_value, x->attr_frequency);
	x->portamento->set_attr(tt_ramp::k_ramp_ms, x->attr_portamento_time);		// This implicitly triggers the ramp

	return MAX_ERR_NONE;
	#pragma unused(attr)	
}


// ATTRIBUTE: resonance
t_max_err attr_set_resonance(t_svf *x, void *attr, long argc, t_atom *argv)
{
	short i;
	x->attr_resonance = atom_getfloat(argv);
	for(i=0; i<NUM_FILTERS; i++)
		x->filter[i]->set_attr(tt_svf::k_resonance, x->attr_resonance);
	
	return MAX_ERR_NONE;
	#pragma unused(attr)
}


// ATTRIBUTE: depth
t_max_err attr_set_lfo_freq_depth(t_svf *x, void *attr, long argc, t_atom *argv)
{
	x->attr_lfo_freq_depth = atom_getfloat(argv);
	if(x->attr_lfo_freq_depth > (x->attr_frequency - 1))
		x->attr_lfo_freq_depth = x->attr_frequency - 1;
	x->lfo->set_attr(tt_lfo::k_depth, x->attr_lfo_freq_depth);		
	
	return MAX_ERR_NONE;
	#pragma unused(attr)
}


// ATTRIBUTE: speed
t_max_err attr_set_lfo_freq_speed(t_svf *x, void *attr, long argc, t_atom *argv)
{
	x->attr_lfo_freq_speed = TTClip<double>(atom_getfloat(argv), 0.0f, 200.0f);
	x->lfo->set_attr(tt_lfo::k_frequency, x->attr_lfo_freq_speed);		
	
	return MAX_ERR_NONE;
	#pragma unused(attr)
}


// ATTRIBUTE: set the shape of the LFO
t_max_err attr_set_lfo_freq_shape(t_svf *x, void *attr, long argc, t_atom *argv)
{
	if(argc && argv){
		x->attr_lfo_freq_shape = atom_getsym(argv);
		if(x->attr_lfo_freq_shape == gensym("sine")) 
			x->lfo->set_attr(tt_lfo::k_mode, tt_lfo::k_mode_sine);
		else if(x->attr_lfo_freq_shape == gensym("triangle")) 
			x->lfo->set_attr(tt_lfo::k_mode, tt_lfo::k_mode_triangle);
		else if(x->attr_lfo_freq_shape == gensym("square")) 
			x->lfo->set_attr(tt_lfo::k_mode, tt_lfo::k_mode_square);
		else if(x->attr_lfo_freq_shape == gensym("ramp")) 
			x->lfo->set_attr(tt_lfo::k_mode, tt_lfo::k_mode_ramp);
		else if(x->attr_lfo_freq_shape == gensym("sawtooth")) 
			object_error((t_object *)x, "sorry - sawtooth isn't quite ready...");
		else
			object_error((t_object *)x, "invalid mode specified");
	}
	return MAX_ERR_NONE;
	#pragma unused(attr)
}


// set the portamento time
void svf_portamento(t_svf *x, double value)
{
	x->attr_portamento_time = value;
}


void svf_perform_mono64(t_svf *x, t_object *dsp64, double **ins, long numins, double **outs, long numouts, long sampleframes, long flags, void *userparam)
{
    x->signal_in[0]->set_vector(ins[0]);
    x->signal_out[0]->set_vector(outs[0]);
	x->signal_in[0]->vectorsize = sampleframes;
			
	// Run the portamento for the filter cf
	x->portamento->dsp_vector_calc(x->audio_sig[0]);
	x->freq = *(x->audio_sig[0]->vector);		// the portamento is only vector accurate, so only the first value is needed

	// Apply the modulation to the cf of the filters
	if(x->attr_lfo_freq_toggle){
		x->lfo->dsp_vector_calc(x->audio_sig[0]);
		x->freq += (*(x->audio_sig[0]->vector));	
	}	

	x->filter[LEFT]->set_attr(tt_svf::k_frequency, x->freq);
	x->filter[LEFT]->dsp_vector_calc(x->signal_in[0], x->signal_out[0]);
}


void svf_perform64(t_svf *x, t_object *dsp64, double **ins, long numins, double **outs, long numouts, long sampleframes, long flags, void *userparam)
{
    x->signal_in[0]->set_vector(ins[0]); 	// Input
    x->signal_in[1]->set_vector(ins[1]); 	// Input
    x->signal_out[0]->set_vector(outs[0]);	// Output
    x->signal_out[1]->set_vector(outs[1]);	// Output
	x->signal_in[0]->vectorsize = x->signal_in[1]->vectorsize = sampleframes;

	// Run the portamento for the filter cf
	x->portamento->dsp_vector_calc(x->audio_sig[0]);
	x->freq = *(x->audio_sig[0]->vector);		// the portamento is only vector accurate, so only the first value is needed

	// Apply the modulation to the cf of the filters
	if(x->attr_lfo_freq_toggle){
		x->lfo->dsp_vector_calc(x->audio_sig[0]);
		x->freq += (*(x->audio_sig[0]->vector));	
	}	
	x->filter[LEFT]->set_attr(tt_svf::k_frequency, x->freq);
	x->filter[RIGHT]->set_attr(tt_svf::k_frequency, x->freq);

	x->filter[LEFT]->dsp_vector_calc(x->signal_in[0], x->signal_out[0]);
	x->filter[RIGHT]->dsp_vector_calc(x->signal_in[1], x->signal_out[1]);
}


void svf_dsp64(t_svf *x, t_object *dsp64, short *count, double samplerate, long maxvectorsize, long flags)
{
	short i;
	
	// Set sample-rate and vectorsize for member objects
	for(i=0; i<NUM_FILTERS; i++){
		x->filter[i]->set_sr(samplerate);
		x->filter[i]->set_vectorsize(maxvectorsize);
		x->filter[i]->clear();
	}
	x->lfo->set_sr(samplerate);
	x->lfo->set_vectorsize(maxvectorsize);
	x->portamento->set_sr(samplerate);
	x->portamento->set_vectorsize(maxvectorsize);
	
	// Re-allocate audio signals
	for(i=0; i<NUM_AUDIO_SIGNALS; i++){
		x->audio_sig[i]->alloc(maxvectorsize);
	}
	
	// add correct perform routine to MSP's call chain
	if (count[1] && count[3])	// STEREO
		object_method(dsp64, gensym("dsp_add64"), (t_object*)x, svf_perform64, 0, NULL);
	else
		object_method(dsp64, gensym("dsp_add64"), (t_object*)x, svf_perform_mono64, 0, NULL);
}

