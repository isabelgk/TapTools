/* 
 *	External object for Max/MSP
 *	Copyright © 2001 by Timothy Place
 * 
 *	License: This code is licensed under the terms of the "New BSD License"
 *	http://creativecommons.org/licenses/BSD/
 */

#include "TTClassWrapperMax.h"
 
#define MAX_NUM_CHANNELS 32

// Data Structure for this object
typedef struct _fade{
	t_pxobject 			x_obj;
	long				attr_shape;
	long				attr_mode;
	float				attr_position;
	TTUInt16			numChannels;
	TTAudioObjectBase*		xfade;			// crossfade object from the TTBlue library
	TTAudioSignal*		audioIn1;
	TTAudioSignal*		audioIn2;
	TTAudioSignal*		audioInControl;
	TTAudioSignal*		audioOut;
} t_fade;

// Prototypes for methods
void *fade_new(t_symbol *s, long argc, t_atom *argv);				// New Object Creation Method
void fade_dsp64(t_fade *x, t_object *dsp64, short *count, double samplerate, long maxvectorsize, long flags); // ../../../../Jamoma/Core/DSP/library/build/JamomaDSP.dylib64 Method
void fade_assist(t_fade *x, void *b, long m, long a, char *s);		// Assistance Method
void fade_float(t_fade *x, double value );							// Float Method
void fade_free(t_fade *x);
t_max_err attr_set_position(t_fade *x, void *attr, long argc, t_atom *argv);
t_max_err attr_set_shape(t_fade *x, void *attr, long argc, t_atom *argv);
t_max_err attr_set_mode(t_fade *x, void *attr, long argc, t_atom *argv);
void fade_perform64_1(t_fade *x, t_object *dsp64, double **ins, long numins, double **outs, long numouts, long sampleframes, long flags, void *userparam);
void fade_perform64_2(t_fade *x, t_object *dsp64, double **ins, long numins, double **outs, long numouts, long sampleframes, long flags, void *userparam);

// Globals
static t_class	*s_fade_class;


extern "C" TT_EXTENSION_EXPORT TTErr TTLoadJamomaExtension_Crossfade(void);


/************************************************************************************/
// Main() Function

extern "C" int C74_EXPORT main(void)
{
	t_class *c;
	
	TTDSPInit();
	TTLoadJamomaExtension_Crossfade();
	
	common_symbols_init();
	
	// Define our class
	c = class_new("tap.crossfade~", (method)fade_new, (method)fade_free, sizeof(t_fade), (method)0L, A_GIMME, 0);

	// Make methods accessible for our class: 
	class_addmethod(c, (method)fade_float,				"float", A_FLOAT, 0L);
	class_addmethod(c, (method)fade_dsp64,				"dsp64", A_CANT, 0);
    class_addmethod(c, (method)object_obex_dumpout, 	"dumpout", A_CANT,0);
    class_addmethod(c, (method)fade_assist, 			"assist", A_CANT, 0L);
	class_addmethod(c, (method)stdinletinfo,			"inletinfo",	A_CANT, 0);

	// Add attributes to our class:
	CLASS_ATTR_LONG(c,		"shape",		0,	t_fade, attr_shape);
	CLASS_ATTR_ACCESSORS(c,	"shape",		NULL, attr_set_shape);
	CLASS_ATTR_ENUMINDEX(c,	"shape",		0, "EqualPower Linear");
	
	CLASS_ATTR_LONG(c,		"mode",			0,	t_fade, attr_mode);
	CLASS_ATTR_ACCESSORS(c,	"mode",			NULL, attr_set_mode);
	CLASS_ATTR_ENUMINDEX(c,	"mode",			0, "LookupTable Calculate");
	
	CLASS_ATTR_DOUBLE(c,	"position",		0,	t_fade, attr_position);
	CLASS_ATTR_ACCESSORS(c,	"position",		NULL, attr_set_position);	
	
	// Setup our class to work with MSP
	class_dspinit(c);
	
	// Finalize our class
	s_fade_class = c; class_register(_sym_box, c);
	return 0;
}


/************************************************************************************/
// Object Life

