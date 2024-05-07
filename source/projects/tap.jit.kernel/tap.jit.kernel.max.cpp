/* 
 *	External object for Max/MSP
 *	Copyright © 2003 by Timothy Place
 * 
 *	License: This code is licensed under the terms of the "New BSD License"
 *	http://creativecommons.org/licenses/BSD/
 */

#include "ext.h"
#include "jit.common.h"
#include "max.jit.mop.h"
#include "commonsyms.h"
#include "TTClassWrapperMax.h"

// Class Struct Definition
typedef struct _max_jit_kernel 
{
	t_object		ob;
	void			*obex;
} t_max_jit_kernel;


// Prototypes
t_jit_err jit_kernel_init(void); 
void *max_jit_kernel_new(t_symbol *s, long argc, t_atom *argv);
void max_jit_kernel_free(t_max_jit_kernel *x);
void max_jit_kernel_outputmatrix(t_max_jit_kernel *x);


// Class Globals
void *max_jit_kernel_class;


/**************************************************************************************/
// MAIN
extern "C" int C74_EXPORT main(void)
{	
	void *p,*q;
	
	common_symbols_init();
	jit_kernel_init();	
	setup((t_messlist**)&max_jit_kernel_class,  (method)max_jit_kernel_new, (method)max_jit_kernel_free, (short)sizeof(t_max_jit_kernel), 
		0L, A_GIMME, 0);

	p = max_jit_classex_setup(calcoffset(t_max_jit_kernel,obex));
	q = jit_class_findbyname(gensym("jit_kernel"));    
    max_jit_classex_mop_wrap(p,q,MAX_JIT_MOP_FLAGS_OWN_OUTPUTMATRIX|MAX_JIT_MOP_FLAGS_OWN_JIT_MATRIX); 		
    max_jit_classex_standard_wrap(p,q,0); 	
	max_addmethod_usurp_low((method)max_jit_kernel_outputmatrix, "outputmatrix");	
    addmess((method)max_jit_mop_assist, "assist", A_CANT,0);
    
	return 0;
}


/**************************************************************************************/
// NEW OBJECT METHOD

void *max_jit_kernel_new(t_symbol *s, long argc, t_atom *argv)
{
	t_max_jit_kernel *x;
	void *o;

	if (x=(t_max_jit_kernel *)max_jit_obex_new(max_jit_kernel_class,gensym("jit_kernel"))) {
		if (o=jit_object_new(gensym("jit_kernel"))) {
			max_jit_mop_setup_simple(x,o,argc,argv);			
			max_jit_attr_args(x,argc,argv);
		} else {
			error("jit.kernel: could not allocate object");
			freeobject((object *)x);
		}
	}
	return (x);
}


/**************************************************************************************/
// OTHER METHODS

// MOP
void max_jit_kernel_outputmatrix(t_max_jit_kernel *x)
{
	//t_atom a;
	long outputmode=max_jit_mop_getoutputmode(x);
	void *mop=max_jit_obex_adornment_get(x, _sym_jit_mop);
	t_jit_err err;	
	
	if (outputmode&&mop) { //always output unless output mode is none
		if (outputmode==1) {
			if (err=(t_jit_err)jit_object_method(
				max_jit_obex_jitob_get(x), 
				_sym_matrix_calc,
				jit_object_method(mop, _sym_getinputlist),
				jit_object_method(mop, _sym_getoutputlist)))						
			{
				jit_error_code(x,err); 
			} else {
				max_jit_mop_outputmatrix(x);
			}
		}
	}	
}


// FREE MEMORY
void max_jit_kernel_free(t_max_jit_kernel *x)
{
	max_jit_mop_free(x);
	jit_object_free(max_jit_obex_jitob_get(x));
	max_jit_obex_free(x);
}

