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
#include "../ttblue/tt_limiter.h"
#include "../ttblue/tt_clip.h"
#include "../ttblue/tt_copy.h"
#include "../ttblue/tt_dcblock.h"
#include "../ttblue/tt_downsample.h"
#include "../ttblue/tt_upsample.h"
#include "../ttblue/tt_comb.h"
#include "../ttblue/tt_lfo.h"
#include "../ttblue/tt_allpass.h"
#include "../ttblue/tt_lowpass_onepole.h"
#include "../ttblue/tt_mixer_mono.h"
#include "../ttblue/tt_crossfade.h"
#include "../ttblue/tt_gain.h"
#include "../ttblue/tt_verb.h"

#define NUM_INPUTS 2
#define NUM_OUTPUTS 2
#define NUM_TEMP_SIGNALS 3
#define LEFT 0
#define RIGHT 1

// Data Structure for this object
typedef struct _verb						
{
    t_pxobject 			x_obj;
	short				vector_size;
	int					sr;
	tt_limiter			*limiter;
	tt_clip				*clipper;
	tt_copy				*copy;
	tt_dcblock			*dcblocker;
	tt_downsample		*downsampler;
	tt_upsample			*upsampler;
	double				attr_mix;
	double				attr_gain;
	long				attr_bypass;
	long				attr_mute;
	long				attr_clip;
	long				attr_limit;
	double				attr_limiter_threshold;
	double				attr_limiter_lookahead;
	double				attr_limiter_release;
	long				attr_dcblock;
	long				attr_downsample;
	short				downsample;
	
	tt_verb			*reverb[2];					// taptools reverb (stereo pair)
	tt_audio_signal	*signal[NUM_TEMP_SIGNALS];	 
    tt_audio_signal	*signal_in[NUM_INPUTS];
    tt_audio_signal	*signal_out[NUM_OUTPUTS];
												// OBJECT-SPECIFIC ATTRIBUTES
	double			attr_delay;						// delay time in ms
	double			attr_decay;						// decay time in seconds
	double			attr_damping;					// cf freq in hz for damping filter
	double			attr_moddepth;					// depth of LFO
	double			attr_modfreq;					// freq of LFO
	double			attr_lowpass;					// cf freq in hz for master output filter
	long			attr_use_er;					// toggle for early-reflections
} t_verb;


// Prototypes for methods: need a method for each incoming message type
void *verb_new(t_symbol *msg, long argc, t_atom *argv);						// Instance constructor
void verb_free(t_verb *x);														// Instance destructor
void verb_perform64(t_verb *x, t_object *dsp64, double **ins, long numins, double **outs, long numouts, long sampleframes, long flags, void *userparam);													// Perform (vector calculation) method
void verb_perform_stereo64(t_verb *x, t_object *dsp64, double **ins, long numins, double **outs, long numouts, long sampleframes, long flags, void *userparam);											// Perform (vector calculation) method
void verb_dsp64(t_verb *x, t_object *dsp64, short *count, double samplerate, long maxvectorsize, long flags);							// ../../../../Jamoma/Core/DSP/library/build/JamomaDSP.dylib-Chain construction method
void verb_assist(t_verb *x, void *b, long msg, long arg, char *dst);			// Assistance method
void verb_clear(t_verb *x);														// Reset/Refresh method
void verb_alloc(t_verb *x, int vector_size);
t_max_err attr_set_mix(t_verb *x, void *attr, long argc, t_atom *argv);
t_max_err attr_set_gain(t_verb *x, void *attr, long argc, t_atom *argv);
t_max_err attr_set_bypass(t_verb *x, void *attr, long argc, t_atom *argv);
t_max_err attr_set_mute(t_verb *x, void *attr, long argc, t_atom *argv);
t_max_err attr_set_clip(t_verb *x, void *attr, long argc, t_atom *argv);
t_max_err attr_set_limit(t_verb *x, void *attr, long argc, t_atom *argv);
t_max_err attr_set_limiter_threshold(t_verb *x, void *attr, long argc, t_atom *argv);
t_max_err attr_set_limiter_lookahead(t_verb *x, void *attr, long argc, t_atom *argv);
t_max_err attr_set_limiter_release(t_verb *x, void *attr, long argc, t_atom *argv);
t_max_err attr_set_dcblock(t_verb *x, void *attr, long argc, t_atom *argv);
t_max_err attr_set_downsample(t_verb *x, void *attr, long argc, t_atom *argv);
void do_downsample(t_verb *x, long value);
t_max_err attr_set_delay(t_verb *x, void *attr, long argc, t_atom *argv);		// OBJECT SPECIFIC ATTRIBUTES
t_max_err attr_set_decay(t_verb *x, void *attr, long argc, t_atom *argv);
t_max_err attr_set_damping(t_verb *x, void *attr, long argc, t_atom *argv);
t_max_err attr_set_modfreq(t_verb *x, void *attr, long argc, t_atom *argv);
t_max_err attr_set_moddepth(t_verb *x, void *attr, long argc, t_atom *argv);
t_max_err attr_set_lowpass(t_verb *x, void *attr, long argc, t_atom *argv);
t_max_err attr_set_er(t_verb *x, void *attr, long argc, t_atom *argv);
void 		verb_anything(t_verb *x, t_symbol *msg, long argc, t_atom *argv);


