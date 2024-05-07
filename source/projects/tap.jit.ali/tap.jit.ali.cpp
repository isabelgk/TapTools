/* 
 *	External object for Max/MSP
 *	Copyright © 2003 by Timothy Place
 * 
 *	License: This code is licensed under the terms of the "New BSD License"
 *	http://creativecommons.org/licenses/BSD/
 */

//	Based on patches by Ali Momeni for the NIME'03 conference
 

// NEED TO WRITE A CUSTOM SET_ATTRIBUTE METHOD FOR out_matrix !!!

#include "TTClassWrapperMax.h"
#include "jit.common.h"

#define JIT_2D_GET_FLOAT32(bp,info,plane,x,y) \
	(*(float *)(((uchar *)(bp))+(plane*4)+((info)->dimstride[0]*(x))+((info)->dimstride[1]*(y))))
#define JIT_3D_GET_FLOAT32(bp,info,plane,x,y,z) \
	(*(float *)(((uchar *)(bp))+(plane*4)+((info)->dimstride[0]*(x))+((info)->dimstride[1]*(y))+((info)->dimstride[2]*(z))))
#define JIT_2D_SET_FLOAT32(bp,info,plane,x,y,val) \
	(*(float *)(((uchar *)(bp))+(plane*4)+((info)->dimstride[0]*(x))+((info)->dimstride[1]*(y)))=(val))

#define MAX_LIST_SIZE 500L


// Class Struct Definition
typedef struct _ali 
{
	t_object	ob;
	void 		*outlet[2];
	void		*matrix;				// pointer to the internal matrix
	t_symbol	*data_matrix; 			// ATTRIBUTE
	t_symbol	*space_matrix; 			// ATTRIBUTE
	t_symbol	*out_matrix; 			// ATTRIBUTE
	t_symbol	*output_type; 			// ATTRIBUTE
	long		data_interpolation; 	// ATTRIBUTE
	long		data_depthclip; 		// ATTRIBUTE
	long		data_widthclip;			// ATTRIBUTE
	t_atom 		outlist[MAX_LIST_SIZE];	// Temporary storage of output to be sent
} t_ali;

// Prototypes
void *ali_new(t_symbol *s, long argc, t_atom *argv);
void ali_free(t_ali *x);
void ali_assist(t_ali *x, void *b, long m, long a, char *s);
void ali_coords(t_ali *x, double xin, double yin);

// Class Globals
static t_class *ali_class;		 	
static t_symbol *ps_list;
static t_symbol *ps_jit_matrix;

/**************************************************************************************/
// MAIN

extern "C" int C74_EXPORT main(void)
{	
	long attrflags = 0;
	t_class *c;
	t_object *attr;
	
	c = (t_class*)class_new("tap.jit.ali", (method)ali_new, (method)ali_free, sizeof(t_ali), (method)0L, A_GIMME, 0);

		
	common_symbols_init();
	class_addmethod(c, (method)ali_coords,			"coords", A_FLOAT, A_FLOAT, 0L);
    class_addmethod(c, (method)ali_assist, 			"assist", A_CANT, 0L);
	class_addmethod(c, (method)stdinletinfo,		"inletinfo",	A_CANT, 0);

	// attrflags = JIT_ATTR_GET_DEFER_LOW | JIT_ATTR_SET_USURP_LOW ;

	// ATTRIBUTE: data_matrix
	attr = attr_offset_new("data_matrix", _sym_symbol, attrflags, (method)0, (method)0, calcoffset(t_ali, data_matrix));
	class_addattr(c, attr);

	// ATTRIBUTE: space_matrix
	attr = attr_offset_new("space_matrix", _sym_symbol, attrflags, (method)0, (method)0, calcoffset(t_ali, space_matrix));
	class_addattr(c, attr);
	
	// ATTRIBUTE: out_matrix (for output matrix)
	attr = attr_offset_new("out_matrix", _sym_symbol, attrflags, (method)0, (method)0, calcoffset(t_ali, out_matrix));
	class_addattr(c, attr);

	// ATTRIBUTE: output_type
	attr = attr_offset_new("output_type", _sym_symbol, attrflags, (method)0, (method)0, calcoffset(t_ali, output_type));
	class_addattr(c, attr);

	// ATTRIBUTE: data_interpolation
	attr = attr_offset_new("data_interpolation", _sym_long, attrflags, (method)0, (method)0, calcoffset(t_ali, data_interpolation));
	class_addattr(c, attr);

	// ATTRIBUTE: data_depthclip
	attr = attr_offset_new("data_depthclip", _sym_long, attrflags, (method)0, (method)0, calcoffset(t_ali, data_depthclip));
	class_addattr(c, attr);
		
	// ATTRIBUTE: data_widthclip
	attr = attr_offset_new("data_widthclip", _sym_long, attrflags, (method)0, (method)0, calcoffset(t_ali ,data_widthclip));
	class_addattr(c, attr);

class_register(_sym_box, c); 	ali_class = c;

	// init globals
	ps_list = gensym("list");
	ps_jit_matrix = gensym("jit_matrix");
}


