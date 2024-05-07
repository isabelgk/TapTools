/* 
 *	External object for Max/MSP
 *	Copyright © 2007 by Timothy Place
 * 
 *	License: This code is licensed under the terms of the "New BSD License"
 *	http://creativecommons.org/licenses/BSD/
 */

/* 
	tap.rotate
 	perform 3D rotations on sets of coordinates
	algorithm provided by stephan moore
*/


#if NOCP
#include "ext.h"
#include "ext_obex.h"
#include "commonsyms.h"
#else
#include "TTClassWrapperMax.h"
#endif

#define MAX_NUMSETS 128

static t_class *rotate_class;				// Required: Global pointer for our class

typedef struct _rotate{
	t_object		obj;
	void			*inlets[3];		// x coords, y coords, z coords, rotation anchor point
	void 			*outlets[3];	// x coords, y coords, z coords
	long			inletnum;
	long			numsets;
	float			x[MAX_NUMSETS];
	float			y[MAX_NUMSETS];
	float			z[MAX_NUMSETS];
	float			rot_x;
	float			rot_y;
	float			rot_z;
	float			rot_x_rad;
	float			rot_y_rad;
	float			rot_z_rad;
} t_rotate;


// Prototypes for our methods:
void*	rotate_new(t_symbol *s, long argc, t_atom *argv);
void 	rotate_assist(t_rotate *x, void *b, long msg, long arg, char *dst);
void 	rotate_bang(t_rotate *obj);
void 	rotate_float(t_rotate *obj, double val);
void 	rotate_list(t_rotate *x, t_symbol *msg, short argc, t_atom *argv);
void 	rotate_applyrotation(t_rotate *x, long setnum);
void 	rotate_cartopol(double real, double imaginary, double *out1, double *out2);
void 	rotate_poltocar(double magnitude, double phase, double *out1, double *out2);


/************************************************************************************/
// Main() Function

void ext_main(void* r)
{
	t_class *c = class_new("tap.rotate",(method)rotate_new, (method)0L, sizeof(t_rotate), 
		(method)0L, A_GIMME, 0);

#if NOCP
    class_addmethod(c, (method)object_obex_dumpout, 		"dumpout", A_CANT,0);
#else
		common_symbols_init();														// Initialize TapTools
#endif
 	class_addmethod(c, (method)rotate_bang,		"bang", 	0L);
 	class_addmethod(c, (method)rotate_float,	"float", 	A_FLOAT, 0L);
  	class_addmethod(c, (method)rotate_list,		"list", 	A_GIMME, 0L);	
	class_addmethod(c, (method)rotate_assist,	"assist", 	A_CANT, 0L); 
	class_addmethod(c, (method)stdinletinfo,	"inletinfo",	A_CANT, 0);
/*
	// ATTRIBUTE: numsets
	attr = attr_offset_new("numsets", _sym_long, attrflags,
		(method)0, (method)0, calcoffset(t_route, partialmatch));
	class_addattr(c, attr);	
*/
#if NOCP
	class_register(CLASS_BOX, c);
	rotate_class = c;
#else
class_register(_sym_box, c); 	rotate_class = c;
#endif
}


/************************************************************************************/
// Object Creation Method

void *rotate_new(t_symbol *s, long argc, t_atom *argv)
{
#if NOCP
	t_rotate	*x = (t_rotate *)object_alloc(rotate_class);
#else
	t_rotate	*x = (t_rotate *)object_alloc(rotate_class);;
#endif

	if(x){
		object_obex_store((void *)x, _sym_dumpout, (object *)outlet_new(x,NULL));	// dumpout
			
		x->inlets[2] = proxy_new(x, 3, &x->inletnum);			
		x->inlets[1] = proxy_new(x, 2, &x->inletnum);			
		x->inlets[0] = proxy_new(x, 1, &x->inletnum);			
		x->outlets[2] = outlet_new(x, 0);
		x->outlets[1] = outlet_new(x, 0);
		x->outlets[0] = outlet_new(x, 0);
		x->numsets = 1;
		
		attr_args_process(x,argc,argv);			 //handle attribute args	
	}
	return x;
}


/************************************************************************************/
// Methods bound to input/inlets

