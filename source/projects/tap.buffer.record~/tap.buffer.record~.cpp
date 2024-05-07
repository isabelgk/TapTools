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
#include "../ttblue/tt_audio_base.h"

#define MAX_FADE_SIZE 256

typedef struct _record {
    t_pxobject	x_obj;
    double		increment_steps[MAX_FADE_SIZE];	// Store the increment step sizes for the buffer
	long		current_record_loc;    			// Store the redord head's position in the buffer
    t_symbol 	*sym;							// Current Buffer Name
    t_buffer 	*buf;							// The Buffer's Reference
    long 		l_chan;							// Current Channel of buffer
    long		record_toggle;		
    long		loop_toggle;		
    long		fade;							// length of the fade in samples
} t_record;

// Prototypes for methods: need a method for each incoming message type
void *record_new(t_symbol *msg, short argc, t_atom *argv);			// New Object Creation Method
t_int *record_perform(t_int *w);									// An MSP Perform (signal) Method
void record_dsp(t_record *x, t_signal **sp, short *count);			// ../../../../Jamoma/Core/DSP/library/build/JamomaDSP.dylib Method
void record_assist(t_record *x, void *b, long m, long a, char *s);	// Assistance Method
t_max_err record_notify(t_record *x, t_symbol *s, t_symbol *msg, void *sender, void *data);
void record_int(t_record *x, long value);							// Toggle on/off
void record_doset(t_record *x, t_symbol *s);
void record_set(t_record *x, t_symbol *s);							// Sets the buffer
void record_loop(t_record *x, long value);							// Loop toggle.
void record_fade(t_record *x, long value);
void record_chan(t_record *x, long value);							// Set record channel

static t_class*		s_record_class;
static t_symbol*	ps_globalsymbol_binding;
static t_symbol*	ps_globalsymbol_unbinding;
static t_symbol*	ps_buffer_modified;


/************************************************************************************/
// Main() Function

extern "C" int C74_EXPORT main(void)
{
	t_class *c = class_new("tap.buffer.record~",(method)record_new, (method)dsp_free, sizeof(t_record), (method)0L, A_GIMME, 0);

	common_symbols_init();

	class_addmethod(c, (method)record_int,		"int", A_LONG, 0L);
	class_addmethod(c, (method)record_set,		"set", A_SYM, 0L);
 	class_addmethod(c, (method)record_dsp, 		"dsp", A_CANT, 0L);		
	class_addmethod(c, (method)record_notify,	"notify",		A_CANT,	0);
	class_addmethod(c, (method)record_assist, 	"assist", A_CANT, 0L); 
	class_addmethod(c, (method)stdinletinfo,	"inletinfo",	A_CANT, 0);

	CLASS_ATTR_LONG(c,		"loop",			0,	t_record, loop_toggle);
	CLASS_ATTR_STYLE(c,		"loop",			0,	"onoff");
	
	CLASS_ATTR_LONG(c,		"fade",			0,	t_record, fade);
	CLASS_ATTR_LONG(c,		"channel",		0,	t_record, l_chan);

	class_dspinit(c);									// Setup object's class to work with MSP
	class_register(_sym_box, c);
	s_record_class = c;

	ps_globalsymbol_binding = gensym("globalsymbol_binding");
	ps_globalsymbol_unbinding = gensym("globalsymbol_unbinding");
	ps_buffer_modified = gensym("buffer_modified");	
	return 0;
}


/************************************************************************************/
// Object Creation Method

