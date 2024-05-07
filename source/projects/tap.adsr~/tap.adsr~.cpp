/* 
 *	External object for Max/MSP
 *	Copyright © 2003 by Timothy Place
 * 
 *	License: This code is licensed under the terms of the "New BSD License"
 *	http://creativecommons.org/licenses/BSD/
 */

#include "TTClassWrapperMax.h"


typedef struct _adsr				// Data Structure for this object
{
    t_pxobject 		x_obj;
	TTAudioObjectBasePtr	adsr;
    TTAudioSignalPtr	audioIn;
    TTAudioSignalPtr	audioOut;
	long			attr_trigger;
	double			attr_attack;
	double			attr_decay;
	double			attr_sustain;
	double			attr_release;
	t_symbol*		attr_mode;
} t_adsr;


// Prototypes for methods: need a method for each incoming message type
void *adsr_new(t_symbol *msg, short argc, t_atom *argv);				// New Object Creation Method
void adsr_free(t_adsr *x);
void adsr_assist(t_adsr *x, void *b, long msg, long arg, char *dst);	// Assistance Method

void adsr_dsp(t_adsr *x, t_signal **sp, short *count);
void adsr_dsp64(t_adsr *x, t_object *dsp64, short *count, double samplerate, long maxvectorsize, long flags);					// ../../../../Jamoma/Core/DSP/library/build/JamomaDSP.dylib Method
t_int *adsr_perform(t_int *w);
void adsr_perform64(t_adsr *x, t_object *dsp64, double **ins, long numins, double **outs, long numouts, long sampleframes, long flags, void *userparam);											// An MSP Perform (signal) Method
t_int *adsr_perform2(t_int *w);
void adsr_perform264(t_adsr *x, t_object *dsp64, double **ins, long numins, double **outs, long numouts, long sampleframes, long flags, void *userparam);

t_max_err  attr_set_trigger(t_adsr *x, void *attr, long argc, t_atom *argv);
t_max_err  attr_set_attack(t_adsr *x, void *attr, long argc, t_atom *argv);
t_max_err  attr_set_decay(t_adsr *x, void *attr, long argc, t_atom *argv);
t_max_err  attr_set_sustain(t_adsr *x, void *attr, long argc, t_atom *argv);
t_max_err  attr_set_release(t_adsr *x, void *attr, long argc, t_atom *argv);
t_max_err  attr_set_mode(t_adsr *x, void *attr, long argc, t_atom *argv);

extern "C" TTErr TTLoadJamomaExtension_GeneratorLib(void);

// Globals
static t_class	*adsr_class;
static t_symbol	*ps_linear;
static t_symbol	*ps_exponential;
static t_symbol *ps_hybrid;

/************************************************************************************/
// Main() Function


extern "C" int C74_EXPORT main(void)
{
	t_class *c;
	
	c = class_new("tap.adsr~", (method)adsr_new, (method)adsr_free, sizeof(t_adsr), (method)0L, A_GIMME, 0);

	TTDSPInit();
	TTLoadJamomaExtension_GeneratorLib();
	
	common_symbols_init();
 	class_addmethod(c, (method)adsr_dsp,		"dsp", A_CANT, 0);		
	class_addmethod(c, (method)adsr_dsp64, "dsp64", A_CANT, 0);
	class_addmethod(c, (method)adsr_assist,		"assist", A_CANT, 0); 
	class_addmethod(c, (method)stdinletinfo,	"inletinfo",	A_CANT, 0);

	CLASS_ATTR_LONG(c,			"trigger",	0,	t_adsr, attr_trigger);
	CLASS_ATTR_ACCESSORS(c,		"trigger",	NULL, attr_set_trigger);
	CLASS_ATTR_STYLE(c,			"trigger",	0,	"onoff");

	CLASS_ATTR_DOUBLE(c,			"attack",	0,	t_adsr, attr_attack);
	CLASS_ATTR_ACCESSORS(c,		"attack",	NULL, attr_set_attack);

	CLASS_ATTR_DOUBLE(c,			"decay",	0,	t_adsr, attr_decay);
	CLASS_ATTR_ACCESSORS(c,		"decay",	NULL, attr_set_decay);

	CLASS_ATTR_DOUBLE(c,			"sustain",	0,	t_adsr, attr_sustain);
	CLASS_ATTR_ACCESSORS(c,		"sustain",	NULL, attr_set_sustain);

	CLASS_ATTR_DOUBLE(c,			"release",	0,	t_adsr, attr_release);
	CLASS_ATTR_ACCESSORS(c,		"release",	NULL, attr_set_release);

	CLASS_ATTR_SYM(c,			"mode",		0,	t_adsr, attr_mode);
	CLASS_ATTR_ACCESSORS(c,		"mode",		NULL, attr_set_mode);
	CLASS_ATTR_ENUM(c,			"mode",		0, "hybrid linear exponential");
	
	class_dspinit(c);									// Setup object's class to work with MSP
	adsr_class = c; class_register(_sym_box, c);

	// initialize class globals
	ps_linear = gensym("linear");
	ps_exponential = gensym("exponential");
	ps_hybrid = gensym("hybrid");
}