// Globals
static t_class *verb_class;							// Required. Global pointing to this class


/************************************************************************************/
// Main() Function

extern "C" int C74_EXPORT main(void)
{
	t_class *c;
	
	c = class_new("tap.verb~",(method)verb_new, (method)verb_free, sizeof(t_verb), 
		(method)0L, A_GIMME, 0);

		common_symbols_init();								// Initialize TapTools

	// ADD ATTRIBUTES
	CLASS_ATTR_DOUBLE(c,		"delay",		0,	t_verb, attr_delay);
	CLASS_ATTR_ACCESSORS(c,	"delay",		NULL, attr_set_delay);

	CLASS_ATTR_DOUBLE(c,		"decay",		0,	t_verb, attr_decay);
	CLASS_ATTR_ACCESSORS(c,	"decay",		NULL, attr_set_decay);

	CLASS_ATTR_DOUBLE(c,		"damping",		0,	t_verb, attr_damping);
	CLASS_ATTR_ACCESSORS(c,	"damping",		NULL, attr_set_damping);

	CLASS_ATTR_DOUBLE(c,		"modfreq",		0,	t_verb, attr_modfreq);
	CLASS_ATTR_ACCESSORS(c,	"modfreq",		NULL, attr_set_modfreq);

	CLASS_ATTR_DOUBLE(c,		"moddepth",		0,	t_verb, attr_moddepth);
	CLASS_ATTR_ACCESSORS(c,	"moddepth",		NULL, attr_set_moddepth);

	CLASS_ATTR_DOUBLE(c,		"lowpass",		0,	t_verb, attr_lowpass);
	CLASS_ATTR_ACCESSORS(c,	"lowpass",		NULL, attr_set_lowpass);

	CLASS_ATTR_DOUBLE(c,		"use_early_reflections",		0,	t_verb, attr_use_er);
	CLASS_ATTR_ACCESSORS(c,	"use_early_reflections",		NULL, attr_set_er);
	CLASS_ATTR_STYLE(c,		"use_early_reflections",		0,	"onoff");

	CLASS_ATTR_LONG(c,		"bypass",		0,	t_verb, attr_bypass);
	CLASS_ATTR_ACCESSORS(c,	"bypass",		NULL, attr_set_bypass);
	CLASS_ATTR_STYLE(c,		"bypass",		0,	"onoff");
	
	CLASS_ATTR_LONG(c,		"mute",		0,	t_verb, attr_mute);
	CLASS_ATTR_ACCESSORS(c,	"mute",		NULL, attr_set_mute);
	CLASS_ATTR_STYLE(c,		"mute",		0,	"onoff");
	
	CLASS_ATTR_LONG(c,		"clip",		0,	t_verb, attr_clip);
	CLASS_ATTR_ACCESSORS(c,	"clip",		NULL, attr_set_clip);
	CLASS_ATTR_STYLE(c,		"clip",		0,	"onoff");
	
	CLASS_ATTR_LONG(c,		"limit",		0,	t_verb, attr_limit);
	CLASS_ATTR_ACCESSORS(c,	"limit",		NULL, attr_set_limit);
	CLASS_ATTR_STYLE(c,		"limit",		0,	"onoff");
	
	CLASS_ATTR_DOUBLE(c,		"limiter_threshold",		0,	t_verb, attr_limiter_threshold);
	CLASS_ATTR_ACCESSORS(c,	"limiter_threshold",		NULL, attr_set_limiter_threshold);
	
	CLASS_ATTR_DOUBLE(c,		"limiter_lookahead",		0,	t_verb, attr_limiter_lookahead);
	CLASS_ATTR_ACCESSORS(c,	"limiter_lookahead",		NULL, attr_set_limiter_lookahead);
	
	CLASS_ATTR_DOUBLE(c,		"limiter_release",		0,	t_verb, attr_limiter_release);
	CLASS_ATTR_ACCESSORS(c,	"limiter_release",		NULL, attr_set_limiter_release);
	
	CLASS_ATTR_LONG(c,		"dcblock",		0,	t_verb, attr_dcblock);
	CLASS_ATTR_ACCESSORS(c,	"dcblock",		NULL, attr_set_dcblock);
	CLASS_ATTR_STYLE(c,		"dcblock",		0,	"onoff");
	
	CLASS_ATTR_DOUBLE(c,		"downsample",		0,	t_verb, attr_downsample);
	CLASS_ATTR_ACCESSORS(c,	"downsample",		NULL, attr_set_downsample);

	CLASS_ATTR_DOUBLE(c,		"mix",		0,	t_verb, attr_mix);
	CLASS_ATTR_ACCESSORS(c,	"mix",		NULL, attr_set_mix);
	
	CLASS_ATTR_DOUBLE(c,		"gain",		0,	t_verb, attr_gain);
	CLASS_ATTR_ACCESSORS(c,	"gain",		NULL, attr_set_gain);

	// ADD METHODS
	class_addmethod(c, (method)verb_dsp64, "dsp64", A_CANT, 0);
    class_addmethod(c, (method)verb_assist, 				"assist", 				A_CANT, 0L); 
	class_addmethod(c, (method)verb_clear, 					"clear", 				0L);			// clear out any blown-up filters
	class_addmethod(c, (method)attr_set_delay,				"/delay",				A_GIMME, 0);
	class_addmethod(c, (method)attr_set_decay,				"/decay",				A_GIMME, 0);
	class_addmethod(c, (method)attr_set_damping,			"/damping",				A_GIMME, 0);
	class_addmethod(c, (method)attr_set_modfreq,			"/modfreq",				A_GIMME, 0);
	class_addmethod(c, (method)attr_set_moddepth,			"/moddepth",			A_GIMME, 0);
	class_addmethod(c, (method)attr_set_lowpass,			"/lowpass",				A_GIMME, 0);
	class_addmethod(c, (method)attr_set_er,					"/early_reflections",	A_GIMME, 0);
	class_addmethod(c, (method)attr_set_clip,				"/clip",				A_GIMME, 0);
	class_addmethod(c, (method)attr_set_limit,				"/limiter/enable",		A_GIMME, 0);
	class_addmethod(c, (method)attr_set_limiter_threshold,	"/limiter/threshold",	A_GIMME, 0);
	class_addmethod(c, (method)attr_set_limiter_lookahead,	"/limiter/lookahead",	A_GIMME, 0);
	class_addmethod(c, (method)attr_set_limiter_release,	"/limiter/release",		A_GIMME, 0);
	class_addmethod(c, (method)attr_set_dcblock,			"/dcblock",				A_GIMME, 0);
	class_addmethod(c, (method)attr_set_mix,				"/mix",					A_GIMME, 0);
	class_addmethod(c, (method)attr_set_gain,				"/gain",				A_GIMME, 0);
	class_addmethod(c, (method)attr_set_mute,				"/audio/mute",			A_GIMME, 0);
	class_addmethod(c, (method)verb_anything,				"anything",				A_GIMME, 0);
	class_addmethod(c, (method)stdinletinfo,				"inletinfo",			A_CANT, 0);

	// WRAP UP
    class_dspinit(c);									// Setup object's class to work with MSP
	class_register(_sym_box, c); 	
	verb_class = c;
}


