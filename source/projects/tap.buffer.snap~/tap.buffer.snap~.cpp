/* 
 *	External object for Max/MSP
 *	Copyright © 2004 by Timothy Place
 * 
 *	License: This code is licensed under the terms of the "New BSD License"
 *	http://creativecommons.org/licenses/BSD/
 */

#include "TTClassWrapperMax.h"
#include "ext_globalsymbol.h"
#include "buffer.h"
#include "ext_atomic.h"

typedef struct _snap{
    t_pxobject 	obj;
    t_symbol*	name;			// Current Buffer Name
    t_buffer*	buffer;			// Buffer Reference
    long		channel;		// Current Channel of buffer [1, 4]
    void*		outlet_2;		// Floating-point outlet
	t_symbol*	attr_mode;		// mode 0 = ms, mode 1 = samples
	double		srx;			// SR-based value for the ms->samples conversion
	double		sry;			// SR-based value for the samples->ms conversion
	long		changed;		// Flag to indicate if the buffer changed since the last time.
} t_snap;


// Function & Method Prototypes
t_int *snap_perform(t_int *w);
void snap_dsp(t_snap *x, t_signal **sp, short *count);
void snap_set(t_snap *x, t_symbol *s);
void *snap_new(t_symbol *msg, short argc, t_atom *argv);
void snap_assist(t_snap *x, void *b, long m, long a, char *s);
t_max_err snap_notify(t_snap *x, t_symbol *s, t_symbol *msg, void *sender, void *data);
void snap_float(t_snap *x, double value);
void snap_int(t_snap *x, long value);
double snap_calc(t_snap *x, double value);


// Globals
static t_class*		snap_class;
static t_symbol*	ps_ms;
static t_symbol*	ps_samps;
static t_symbol*	ps_globalsymbol_binding;
static t_symbol*	ps_globalsymbol_unbinding;
static t_symbol*	ps_buffer_modified;


/*************************************************************************************/
// Main() Function

extern "C" int C74_EXPORT main(void)
{
	t_class *c;

	c = class_new("tap.buffer.snap~",(method)snap_new, (method)dsp_free, sizeof(t_snap), 
		(method)0L, A_GIMME, 0);

	common_symbols_init();
	class_addmethod(c, (method)snap_int,		"int",			A_LONG, 0L);
	class_addmethod(c, (method)snap_float,		"float",		A_FLOAT, 0L);
	class_addmethod(c, (method)snap_set,		"set",			A_SYM, 0L);
 	class_addmethod(c, (method)snap_dsp,		"dsp",			A_CANT, 0L);		
	class_addmethod(c, (method)snap_notify,		"notify",		A_CANT,	0);
	class_addmethod(c, (method)snap_assist,		"assist",		A_CANT, 0L); 
	class_addmethod(c, (method)stdinletinfo,	"inletinfo",	A_CANT, 0);

	CLASS_ATTR_SYM(c,		"mode",			0,	t_snap, attr_mode);
	CLASS_ATTR_ENUM(c,		"mode",			0,	"ms samps");
	
	CLASS_ATTR_LONG(c,		"channel",		0,	t_snap, channel);

	class_dspinit(c);									// Setup object's class to work with MSP
	class_register(_sym_box, c);
	snap_class = c;

	// Init Globals
	ps_ms = gensym("ms");
	ps_samps = gensym("samps");
	ps_globalsymbol_binding = gensym("globalsymbol_binding");
	ps_globalsymbol_unbinding = gensym("globalsymbol_unbinding");
	ps_buffer_modified = gensym("buffer_modified");
	return 0;
}


/*************************************************************************************/
// Object Creation Routine

void *snap_new(t_symbol *msg, short argc, t_atom *argv)
{
	t_atom_long chan = 1;
	t_symbol 	*name = _sym_nothing;
	t_snap 		*x; 
    long		attrstart;

	attrstart = attr_args_offset(argc, argv);
	// Backward Compatibility - ARG 1
	if(attrstart && argv){
		atom_arg_getsym(&name, 0, attrstart, argv);	// support a normal int argument
		
		// Backward Compatibility - ARG 2
		if((attrstart-1) && (argv-1)){
			atom_arg_getlong(&chan, 0, attrstart+1, argv+1);	// support a normal int argument
		}
	}
 	
	x = (t_snap *)object_alloc(snap_class);
	if(x){
	  	object_obex_store((void *)x, _sym_dumpout, (object *)outlet_new(x,NULL));	// dumpout	
		dsp_setup((t_pxobject *)x, 1);					// Left inlet (proxy)
		x->outlet_2 = floatout(x);						// Create a floating-point Outlet
		outlet_new((t_object *)x, "signal");			// Signal outlet

		x->channel = chan;
		x->attr_mode = ps_ms;							// Default to "ms" mode
		x->srx = sys_getsr() * 0.001;					// Set up our conversion variables based on the global SR
		x->sry = (1.0 / sys_getsr()) * 1000.0;			// They wil be re-inited to local SR when the ../../../../Jamoma/Core/DSP/library/build/JamomaDSP.dylib is started
		if(name != _sym_nothing)
			snap_set(x, name);

		attr_args_process(x, argc, argv);				// handle attribute args							
	}
	return (x);											// Return a pointer to our object to Max...
}



/*************************************************************************************/
// Bound to input/inlet methods

