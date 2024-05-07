/* 
 *	External object for Max/MSP
 *	Copyright © 2002 by Timothy Place
 * 
 *	License: This code is licensed under the terms of the "New BSD License"
 *	http://creativecommons.org/licenses/BSD/
 */


#include "TTClassWrapperMax.h"
#include "jit.common.h"

typedef struct _jit_sum 
{
	t_object			ob;
	void 				*valout;
	char				mode; 			// mode 0: normal operation, mode 1: change notification
	void				*matrix;		// pointer to the internal matrix used in change detection
	t_symbol			*matrix_name;	// name of the internal matrix
} t_jit_sum;


void *jit_sum_new(t_symbol *s, long argc, t_atom *argv);
void jit_sum_free(t_jit_sum *x);
void jit_sum_assist(t_jit_sum *x, void *b, long m, long a, char *s);
void jit_sum_jit_matrix(t_jit_sum *x, t_symbol *s, long argc, t_atom *argv);
void jit_sum_calculate_ndim(t_jit_sum *x, 	long dimcount, long *dim, t_atom *a_coord, t_jit_matrix_info *in_minfo, char *bip);
void jit_sum_change_calculate_ndim(t_jit_sum *x, t_jit_matrix_info *in_minfo, char *in_mdata);


// Globals
static t_class *jit_sum_class;
		 	

/**************************************************************************************/
// MAIN

extern "C" int C74_EXPORT main(void)
{	
	long attrflags = 0;
	t_class *c;
	t_object *attr;
	
	c = class_new("tap.jit.sum", (method)jit_sum_new, (method)jit_sum_free, sizeof(t_jit_sum), 
		(method)0L, A_GIMME, 0);

		common_symbols_init();
    class_addmethod(c, (method)jit_sum_jit_matrix,	"jit_matrix", A_GIMME, 0L);
    class_addmethod(c, (method)jit_sum_assist,		"assist", A_CANT, 0L); 
	class_addmethod(c, (method)stdinletinfo,		"inletinfo",	A_CANT, 0);
	
	// ATTRIBUTE: mode
	attr = attr_offset_new("mode", _sym_char, attrflags,
		(method)0, (method)0L, calcoffset(t_jit_sum, mode));
	class_addattr(c, attr);	

class_register(_sym_box, c); 	jit_sum_class = c;
}


/**************************************************************************************/
// OBJECT LIFE

void *jit_sum_new(t_symbol *s, long argc, t_atom *argv)
{
	t_jit_sum 			*x;
	void 				*m;		// pointer to a matrix (used for output)
	t_jit_matrix_info	info;	// matrix info parameters to set for out matrix

	x = (t_jit_sum *)object_alloc(jit_sum_class);;
	if(x){
		object_obex_store((void *)x, _sym_dumpout, (object *)outlet_new(x,NULL));	// dumpout
		x->valout 	= outlet_new(x,0L);
		x->mode = 0;
		attr_args_process(x,argc,argv); //handle attribute args	
		
		// Set up a matrix for writing to...
		x->matrix_name = jit_symbol_unique();
		jit_matrix_info_default(&info);									// Follow this up with any values I want to set...
		m = jit_object_new(_sym_jit_matrix, &info);					// Create new jitter(matrix) object
		m = jit_object_method(m, _sym_register, x->matrix_name);	// Register the matrix object
		if(!m)
			object_error((t_object *)x, "couldn't allocate/register matrix");
		jit_object_method(m, _sym_clear);							// Clear the matrix contents
		jit_object_attach(x->matrix_name, x);							// Attach this Max object to the registered Jitter object
		x->matrix = m;													// Store the pointer in our object's struct
	}
	return (x);
}

// FREE MEMORY	
void jit_sum_free(t_jit_sum *x)
{
	jit_object_detach(x->matrix_name, x);
	jit_object_free(x->matrix);
}


/**************************************************************************************/
// METHODS


