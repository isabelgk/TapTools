/* 
 *	External object for Max/MSP
 *	Copyright © 2004 by Timothy Place
 * 
 *	License: This code is licensed under the terms of the "New BSD License"
 *	http://creativecommons.org/licenses/BSD/
 */

#include "TTClassWrapperMax.h"


typedef struct _biquadcalc			// Data Structure for this object
{
	t_pxobject		x_obj;
	void			*outlets[1];
	long			attr_sr;
	long			attr_sr_override;
	float			attr_cf;
	float			attr_gain;			// set in dB
	float				gain;			// as a coefficient
	float			attr_q;
	t_symbol		*attr_type;
	long			attr_trigger;		// 0=none, 1=cf, 2=gain, 3=q, 4=all 
} t_biquadcalc;

// Prototypes for methods: need a method for each incoming message type
void *biquadcalc_new(t_symbol *msg, long argc, t_atom *argv);				// New Object Creation Method
void biquadcalc_dsp(t_biquadcalc *x, t_signal **sp, short *count);			// ../../../../Jamoma/Core/DSP/library/build/JamomaDSP.dylib Method
void biquadcalc_assist(t_biquadcalc *x, void *b, long m, long a, char *s);	// Assistance Method

void biquadcalc_bang(t_biquadcalc *x);
t_max_err attr_set_cf(t_biquadcalc *x, void *attr, long argc, t_atom *argv);
t_max_err attr_set_gain(t_biquadcalc *x, void *attr, long argc, t_atom *argv);
t_max_err attr_set_q(t_biquadcalc *x, void *attr, long argc, t_atom *argv);


// Class Globals
static t_class 	*biquadcalc_class;			// Required. Global pointing to this class
static t_symbol	*ps_lowpass;
static t_symbol	*ps_highpass;
static t_symbol	*ps_bandpass;
static t_symbol	*ps_bandstop;
static t_symbol	*ps_peaknotch;
static t_symbol	*ps_lowshelf;
static t_symbol	*ps_highshelf;
static t_symbol	*ps_resonant;
static t_symbol	*ps_allpass;


/************************************************************************************/
// Main() Function

extern "C" int C74_EXPORT main(void)
{
	t_class *c;

	c = class_new("tap.biquadcalc",(method)biquadcalc_new, (method)dsp_free, sizeof(t_biquadcalc), 
		(method)0L, A_GIMME, 0);

	common_symbols_init();									// Initialize TapTools

	CLASS_ATTR_LONG(c,		"sr",			0,	t_biquadcalc, attr_sr);

	CLASS_ATTR_LONG(c,		"sr_override",	0,	t_biquadcalc, attr_sr_override);

	CLASS_ATTR_FLOAT(c,		"cf",			0,	t_biquadcalc, attr_cf);
	CLASS_ATTR_ACCESSORS(c,	"cf",			NULL, attr_set_cf);

	CLASS_ATTR_FLOAT(c,		"gain",			0,	t_biquadcalc, attr_gain);
	CLASS_ATTR_ACCESSORS(c,	"gain",			NULL, attr_set_gain);

	CLASS_ATTR_FLOAT(c,		"q",			0,	t_biquadcalc, attr_q);
	CLASS_ATTR_ACCESSORS(c,	"q",			NULL, attr_set_q);

	CLASS_ATTR_LONG(c,		"trigger",		0,	t_biquadcalc, attr_trigger);
	CLASS_ATTR_ENUMINDEX(c,	"trigger",		0,	"none cf gain q all");
	
	CLASS_ATTR_SYM(c,		"type",			0,	t_biquadcalc, attr_type);
	CLASS_ATTR_ENUM(c,		"type",			0,	"lowpass highpass bandpass bandstop peaknotch lowshelf highshelf resonant allpass");

	class_addmethod(c, (method)biquadcalc_bang,		"bang", 0L);
 	class_addmethod(c, (method)biquadcalc_dsp, 		"dsp", A_CANT, 0L);		
	class_addmethod(c, (method)biquadcalc_assist, 	"assist", A_CANT, 0L); 
	class_addmethod(c, (method)stdinletinfo,		"inletinfo",	A_CANT, 0);

	class_dspinit(c);									// Setup object's class to work with MSP
	biquadcalc_class = c; class_register(_sym_box, c);
	
	// Init Globals
	ps_lowpass = gensym("lowpass");
	ps_highpass = gensym("highpass");
	ps_bandpass = gensym("bandpass");
	ps_bandstop = gensym("bandstop");
	ps_peaknotch = gensym("peaknotch");
	ps_lowshelf = gensym("lowshelf");
	ps_highshelf = gensym("highshelf");
	ps_resonant = gensym("resonant");
	ps_allpass = gensym("allpass");
}


/************************************************************************************/
// Object Creation Method

