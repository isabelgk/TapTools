/* 
 *	External object for Max/MSP
 *	Copyright © 2015 by Timothy Place
 * 
 *	License: This code is licensed under the terms of the "New BSD License"
 *	http://creativecommons.org/licenses/BSD/
 */

#include "ext.h"
#include "z_dsp.h"
#include "ext_obex.h"
#include "Jamoma.h"


typedef struct _fourpole {
	t_pxobject					obj;
	Jamoma::LowpassFourPole*	filter;	// Max mallocs the sizeof the struct and assigns it, so we have to use a pointer
	Jamoma::SampleBundle*		inputBuffer;
} t_fourpole;


void *fourpole_new(t_symbol *s, short argc, t_atom *argv);
void fourpole_free(t_fourpole *x);
void fourpole_perform64(t_fourpole *x, t_object *dsp64, double **ins, long numins, double **outs, long numouts, long sampleframes, long flags, void *userparam);									// An MSP Perform (signal) Method
void fourpole_float(t_fourpole *x, double phase);						//
void fourpole_int(t_fourpole *x, long n);								//
void fourpole_dsp64(t_fourpole *x, t_object *dsp64, short *count, double samplerate, long maxvectorsize, long flags);			// ../../../../Jamoma/Core/DSP/library/build/JamomaDSP.dylib Method
void fourpole_assist(t_fourpole *x, void *b, long m, long a, char *s);	// Assistance Method
void fourpole_clear(t_fourpole *x);										// "Clear" Method

t_max_err fourpole_setfrequency(t_fourpole* x, void* attr, long argc, t_atom* argv);
t_max_err fourpole_getfrequency(t_fourpole* x, void* attr, long* argc, t_atom** argv);
t_max_err fourpole_setresonance(t_fourpole* x, void* attr, long argc, t_atom* argv);
t_max_err fourpole_getresonance(t_fourpole* x, void* attr, long* argc, t_atom** argv);

// Globals
static t_class*	s_fourpole_class = NULL;

/************************************************************************************/
// Class Setup

extern "C" int C74_EXPORT main(void)
{
	t_class* c = class_new("tap.fourpole~", (method)fourpole_new, (method)fourpole_free, sizeof(t_fourpole), (method)nullptr, A_GIMME, 0);

    class_addmethod(c, (method)fourpole_clear,			"clear", 0L);
    class_addmethod(c, (method)fourpole_assist, 		"assist", A_CANT, 0L);
    class_addmethod(c, (method)object_obex_dumpout, 	"dumpout", A_CANT,0);      
	class_addmethod(c, (method)stdinletinfo,			"inletinfo",	A_CANT, 0);
	class_addmethod(c, (method)fourpole_dsp64,			"dsp64", A_CANT, 0);
	
	class_addattr(c, attr_offset_new("frequency", gensym("float64"), 0,
									 (method)fourpole_getfrequency, (method)fourpole_setfrequency, 0));

	class_addattr(c, attr_offset_new("resonance", gensym("float64"), 0,
									 (method)fourpole_getresonance, (method)fourpole_setresonance, 0));

	class_dspinit(c);									// Setup object's class to work with MSP
	class_register(CLASS_BOX, c);
	s_fourpole_class = c; 
}


/************************************************************************************/
// Instance Lifecycle

void *fourpole_new(t_symbol *s, short argc, t_atom *argv)
{
	t_fourpole *x = (t_fourpole*)object_alloc(s_fourpole_class);

	if (x) {
		// stereo
		dsp_setup((t_pxobject *)x, 2);
		outlet_new((t_object *)x, "signal");
		outlet_new((t_object *)x, "signal");
		
		x->filter = new Jamoma::LowpassFourPole;
		x->filter->sampleRate = sys_getsr();
		
		x->inputBuffer = new Jamoma::SampleBundle;
		
		attr_args_process(x, argc, argv);
	}
	return(x);
}


void fourpole_free(t_fourpole *x)
{
	dsp_free((t_pxobject *)x);
	delete x->filter;
	delete x->inputBuffer;
}


/************************************************************************************/
// Methods bound to input/inlets

// Method for Assistance Messages
void fourpole_assist(t_fourpole *x, void *b, long msg, long arg, char *dst)
{
	if(msg==1){ 	// Inlets
		switch(arg){
			case 0: strcpy(dst, "(signal) Input"); break;
			case 1: strcpy(dst, "(signal/float) Delay Time in ms"); break;
			case 2: strcpy(dst, "(double) Decay Time in seconds"); break;
		}
	}
	else if(msg==2){ // Outlets
		switch(arg){
			case 0: strcpy(dst, "(signal) Filtered Output"); break;
			case 1: strcpy(dst, "dumpout"); break;
		}
	}	
}


// Method for the "clear" message
void fourpole_clear(t_fourpole *x)
{
	x->filter->clear();
}


// Attributes

t_max_err fourpole_setfrequency(t_fourpole* x, void* attr, long argc, t_atom* argv)
{
	x->filter->frequency = atom_getfloat(argv);
	return MAX_ERR_NONE;
}


t_max_err fourpole_getfrequency(t_fourpole* x, void* attr, long* argc, t_atom** argv)
{
	*argc = 1;
	if (!(*argv)) // otherwise use memory passed in
		*argv = (t_atom*)sysmem_newptr(sizeof(t_atom) * (*argc));
	
	atom_setfloat(*argv, x->filter->frequency);
	return MAX_ERR_NONE;
}


t_max_err fourpole_setresonance(t_fourpole* x, void* attr, long argc, t_atom* argv)
{
	x->filter->q = atom_getfloat(argv);
	return MAX_ERR_NONE;
}


t_max_err fourpole_getresonance(t_fourpole* x, void* attr, long* argc, t_atom** argv)
{
	*argc = 1;
	if (!(*argv)) // otherwise use memory passed in
		*argv = (t_atom*)sysmem_newptr(sizeof(t_atom) * (*argc));

	atom_setfloat(*argv, x->filter->q);
	return MAX_ERR_NONE;
}


void fourpole_perform64(t_fourpole *x, t_object *dsp64, double **ins, long numins, double **outs, long numouts, long sampleframes, long flags, void *userparam)
{
	x->inputBuffer->copyIn(0, sampleframes, ins[0]);
	x->inputBuffer->copyIn(1, sampleframes, ins[1]);
	
	auto outputBuffer = (*x->filter)(*x->inputBuffer);
	
	outputBuffer[0].copyOut(0, sampleframes, outs[0]);
	outputBuffer[0].copyOut(1, sampleframes, outs[1]);
}


void fourpole_dsp64(t_fourpole *x, t_object *dsp64, short *count, double samplerate, long maxvectorsize, long flags)
{
	x->filter->sampleRate = samplerate;
	x->filter->clear();

	x->inputBuffer->resizeChannels(2);
	x->inputBuffer->resizeFrames(maxvectorsize);
	
	object_method(dsp64, gensym("dsp_add64"), (t_object*)x, fourpole_perform64, 0, NULL);
}