// ASSISTANCE STRINGS
void jit_sum_assist(t_jit_sum *x, void *b, long m, long a, char *s)
{
	if (m == 1) { //input
		strcpy(s,"(matrix) in");
	} else { //output
		switch (a) {
		case 0:
			strcpy(s,"(int) sum of values");
			break; 			
		case 1:
			strcpy(s,"dumpout");
			break; 			
		}
	}
}


void jit_sum_jit_matrix(t_jit_sum *x, t_symbol *s, long argc, t_atom *argv)
{
	void *matrix;
	long i,dimcount,dim[JIT_MATRIX_MAX_DIMCOUNT];
	long in_savelock;
	t_jit_matrix_info in_minfo;
	char *in_mdata;
	t_atom a_coord[JIT_MATRIX_MAX_DIMCOUNT];
	
	if (argc&&argv) {
		//find matrix
		matrix = jit_object_findregistered(atom_getsym(argv));
		if (matrix&&jit_object_method(matrix, _sym_class_jit_matrix)) {
			//calculate
			in_savelock = (long) jit_object_method(matrix, _sym_lock,1);
			jit_object_method(matrix, _sym_getinfo, &in_minfo);
			jit_object_method(matrix, _sym_getdata, &in_mdata);
			
			if(!in_mdata) { 
				//jit_error_sym(x, gensym("err_calculate"));
				jit_object_method(matrix, _sym_lock, in_savelock);
				goto out;
			}
			
			//get dimensions/planecount 
			dimcount = in_minfo.dimcount;
			for (i=0;i<dimcount;i++) {
				dim[i] = in_minfo.dim[i];
			}		
			
			//calculate
			if(x->mode == 0)	// default
				jit_sum_calculate_ndim(x, dimcount, dim, a_coord, &in_minfo, in_mdata);		
			else				// change flag
				jit_sum_change_calculate_ndim(x, &in_minfo, in_mdata);		

			
			jit_object_method(matrix, _sym_lock, in_savelock);				
			max_jit_obex_dumpout(x, gensym("done"), 0, 0L);			
		} else {
			//jit_error_sym(x, gensym("err_calculate"));
		}
	}
out:
	return;
}



