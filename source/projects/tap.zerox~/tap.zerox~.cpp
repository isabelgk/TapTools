/* 
 *	External object for Max/MSP
 *	Copyright © 2003 by Timothy Place
 * 
 *	License: This code is licensed under the terms of the "New BSD License"
 *	http://creativecommons.org/licenses/BSD/
 */

#include "TTClassWrapperMax.h"

#define NUM_INPUTS 1
#define NUM_OUTPUTS 2

// Data Structure for this object
typedef struct _zerox {
	t_pxobject 		x_obj;			// This object - must be first
	TTAudioObjectBasePtr	zeroxUnit;
    TTAudioSignalPtr	signalIn;
    TTAudioSignalPtr	signalOut;
	long			attr_size;		// 
} t_zerox;

// Prototypes for methods: need a method for each incoming message type
void*		zerox_new(t_symbol *msg, long argc, t_atom *argv);
void		zerox_free(t_zerox *x);
void		zerox_assist(t_zerox *x, void *b, long m, long a, char *s);
t_max_err	attr_set_size(t_zerox *x, void *attr, long argc, t_atom *argv);
void		zerox_dsp64(t_zerox *x, t_object *dsp64, short *count, double samplerate, long maxvectorsize, long flags); // ../../../../Jamoma/Core/DSP/library/build/JamomaDSP.dylib64 Method
extern "C" TTErr TTLoadJamomaExtension_AnalysisLib(void);

// Globals
static t_class *zerox_class;					// Required. Global pointing to this class


/************************************************************************************/
// Main() Function

extern "C" int C74_EXPORT main(void)
{
	t_class *c;

	c = class_new("tap.zerox~",(method)zerox_new, (method)zerox_free, sizeof(t_zerox), (method)0L, A_GIMME, 0);

	TTDSPInit();
	TTLoadJamomaExtension_AnalysisLib();
	
	common_symbols_init();
	
	class_addmethod(c, (method)zerox_dsp64,		"dsp64", A_CANT, 0);
    class_addmethod(c, (method)zerox_assist, 	"assist", A_CANT, 0L); 
	class_addmethod(c, (method)stdinletinfo,	"inletinfo",	A_CANT, 0);

	CLASS_ATTR_LONG(c,		"size",		0,	t_zerox, attr_size);
	CLASS_ATTR_ACCESSORS(c,	"size",		NULL, attr_set_size);
		
	class_dspinit(c);				// Setup object's class to work with MSP
class_register(_sym_box, c); 	zerox_class = c;
	return 0;
}


/************************************************************************************/
// Object Life

// Create
void *zerox_new(t_symbol *msg, long argc, t_atom *argv)
{
	t_zerox *x = (t_zerox *)object_alloc(zerox_class);;
	
	if (x) {
		object_obex_store((void *)x, _sym_dumpout, (object *)outlet_new(x, NULL));	// dumpout
		dsp_setup((t_pxobject *)x, 1);				// Create Object and 1 Inlet (last argument)
		outlet_new((t_pxobject *)x, "signal");		// Create a signal Outlet
		outlet_new((t_pxobject *)x, "signal");		// Create a signal Outlet
		
		TTObjectBaseInstantiate(TT("zerocross"), &x->zeroxUnit, 1);
		TTObjectBaseInstantiate(TT("audiosignal"), &x->signalIn, 2);
		TTObjectBaseInstantiate(TT("audiosignal"), &x->signalOut, 2);

		x->attr_size = 0;
		attr_args_process(x, argc, argv); 			//handle attribute args			
	}
	return (x);										// Return the pointer
}


// Destroy
void zerox_free(t_zerox *x)
{
	dsp_free((t_pxobject *)x);
	TTObjectBaseRelease(&x->zeroxUnit);
	TTObjectBaseRelease(&x->signalIn);
	TTObjectBaseRelease(&x->signalOut);
}


/************************************************************************************/
// Methods bound to input/inlets

// Method for Assistance Messages
void zerox_assist(t_zerox *x, void *b, long msg, long arg, char *dst)
{
	if(msg==1) 			// Inlet
		strcpy(dst, "(signal) Input");
	else if(msg==2){ 	// Outlet
		switch(arg){
			case 0: strcpy(dst, "(signal) number of zero crossings"); break;
			case 1: strcpy(dst, "(signal) trigger"); break;
			case 2: strcpy(dst, "(signal) dumpout"); break;
		}
	}
}


// ATTRIBUTE: analysis size
t_max_err attr_set_size(t_zerox *x, void *attr, long argc, t_atom *argv)
{
	x->attr_size = atom_getlong(argv);
	x->zeroxUnit->setAttributeValue(TT("size"), (TTUInt16)x->attr_size);
	return MAX_ERR_NONE;
}


// Perform (signal) Method
void zerox_perform64(t_zerox *x, t_object *dsp64, double **ins, long numins, double **outs, long numouts, long sampleframes, long flags, void *userparam)
{
	TTUInt16	vs = x->signalIn->getVectorSizeAsInt();
	
	x->signalIn->setVector(0, vs, ins[0]);
	x->zeroxUnit->process(x->signalIn, x->signalOut);
	x->signalOut->getVectorCopy(0, vs, outs[0]);
	x->signalOut->getVectorCopy(1, vs, outs[1]);	
}


// ../../../../Jamoma/Core/DSP/library/build/JamomaDSP.dylib Method
void zerox_dsp64(t_zerox *x, t_object *dsp64, short *count, double samplerate, long maxvectorsize, long flags)
{
	x->zeroxUnit->sendMessage(TT("clear"));
	x->zeroxUnit->setAttributeValue(kTTSym_sampleRate, samplerate);
	
	x->signalIn->setAttributeValue(kTTSym_vectorSize,  (TTUInt16)maxvectorsize);
	x->signalOut->setAttributeValue(kTTSym_vectorSize, (TTUInt16)maxvectorsize);
	
	x->signalOut->sendMessage(TT("alloc"));
	object_method(dsp64, gensym("dsp_add64"), x, zerox_perform64, 0, NULL);
}