// Create
void *fade_new(t_symbol *s, long argc, t_atom *argv)
{
	long attrstart = attr_args_offset(argc, argv);		// support normal arguments
	short i;
	
	t_fade *x = (t_fade *)object_alloc(s_fade_class);
	if(x){
		object_obex_store((void *)x, _sym_dumpout, (object *)outlet_new(x, NULL));	// dumpout
		
		// if there is just one arg then we treat it as the number of channels like in tt.xfade~
		// if there are more than that then we must be doing something for backwards compatibility mode
		
		x->numChannels = 1;
		if(attrstart && argv){
			int argument = atom_getlong(argv);
			x->numChannels = TTClip(argument, 1, MAX_NUM_CHANNELS);
		}

		if(attrstart >= 2){
			if(attrstart >= 1) 
				object_attr_setlong(x, _sym_mode, atom_getlong(argv+0));
			if(attrstart >= 2) 
				object_attr_setlong(x, gensym("shape"), atom_getlong(argv+1));
			if(attrstart >= 3) 
				object_attr_setlong(x, _sym_position, atom_getfloat(argv+2));
		}
		
		dsp_setup((t_pxobject *)x, (x->numChannels * 2) + 1);	// Create Object and N Inlets (last argument)
		x->x_obj.z_misc = Z_NO_INPLACE;  					// ESSENTIAL!   		
		for(i=0; i< (x->numChannels); i++)
			outlet_new((t_pxobject *)x, "signal");			// Create a signal Outlet   		
		
		//x->xfade = new TTCrossfade(x->numChannels);			// Constructors
		TTObjectBaseInstantiate(TT("crossfade"),	&x->xfade,			x->numChannels);
		TTObjectBaseInstantiate(kTTSym_audiosignal,	&x->audioIn1,		x->numChannels);
		TTObjectBaseInstantiate(kTTSym_audiosignal,	&x->audioIn2,		x->numChannels);
		TTObjectBaseInstantiate(kTTSym_audiosignal,	&x->audioInControl,	x->numChannels);
		TTObjectBaseInstantiate(kTTSym_audiosignal,	&x->audioOut,		x->numChannels);
		
		x->xfade->setAttributeValue(TT("mode"), TT("lookup"));
		x->xfade->setAttributeValue(TT("shape"), TT("equalPower"));
		x->xfade->setAttributeValue(TT("position"), 0.5);
		
		attr_args_process(x, argc, argv);					// handle attribute args				
	}
	return (x);												// Return the pointer
}


// Destroy
void fade_free(t_fade *x)
{
	dsp_free((t_pxobject *)x);		// Always call dsp_free first in this routine
	
	TTObjectBaseRelease(&x->xfade);
	TTObjectBaseRelease(&x->audioIn1);
	TTObjectBaseRelease(&x->audioIn2);
	TTObjectBaseRelease(&x->audioInControl);
	TTObjectBaseRelease(&x->audioOut);
}


/************************************************************************************/
// Methods bound to input/inlets

// Method for Assistance Messages
void fade_assist(t_fade *x, void *b, long msg, long arg, char *dst)
{
	if(msg==1){ 	// Inlets
		if(arg < x->numChannels) 
			strcpy(dst, "(signal) Input A");
		else if(arg < x->numChannels*2) 
			strcpy(dst, "(signal) Input B");
		else 
			strcpy(dst, "(float/signal) Crossfade Position");
	}
	else if(msg==2){ // Outlets
		if(arg < x->numChannels) 
			strcpy(dst, "(signal) Crossfaded between A and B");
		else 
			strcpy(dst, "dumpout");
	}
}


// Float Method
void fade_float(t_fade *x, double value)
{
	x->attr_position = value;
	x->xfade->setAttributeValue(TT("position"), value);
}


// ATTRIBUTE: position
t_max_err attr_set_position(t_fade *x, void *attr, long argc, t_atom *argv)
{
	fade_float(x, atom_getfloat(argv));
	return MAX_ERR_NONE;
}