/**************************************************************************************/
// NEW OBJECT METHOD

void *ali_new(t_symbol *s, long argc, t_atom *argv)
{
	t_ali 				*x;		// pointer to a stuct for our object
	void 				*m;		// pointer to a matrix (used for output)
	t_jit_matrix_info	info;	// matrix info parameters to set for out matrix

	x = (t_ali *)object_alloc(ali_class);;
	if (x) {
		object_obex_store((void *)x, _sym_dumpout, (object *)outlet_new(x, NULL));	// dumpout

		x->outlet[1] = outlet_new(x,0L);				// interpolated data output
		x->outlet[0] = outlet_new(x,0L);				// output for weights

		// Set Defaults
		x->output_type = ps_list;
		x->data_interpolation = 1;
		x->data_depthclip = 0;
		x->data_widthclip = 0;
		x->space_matrix = _sym_nothing;
		x->data_matrix = _sym_nothing;
		x->out_matrix = symbol_unique();
		
		attr_args_process(x,argc,argv); //handle attribute args	
		
		// Set up a matrix for writing to...
		jit_matrix_info_default(&info);								// Follow this up with any values I want to set...
		info.type = _sym_float32;
		m = jit_object_new(_sym_jit_matrix, &info);					// Create new jitter(matrix) object
		if (m)
			m = jit_object_method(m, _sym_register, x->out_matrix);	// Register the matrix object
		else
			object_error((t_object *)x, "couldn't allocate/register matrix");
//		jit_object_attach(x->out_matrix, x);						// Attach this Max object to the registered Jitter object
		x->matrix = m;												// Store the pointer in our object's struct
	}
	return (x);
}


// FREE MEMORY	
void ali_free(t_ali *x)
{
	//	jit_object_detach(x->out_matrix, x);
	jit_object_free(x->matrix);
}


/**************************************************************************************/
// BOUND METHODS

// ASSISTANCE STRINGS
void ali_assist(t_ali *x, void *b, long m, long a, char *s)
{
	if (m == 1){ 									//input
		strcpy(s,"control space coordinates");
	}
	else{ 											//output
		switch (a){
			case 0:
				strcpy(s,"(list/bang) interpolated data"); break; 			
			case 1:
				strcpy(s,"(list) weights"); break; 			
			case 2:
				strcpy(s,"dumpout"); break; 			
		}
	}
}