void *biquadcalc_new(t_symbol *msg, long argc, t_atom *argv)
{
	t_biquadcalc *x = (t_biquadcalc *)object_alloc(biquadcalc_class);
	if(x){
    	object_obex_store((void *)x, _sym_dumpout, (object *)outlet_new(x,NULL));	// dumpout		
		dsp_setup((t_pxobject *)x, 1);				// Create Object and 1 Inlet (last argument)
		x->outlets[0] = outlet_new(x,0L);			// create an outlet and store the pointer in the struct

		// Set initial state
		x->attr_sr = (long)sys_getsr();
		x->attr_sr_override = 0;
		x->attr_cf = 1000;
		x->attr_gain = 0.0;		// set in dB
		x->attr_q = 1.0;
		x->attr_type = ps_lowpass;
		x->attr_trigger = 4;	// 0=none, 1=cf, 2=gain, 3=q, 4=all 

		attr_args_process(x, argc, argv);					//handle attribute args	


	}	
	return (x);										// Return the pointer
}


/************************************************************************************/
// Methods bound to input/inlets

// Method for Assistance Messages
void biquadcalc_assist(t_biquadcalc *x, void *b, long msg, long arg, char *dst)
{
	if(msg==1){ 		// Inlets
		switch(arg){
			case 0: strcpy(dst, "set attributes"); break;
		}
	}
	else if(msg==2){	// Outlets
		switch(arg){
			case 0: strcpy(dst, "(list) connect to biquad~"); break;
			case 1: strcpy(dst, "(list) dumpout"); break;
		}
	}
}


// ATTRIBUTE
t_max_err attr_set_cf(t_biquadcalc *x, void *attr, long argc, t_atom *argv)
{
	if(argc && argv){
		x->attr_cf = atom_getfloat(argv);
		if((x->attr_trigger == 1) || (x->attr_trigger == 4))
			biquadcalc_bang(x);
	}
	return MAX_ERR_NONE;
	#pragma unused(attr)	
}


// ATTRIBUTE
t_max_err attr_set_gain(t_biquadcalc *x, void *attr, long argc, t_atom *argv)
{
	if(argc && argv){
		x->attr_gain = atom_getfloat(argv);
		x->gain = TTDecibelsToLinearGain(x->attr_gain);
		if((x->attr_trigger == 2) || (x->attr_trigger == 4))
			biquadcalc_bang(x);
	}
	return MAX_ERR_NONE;
	#pragma unused(attr)	
}


// ATTRIBUTE
t_max_err attr_set_q(t_biquadcalc *x, void *attr, long argc, t_atom *argv)
{
	if(argc && argv){
		x->attr_q = atom_getfloat(argv);
		if((x->attr_trigger == 3) || (x->attr_trigger == 4))
			biquadcalc_bang(x);
	}
	return MAX_ERR_NONE;
	#pragma unused(attr)	
}


#define A0 0
#define A1 1
#define A2 2
#define B1 3
#define B2 4

