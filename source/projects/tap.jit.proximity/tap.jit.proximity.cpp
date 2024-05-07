/* 
 *	External object for Max/MSP
 *	Copyright © 2003 by Timothy Place
 * 
 *	License: This code is licensed under the terms of the "New BSD License"
 *	http://creativecommons.org/licenses/BSD/
 */

#include "TTClassWrapperMax.h"
#include "jit.common.h"

#define JIT_2D_GET_FLOAT32(bp,info,plane,x,y) \
	(*((float *)(((uchar*)(bp))+  \
	((info)->dimstride[0]*(x))+ \
	((info)->dimstride[1]*(y))+ \
	(plane))))

typedef struct _proximity{
	t_object			ob;
	void 				*outlet[1];
	t_symbol			*name; 			// ATTRIBUTE
} t_proximity;

void *proximity_new(t_symbol *s, long argc, t_atom *argv);
void proximity_free(t_proximity *x);
void proximity_assist(t_proximity *x, void *b, long m, long a, char *s);
void proximity_coords(t_proximity *x, double xin, double yin);


static t_class *proximity_class;		 	


/**************************************************************************************/
// MAIN

extern "C" int C74_EXPORT main(void)
{
	long attrflags = 0;
	t_class *c;
	t_object *attr;
	
	c = class_new("tap.jit.proximity", (method)proximity_new, (method)proximity_free, sizeof(t_proximity), 
		(method)0L, A_GIMME, 0);

	common_symbols_init();
	
    class_addmethod(c, (method)proximity_coords,	"coords", A_FLOAT, A_FLOAT, 0L);
    class_addmethod(c, (method)proximity_assist, 	"assist", A_CANT, 0L); 
	class_addmethod(c, (method)stdinletinfo,		"inletinfo",	A_CANT, 0);

	// attrflags = JIT_ATTR_GET_DEFER_LOW | JIT_ATTR_SET_USURP_LOW ;
	
	// ATTRIBUTE: matrix name
	attr = attr_offset_new("name", _sym_symbol, attrflags, (method)0, (method)0, calcoffset(t_proximity, name));
	class_addattr(c, attr);
	
class_register(_sym_box, c); 	proximity_class = c;
}


/**************************************************************************************/
// NEW OBJECT METHOD

void *proximity_new(t_symbol *s, long argc, t_atom *argv)
{
	t_proximity *x;
	
	x = (t_proximity *)object_alloc(proximity_class);;
	if(x){
		object_obex_store((void *)x, _sym_dumpout, (object *)outlet_new(x,NULL));	// dumpout

		x->outlet[0] = outlet_new(x,0L);
		x->name = _sym_nothing;
		// no normal args, no matrices
		attr_args_process(x,argc,argv);
	}
	return (x);
}


/**************************************************************************************/
// BOUND METHODS

// ASSISTANCE STRINGS
void proximity_assist(t_proximity *x, void *b, long m, long a, char *s)
{
	if (m == 1){ 									//input
		strcpy(s,"control space coordinates");
	}
	else{ 											//output
		switch (a){
		case 0:
			strcpy(s,"(list) interpolated data");
			break; 			
		case 1:
			strcpy(s,"dumpout");
			break; 			
		}
	}
}


// The Big Beef
void proximity_coords(t_proximity *x, double xin, double yin)
{
	long i, j;//,j,k,n,maxsize;
	float tempx, tempy, sum = 100000;
	float xdifs[250];
	float ydifs[250];
	t_atom outlist[250];
	
	void *matrix;
	long dimcount,dim[JIT_MATRIX_MAX_DIMCOUNT];
	long in_savelock;
	t_jit_matrix_info in_minfo;
	char *in_bp;

	long coors[2];
		coors[0] = xin;
		coors[1] = yin;	

	// Store a pointer to the specified matrix object
	matrix = jit_object_findregistered(x->name);
	
	// CALCULATION LOOP (assuming the matrix is valid and has data)
	if (matrix&&jit_object_method(matrix, _sym_class_jit_matrix)){
		in_savelock = (long) jit_object_method(matrix,_sym_lock,1);
		jit_object_method(matrix,_sym_getinfo,&in_minfo);
		jit_object_method(matrix,_sym_getdata,&in_bp);
		
		// ERROR HANDLING FOR DATALESS MATRICES
		if (!in_bp) { 
			//jit_error_sym(x, gensym("err_calculate"));
			jit_object_method(matrix, _sym_lock,in_savelock);
			goto out;
		}
		// get dimensions/planecount/etc.
		dimcount = in_minfo.dimcount;
		for (i=0;i<dimcount;i++) {
			dim[i] = in_minfo.dim[i];
		}	

		// ************************************
		// DO IT!		
		
		for(i=0; i < dim[1]; i++){		// for the y size of the data matrix
			tempx = JIT_2D_GET_FLOAT32(in_bp, &in_minfo, 0, 0, i);
			tempy = JIT_2D_GET_FLOAT32(in_bp, &in_minfo, 0, 1, i);
			xdifs[i] = fabs(coors[0] - tempx);
			ydifs[i] = fabs(coors[1] - tempy);
		}
		for(j=0; j<i; j++){
			if((xdifs[j] + ydifs[j]) < sum){
				sum = xdifs[j] + ydifs[j];
				atom_setlong(&(outlist[0]),j+1);
				atom_setfloat(&(outlist[1]), JIT_2D_GET_FLOAT32(in_bp, &in_minfo, 0, 0, j));
				atom_setfloat(&(outlist[2]), JIT_2D_GET_FLOAT32(in_bp, &in_minfo, 0, 1, j));
			}
		}
		outlet_list(x->outlet[0], 0L, 3, &outlist[0]);
		// ************************************		
		
		jit_object_method(matrix, _sym_lock,in_savelock);		
	}
	else{
		//jit_error_sym(x, gensym("err_calculate"));
	}
out:
	return;
}


// FREE MEMORY	
void proximity_free(t_proximity *x)
{
	;
}