/************************************************************************************/
// Object Life

// Creation
void *verb_new(t_symbol *msg, long argc, t_atom *argv)
{
	t_verb	*x;
	short	i;
	
	x = (t_verb *)object_alloc(verb_class);;
	if(x){
		object_obex_store((void *)x, (t_symbol *) _sym_dumpout, (object *)outlet_new(x, NULL));	// dumpout outlet. could cache in struct too
		dsp_setup((t_pxobject *)x, 2);								// 2 inlets
	    x->x_obj.z_misc = Z_NO_INPLACE + Z_PUT_FIRST;  							// ESSENTIAL!   
		outlet_new((t_object *)x, "signal");						// 2 outlets (in addition to dumpout)...
		outlet_new((t_object *)x, "signal");

		x->sr = sys_getsr();										// Set Tap.Tools global SR...
		//taptools_max::set_global_sr(x->sr);
		
		x->vector_size = sys_getblksize();							// Set Tap.Tools global vector size...
		//taptools_max:: set_global_vectorsize(x->vector_size);
		
		for(i=0; i < NUM_TEMP_SIGNALS; i++)							// allocate temp signals with the global vs...
			x->signal[i] = new tt_audio_signal(x->vector_size);	
		for(i=0; i<NUM_INPUTS; i++)
			x->signal_in[i] = new tt_audio_signal;
		for(i=0; i<NUM_OUTPUTS; i++)
			x->signal_out[i] = new tt_audio_signal;
		
		// Create Tap.Tools Objects
		x->reverb[LEFT] = new tt_verb;								// tt_verb core object
		x->reverb[RIGHT] = new tt_verb;

		x->clipper = new tt_clip;
		x->limiter = new tt_limiter;
		x->copy = new tt_copy;
		x->dcblocker = new tt_dcblock;
		x->downsampler = new tt_downsample;
		x->upsampler = new tt_upsample;
		
		// Set defaults
		x->attr_bypass = 0;
		x->attr_clip = 0;
		x->attr_damping = 20000;
		x->attr_dcblock = 1;
		x->attr_decay = 3.5;
		x->attr_delay = 100;
		x->attr_downsample = 0;
		x->attr_gain = 0;
		x->attr_limit = 1;
		x->attr_limiter_lookahead = x->limiter->get_attr(tt_limiter::k_lookahead);
		x->attr_limiter_release = x->limiter->get_attr(tt_limiter::k_release);
		x->attr_limiter_threshold = x->limiter->get_attr(tt_limiter::k_threshold);
		x->attr_lowpass = 15000;
		x->attr_mix = 100;
		x->attr_moddepth = 0.1;
		x->attr_modfreq	= 0.1;	
		x->attr_mute = 0;
		x->attr_use_er = 1;
		x->downsample = 1;
		
		attr_args_process(x,argc,argv); //handle attribute args	
	}
	
	return (x);										// Return the pointer
	#pragma unused(msg)
}