void *record_new(t_symbol *msg, short argc, t_atom *argv)
{
	t_atom_long chan = 1, fade = 5;
	t_symbol *name = 0;
    t_record *x;
    long	attrstart;

	attrstart = attr_args_offset(argc, argv);
	// Backward Compatibility - ARG 1
	if(attrstart && argv){
		atom_arg_getsym(&name, 0, attrstart, argv);	// support a normal int argument
		
		// Backward Compatibility - ARG 2
		if((attrstart-1) && (argv-1)){
			atom_arg_getlong(&chan, 0, attrstart+1, argv+1);	// support a normal int argument

			// Backward Compatibility - ARG 2
			if((attrstart-2) && (argv-2)){
				atom_arg_getlong(&fade, 0, attrstart+2, argv+2);	// support a normal int argument
			}
		}
	}
 
    x = (t_record *)object_alloc(s_record_class);	
	if(x){
	   	object_obex_store((void *)x, _sym_dumpout, (object *)outlet_new(x,NULL));	// dumpout	
	    dsp_setup((t_pxobject *)x,1);						// Create Object and 1 Inlet (last argument)
	    outlet_new((t_pxobject *)x, "signal");				// Create a signal Outlet

		x->record_toggle = 0;
		x->loop_toggle = 0;
		if(fade == 0) 
			fade = 10;										// length of the fade in samples
		record_set(x, name);								// Buffer name argument
		x->l_chan = TTClip<int>(chan, 1, 4);	// Channel argument
		x->fade = TTClip<int>(fade, 1, 255);	// Fade argument
		
		attr_args_process(x, argc, argv);					// handle attribute args
	}
    return (x);												// Return the pointer
}


/************************************************************************************/
// Methods bound to input/inlets

// Method for Assistance Messages
void record_assist(t_record *x, void *b, long msg, long a, char *dst)
{
	if(msg==1) 		// Inlets
		strcpy(dst, "(signal/commands) Input");
	else if(msg==2) 	// Outlets
		strcpy(dst, "(signal) Sync Output");
}


t_max_err record_notify(t_record *x, t_symbol *s, t_symbol *msg, void *sender, void *data)
{
	if (msg == ps_globalsymbol_binding)
		x->buf = (t_buffer*)x->sym->s_thing;
	else if (msg == ps_globalsymbol_unbinding)
		x->buf = NULL;
	//else if (msg == ps_buffer_modified)
	//	x->changed = true;
	
	return MAX_ERR_NONE;
}


// Method for Ints
void record_int(t_record *x, long value)
{
	x->record_toggle = value;
	if(value==0) 
		x->current_record_loc = 0;	
}