// Method for Assistance Messages
void rotate_assist(t_rotate *x, void *b, long msg, long arg, char *dst)
{
	if(msg == 1){ 					// Inlets
		switch(arg){
			case 0: strcpy(dst, "list of x coords"); break;
			case 1: strcpy(dst, "list of y coords"); break;
			case 2: strcpy(dst, "list of z coords"); break;
			case 3: strcpy(dst, "list (x y z) to define rotation"); break;
		}
	}
	else{ 							// Outlets
		switch(arg){
			case 0: strcpy(dst, "Output (list of x coords)"); break;
			case 1: strcpy(dst, "Output (list of y coords)"); break;
			case 2: strcpy(dst, "Output (list of z coords)"); break;
			case 3: strcpy(dst, "dumpout"); break;
 		}
 	}		
}


void rotate_bang(t_rotate *obj)
{
	short	i;
	double 	x, y, z;
	double 	x2, y2, z2;
	t_atom	ax[MAX_NUMSETS],
			ay[MAX_NUMSETS],
			az[MAX_NUMSETS];

	for(i=0; i < obj->numsets; i++){
		x = obj->x[i];
		y = obj->y[i];
		z = obj->z[i];
	
		// z-rotate
		rotate_cartopol(x, y, &x2, &y2);
		y2 += obj->rot_z_rad;
		rotate_poltocar(x2, y2, &x, &y);

		// y-rotate
		rotate_cartopol(z, x, &z2, &x2);
		x2 += obj->rot_y_rad;
		rotate_poltocar(z2, x2, &z, &x);

		// x-rotate
		rotate_cartopol(y, z, &y2, &z2);
		z2 += obj->rot_x_rad;
		rotate_poltocar(y2, z2, &y, &z);
	
		atom_setfloat(ax+i, x);
		atom_setfloat(ay+i, y);
		atom_setfloat(az+i, z);
	}
	
	outlet_anything(obj->outlets[2], _sym_list, obj->numsets, az);
	outlet_anything(obj->outlets[1], _sym_list, obj->numsets, ay);
	outlet_anything(obj->outlets[0], _sym_list, obj->numsets, ax);
}


void rotate_float(t_rotate *obj, double val)
{
	if(obj->inletnum == 0){			// set x coord, trigger output
		obj->numsets = 1;
		obj->x[0] = val;
		rotate_bang(obj);
	}
	else if(obj->inletnum == 1)
		obj->y[0] = val;
	else if(obj->inletnum == 2)
		obj->z[0] = val;
}


// LIST INPUT
void rotate_list(t_rotate *obj, t_symbol *msg, short argc, t_atom *argv)
{	
	short	i;
	
	if(obj->inletnum == 0){			// set x coord, trigger output
		obj->numsets = argc;
		for(i=0; i < argc; i++)
			obj->x[i] = atom_getfloat(argv+i);
		rotate_bang(obj);
	}
	else if(obj->inletnum == 1){	// set y coords		
		for(i=0; i < argc; i++)
			obj->y[i] = atom_getfloat(argv+i);
	}
	else if(obj->inletnum == 2){	// set z coords
		for(i=0; i < argc; i++)
			obj->z[i] = atom_getfloat(argv+i);
	}
	else if(obj->inletnum == 3){	// set rotation
		if(argc != 3){
			object_error((t_object *)obj, "wrong number of list elements");
			return;
		}
		obj->rot_x = atom_getfloat(argv+0);
		obj->rot_y = atom_getfloat(argv+1);
		obj->rot_z = atom_getfloat(argv+2);
		obj->rot_x_rad = (obj->rot_x / 180.0) * 3.1415926535897932;
		obj->rot_y_rad = (obj->rot_y / 180.0) * 3.1415926535897932;
		obj->rot_z_rad = (obj->rot_z / 180.0) * 3.1415926535897932;	
	}
}


void rotate_applyrotation(t_rotate *x, long setnum)
{
}


void rotate_cartopol(double real, double imaginary, double *out1, double *out2)
{
	double	magnitude, 
			phase;

	magnitude = sqrt((real * real) + (imaginary * imaginary));
	
	if (real == 0)
		real = 0.000001;				// prevent divide by zero
	phase = atan(imaginary / real);	
	if ((real < 0) && (imaginary < 0))		// arctangent corrections
		phase = phase - 3.1415926535897932;
	else if ((real < 0) && (imaginary >= 0))
		phase = phase + 3.1415926535897932;

	*out1 = magnitude;
	*out2 = phase;	
}


void rotate_poltocar(double magnitude, double phase, double *out1, double *out2)
{
	double	real, 
			imaginary;
							
	real = magnitude * cos(phase);
	imaginary = magnitude * sin(phase);

	*out1 = real;
	*out2 = imaginary;	
}