// Destruction
void verb_free(t_verb *x)
{
	short i;
	dsp_free((t_pxobject *)x);
	delete x->reverb[LEFT];
	delete x->reverb[RIGHT];
	delete x->clipper;
	delete x->limiter;
	delete x->copy;
	delete x->dcblocker;
	delete x->downsampler;
	delete x->upsampler;
	
	// FREE THE TEMP VECTORS
	for(i=0; i<NUM_TEMP_SIGNALS; i++)
		delete x->signal[i];
	for(i=0; i<NUM_INPUTS; i++)
		delete x->signal_in[i];
	for(i=0; i<NUM_OUTPUTS; i++)
		delete x->signal_out[i];	
}




/************************************************************************************/
// Methods bound to input/inlets

// Method for Assistance Messages
void verb_assist(t_verb *x, void *b, long msg, long arg, char *dst)
{
	if(msg==1) 	// Inlets
		strcpy(dst, "(signal) input, control messages");		
	else if(msg==2) // Outlets
		strcpy(dst, "(signal) Filtered output");
	#pragma unused(x)
	#pragma unused(b)
	#pragma unused(arg)
}


// Set base delay time
t_max_err attr_set_delay(t_verb *x, void *attr, long argc, t_atom *argv)
{
	if(argc && argv){
		x->attr_delay = atom_getfloat(argv);
		x->reverb[LEFT]->set_attr(tt_verb::k_delay, x->attr_delay);
		x->reverb[RIGHT]->set_attr(tt_verb::k_delay, x->attr_delay);
	}
	return MAX_ERR_NONE;
	#pragma unused(attr)
}


