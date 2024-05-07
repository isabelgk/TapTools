/* 
 *	External object for Max/MSP
 *	Copyright © 1999 by Timothy Place
 * 
 *	License: This code is licensed under the terms of the "New BSD License"
 *	http://creativecommons.org/licenses/BSD/
 */

#include "TTClassWrapperMax.h"


static t_class *this_class;				// Required. Global pointing to this class 

typedef struct _prime{				// Data structure for this object 
	t_object	p_ob;					// Must always be the first field; used by Max
	void	*right_inlet;
	long 	p_value;				// Value from which to determine next prime number
	void 	*p_out;					// Pointer to outlet. need one for each outlet 
} t_prime;

		
// Prototypes for methods: need a method for each incoming message
void	*prime_new(long value);								// object creation method  
void 	prime_free(t_prime *x);
void	prime_bang(t_prime *x);							// method for "bang" message 
void	prime_int(t_prime *x, long value);				// method for int input
void	prime_assist(t_prime *prime, void *b, long m, long a, char *s); 	// assistance messages


/*********************************************************/
//Main() Function

extern "C" int C74_EXPORT main(void)
{
	t_class *c;

	c = class_new("tap.prime", (method)prime_new, (method)prime_free, sizeof(t_prime), 
		(method)0L, A_DEFLONG, 0);

		common_symbols_init();									// Initialize TapTools
	class_addmethod(c, (method)prime_int,		"int", A_LONG, 0L);
 	class_addmethod(c, (method)prime_bang, 		"bang", 0L);		
	class_addmethod(c, (method)prime_assist, 	"assist", A_CANT, 0L); 
	class_addmethod(c, (method)stdinletinfo,	"inletinfo",	A_CANT, 0);

class_register(_sym_box, c); 	this_class = c;
}


/*********************************************************/
//Object Creation Function

void *prime_new(long value)
{
	t_prime *x;

	x = (t_prime *)object_alloc(this_class);	// create the new instance and return a pointer to it
	if (x) {
    	object_obex_store((void *)x, _sym_dumpout, (object *)outlet_new(x,NULL));	// dumpout	
		x->p_out = intout(x);		//Create the outlet
		x->right_inlet = proxy_new(x, 1, 0L);
		x->p_value = value;				//Init the prime value
	}
	return(x);						// must return a pointer to the new instance 
}


void prime_free(t_prime *x)
{
	freeobject((t_object *)x->right_inlet);
}


/*********************************************************/
//Bound to inlet Functions

// Method for Assistance Messages
void prime_assist(t_prime *x, void *b, long msg, long arg, char *dst)
{
	if(msg==1){ 	// Inlets
		switch(arg){
			case 0: strcpy(dst, "(bang/int) Step to the next prime number"); break;
			case 1: strcpy(dst, "(int) Set the next prime to step to"); break;
		}
	}
	else if(msg==2) // Outlets
		strcpy(dst, "(int) Prime number");
}


// BANG in left inlet
void prime_bang(t_prime *x)
{
	x->p_value = TTPrime(x->p_value);	// run the primeFunc function on the stored value and store the result
	outlet_int(x->p_out, x->p_value);	// output the result	
}


// INT input
void prime_int(t_prime *x, long value)
{
	long inletnum = proxy_getinlet((object *)x);

	if(inletnum==0){
		x->p_value = TTPrime(value);	// run the primeFunc function on the input value and store the result
		outlet_int(x->p_out, x->p_value);		// output the result
	}
	else if(inletnum==1){
		x->p_value = value;					//Set the value
	}
}
