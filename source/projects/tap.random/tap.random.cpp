/* 
 *	External object for Max/MSP
 *	Copyright © 2001 by Timothy Place
 * 
 *	License: This code is licensed under the terms of the "New BSD License"
 *	http://creativecommons.org/licenses/BSD/
 */

#include "TTClassWrapperMax.h"
//#include <stdlib.h>

static t_class *taprandom_class;				// Required. Global pointing to this class 

// Data structure for this object 
typedef struct _taprandom{
	Object	p_ob;					// Must always be the first field; used by Max
	double	p_value;				// Value from which to determine next number
	void	*p_out;					// Pointer to outlet. need one for each outlet 
} t_taprandom;
		
// Prototypes for methods: need a method for each incoming message
void *taprandom_new(void);															// object creation method  
void taprandom_bang(t_taprandom *taprandom);										// method for "bang" message
void taprandom_assist(t_taprandom *taprandom, void *b, long m, long a, char *s); 	// assistance messages


/*********************************************************/
//Main() Function

extern "C" int C74_EXPORT main(void)
{
	t_class *c;

	c = class_new("tap.random",(method)taprandom_new, (method)0L, sizeof(t_taprandom), 
		(method)0L, 0);

		common_symbols_init();										// Initialize TapTools
    class_addmethod(c, (method)taprandom_bang,		"bang", 0L);
    class_addmethod(c, (method)taprandom_assist, 	"assist", A_CANT, 0L); 
	class_addmethod(c, (method)stdinletinfo,		"inletinfo",	A_CANT, 0);

class_register(_sym_box, c); 	taprandom_class = c;
}


/*********************************************************/
//Object Creation Function

void *taprandom_new(void)
{
	t_taprandom *x;
	x = (t_taprandom *)object_alloc(taprandom_class);;	// create the new instance and return a pointer to it 
	if(x){
		object_obex_store((void *)x, _sym_dumpout, (object *)outlet_new(x,NULL));	// dumpout	
		x->p_out = floatout(x);					// Create the outlet
	}
	return(x);									// must return a pointer to the new instance 
}


/*********************************************************/
//Bound to inlet Functions


// Method for Assistance Messages
void taprandom_assist(t_taprandom *taprandom, void *b, long msg, long arg, char *dst)
{
	if(msg==1) 	// Inlets
		strcpy(dst, "(bang) Generate a random number");
	else if(msg==2) // Outlets
		strcpy(dst, "(float) Random number");
}


// BANG in left inlet
void taprandom_bang(t_taprandom *taprandom)
{
	int i;
	int randx = rand();

	i = ((randx = randx*1103515245 + 12345)>>16) & 077777;
	taprandom->p_value = (float)i/16384. - 1.;

	outlet_float(taprandom->p_out, taprandom->p_value);		// output the result
}