// Set the decay time
t_max_err attr_set_decay(t_verb *x, void *attr, long argc, t_atom *argv)
{
	if(argc && argv){
		x->attr_decay = atom_getfloat(argv);
		x->reverb[LEFT]->set_attr(tt_verb::k_decay, x->attr_decay);
		x->reverb[RIGHT]->set_attr(tt_verb::k_decay, x->attr_decay);
	}
	return MAX_ERR_NONE;
	#pragma unused(attr)	
}


// Set damping filter cutoff
t_max_err attr_set_damping(t_verb *x, void *attr, long argc, t_atom *argv)
{
	if(argc && argv){
		x->attr_damping = atom_getfloat(argv);
		x->reverb[LEFT]->set_attr(tt_verb::k_damping, x->attr_damping);
		x->reverb[RIGHT]->set_attr(tt_verb::k_damping, x->attr_damping);
	}
	return MAX_ERR_NONE;
	#pragma unused(attr)	
}


// Set base LFO freq
t_max_err attr_set_modfreq(t_verb *x, void *attr, long argc, t_atom *argv)
{
	if(argc && argv){
		x->attr_modfreq = atom_getfloat(argv);
		x->reverb[LEFT]->set_attr(tt_verb::k_modfreq, x->attr_modfreq);
		x->reverb[RIGHT]->set_attr(tt_verb::k_modfreq, x->attr_modfreq);
	}
	return MAX_ERR_NONE;
	#pragma unused(attr)	
}


// Set base LFO depth
t_max_err attr_set_moddepth(t_verb *x, void *attr, long argc, t_atom *argv)
{
	if(argc && argv){
		x->attr_moddepth = atom_getfloat(argv);
		x->reverb[LEFT]->set_attr(tt_verb::k_moddepth, x->attr_moddepth);
		x->reverb[RIGHT]->set_attr(tt_verb::k_moddepth, x->attr_moddepth);
	}
	return MAX_ERR_NONE;
	#pragma unused(attr)	
}


// Set lowpass cutoff
t_max_err attr_set_lowpass(t_verb *x, void *attr, long argc, t_atom *argv)
{
	if(argc && argv){
		x->attr_lowpass = atom_getfloat(argv);
		x->reverb[LEFT]->set_attr(tt_verb::k_lowpass, x->attr_lowpass);
		x->reverb[RIGHT]->set_attr(tt_verb::k_lowpass, x->attr_lowpass);
	}
	return MAX_ERR_NONE;
	#pragma unused(attr)	
}


// Set use_early_reflections
t_max_err attr_set_er(t_verb *x, void *attr, long argc, t_atom *argv)
{
	if(argc && argv){
		x->attr_use_er = atom_getlong(argv);
		x->reverb[LEFT]->set_attr(tt_verb::k_use_early_reflections, x->attr_use_er);
		x->reverb[RIGHT]->set_attr(tt_verb::k_use_early_reflections, x->attr_use_er);
	}
	return MAX_ERR_NONE;
	#pragma unused(attr)	
}