//recursive function to handle higher dimension matrices, by processing 2D sections at a time 
void jit_sum_calculate_ndim(t_jit_sum *x, long dimcount, long *dim, t_atom *a_coord, t_jit_matrix_info *in_minfo, char *bip)
{
	t_atom a_val[JIT_MATRIX_MAX_PLANECOUNT];
	long i,j,k,n;//,maxsize;
	long rowstride;
	uchar *ip;
	double	mySum, myRowAccum, myColAccum;
	double	myDivisor;	
	t_atom my_val[3];
	
	mySum = 0;
	myColAccum = 0;
	myRowAccum = 0;
	
	if (dimcount<1) return; //safety
	
	switch(dimcount) {
	case 1:
		dim[1] = 1;
	case 2:
		n = dim[0];
		switch(x->mode) {
		case 1:
			if (in_minfo->type==_sym_char) {
				object_post((t_object *)x, "I am A");
				rowstride = in_minfo->dimstride[1];
				for (j=0;j<dim[0];j++) {
					ip = (uchar *)(bip + j * in_minfo->dimstride[0]);	
					atom_setlong(&(a_coord[0]),j);	
					for (i=0;i<dim[1];i++){
						atom_setlong(&(a_coord[1]),i);
						for (k=0;k<in_minfo->planecount;k++) {
							atom_setlong(&(a_val[k]),(((uchar *)ip)[k]) + mySum);
						}

						// BECAUSE THEY WANTED TO MAKE LISTS FOR MULTIPLANE (UNLIKE ME)
						//if (in_minfo->planecount>1)	
						//	outlet_anything(x->valout,_jit_sym_list,in_minfo->planecount,a_val);
						//else 
						
							outlet_int(x->valout,a_val[0].a_w.w_long);
						ip += rowstride;
					}
				}
			} else if (in_minfo->type==_sym_long) {
				rowstride = in_minfo->dimstride[1];// >> 2L;
				for (j=0;j<dim[0];j++){
					ip = (uchar *)(bip + j * in_minfo->dimstride[0]);					
					atom_setlong(&(a_coord[0]),j);	
					for (i=0;i<dim[1];i++) {	
						atom_setlong(&(a_coord[1]),i);	
						for (k=0;k<in_minfo->planecount;k++) {
							atom_setlong(&(a_val[k]),((long *)ip)[k]);
						}

						if (in_minfo->planecount>1)	
							outlet_anything(x->valout,_sym_list,in_minfo->planecount,a_val);
						else 
							outlet_int(x->valout,a_val[0].a_w.w_long);
						ip += rowstride;
					}
				}
			} else if (in_minfo->type==_sym_float32) {
				rowstride = in_minfo->dimstride[1];// >> 2L;
				for (j=0;j<dim[0];j++){
					ip = (uchar *)(bip + j * in_minfo->dimstride[0]);					
					atom_setlong(&(a_coord[0]),j);	
					for (i=0;i<dim[1];i++) {	
						atom_setlong(&(a_coord[1]),i);	
						for (k=0;k<in_minfo->planecount;k++) {
							atom_setfloat(&(a_val[k]),((float *)ip)[k]);
						}

						if (in_minfo->planecount>1)	
							outlet_anything(x->valout,_sym_list,in_minfo->planecount,a_val);
						else 
							outlet_float(x->valout,a_val[0].a_w.w_float);
						ip += rowstride;
					}
				}
			} else if (in_minfo->type==_sym_float64) {
				rowstride = in_minfo->dimstride[1];// >> 3L;
				for (j=0;j<dim[0];j++){
					ip = (uchar *)(bip + j * in_minfo->dimstride[0]);					
					atom_setlong(&(a_coord[0]),j);	
					for (i=0;i<dim[1];i++) {	
						atom_setlong(&(a_coord[1]),i);	
						for (k=0;k<in_minfo->planecount;k++) {
							atom_setfloat(&(a_val[k]),((double *)ip)[k]);
						}

						if (in_minfo->planecount>1)	
							outlet_anything(x->valout,_sym_list,in_minfo->planecount,a_val);
						else 
							outlet_float(x->valout,a_val[0].a_w.w_float);
						ip += rowstride;
					}
				}
			} 
			break;
		case 0:						// THIS IS THE PART OF THE CODE THAT ACTUALLY WORKS
									// FOR SOME REASON IT ONLY WORKS WITH UNIPLANE DATA
		default:
			if (in_minfo->type==_sym_char) {
				myDivisor = /*255 **/ dim[0] * dim[1] * in_minfo->planecount;
				for (i=0;i<dim[1];i++){						// Y loop
					ip = (uchar *)(bip + i*in_minfo->dimstride[1]);					
					atom_setlong(&(a_coord[1]),i);	
					for (j=0;j<dim[0];j++) {				// X loop (nested)
						atom_setlong(&(a_coord[0]),j);	
						for (k=0;k<in_minfo->planecount;k++) {   // PLANE loop (double-nested)
							atom_setlong(&(a_val[k]),((uchar *)ip)[(j*in_minfo->planecount)+k]);
							if(a_val[0].a_w.w_long != 0){
								mySum++;	// Increment the sum
								myRowAccum += a_coord[1].a_w.w_long;
								myColAccum += a_coord[0].a_w.w_long;
							}		
						}
					}
				}
				
				if(mySum != 0){
					myRowAccum = myRowAccum / mySum;	
					myColAccum = myColAccum / mySum;
				}	
				
				atom_setsym(&(my_val[0]),gensym("quantity"));					
				atom_setfloat(&(my_val[1]),mySum/myDivisor);
				outlet_list(x->valout, 0L, 2, (atom *)&my_val);
				
				atom_setsym(&(my_val[0]),gensym("location"));								
				atom_setfloat(&(my_val[1]),myColAccum/dim[0]);
				atom_setfloat(&(my_val[2]),myRowAccum/dim[1]);
				outlet_list(x->valout, 0L, 3, (atom *)&my_val);

				
				
				
				
				
			} else if (in_minfo->type==_sym_long) {
				for (i=0;i<dim[1];i++){
					ip = (uchar *)(bip + i*in_minfo->dimstride[1]);					
					atom_setlong(&(a_coord[1]),i);	
					for (j=0;j<dim[0];j++) {	
						atom_setlong(&(a_coord[0]),j);	
						for (k=0;k<in_minfo->planecount;k++) {
							atom_setlong(&(a_val[k]),((long *)ip)[(j*in_minfo->planecount)+k]);
						}

						if (in_minfo->planecount>1)	
							outlet_anything(x->valout,_sym_list,in_minfo->planecount,a_val);
						else 
							outlet_int(x->valout,a_val[0].a_w.w_long);
					}
				}
			} else if (in_minfo->type==_sym_float32) {
				for (i=0;i<dim[1];i++){
					ip = (uchar *)(bip + i*in_minfo->dimstride[1]);					
					atom_setlong(&(a_coord[1]),i);	
					for (j=0;j<dim[0];j++) {	
						atom_setlong(&(a_coord[0]),j);	
						for (k=0;k<in_minfo->planecount;k++) {
							atom_setfloat(&(a_val[k]),((float *)ip)[(j*in_minfo->planecount)+k]);
						}

						if (in_minfo->planecount>1)	
							outlet_anything(x->valout,_sym_list,in_minfo->planecount,a_val);
						else 
							outlet_float(x->valout,a_val[0].a_w.w_float);
					}
				}
			} else if (in_minfo->type==_sym_float64) {
				for (i=0;i<dim[1];i++){
					ip = (uchar *)(bip + i*in_minfo->dimstride[1]);					
					atom_setlong(&(a_coord[1]),i);	
					for (j=0;j<dim[0];j++) {	
						atom_setlong(&(a_coord[0]),j);	
						for (k=0;k<in_minfo->planecount;k++) {
							atom_setfloat(&(a_val[k]),((double *)ip)[(j*in_minfo->planecount)+k]);
						}

						if (in_minfo->planecount>1)	
							outlet_anything(x->valout,_sym_list,in_minfo->planecount,a_val);
						else 
							outlet_float(x->valout,a_val[0].a_w.w_float);
					}
				}
			} 
			break;
		}
		break;
	default:
		object_post((t_object *)x, "I am C");
		for	(i=0;i<dim[dimcount-1];i++) {
			ip = (uchar *)(bip + i*in_minfo->dimstride[dimcount-1]);
			atom_setlong(&(a_coord[dimcount-1]),i);	
			jit_sum_calculate_ndim(x,dimcount-1,dim,a_coord,in_minfo, (char *)ip);
		}
	}
}