// BANG: Do the calculation!
void biquadcalc_bang(t_biquadcalc *x)
{
	// Variables for RBJ Cookbook Calculations
	//	see http://www.harmony-central.com/Computer/Programming/Audio-EQ-Cookbook.txt
	//	for more information.
	
	float	Fs = x->attr_sr;
	float	f0 = x->attr_cf;
	float	dBgain = x->attr_gain;
	float	Q = x->attr_q;				// our "q" attr acts as a catchall for "steepness"
//	float	BW = x->attr_q;
	float	S = x->attr_q;
	
	// Calculate intermediate variables
	float	A = pow(10.0, dBgain/40.0);
	float	w0 = 6.283185307 * f0 / Fs;
//	float	cos_w0 = cos(w0);
	float	sin_w0 = sin(w0);
	float	alpha = 0;
	
	// Variable for storage of the results
	t_atom 	results[5];	// A0, A1, A2, B1, B2 (THESE NAMES USE THE MAX BIQUAD CONVENTION - OPPOSITE THE RBJ!)
	float	temp;
	
	// Calculate alpha
	if((x->attr_type == ps_lowshelf) || (x->attr_type == ps_highshelf))
		alpha = sin_w0 / 2.0 * sqrt((A + 1/A) * (1/S - 1) + 2.0);		// case: S
	else
		alpha = sin_w0 / (2.0 * Q);										// case: Q
	
	
	// Calculate
	if(x->attr_type == ps_lowpass){
		temp = 1 + alpha;	// B0
		atom_setfloat(&results[A0], ((1 - cos(w0)) / 2) / temp);
		atom_setfloat(&results[A1], (1 - cos(w0)) / temp);
		atom_setfloat(&results[A2], ((1 - cos(w0)) / 2) / temp);
		atom_setfloat(&results[B1], (-2 * cos(w0)) / temp);
		atom_setfloat(&results[B2], (1 - alpha) / temp);
		
		outlet_list(x->outlets[0], 0L, 5, results);		// output the result	
	}
	else if(x->attr_type == ps_highpass){
		temp = 1 + alpha;	// B0
		atom_setfloat(&results[A0], ((1 + cos(w0)) / 2) / temp);
		atom_setfloat(&results[A1], (-(1 + cos(w0))) / temp);
		atom_setfloat(&results[A2], ((1 + cos(w0)) / 2) / temp);
		atom_setfloat(&results[B1], (-2 * cos(w0)) / temp);
		atom_setfloat(&results[B2], (1 - alpha) / temp);
		
		outlet_list(x->outlets[0], 0L, 5, results);		// output the result	
	}
	else if(x->attr_type == ps_bandpass){
		temp = 1 + alpha;	// B0
		atom_setfloat(&results[A0], (alpha) / temp);
		atom_setfloat(&results[A1], 0.0);
		atom_setfloat(&results[A2], (-alpha) / temp);
		atom_setfloat(&results[B1], (-2 * cos(w0)) / temp);
		atom_setfloat(&results[B2], (1 - alpha) / temp);
		
		outlet_list(x->outlets[0], 0L, 5, results);		// output the result	
	}
	else if(x->attr_type == ps_bandstop){	
		temp = 1 + alpha;	// B0
		atom_setfloat(&results[A0], 1.0 / temp);
		atom_setfloat(&results[A1], (-2 * cos(w0)) / temp);
		atom_setfloat(&results[A2], 1.0 / temp);
		atom_setfloat(&results[B1], (-2 * cos(w0)) / temp);
		atom_setfloat(&results[B2], (1 - alpha) / temp);
		
		outlet_list(x->outlets[0], 0L, 5, results);		// output the result	
	}
	else if(x->attr_type == ps_peaknotch){
		temp = 1 + alpha/A;	// B0
		atom_setfloat(&results[A0], (1 + alpha * A) / temp);
		atom_setfloat(&results[A1], (-2 * cos(w0)) / temp);
		atom_setfloat(&results[A2], (1 - alpha * A) / temp);
		atom_setfloat(&results[B1], (-2 * cos(w0)) / temp);
		atom_setfloat(&results[B2], (1 - alpha / A) / temp);
		
		outlet_list(x->outlets[0], 0L, 5, results);		// output the result	
	}
	else if(x->attr_type == ps_lowshelf){
		temp = (A+1) + (A-1) * cos(w0) + 2 * sqrt(A) * alpha;	// B0
		atom_setfloat(&results[A0], (A*( (A+1) - (A-1)*cos(w0) + 2*sqrt(A)*alpha )) / temp);
		atom_setfloat(&results[A1], (2*A*( (A-1) - (A+1)*cos(w0))) / temp);
		atom_setfloat(&results[A2], (A*( (A+1) - (A-1)*cos(w0) - 2*sqrt(A)*alpha )) / temp);
		atom_setfloat(&results[B1], (-2*( (A-1) + (A+1)*cos(w0))) / temp);
		atom_setfloat(&results[B2], ((A+1) + (A-1)*cos(w0) - 2*sqrt(A)*alpha) / temp);
		
		outlet_list(x->outlets[0], 0L, 5, results);		// output the result	
	}
	else if(x->attr_type == ps_highshelf){
		temp = (A+1) - (A-1)*cos(w0) + 2*sqrt(A)*alpha;	// B0
		atom_setfloat(&results[A0], (A*( (A+1) + (A-1)*cos(w0) + 2*sqrt(A)*alpha )) / temp);
		atom_setfloat(&results[A1], (-2*A*( (A-1) + (A+1)*cos(w0))) / temp);
		atom_setfloat(&results[A2], (A*( (A+1) + (A-1)*cos(w0) - 2*sqrt(A)*alpha )) / temp);
		atom_setfloat(&results[B1], (2*( (A-1) - (A+1)*cos(w0))) / temp);
		atom_setfloat(&results[B2], ((A+1) - (A-1)*cos(w0) - 2*sqrt(A)*alpha) / temp);
		
		outlet_list(x->outlets[0], 0L, 5, results);		// output the result	
	}
	else if(x->attr_type == ps_allpass){
		temp = 1 + alpha;	// B0
		atom_setfloat(&results[A0], (1 - alpha) / temp);
		atom_setfloat(&results[A1], (-2 * cos(w0)) / temp);
		atom_setfloat(&results[A2], 1.0);
		atom_setfloat(&results[B1], (-2 * cos(w0)) / temp);
		atom_setfloat(&results[B2], (1 - alpha) / temp);
		
		outlet_list(x->outlets[0], 0L, 5, results);		// output the result	
	}
	else{
		object_error((t_object *)x, "invalide filter type specified");
	}
}



// ../../../../Jamoma/Core/DSP/library/build/JamomaDSP.dylib Method
void biquadcalc_dsp(t_biquadcalc *x, t_signal **sp, short *count)
{
	x->attr_sr = long(sp[0]->s_sr);
	#pragma unused(count)
}