// The Big Beef
void ali_coords(t_ali *x, double xin, double yin)
{
	long i,j;//,k,n,maxsize;
	float temp, temp2, sum = 0;
	t_atom my_val[250];
	long data_depthclip = x->data_depthclip;
	long data_widthclip = x->data_widthclip;
	long coors[2];
	
	// FOR THE SPACE MATRIX
	void *matrix;
	long dimcount,dim[JIT_MATRIX_MAX_DIMCOUNT];
	long in_savelock;
	t_jit_matrix_info in_minfo;
	char *in_bp;
	
	// FOR THE DATA MATRIX
	void *data_matrix;
	long data_dimcount, data_dim[JIT_MATRIX_MAX_DIMCOUNT];
	long data_in_savelock;
	t_jit_matrix_info data_in_minfo;
	char *data_in_bp;
	
	// FOR THE OUT MATRIX
	void *out_matrix;
	long out_savelock;//, out_offset0, out_offset1;
	t_jit_matrix_info out_minfo;
	char *out_bp;//,*out_p;
	
	
	// Limit the input 
	xin = TTClip(xin, 0.0, 1.0);
	yin = TTClip(yin, 0.0, 1.0);
	
	// Store a pointer to the data for the specified matrix
	matrix = jit_object_findregistered(x->space_matrix);
	data_matrix = jit_object_findregistered(x->data_matrix);
	out_matrix = x->matrix;
	
	// DO IT
	if (matrix&&jit_object_method(matrix, _sym_class_jit_matrix)){
		in_savelock = (long) jit_object_method(matrix,_sym_lock,1);
		jit_object_method(matrix,_sym_getinfo,&in_minfo);
		jit_object_method(matrix,_sym_getdata,&in_bp);
		
		// ERROR HANDLING
		if (!in_bp) { 
			//jit_error_sym(x, gensym("err_calculate"));
			jit_object_method(matrix,_sym_lock,in_savelock);
			goto out;
		}
		
		//get dimensions/planecount 
		dimcount = in_minfo.dimcount;
		for (i=0;i<dimcount;i++) {
			dim[i] = in_minfo.dim[i];
		}
	
		// FOR THE OTHER MATRIX
		if (data_matrix&&jit_object_method(data_matrix, _sym_class_jit_matrix)) {
			//calculate
			data_in_savelock = (long) jit_object_method(data_matrix, _sym_lock,1);
			jit_object_method(data_matrix, _sym_getinfo,&data_in_minfo);
			jit_object_method(data_matrix, _sym_getdata,&data_in_bp);
			
			// SOME KIND OF ERROR HANDLING
			if (!data_in_bp) { 
				//jit_error_sym(x, gensym("err_calculate"));
				jit_object_method(data_matrix, _sym_lock, data_in_savelock);
				goto out;
			}
			
			//get dimensions/planecount 
			data_dimcount = data_in_minfo.dimcount;
			for (i=0;i<data_dimcount;i++) {
				data_dim[i] = data_in_minfo.dim[i];
			}
		}
		else{
			//jit_error_sym(x, gensym("err_calculate"));
			goto out;
		}
	
			
		coors[0] = xin * (dim[0] - 1);
		coors[1] = yin * (dim[1] - 1);
		if(data_widthclip == 0) data_widthclip = data_dim[0];	
		if(data_depthclip == 0) data_depthclip = data_dim[1];
		
		// AT THIS POINT I HAVE:
		//	x - the pointer to this object
		//	dimcount - the number of dimensions
		//	dim - an array with the sizes of each dimension
		//	a_coord - an array of atoms
		//	in_minfo - various info about the matrix at hand
		//	in_bp - a pointer to the data
		
		// prepare the output matrix
		if (out_matrix&&jit_object_method(out_matrix, _sym_class_jit_matrix)) {
			out_savelock = (long) jit_object_method(out_matrix, _sym_lock,1);
		
			// Set the output matrix up
			jit_object_method(out_matrix, _sym_getinfo,&out_minfo);
			out_minfo.type= _sym_float32;
			out_minfo.planecount = 1;
			out_minfo.dimcount = 2;
			out_minfo.dim[0] = TTClip(data_widthclip, 1L, data_dim[0]);	// This is the setting we really care about
			out_minfo.dim[1] = 1;												// By making the matrix a 2D matrix, it can be fed into a jit.cellblock
			jit_object_method(out_matrix, _sym_setinfo,&out_minfo);
		
			// Get the pointer to the output matrix
			jit_object_method(out_matrix, _sym_getdata,&out_bp);
			
			// Check for problems
			if (!out_bp){ 
				//jit_error_sym(x, gensym("err_calculate"));
				jit_object_method(out_matrix, _sym_lock,out_savelock);
				goto out;
			}


			for(i=0; i< TTClip(data_depthclip, 1L, data_dim[1]); i++){	// for the y size of the data matrix
				temp = JIT_3D_GET_FLOAT32(in_bp,&in_minfo,0,coors[0],coors[1], i);								
				atom_setfloat(&(my_val[i]),temp);
				sum += temp;
			}
			
			if(sum == 0) goto cleanup;	// no sense in doing anything if the weights are all zero...
			
			sum = 1.0 / sum;
			for(j=0; j<=i; j++){
				my_val[j].a_w.w_float *= sum;		// NORMALIZE...
			}
			outlet_list(x->outlet[1], 0L, i, &my_val[0]);	// OUTPUT THE WEIGHTS

		
			// APPLY WEIGHTS TO THE DATA MATRIX			
			if(x->data_interpolation == 0) goto cleanup;	// bypass
			
			jit_object_method(out_matrix, _sym_clear);	// clear the matrix

			for(i=0; i< TTClip(data_depthclip, 1L, data_dim[1]); i++){
				for(j=0; j<TTClip(data_widthclip,1L,data_dim[0]); j++){
					temp = my_val[i].a_w.w_float * JIT_2D_GET_FLOAT32(data_in_bp,&data_in_minfo,0,j,i);
					temp2 = JIT_2D_GET_FLOAT32(out_bp, &out_minfo, 0, j, 0);		// should optimize this code sometime...
					JIT_2D_SET_FLOAT32(out_bp,&out_minfo, 0, j, 0, temp + temp2);		
				}
			}		

			if(x->output_type != ps_list){			// MATRIX OUTPUT
				atom_setsym(x->outlist+0, x->out_matrix);
				outlet_anything(x->outlet[0], _sym_jit_matrix, 1, x->outlist);	// output the result
			}
			else{
				for(i=0; i < TTClip(data_dim[0],1L,MAX_LIST_SIZE); i++){		// LIST OUTPUT
					temp = JIT_2D_GET_FLOAT32(out_bp, &out_minfo, 0, i, 0);
					atom_setfloat(&(x->outlist[i]),temp);
				}
				outlet_list(x->outlet[0], 0L, i, &x->outlist[0]);
			}
		}		
cleanup:		
		jit_object_method(matrix, _sym_lock,in_savelock);
		jit_object_method(data_matrix, _sym_lock,in_savelock);
		jit_object_method(out_matrix, _sym_lock,in_savelock);			
	} else {
		//jit_error_sym(x, gensym("err_calculate"));
	}
out:
	return;
}