void verb_perform64(t_verb *x, t_object *dsp64, double **ins, long numins, double **outs, long numouts, long sampleframes, long flags, void *userparam)
{
    x->signal_in[0]->set_vector(ins[0]); 	// Input
    x->signal_out[0]->set_vector(outs[0]);	// Output
	short vs = x->signal_in[0]->vectorsize = x->signal_out[0]->vectorsize = sampleframes;	// Vector Size

	if (x->attr_mute){
		while(x->signal_in[0]->vectorsize--)
			*x->signal_out[0]->vector++ = 0.0;
		goto out;
	}
	if (x->attr_bypass){
		while(x->signal_in[0]->vectorsize--)
			*x->signal_out[0]->vector++ = *x->signal_in[0]->vector++;
		goto out;
	}

	// Because all of our temp vectors need to be set for downsampling, and this is different for every object,
	//	the downsamping has to be handled explicitly (it can't be tucked nicely into a macro)
	if(x->attr_downsample){	
		x->downsampler->dsp_vector_calc(x->signal_in[0], x->signal[1]);	// Downsample
		vs /= x->downsample;
		x->signal_in[0]->vectorsize = vs;								// Change signal to new vector size
		x->signal_out[0]->vectorsize = vs;								// Change to new vector size
		x->signal[1]->vectorsize = vs;
		x->signal[0]->vectorsize = vs;
		x->copy->dsp_vector_calc(x->signal[1], x->signal_in[0]);		// Move downsampled vector back to "in"
	}
	
	
	/***************************
	 * OBJECT SPECIFIC 
	 ***************************/	 
	x->reverb[LEFT]->dsp_vector_calc(x->signal_in[0], x->signal[0]);	// Run the core reverb routine


	if(x->attr_limit) x->limiter->dsp_vector_calc(x->signal[0], x->signal_out[0]);
	else if(x->attr_clip) x->clipper->dsp_vector_calc(x->signal[0], x->signal_out[0]);
	else x->copy->dsp_vector_calc(x->signal[0], x->signal_out[0]);
	if(x->attr_dcblock){
		x->copy->dsp_vector_calc(x->signal_out[0], x->signal[0]);
		x->dcblocker->dsp_vector_calc(x->signal[0], x->signal_out[0]);
	}
	if(x->attr_downsample){
		vs *= x->downsample;
		x->signal_out[0]->vectorsize = x->signal[0]->vectorsize = vs;
		x->copy->dsp_vector_calc(x->signal_out[0], x->signal[0]);
		x->upsampler->dsp_vector_calc(x->signal[0], x->signal_out[0]);
	}
out:
    return;
}


void verb_perform_stereo64(t_verb *x, t_object *dsp64, double **ins, long numins, double **outs, long numouts, long sampleframes, long flags, void *userparam)
{
   	
    x->signal_in[0]->set_vector(ins[0]); 	// Input
    x->signal_in[1]->set_vector(ins[1]); 	// Input
    x->signal_out[0]->set_vector(outs[0]);	// Output
    x->signal_out[1]->set_vector(outs[1]);	// Output
	short vs = x->signal_in[0]->vectorsize = x->signal_in[1]->vectorsize = 
		x->signal_out[0]->vectorsize = x->signal_out[1]->vectorsize = sampleframes;	// Vector Size

	if (x->attr_mute){
		while(x->signal_in[0]->vectorsize--){
			*x->signal_out[0]->vector++ = 0.0;
			*x->signal_out[1]->vector++ = 0.0;
		}
		goto out;
	}
	if (x->attr_bypass){
		while(x->signal_in[0]->vectorsize--){
			*x->signal_out[0]->vector++ = *x->signal_in[0]->vector++;
			*x->signal_out[1]->vector++ = * x->signal_in[1]->vector++;
		}
		goto out;
	}

	// Because all of our temp vectors need to be set for downsampling, and this is different for every object,
	//	the downsamping has to be handled explicitly (it can't be tucked nicely into a macro)
	if(x->attr_downsample){	
		x->downsampler->dsp_vector_calc(x->signal_in[0], x->signal_in[1], x->signal[1], x->signal[2]);		// Downsample
		vs /= x->downsample;
		x->signal_in[0]->vectorsize = x->signal_in[1]->vectorsize = vs;				// Change signal to new vector size
		x->signal_out[0]->vectorsize = x->signal_out[1]->vectorsize = vs;			// Change to new vector size
		x->signal[2]->vectorsize = vs;
		x->signal[1]->vectorsize = vs;
		x->signal[0]->vectorsize = vs;
		x->copy->dsp_vector_calc(x->signal[1], x->signal[2], x->signal_in[0], x->signal_in[1]);			// Move downsampled vector back to "in"
	}
	
	
	/***************************
	 * OBJECT SPECIFIC 
	 ***************************/	 
	x->reverb[LEFT]->dsp_vector_calc(x->signal_in[0], x->signal[0]);	// Run the core reverb routine
	x->reverb[RIGHT]->dsp_vector_calc(x->signal_in[1], x->signal[1]);

	// Macro for Standard Tap.Tools Blue Stuff
	if(x->attr_limit) x->limiter->dsp_vector_calc(x->signal[0], x->signal[1], x->signal_out[0], x->signal_out[1]);
	else if(x->attr_clip) x->clipper->dsp_vector_calc(x->signal[0], x->signal[1], x->signal_out[0], x->signal_out[1]);
	else x->copy->dsp_vector_calc(x->signal[0], x->signal[1], x->signal_out[0], x->signal_out[1]);
	if(x->attr_dcblock){
		x->copy->dsp_vector_calc(x->signal_out[0], x->signal_out[1], x->signal[0], x->signal[1]);
		x->dcblocker->dsp_vector_calc(x->signal[0], x->signal[1], x->signal_out[0], x->signal_out[1]);
	}
	if(x->attr_downsample){
		vs *= x->downsample;
		x->signal_out[0]->vectorsize = x->signal_out[1]->vectorsize = x->signal[0]->vectorsize = x->signal[1]->vectorsize = vs;
		x->copy->dsp_vector_calc(x->signal_out[0], x->signal_out[1], x->signal[0], x->signal[1]);
		x->upsampler->dsp_vector_calc(x->signal[0], x->signal[1], x->signal_out[0], x->signal_out[1]);
	}
out:
    return;
}