// ATTRIBUTE: shape
t_max_err attr_set_shape(t_fade *x, void *attr, long argc, t_atom *argv)
{
	x->attr_shape = atom_getlong(argv);
	if(x->attr_shape == 0) 
		x->xfade->setAttributeValue(TT("shape"), TT("equalPower"));
	else 
		x->xfade->setAttributeValue(TT("shape"), TT("linear"));
	
	return MAX_ERR_NONE;
}


// ATTRIBUTE: mode
t_max_err attr_set_mode(t_fade *x, void *attr, long argc, t_atom *argv)
{
	x->attr_mode = atom_getlong(argv);
	if(x->attr_mode == 0) 
		x->xfade->setAttributeValue(TT("mode"), TT("lookup"));
	else 
		x->xfade->setAttributeValue(TT("mode"), TT("calculate"));
	
	return MAX_ERR_NONE;
}


// control rate fading
void fade_perform64_1(t_fade *x, t_object *dsp64, double **ins, long numins, double **outs, long numouts, long sampleframes, long flags, void *userparam)
{
	short		i;
	TTUInt8		numChannels = x->audioIn1->getNumChannelsAsInt();
	TTUInt16	vs = x->audioIn1->getVectorSizeAsInt();
	
	for(i=0; i<numChannels; i++){		
		x->audioIn1->setVector(i, vs, ins[i]);
		x->audioIn2->setVector(i, vs, ins[i+numChannels]);
	}
	
	x->xfade->process(*x->audioIn1, *x->audioIn2, *x->audioOut);
	
	for(i=0; i<numChannels; i++)
		x->audioOut->getVectorCopy(i, vs, outs[i]);
}


// signal rate fading
void fade_perform64_2(t_fade *x, t_object *dsp64, double **ins, long numins, double **outs, long numouts, long sampleframes, long flags, void *userparam)
{
	short		i;
	TTUInt16	numChannels = x->audioIn1->getNumChannelsAsInt();
	TTUInt16	vs = x->audioIn1->getVectorSizeAsInt();
	
	for(i=0; i<numChannels; i++){
		x->audioIn1->setVector(i, vs, ins[i]);
		x->audioIn2->setVector(i, vs, ins[i+numChannels]);
	}
	//TOOO: we cast the signal to a float, if Max can handle 64bit attributes, we should change that
	object_attr_setfloat(x, _sym_position, (t_float)*ins[numChannels*2]);
	
	x->xfade->process(*x->audioIn1, *x->audioIn2, *x->audioOut);
	
	for(i=0; i<numChannels; i++)
		x->audioOut->getVectorCopy(i, vs, outs[i]);
}


// ../../../../Jamoma/Core/DSP/library/build/JamomaDSP.dylib Method
void fade_dsp64(t_fade *x, t_object *dsp64, short *count, double samplerate, long maxvectorsize, long flags)
{
	short		i, j, k;
	TTUInt8		numChannels = 0;	
	
	// audioVectors[] passed to balance_perform() as {x, audioInL[0], audioInR[0], audioOut[0], audioInL[1], audioInR[1], audioOut[1],...}
	for(i=0; i < x->numChannels; i++){
		j = x->numChannels + i;
		k = x->numChannels*2 + i + 1;	// + 1 to account for the position input
		if(count[i] && count[j] && count[k])
			numChannels++;			
	}
	
	x->audioIn1->setNumChannels(numChannels);
	x->audioIn2->setNumChannels(numChannels);
	x->audioOut->setNumChannels(numChannels);
	x->audioIn1->setVectorSizeWithInt((TTUInt16)maxvectorsize);
	x->audioIn2->setVectorSizeWithInt((TTUInt16)maxvectorsize);
	x->audioOut->setVectorSizeWithInt((TTUInt16)maxvectorsize);
	//audioIn will be set in the perform method
	x->audioOut->alloc();	
	
	x->xfade->setAttributeValue(kTTSym_sampleRate, samplerate);
	
	if(count[x->numChannels * 2])		// SIGNAL RATE CROSSFADE CONNECTED
		object_method(dsp64, gensym("dsp_add64"), x, fade_perform64_2, 0, NULL);
	else
		object_method(dsp64, gensym("dsp_add64"), x, fade_perform64_1, 0, NULL);
}
