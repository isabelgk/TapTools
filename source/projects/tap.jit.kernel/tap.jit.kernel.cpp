/* 
 *	External object for Max/MSP
 *	Copyright © 2003 by Timothy Place
 * 
 *	License: This code is licensed under the terms of the "New BSD License"
 *	http://creativecommons.org/licenses/BSD/
 */

#define NO_COMMON_SYMS
#include "ext.h"
#include "ext_obex.h"
#include "jit.common.h"
#include "TTClassWrapperMax.h"

#define JIT_2D_GET_FLOAT32(bp,info,plane,x,y) \
	(*(float *)(((uchar *)(bp))+(plane*4)+((info)->dimstride[0]*(x))+((info)->dimstride[1]*(y))))
#define JIT_3D_GET_FLOAT32(bp,info,plane,x,y,z) \
	(*(float *)(((uchar *)(bp))+(plane*4)+((info)->dimstride[0]*(x))+((info)->dimstride[1]*(y))+((info)->dimstride[2]*(z))))
#define JIT_2D_SET_FLOAT32(bp,info,plane,x,y,val) \
	(*(float *)(((uchar *)(bp))+(plane*4)+((info)->dimstride[0]*(x))+((info)->dimstride[1]*(y)))=(val))


// Jitter object struct
typedef struct _jit_kernel 
{
	t_object		ob;
	long					centercount, sizecount, mode;
	double					center[2], size[2], weight;

} t_jit_kernel;


// Pointer to this jitter object class
void *_jit_kernel_class;


// Prototypes
t_jit_err jit_kernel_init(void); 
t_jit_kernel *jit_kernel_new(void);
void jit_kernel_free(t_jit_kernel *x);
t_jit_err jit_kernel_matrix_calc(t_jit_kernel *x, void *inputs, void *outputs);
void jit_kernel_calculate_ndim(t_jit_kernel *x, long dimcount, long *dim, long planecount, t_jit_matrix_info *out_minfo, char *bop);
void jit_kernel_vector_char		(long n, t_jit_op_info *out); 
void jit_kernel_vector_long		(long n, t_jit_op_info *out); 
void jit_kernel_vector_float32	(long n, t_jit_op_info *out, long x_dim, long y_dim);
void jit_kernel_vector_float64	(long n, t_jit_op_info *out); 


/**************************************************************************************/
// CLASS INIT
t_jit_err jit_kernel_init(void) 
{
	long attrflags=0;
	t_jit_object *attr, *mop;
	
	_jit_kernel_class = jit_class_new("jit_kernel",(method)jit_kernel_new,(method)jit_kernel_free, sizeof(t_jit_kernel),A_CANT,0L); 

	//add mop
	mop = (t_jit_object *)jit_object_new(_sym_jit_mop,0,1); 
	jit_class_addadornment(_jit_kernel_class,mop);
	
	//add methods
	jit_class_addmethod(_jit_kernel_class, (method)jit_kernel_matrix_calc, "matrix_calc", A_CANT, 0L);


	//add attributes	
	attrflags = JIT_ATTR_GET_DEFER_LOW | JIT_ATTR_SET_USURP_LOW;

	// ATTRIBUTE: center -- sets the center location of the kernel
	attr = (t_object *)jit_object_new(_sym_jit_attr_offset_array, "center", _sym_float64, 2, 
		attrflags, (method)0L, (method)0L, calcoffset(t_jit_kernel, centercount),
		calcoffset(t_jit_kernel, center));
	jit_class_addattr(_jit_kernel_class,attr);

	// ATTRIBUTE: size -- sets the x/y sizes of the kernel
	attr = (t_object *)jit_object_new(_sym_jit_attr_offset_array, "size", _sym_float64, 2, 
		attrflags, (method)0L, (method)0L, calcoffset(t_jit_kernel, sizecount),
		calcoffset(t_jit_kernel,size));
	jit_class_addattr(_jit_kernel_class,attr);

	// ATTRIBUTE: weight -- sets the overall weight of the kernel
	attr = (t_object *)jit_object_new(_sym_jit_attr_offset,"weight", _sym_float64,attrflags,
		(method)0L,(method)0L,calcoffset(t_jit_kernel,weight));
	jit_class_addattr(_jit_kernel_class,attr);


	jit_class_register(_jit_kernel_class);

	return JIT_ERR_NONE;
}


/**************************************************************************************/
// INSTANCE CREATOR & DESTRUCTOR

t_jit_kernel *jit_kernel_new(void)
{
	t_jit_kernel *x;
		
	if (x=(t_jit_kernel *)jit_object_alloc(_jit_kernel_class)) {
		x->center[0] = 0.5;
		x->center[1] = 0.5;
		x->size[0] = 1.0;
		x->size[1] = 1.0;
		x->weight = 1.0;
	} else {
		x = NULL;
	}	
	return x;
}

void jit_kernel_free(t_jit_kernel *x)
{
	//nada
}


/**************************************************************************************/
// MATRIX CALCULATION

