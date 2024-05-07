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


typedef struct _peak {
    t_object 	obj;
	void*		outlet;
    t_symbol*	sym;		// Current Buffer Name.
    t_buffer*	buf;		// Buffer Reference.
	long		index;		// The cached index of the peak sample.
	long		changed;	// Flag to indicate if the buffer changed since the last time.
} t_peak;


// Function & Method Prototypes
void*		peak_new(t_symbol *s);
void		peak_free(t_peak *x);
t_max_err	peak_notify(t_peak *x, t_symbol *s, t_symbol *msg, void *sender, void *data);
void		peak_assist(t_peak *x, void *b, long m, long a, char *s);
void		peak_dsp(t_peak *x, t_signal **sp, short *count);
void		peak_set(t_peak *x, t_symbol *s);
void		peak_calc(t_peak *x);


static t_class*		s_peak_class;
static t_symbol*	ps_globalsymbol_binding;
static t_symbol*	ps_globalsymbol_unbinding;
static t_symbol*	ps_buffer_modified;


/*************************************************************************************/
// Main() Function

extern "C" int C74_EXPORT main(void)
{
	t_class *c;
	
	c = class_new("tap.buffer.peak~",(method)peak_new, (method)peak_free, sizeof(t_peak), (method)0L, A_SYM, 0);

	common_symbols_init();
	class_addmethod(c, (method)peak_calc,		"bang",			A_LONG, 0);
	class_addmethod(c, (method)peak_set,		"set",			A_SYM, 0);
	class_addmethod(c, (method)peak_notify,		"notify",		A_CANT,	0);
	class_addmethod(c, (method)peak_assist, 	"assist",		A_CANT, 0); 
	class_addmethod(c, (method)stdinletinfo,	"inletinfo",	A_CANT, 0);

	class_dspinit(c);
	class_register(_sym_box, c);
	s_peak_class = c;
	
	ps_globalsymbol_binding = gensym("globalsymbol_binding");
	ps_globalsymbol_unbinding = gensym("globalsymbol_unbinding");
	ps_buffer_modified = gensym("buffer_modified");
	return 0;
}


/*************************************************************************************/
// Object Creation Routine

void *peak_new(t_symbol *s)
{
    t_peak *x = (t_peak *)object_alloc(s_peak_class);	
	if(x){
	   	object_obex_store((void *)x, _sym_dumpout, (object *)outlet_new(x,NULL));
		x->outlet = outlet_new(x, 0L);
		peak_set(x, s);
	}
    return (x);
}


void peak_free(t_peak *x)
{
	;
}


/*************************************************************************************/
// Bound to input/inlet methods


t_max_err peak_notify(t_peak *x, t_symbol *s, t_symbol *msg, void *sender, void *data)
{
	if (msg == ps_globalsymbol_binding)
		x->buf = (t_buffer*)x->sym->s_thing;
	else if (msg == ps_globalsymbol_unbinding)
		x->buf = NULL;
	else if (msg == ps_buffer_modified)
		x->changed = true;

	return MAX_ERR_NONE;
}


// Set Buffer Method - (also used by ../../../../Jamoma/Core/DSP/library/build/JamomaDSP.dylib method)
void peak_set(t_peak *x, t_symbol *s)
{
	if(s != x->sym){
		x->buf = (t_buffer*)globalsymbol_reference((t_object*)x, s->s_name, "buffer~");
		if(x->sym)
			globalsymbol_dereference((t_object*)x, x->sym->s_name, "buffer~");
		x->sym = s;
		x->changed = true;
	}
}


// Method for Assistance Messages
void peak_assist(t_peak *x, void *b, long msg, long arg, char *dst)
{
	strcpy(dst, "bang to peakalize the buffer");
}


// peak point calculation
void peak_calc(t_peak *x)
{
	if(!x->changed)
		goto done;

	if(!x->buf){
		object_error((t_object *)x, "no buffer is bound to this object");
		return;
	}
	
	{
		t_buffer	*b = x->buf;			// Our Buffer
		float		*tab;					// Will point to our buffer's values
		long		i, chan;
		double		current_samp = 0.0;		// current sample value

		ATOMIC_INCREMENT((int32_t*)&b->b_inuse);
		if (!x->buf->b_valid) {
			ATOMIC_DECREMENT((int32_t*)&b->b_inuse);
			return;
		}
		
		// FIND PEAK VALUE
		tab = b->b_samples;				// point tab to our sample values
		for(chan=0; chan < b->b_nchans; chan++){
			for(i=0; i < b->b_frames; i++){
				if(fabs(tab[(chan * b->b_nchans) + i]) > current_samp){
					current_samp = fabs(tab[(chan * b->b_nchans) + i]);
					x->index = (chan * b->b_nchans) + i;
				}
			}
		}

		ATOMIC_DECREMENT((int32_t*)&b->b_inuse);
	}
	
done:
	x->changed = false;
	outlet_int(x->outlet, x->index);
}