// Perform (signal) Method
t_int *record_perform(t_int *w)
{
   	t_record *x = (t_record *)(w[1]);			// Pointer
    t_float *in = (t_float *)(w[2]);   			// Input   
    t_float *out = (t_float *)(w[3]);			// Output
	int n = (int)(w[4]);						// Vector Size	

	double 	xx, frames_inv, fade_inv;			// Used to hold input, 1 / frames, 1/ fade								
	t_buffer *b = x->buf;						// Our Buffer
	float 	*tab;								// Will point to our buffer's values
	long 	frame_index, sample_index, index_wrap, frames;//, saveinuse;			// record index, wrapped index, buffer length
	long	num_samples;
	char 	chan, nc;							// a counter, current channel, number of channels
	char	fade = x->fade;						// length of the fade in samples
	double	increment_steps[MAX_FADE_SIZE];		// the interpolation step sizes
	short	i;
	
	if (x->x_obj.z_disabled) 
		return (w+5);				// If the object is disabled, skip it
	if (x->buf == 0) 
		return (w+5);				// If the buffer is invalid, skip it
	
	if (x->record_toggle == 0) {	// ../../../../Jamoma/Core/DSP/library/build/JamomaDSP.dylib Loop if NOT recording
		while (n--)
			*out++ = 0;
		return (w+5);
	}
	
	// LOCK DOWN THE BUFFER
	ATOMIC_INCREMENT((int32_t*)&b->b_inuse);
	if (!x->buf->b_valid)
		goto out;
	
	tab = b->b_samples;						// point tab to our sample values
	chan = x->l_chan - 1;					// get the buffer's audio channel to look at
	frames = b->b_frames;					// get the buffer's length
	frames_inv = 1.0 / (double)frames;
	num_samples = b->b_size;;
	nc = b->b_nchans;						// get number of channels
	frame_index = x->current_record_loc;	// Sample location into a local variable
	fade_inv = 1.0 / (float)fade;
	
	for (i=0; i<fade; i++) 
		increment_steps[i] = x->increment_steps[i];		// load increment_steps into a local array
		
	while (n--) {
		xx = *in++;														// Input Sample
		
		// WRAP FRAME LOCATION CORRECTLY
		if (frame_index < 0) 
			frame_index = 0;											// Set a minimum sample index
		else if ((frame_index >= frames) && (x->loop_toggle != 0)) 
			frame_index = 0;											// Set a maximum index based on buffer size, then loop
		else if ((frame_index >= frames) && (x->loop_toggle == 0)){
			x->record_toggle = 0;
			frame_index = 0;
			*out++ = 0;
			goto out;
		}	

		// DE-INTERLEAVE THE CHANNEL WE NEED
		if (nc > 1) 
			sample_index = frame_index * (nc) + chan;
		else 
			sample_index = frame_index;


		// 1. find the final destination, increment it, and write it to the correct location 
		index_wrap = tt_audio_base::onewrap((long)(frame_index - fade), 0L, frames - 1);		// index is in the range of 0 to frames-1
		if (nc > 1) 
			index_wrap = index_wrap * (nc) + chan;
		tab[index_wrap] = tab[index_wrap] + increment_steps[fade -1];	// take the value at the index (wrapped for the length of the fade) and increment it
		
		// 2. shift all of the stored data to the right
		for (i = fade - 1; i > 0; i--)
			increment_steps[i] = increment_steps[i-1];
				
		// 3. get sample value at current index and calculate the step size to get from there to the new one
		increment_steps[0] = (xx - (tab[TTClip(sample_index, 0L, num_samples - 1)])) * fade_inv; 
		
		// 4. increment all of the values and then write them to the buffer
		for (i=0;i < fade - 1;i++) {
			index_wrap = tt_audio_base::onewrap((long)(frame_index - i), 0L, frames - 1);
			if(nc > 1) 
				index_wrap = index_wrap * (nc) + chan;
			tab[index_wrap] = tab[index_wrap] + increment_steps[i];
		}

		*out++ = ((float)(frame_index++)) * frames_inv;	// output the normalized index
	}					                       

	for (i=0;i<fade;i++) 
		x->increment_steps[i] = increment_steps[i];	// store increment_steps from the local array
	x->current_record_loc = frame_index;
	object_method((t_object*)b, gensym("dirty"));
	
out:
	ATOMIC_DECREMENT((int32_t*)&b->b_inuse);
    return (w + 5);						// Return a pointer to the NEXT object in the ../../../../Jamoma/Core/DSP/library/build/JamomaDSP.dylib call chain
}


// Set Buffer Method - (also used by ../../../../Jamoma/Core/DSP/library/build/JamomaDSP.dylib method)
void record_set(t_record *x, t_symbol *s)
{
	if(s != x->sym){
		x->buf = (t_buffer*)globalsymbol_reference((t_object*)x, s->s_name, "buffer~");
		if(x->sym)
			globalsymbol_dereference((t_object*)x, x->sym->s_name, "buffer~");
		x->sym = s;
		//x->changed = true;
	}
}



// ../../../../Jamoma/Core/DSP/library/build/JamomaDSP.dylib Method
void record_dsp(t_record *x, t_signal **sp, short *count)
{
	short i;

	for(i=0; i<MAX_FADE_SIZE; i++)			// Clear our memory when the ../../../../Jamoma/Core/DSP/library/build/JamomaDSP.dylib is turned on...
    	x->increment_steps[i] = 0;
		
	x->current_record_loc = 0;
	dsp_add(record_perform, 4, x, sp[0]->s_vec, sp[1]->s_vec, sp[0]->s_n); // Add Perform Method to the ../../../../Jamoma/Core/DSP/library/build/JamomaDSP.dylib Call Chain
}