/************************************************************************
 * This function is basically duplicating the behavior of jit.change.
 * It is only here because Hipno required it without the extra dependency
 * on yet another Jitter object.  In our case, we are only really
 * concerned with 2D and 3D float32 matricies
 ************************************************************************/

void jit_sum_change_calculate_ndim(t_jit_sum *x, t_jit_matrix_info *in_minfo, char *in_mdata)
{
	long i, j, k;
	long dim_x = in_minfo->dim[0], dim_y = in_minfo->dim[1], dim_z = in_minfo->dim[2];
	
	if(in_minfo->planecount > 1){
		object_error((t_object *)x, "in mode 1: cannot function on multi-plane matrices");
		return;
	}
	
	if(in_minfo->type == _sym_float32){
		float				*cell_value_new, *cell_value_old;	// a pointer to the current cell value
		void 				*old_matrix = x->matrix;			// 
		t_jit_matrix_info 	old_minfo;							// 
		char				*old_mdata;
		bool				flag_changed = false;				// 'true' if there was a change 
		long 				savelock;
		
		jit_object_method(old_matrix, _sym_getinfo, &old_minfo);
		savelock = (long) jit_object_method(old_matrix, _sym_lock, 1);
	
		// STEP 1: Compare info on the incoming matrix and our internal matrix
		if(old_minfo.dimcount != in_minfo->dimcount) flag_changed = true;
		if(old_minfo.dim[0] != dim_x) flag_changed = true;
		if(old_minfo.dim[1] != dim_y) flag_changed = true;
		if(old_minfo.dim[2] != dim_z) flag_changed = true;
		
		if(flag_changed) goto output;
		
		// Get the pointer to the internal matrix
		jit_object_method(old_matrix, _sym_getdata, &old_mdata);
		
		// Check for problems
		if (!old_mdata){ 
			//jit_error_sym(x,  gensym("err_calculate"));
			jit_object_method(old_matrix, _sym_lock, savelock);
			goto bye;
		}

		
		
		
		// STEP 2: Compare the contents of the imcoming matrix and our internal matrix
		switch(in_minfo->dimcount){
			case 1:
				dim_y = 1;
				// fall through the 2D case with (N x 1) sized matrix
			case 2:
				for(i=0; i<dim_x; i++){
					for(j=0; j<dim_y; j++){
						cell_value_new = (float *)(in_mdata + (i * in_minfo->dimstride[1]) + (j * in_minfo->dimstride[0]));
						cell_value_old = (float *)(old_mdata + (i * old_minfo.dimstride[1]) + (j * old_minfo.dimstride[0]));
						//object_post((t_object *)x, "cell value for %i %i is: %f",i ,j, *cell_value_new);
						if(*cell_value_new != *cell_value_old) goto output;
					}
				}			
			break;
			case 3:
				for(k=0; k<dim_z; k++){
					for(i=0; i<dim_x; i++){
						for(j=0; j<dim_y; j++){
							cell_value_new = (float *)(in_mdata + (i * in_minfo->dimstride[1]) + (j * in_minfo->dimstride[0]) + (k * in_minfo->dimstride[2]));
							cell_value_old = (float *)(old_mdata + (i * old_minfo.dimstride[1]) + (j * old_minfo.dimstride[0]) + (k * old_minfo.dimstride[2]));
							//object_post((t_object *)x, "cell value for %i %i %i is: %f",i ,j, k, *cell_value_new);
							if(*cell_value_new != *cell_value_old) goto output;
						}
					}
				}
			break;
			default:
				object_error((t_object *)x, "in mode 1: cannot currently function on matrices with more than 3 dimensions");
				return;
		}
		return;  // matrix did not change
		
		
		// STEP 3: Store the incoming matrix in our internal matrix for future comparisons
output:
		//object_post((t_object *)x, "matrix changed!");
		outlet_anything(x->valout, gensym("matrix_change"), 0, (t_atom *)NIL);
		
		// copy the input matrix into our internal matrix
		old_minfo.dimcount = in_minfo->dimcount;
		old_minfo.dim[0] = dim_x;
		old_minfo.dim[1] = dim_y;
		old_minfo.dim[2] = dim_z;
		old_minfo.type=_sym_float32;
		old_minfo.planecount = 1;
		jit_object_method(old_matrix, _sym_setinfo, &old_minfo);

		for(i=0; i<in_minfo->size; i++)
			old_mdata[i] = *(in_mdata+i);
		
		jit_object_method(old_matrix, _sym_lock, savelock);					
		jit_object_method(old_matrix, _sym_lock, savelock);					
	}
	
	else{
		object_error((t_object *)x, "in mode 1: cannot function on non-float32 matrices");
		return;		
	}
bye:
	return;	
}