void verb_dsp64(t_verb *x, t_object *dsp64, short *count, double samplerate, long maxvectorsize, long flags)
{
	int vs = maxvectorsize;		// Vector Size
	x->sr = samplerate;		// Sample Rate
	
	x->reverb[LEFT]->set_sr(x->sr);
	x->reverb[RIGHT]->set_sr(x->sr);
	
	verb_alloc(x, vs);
	do_downsample(x, x->attr_downsample);	// just make sure our vectorsizes, etc. are all in sync
	
	if (count[1] && count[3])
		object_method(dsp64, gensym("dsp_add64"), (t_object*)x, verb_perform_stereo64, 0, NULL);
	else
		object_method(dsp64, gensym("dsp_add64"), (t_object*)x, verb_perform64, 0, NULL);
}


// MEMORY ALLOCATION
void verb_alloc(t_verb *x, int vector_size)
{
	int i;
	
	// Memory Allocation for Temporary Vectors
	if(x->vector_size != vector_size){			// If the vector size has changed...
		x->vector_size = vector_size;			// Store the new vector size in our struct
		x->reverb[LEFT]->set_vectorsize(vector_size);
		x->reverb[RIGHT]->set_vectorsize(vector_size);
		for(i=0; i<NUM_TEMP_SIGNALS; i++){
			x->signal[i]->alloc(vector_size);	// Allocate memory and store addresses
		}
	}
   	verb_clear(x);
}


// CLEAR
void verb_clear(t_verb *x)
{
	x->reverb[LEFT]->clear();
	x->reverb[RIGHT]->clear();
	x->limiter->clear();		// TAPTOOLS BLUE - There is no macro for this, but it is recommended...
	x->dcblocker->clear();
}

/************************************************************************************/
// TAP.TOOLS BLUE - STANDARD ATTRIBUTES / METHODS / FUNCTIONS

t_max_err attr_set_bypass(t_verb *x, void *attr, long argc, t_atom *argv){
	if(argc && argv){
		x->attr_bypass = atom_getlong(argv);
	}
	return MAX_ERR_NONE;
}

t_max_err attr_set_mute(t_verb *x, void *attr, long argc, t_atom *argv){
	if(argc && argv){
		x->attr_mute = atom_getlong(argv);
	}
	return MAX_ERR_NONE;
}

t_max_err attr_set_clip(t_verb *x, void *attr, long argc, t_atom *argv){
	if(argc && argv){
		x->attr_clip = atom_getlong(argv);
	}
	return MAX_ERR_NONE;
}

t_max_err attr_set_limit(t_verb *x, void *attr, long argc, t_atom *argv){
	if(argc && argv){
		x->attr_limit = atom_getlong(argv);
	}
	return MAX_ERR_NONE;
}