//
//recursive functions to handle higher dimension matrices, by processing 2D sections at a time
//

void jit_kernel_calculate_ndim(t_jit_kernel *x, long dimcount, long *dim, long planecount, t_jit_matrix_info *out_minfo, char *bop)
{
	long i,j,width,height;//, index;
//	long mode;//
//	float indperc;
//	uchar *ip,*op;
	float center[2], size[2], weight;
	
	float temp1, temp2, temp3;
	
	center[0] = x->center[1];	// Not sure why, but these need to be reversed
	center[1] = x->center[0];
	size[0] = 1.0 / x->size[1];	// (see above)
	size[1] = 1.0 / x->size[0];
	weight = 1.0 / x->weight;

	if (dimcount<1) return; 	//SAFETY
	
	switch(dimcount){
	case 1:
		dim[1]=1;				// Set this and then fall through
	case 2:
		width  = dim[0];
		height = dim[1];
				
		for (i=0;i<height;i++){								// TRAVERSE Y-AXIS
			for (j=0;j<width;j++) {								// TRAVERSE X-AXIS
	
				temp1 = i - (width * center[0]);
				temp2 = j - (height * center[1]);
				
				temp1 = (float)temp1 / (float)(width * 0.5);			// scale down
				temp2 = (float)temp2 / (float)(height * 0.5);			// scale down
				temp1 = temp1 * size[0];								// size: x
				temp2 = temp2 * size[1];								// size: y
				
				temp3 = sqrt((temp1 * temp1) + (temp2 * temp2));		// pythagrean
				//temp3 = temp3 * width;								// ?
				temp3 = temp3 * weight;									// weight
				
				temp3 -= 1.0;
				temp3 *= -1.0;
				temp3 = TTClip<float>(temp3, 0.0001F, 1.0F);

				JIT_2D_SET_FLOAT32(bop, out_minfo, 1, j-1, i, temp3);
			}
		}
	}
}


















t_jit_err jit_kernel_matrix_calc(t_jit_kernel *x, void *inputs, void *outputs)
{
	t_jit_err err=JIT_ERR_NONE;
	long out_savelock;
	t_jit_matrix_info out_minfo;
	char *out_bp;
	long i,dimcount,planecount,dim[JIT_MATRIX_MAX_DIMCOUNT];
	void *out_matrix;
	
	out_matrix 	= jit_object_method(outputs, _sym_getindex,0);

	if (x&&out_matrix) {
		out_savelock = (long) jit_object_method(out_matrix, _sym_lock,1);
		
		jit_object_method(out_matrix, _sym_getinfo,&out_minfo);
		
		jit_object_method(out_matrix, _sym_getdata,&out_bp);
		
		if(!out_bp){
			err=JIT_ERR_INVALID_OUTPUT;
			goto out;
		}
		
		//get dimensions/planecount
		dimcount   = out_minfo.dimcount;
		planecount = out_minfo.planecount;			
		
		for(i=0;i<dimcount;i++){
			dim[i] = out_minfo.dim[i];
		}
				
		jit_kernel_calculate_ndim(x, dimcount, dim, planecount, &out_minfo, out_bp);
	} 
	else{
		return JIT_ERR_INVALID_PTR;
	}
	
out:
	jit_object_method(out_matrix,gensym("lock"),out_savelock);
	return err;
}


// NEXT LAYER OF CALCULATION


/**************************************************************************************/
// ACTUAL CALCULATION LOOPS

// CHAR
void jit_kernel_vector_char(long n, t_jit_op_info *out) 
{
	uchar *op;
	long os;
	
	op  = ((uchar *)out->p);
	os  = out->stride; 
	
	if (os==1){
		++n;--op;
		while(--n){
			*++op = 0.5; //shift right to remove low order correlation
		}		
	} 
	else{
		while(n--){
			*op = 1.0;
			op+=os;
		}
	}
}


// LONG
void jit_kernel_vector_long(long n, t_jit_op_info *out) 
{
	long *op;
	long os;
	
	op  = ((long *)out->p);
	os  = out->stride; 
	
	if (os==1){
		++n;--op;
		while(--n){
			*++op = 0.5; //shift right to remove low order correlation
		}		
	} 
	else{
		while (n--) {
			*op = 1.0;
			op+=os;
		}
	}
}


// FLOAT32
void jit_kernel_vector_float32(long n, t_jit_op_info *out, long x_dim, long y_dim) 
{
	float *op;
	long os;
//	long width, height;
	
	op  = ((float *)out->p);
	os  = out->stride; 
		
	if (os==1){
		++n;--op;
		while (--n){
			*++op = 0.5; 
		}		
	}
}


// FLOAT64
void jit_kernel_vector_float64(long n, t_jit_op_info *out) 
{
	double *op;
	long os;
	
	op  = ((double *)out->p);
	os  = out->stride; 
	
	if (os==1) {
		++n;--op;
		while (--n) {
			*++op = 0.5; 
		}		
	} else {
		while (n--) {
			*op = 1.0; 	
			op+=os;
		}
	}
}