/************************************************************************************/
// Object Creation Method

void *adsr_new(t_symbol *msg, short argc, t_atom *argv)
{
    t_adsr*		x;
	TTUInt16	maxNumChannels = 1;
    
    x = (t_adsr *)object_alloc(adsr_class);
    if(x){
    	object_obex_store((void *)x, _sym_dumpout, (object *)outlet_new(x,NULL));	// dumpout
	    dsp_setup((t_pxobject *)x,1);				// Create Object and 1 Inlet (last argument)
	    outlet_new((t_pxobject *)x, "signal");		// Create a signal Outlet

		TTObjectBaseInstantiate(kTTSym_adsr,		&x->adsr,		maxNumChannels);
		TTObjectBaseInstantiate(kTTSym_audiosignal,	&x->audioIn,	1);
		TTObjectBaseInstantiate(kTTSym_audiosignal,	&x->audioOut,	1);
				
		x->attr_trigger = 0;						// Set Defaults in Struct
		x->attr_attack = 50.0;
		x->attr_decay = 100.0;
		x->attr_sustain = -6.0;
		x->attr_release = 500.0;
		x->attr_mode = ps_linear;
		
		attr_args_process(x, argc, argv);
	}		
	return (x);
}

// Memory Deallocation
void adsr_free(t_adsr *x)
{	
	dsp_free((t_pxobject *)x);
	TTObjectBaseRelease(&x->adsr);
	TTObjectBaseRelease(&x->audioIn);
	TTObjectBaseRelease(&x->audioOut);
}


/************************************************************************************/
// Methods bound to input/inlets

// Method for Assistance Messages
void adsr_assist(t_adsr *x, void *b, long msg, long arg, char *dst)
{
	if(msg==1) 	// Inlets
		strcpy(dst, "(signal/anything) input, control messages");		
	else if(msg==2){ // Outlets
		switch(arg){
			case 0: strcpy(dst, "(signal) envelope output"); break;
			case 1: strcpy(dst, "dumpout"); break;
		}
	}	
}


// set envelope trigger
t_max_err attr_set_trigger(t_adsr *x, void *attr, long argc, t_atom *argv)
{
	x->attr_trigger = atom_getlong(argv);
	x->adsr->setAttributeValue(TT("trigger"), (TTBoolean)x->attr_trigger);
	return MAX_ERR_NONE;
}

// ATTRIBUTE:
t_max_err attr_set_attack(t_adsr *x, void *attr, long argc, t_atom *argv)
{
	x->attr_attack = atom_getfloat(argv);
	x->adsr->setAttributeValue(TT("attack"), x->attr_attack);
	return MAX_ERR_NONE;
}

// ATTRIBUTE:
t_max_err attr_set_decay(t_adsr *x, void *attr, long argc, t_atom *argv)
{
	x->attr_decay = atom_getfloat(argv);
	x->adsr->setAttributeValue(TT("decay"), x->attr_decay);
	return MAX_ERR_NONE;
}

// ATTRIBUTE:
t_max_err attr_set_sustain(t_adsr *x, void *attr, long argc, t_atom *argv)
{
	x->attr_sustain = atom_getfloat(argv);
	x->adsr->setAttributeValue(TT("sustain"), x->attr_sustain);
	return MAX_ERR_NONE;
}

// ATTRIBUTE:
t_max_err attr_set_release(t_adsr *x, void *attr, long argc, t_atom *argv)
{
	x->attr_release = atom_getfloat(argv);
	x->adsr->setAttributeValue(TT("release"), x->attr_release);
	return MAX_ERR_NONE;
}