t_max_err attr_set_limiter_threshold(t_verb *x, void *attr, long argc, t_atom *argv){
	if(argc && argv){
		x->attr_limiter_threshold = atom_getlong(argv);
		x->limiter->set_attr(tt_limiter::k_threshold, x->attr_limiter_threshold);
	}
	return MAX_ERR_NONE;
}

t_max_err attr_set_limiter_lookahead(t_verb *x, void *attr, long argc, t_atom *argv){
	if(argc && argv){
		x->attr_limiter_lookahead = atom_getlong(argv);
		x->limiter->set_attr(tt_limiter::k_lookahead, x->attr_limiter_lookahead);
	}
	return MAX_ERR_NONE;
}

t_max_err attr_set_limiter_release(t_verb *x, void *attr, long argc, t_atom *argv){
	if(argc && argv){
		x->attr_limiter_release = atom_getlong(argv);
		x->limiter->set_attr(tt_limiter::k_release, x->attr_limiter_release);
	}
	return MAX_ERR_NONE;
}

t_max_err attr_set_dcblock(t_verb *x, void *attr, long argc, t_atom *argv){
	if(argc && argv){
		x->attr_dcblock = atom_getlong(argv);
	}
	return MAX_ERR_NONE;
}

t_max_err attr_set_downsample(t_verb *x, void *attr, long argc, t_atom *argv){
	if(argc && argv){
		do_downsample(x, atom_getlong(argv));
	}
	return MAX_ERR_NONE;
}

// ATTRIBUTE: mix
t_max_err attr_set_mix(t_verb *x, void *attr, long argc, t_atom *argv)
{
	if(argc && argv){
		x->attr_mix = atom_getlong(argv);
		x->reverb[LEFT]->set_attr(tt_verb::k_mix, x->attr_mix);
		x->reverb[RIGHT]->set_attr(tt_verb::k_mix, x->attr_mix);		
	}
	return MAX_ERR_NONE;
	#pragma unused(attr)	
}

// ATTRIBUTE: gain
t_max_err attr_set_gain(t_verb *x, void *attr, long argc, t_atom *argv)
{
	if(argc && argv){
		x->attr_gain = atom_getlong(argv);
		x->reverb[LEFT]->set_attr(tt_verb::k_gain, x->attr_gain);
		x->reverb[RIGHT]->set_attr(tt_verb::k_gain, x->attr_gain);
	}
	return MAX_ERR_NONE;
	#pragma unused(attr)	
}

// FUNCTION REQUIRED BY THE STANDARD TAP.TOOLS ACCESSORS
void do_downsample(t_verb *x, long value)
{
	x->attr_downsample = value;
	if(x->attr_downsample == 0)
		x->downsample = 1;
	else if(x->attr_downsample == 1)
		x->downsample = 2;
	else if(x->attr_downsample == 2)
		x->downsample = 4;
	else if(x->attr_downsample == 3)
		x->downsample = 8;
	x->downsampler->set_attr(tt_downsample::k_factor, x->downsample);
	x->upsampler->set_attr(tt_upsample::k_factor, x->downsample);
	x->limiter->set_sr(x->sr / x->downsample);
	x->clipper->set_sr(x->sr / x->downsample);
	x->dcblocker->set_sr(x->sr / x->downsample);
	x->limiter->set_vectorsize(x->vector_size / x->downsample);
	x->clipper->set_vectorsize(x->vector_size / x->downsample);
	x->dcblocker->set_vectorsize(x->vector_size / x->downsample);

	x->reverb[LEFT]->set_sr(x->sr / x->downsample);					// Object specific...
	x->reverb[LEFT]->set_vectorsize(x->vector_size / x->downsample);
	x->reverb[RIGHT]->set_sr(x->sr / x->downsample);					// Object specific...
	x->reverb[RIGHT]->set_vectorsize(x->vector_size / x->downsample);
}


// when used as the algorithm for a module, we use this to suppress errors for unhandles messages
void verb_anything(t_verb *x, t_symbol *msg, long argc, t_atom *argv)
{
	//object_post((t_object *)x, "anything: %s", msg->s_name);
}