// Method for Assistance Messages
void snap_assist(t_snap *x, void *b, long msg, long arg, char *dst)
{
	if(msg==1) 		// Inlets
		strcpy(dst, "(signal/float) Input");
	else if(msg==2){ 	// Outlets
		switch(arg){
			case 0: strcpy(dst, "(signal) Output"); break;
			case 1: strcpy(dst, "(float) Output"); break;
		}
	}
}


t_max_err snap_notify(t_snap *x, t_symbol *s, t_symbol *msg, void *sender, void *data)
{
	if (msg == ps_globalsymbol_binding)
		x->buffer = (t_buffer*)x->name->s_thing;
	else if (msg == ps_globalsymbol_unbinding)
		x->buffer = NULL;
	else if (msg == ps_buffer_modified)
		x->changed = true;
	
	return MAX_ERR_NONE;
}


// method for float input
void snap_float(t_snap *x, double value)
{
	double snap;
	
	if(x->buffer != 0){
		snap = snap_calc(x, value);
		outlet_float(x->outlet_2, snap);		// output the result		
	}
	else
		object_error((t_object *)x, "invalid buffer");
}


// method for int input
void snap_int(t_snap *x, long value)
{
	snap_float(x, value);
}


// Perform (signal) method
t_int *snap_perform(t_int *w)
{
    t_snap 		*x = (t_snap *)(w[1]);		// Pointer to object
    t_float 	*in = (t_float *)(w[2]);	// input
    t_float 	*out = (t_float *)(w[3]);	// output
    int 		n = (int)(w[4]);			// vector size
 	double 		snap;

	if(x->obj.z_disabled) 
		goto end;
		
    if(x->buffer == 0){
		while(n--)
			*out++ = 0;
    }
	else{
		while(n--){
			snap = *in++;
			snap = snap_calc(x, snap);
			*out++ = snap;	
		}
	}
	
end:
	return w+5;
}


// Set Buffer Method - (also used by ../../../../Jamoma/Core/DSP/library/build/JamomaDSP.dylib method)
void snap_set(t_snap *x, t_symbol *s)
{
	if(s != x->name){
		x->buffer = (t_buffer*)globalsymbol_reference((t_object*)x, s->s_name, "buffer~");
		if(x->name)
			globalsymbol_dereference((t_object*)x, x->name->s_name, "buffer~");
		x->name = s;
		x->changed = true;
	}
}


// ../../../../Jamoma/Core/DSP/library/build/JamomaDSP.dylib method
void snap_dsp(t_snap *x, t_signal **sp, short *count)
{
	x->srx = sp[0]->s_sr * 0.001;				// Init these using the local sample rate
	x->sry = (1.0 / sp[0]->s_sr) * 1000.0;		//	...
	
    if(count[0])	// If a signal is connected to the first inlet (inlet 0)
    	dsp_add(snap_perform, 4, x, sp[0]->s_vec, sp[1]->s_vec, sp[0]->s_n);
}


/*************************************************************************************/
// Additional Functions used by object

// snap point calculation
double snap_calc(t_snap *x, double value)
{
	t_buffer 	*b = x->buffer;				// Our Buffer
	float 		*tab;						// Will point to our buffer's values
	long 		snap,index,index1,index2,chan,frames,nc,i;
	double 		current_samp;				// current sample value
	short 		above_zero, flag;			// flags

	ATOMIC_INCREMENT((int32_t*)&b->b_inuse);
	if(!b->b_valid)
		goto out;
	
	tab = b->b_samples;				// point tab to our sample values
	chan = x->channel - 1;			// get the buffer's audio channel to look at [0, 3]
	frames = b->b_frames;			// get the buffer's length
	nc = b->b_nchans;				// get number of channels

	if((chan + 1) > nc)
		goto out;
	if(frames == 0)
		goto out;
		
	if(x->attr_mode == ps_ms) 
		value = value * x->srx;	// Convert to samples if neccessary	
	snap = round(value);					// from our argument (rounds to long)
	snap = TTClip(snap, 0L, frames-1);		// limit range

	index = snap * nc + chan;

	current_samp = tab[index];		// get the sample value at the user specified snap		
	if(current_samp != 0){			// If the current sample is zero, skip all this...
		flag = 0;
		i = 0;											// Init variables
		above_zero = (current_samp > 0);				// boolean for whether or not the current sample is > zero

		while(!flag){												// 	Do this loop until it is flagged
			i += nc;													// increment the snap-distance counter
			index1 = index+i;
			index2 = index-i;
			index1 = TTClip(index1, 0L, frames * nc - 1);	// limit ranges...
			index2 = TTClip(index2, 0L, frames * nc - 1);
			
			if(above_zero != (tab[index1] > 0))
				flag = 1;											// flag as a "1" if nearest zerox is at a higher snap
			else if(above_zero != (tab[index2] > 0))
				flag = 2;											// flag as a "2" if nearest zerox is at a lower snap

			if((index1 > (frames * nc - 1)) && (index2 < 0))		// out of range?
				flag = 3;											// flag it to stop the while loop
		}
		if(flag == 1)
			snap = index1 + 1;		// THE PLUS 1 IS A HACK	- THESE HACKS DEAL WITH SOME ROUNDOFF OR SOMETHING SOMEWHERE IN HERE
		else if(flag == 2)			
			snap = index2 + 2;		// THE PLUS 2 IS A HACK
	}

	snap = (snap - chan) / nc;		// index to frames
	if(x->attr_mode == ps_ms)
		value = snap * x->sry;
	else
		value = snap;

out:
	ATOMIC_DECREMENT((int32_t*)&b->b_inuse);
	return value;					// return the result in samples					
}
