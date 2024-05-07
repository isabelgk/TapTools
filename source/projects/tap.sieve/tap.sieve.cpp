/* 
 *	External object for Max/MSP
 *	Copyright © 2000 by Timothy Place
 * 
 *	License: This code is licensed under the terms of the "New BSD License"
 *	http://creativecommons.org/licenses/BSD/
 */

#include "TTClassWrapperMax.h"		

typedef struct _sieve{
	t_object	obj;
	void		*outlet;
	void		*right_inlet;
	long		inletnum;
	long		value;			// value that we are trying to match
}t_sieve;

// Prototypes for methods: need a method for each incoming message
void *sieve_new(t_symbol *msg, short argc, t_atom *argv);
void sieve_free(t_sieve *x);
void sieve_int(t_sieve *x, long value);
void sieve_assist(t_sieve *sieve, void *b, long m, long a, char *s);

static t_class *s_sieve_class;


/*********************************************************/
//Main() Function

extern "C" int C74_EXPORT main(void)
{
	t_class *c;

	c = class_new("tap.sieve",(method)sieve_new, (method)sieve_free, sizeof(t_sieve), (method)0L, A_GIMME, 0);

		common_symbols_init();
	class_addmethod(c, (method)sieve_int,		"int", A_LONG, 0L);
	class_addmethod(c, (method)sieve_assist, 	"assist", A_CANT, 0L); 
	class_addmethod(c, (method)stdinletinfo,	"inletinfo",	A_CANT, 0);

class_register(_sym_box, c); 	s_sieve_class = c;
}


/*********************************************************/
//Object Creation Function

void *sieve_new(t_symbol *msg, short argc, t_atom *argv)
{
	t_sieve *x;
	long	attrstart;
	long	argument = 0;

	attrstart = attr_args_offset(argc, argv);
	if (attrstart && argv)
		argument = atom_getlong(argv);	// support a normal int argument for bwc

	x = (t_sieve *)object_alloc(s_sieve_class);;// create the new instance and return a pointer to it
	if(x){
    	object_obex_store((void *)x, _sym_dumpout, (object *)outlet_new(x,NULL));	// dumpout	
		x->value = argument;
		x->outlet = intout(x);
		x->right_inlet = proxy_new(x, 1, 0L);
		
		attr_args_process(x,argc,argv); //handle attribute args			
	}
	return(x);
}


void sieve_free(t_sieve *x)
{
	freeobject((t_object *)x->right_inlet);
}

/*********************************************************/
//Bound to inlet Functions

// Method for Assistance Messages
void sieve_assist(t_sieve *sieve, void *b, long msg, long arg, char *dst)
{
	if(msg==1){ 	// Inlets
		switch(arg){
			case 0: strcpy(dst, "Input to be tested"); break;
			case 1: strcpy(dst, "Set a new item to match"); break;
		}
	}
	else if(msg==2){ // Outlets
		switch(arg){
			case 0: strcpy(dst, "The matched input"); break;
			case 1: strcpy(dst, "dumpout"); break;
		}
	}
}


// INT in left inlet
void sieve_int(t_sieve *x, long value)
{
	int	i;
	long inletnum = proxy_getinlet((object *)x);

	if(inletnum==0){
		i = x->value;
		if( value == i)
			outlet_int(x->outlet, x->value);
	}
	else{
		x->value = value;
	}
}