// ATTRIBUTE:
t_max_err attr_set_mode(t_adsr *x, void *attr, long argc, t_atom *argv)
{
	if(argc && argv){
		switch(argv[0].a_type){
			case A_SYM:			// Symbol arg is the newer (and prefered) way to do it
				if(atom_getsym(argv) == ps_linear){
					x->adsr->setAttributeValue(TT("mode"), TT("linear"));
					x->attr_mode = ps_linear;
				}
				else if(atom_getsym(argv) == ps_exponential){
					x->adsr->setAttributeValue(TT("mode"), TT("exponential"));
					x->attr_mode = ps_exponential;
				}
				else if(atom_getsym(argv) == ps_hybrid){
					x->adsr->setAttributeValue(TT("mode"), TT("hybrid"));
					x->attr_mode = ps_exponential;
				}
				else
					object_error((t_object *)x, "not a valid mode");
				break;
		}
	}
	return MAX_ERR_NONE;
}


// Perform (signal) Method: Control-Rate Trigger
t_int *adsr_perform(t_int *w)
{
   	t_adsr		*x = (t_adsr*)(w[1]);
	t_float		*out = (t_float*)(w[2]);
	int			vs = (long)(w[3]);
	
	if (!x->x_obj.z_disabled)
		x->adsr->process(x->audioOut);

	x->audioOut->getVector(0, vs, out);
    return w+4;
}


void adsr_perform64(t_adsr *x, t_object *dsp64, double **ins, long numins, double **outs, long numouts, long sampleframes, long flags, void *userparam)
{
   	
	double	*out = outs[0];
	int			vs = sampleframes;
	
	x->adsr->process(x->audioOut);

	x->audioOut->getVectorCopy(0, vs, out);
    return;
}



// Perform (signal) Method: Signal-Rate Trigger
t_int *adsr_perform2(t_int *w)
{
   	t_adsr		*x = (t_adsr*)(w[1]);
	t_float		*in = (t_float*)(w[2]);
	t_float		*out = (t_float*)(w[3]);
	int			 vs = (long)(w[4]);

	x->audioIn->setVector(0, vs, in);
	
	if (!x->x_obj.z_disabled) 
		x->adsr->process(x->audioIn, x->audioOut);
	
	x->audioOut->getVector(0, vs, out);
    return w+5;
}


void adsr_perform264(t_adsr *x, t_object *dsp64, double **ins, long numins, double **outs, long numouts, long sampleframes, long flags, void *userparam)
{
   	
	double	*in = ins[0];
	double	*out = outs[0];
	int			 vs = sampleframes;

	x->audioIn->setVector(0, vs, in);
	
	x->adsr->process(x->audioIn, x->audioOut);
	
	x->audioOut->getVectorCopy(0, vs, out);
    return;
}



// ../../../../Jamoma/Core/DSP/library/build/JamomaDSP.dylib Method
void adsr_dsp(t_adsr *x, t_signal **sp, short *count)
{
	x->adsr->setAttributeValue(kTTSym_sampleRate, sp[0]->s_sr);
	
	x->audioIn->setAttributeValue(kTTSym_vectorSize, (TTUInt16)sp[0]->s_n);
	x->audioOut->setAttributeValue(kTTSym_vectorSize, (TTUInt16)sp[0]->s_n);
		
	//audioIn will be set in the perform method
	x->audioOut->sendMessage(kTTSym_alloc);
	
	if(count[0])		// Signal Input...
		dsp_add(adsr_perform2, 4, x, sp[0]->s_vec, sp[1]->s_vec, sp[0]->s_n);
	else				// No Signal Input...
		dsp_add(adsr_perform, 3, x, sp[1]->s_vec, sp[0]->s_n);
}


void adsr_dsp64(t_adsr *x, t_object *dsp64, short *count, double samplerate, long maxvectorsize, long flags)
{
	x->adsr->setAttributeValue(kTTSym_sampleRate, samplerate);
	
	x->audioIn->setAttributeValue(kTTSym_vectorSize, (TTUInt16)maxvectorsize);
	x->audioOut->setAttributeValue(kTTSym_vectorSize, (TTUInt16)maxvectorsize);
		
	//audioIn will be set in the perform method
	x->audioOut->sendMessage(kTTSym_alloc);
	
	if(count[0])		// Signal Input...
		object_method(dsp64, gensym("dsp_add64"), (t_object*)x, adsr_perform264, 0, NULL);
	else				// No Signal Input...
		object_method(dsp64, gensym("dsp_add64"), (t_object*)x, adsr_perform64, 0, NULL);
}

