/* 
 *	External object for Max/MSP
 *	Copyright © 2003 by Timothy Place
 * 
 *	License: This code is licensed under the terms of the "New BSD License"
 *	http://creativecommons.org/licenses/BSD/
 */


#include "TTClassWrapperMax.h"
#include "jit.common.h"
#include <math.h>
#include "../ttblue/tt_audio_base.h"

#define NUM_PLANES 4
#define	ALPHA 0
#define HUE 1
#define SATURATION 2
#define LIGHTNESS 3
#define BRIGHTNESS 3
#define LUMINANCE 3


#define NUM_TRACKERS 4
#define TRACKER_1 0
#define TRACKER_2 1
#define TRACKER_3 2
#define TRACKER_4 3

// Struct for the object
typedef struct _max_jit_colortrack 
{
	t_object	ob;						// required for all max objects
	void 		*valout;				// pointer to our main outlet
	
	char		output_bounds[NUM_TRACKERS], output_center[NUM_TRACKERS], output_size[NUM_TRACKERS];	// ATTRIBUTES: flags for what to output
	long		hue[NUM_TRACKERS], hue_variation[NUM_TRACKERS];
	long		saturation[NUM_TRACKERS], saturation_variation[NUM_TRACKERS];
	long		brightness[NUM_TRACKERS], brightness_variation[NUM_TRACKERS];

	char		use[NUM_TRACKERS];				// Used internally - based on the above 3 attributes
	char		hue_check[NUM_TRACKERS];		// bool indicates that the match range crosses 0 on the color wheel
	long		min[NUM_TRACKERS][NUM_PLANES];				// ATTRIBUTES: LOW BOUNDS	(AHSL - but alpha channel is ignored in the implementation)
	long		max[NUM_TRACKERS][NUM_PLANES];				// ATTRIBUTES: HIGH BOUNDS	(AHSL)
} t_max_jit_colortrack;


// Prototypes
void *max_jit_colortrack_new(t_symbol *s, long argc, t_atom *argv);
void max_jit_colortrack_free(t_max_jit_colortrack *x);
void max_jit_colortrack_assist(t_max_jit_colortrack *x, void *b, long m, long a, char *s);
void max_jit_colortrack_jit_matrix(t_max_jit_colortrack *x, t_symbol *s, long argc, t_atom *argv);
void jit_colortrack_calculate_ndim(t_max_jit_colortrack *x, long dimcount, long *dim, t_jit_matrix_info *in_minfo, char *bip);
void rgb2hsl(uchar red, uchar green, uchar blue, short *hue, short *saturation, short *lightness);

// Prototypes: ATTRIBUTE SET METHODS
t_jit_err colortrack_set_hue_1(t_max_jit_colortrack *x, void *attr, long argc, t_atom *argv);
t_jit_err colortrack_set_hue_variation_1(t_max_jit_colortrack *x, void *attr, long argc, t_atom *argv);
t_jit_err colortrack_set_hue_2(t_max_jit_colortrack *x, void *attr, long argc, t_atom *argv);
t_jit_err colortrack_set_hue_variation_2(t_max_jit_colortrack *x, void *attr, long argc, t_atom *argv);
t_jit_err colortrack_set_hue_3(t_max_jit_colortrack *x, void *attr, long argc, t_atom *argv);
t_jit_err colortrack_set_hue_variation_3(t_max_jit_colortrack *x, void *attr, long argc, t_atom *argv);
t_jit_err colortrack_set_hue_4(t_max_jit_colortrack *x, void *attr, long argc, t_atom *argv);
t_jit_err colortrack_set_hue_variation_4(t_max_jit_colortrack *x, void *attr, long argc, t_atom *argv);

t_jit_err colortrack_set_saturation_1(t_max_jit_colortrack *x, void *attr, long argc, t_atom *argv);
t_jit_err colortrack_set_saturation_variation_1(t_max_jit_colortrack *x, void *attr, long argc, t_atom *argv);
t_jit_err colortrack_set_saturation_2(t_max_jit_colortrack *x, void *attr, long argc, t_atom *argv);
t_jit_err colortrack_set_saturation_variation_2(t_max_jit_colortrack *x, void *attr, long argc, t_atom *argv);
t_jit_err colortrack_set_saturation_3(t_max_jit_colortrack *x, void *attr, long argc, t_atom *argv);
t_jit_err colortrack_set_saturation_variation_3(t_max_jit_colortrack *x, void *attr, long argc, t_atom *argv);
t_jit_err colortrack_set_saturation_4(t_max_jit_colortrack *x, void *attr, long argc, t_atom *argv);
t_jit_err colortrack_set_saturation_variation_4(t_max_jit_colortrack *x, void *attr, long argc, t_atom *argv);

t_jit_err colortrack_set_brightness_1(t_max_jit_colortrack *x, void *attr, long argc, t_atom *argv);
t_jit_err colortrack_set_brightness_variation_1(t_max_jit_colortrack *x, void *attr, long argc, t_atom *argv);
t_jit_err colortrack_set_brightness_2(t_max_jit_colortrack *x, void *attr, long argc, t_atom *argv);
t_jit_err colortrack_set_brightness_variation_2(t_max_jit_colortrack *x, void *attr, long argc, t_atom *argv);
t_jit_err colortrack_set_brightness_3(t_max_jit_colortrack *x, void *attr, long argc, t_atom *argv);
t_jit_err colortrack_set_brightness_variation_3(t_max_jit_colortrack *x, void *attr, long argc, t_atom *argv);
t_jit_err colortrack_set_brightness_4(t_max_jit_colortrack *x, void *attr, long argc, t_atom *argv);
t_jit_err colortrack_set_brightness_variation_4(t_max_jit_colortrack *x, void *attr, long argc, t_atom *argv);

t_jit_err colortrack_set_output_bounds_1(t_max_jit_colortrack *x, void *attr, long argc, t_atom *argv);
t_jit_err colortrack_set_output_bounds_2(t_max_jit_colortrack *x, void *attr, long argc, t_atom *argv);
t_jit_err colortrack_set_output_bounds_3(t_max_jit_colortrack *x, void *attr, long argc, t_atom *argv);
t_jit_err colortrack_set_output_bounds_4(t_max_jit_colortrack *x, void *attr, long argc, t_atom *argv);
t_jit_err colortrack_set_output_center_1(t_max_jit_colortrack *x, void *attr, long argc, t_atom *argv);
t_jit_err colortrack_set_output_center_2(t_max_jit_colortrack *x, void *attr, long argc, t_atom *argv);
t_jit_err colortrack_set_output_center_3(t_max_jit_colortrack *x, void *attr, long argc, t_atom *argv);
t_jit_err colortrack_set_output_center_4(t_max_jit_colortrack *x, void *attr, long argc, t_atom *argv);
t_jit_err colortrack_set_output_size_1(t_max_jit_colortrack *x, void *attr, long argc, t_atom *argv);
t_jit_err colortrack_set_output_size_2(t_max_jit_colortrack *x, void *attr, long argc, t_atom *argv);
t_jit_err colortrack_set_output_size_3(t_max_jit_colortrack *x, void *attr, long argc, t_atom *argv);
t_jit_err colortrack_set_output_size_4(t_max_jit_colortrack *x, void *attr, long argc, t_atom *argv);

// Attribute Utilities
void colortrack_sethue(t_max_jit_colortrack *x, short tracker_num, short value);
void colortrack_sethuevar(t_max_jit_colortrack *x, short tracker_num, short value);
void colortrack_setsaturation(t_max_jit_colortrack *x, short tracker_num, short value);
void colortrack_setsaturationvar(t_max_jit_colortrack *x, short tracker_num, short value);
void colortrack_setbrightness(t_max_jit_colortrack *x, short tracker_num, short value);
void colortrack_setbrightnessvar(t_max_jit_colortrack *x, short tracker_num, short value);
void colortrack_setoutputbounds(t_max_jit_colortrack *x, short tracker_num, short value);
void colortrack_setoutputcenter(t_max_jit_colortrack *x, short tracker_num, short value);
void colortrack_setoutputsize(t_max_jit_colortrack *x, short tracker_num, short value);


// Globals
static t_class *max_jit_colortrack_class;		// pointer to this class
		 	

/**************************************************************************************/
// MAIN

extern "C" int C74_EXPORT main(void)
{	
	long attrflags = 0;
	t_class *c;
	t_object *attr;
	
	c = class_new("tap.jit.colortrack", (method)max_jit_colortrack_new, (method)max_jit_colortrack_free, sizeof(t_max_jit_colortrack), 
		(method)0L, A_GIMME, 0);

		common_symbols_init();															// Initialize TapTools
    class_addmethod(c, (method)max_jit_colortrack_jit_matrix,	"jit_matrix", A_GIMME, 0L);	//at beginning of messlist for speed
    class_addmethod(c, (method)max_jit_colortrack_assist,		"assist", A_CANT, 0L); 
	class_addmethod(c, (method)stdinletinfo,					"inletinfo",	A_CANT, 0);
	
// 	attrflags = JIT_ATTR_GET_DEFER_LOW | JIT_ATTR_SET_USURP_LOW ;

	// ATTRIBUTE: output_bounds_1
	attr = attr_offset_new("output_bounds_1", _sym_char, attrflags,
		(method)0, (method)colortrack_set_output_bounds_1, calcoffset(t_max_jit_colortrack, output_bounds[TRACKER_1]));
	class_addattr(c, attr);
	
	// ATTRIBUTE: output_bounds_2
	attr = attr_offset_new("output_bounds_2", _sym_char, attrflags,
		(method)0, (method)colortrack_set_output_bounds_2, calcoffset(t_max_jit_colortrack, output_bounds[TRACKER_2]));
	class_addattr(c, attr);
	
	// ATTRIBUTE: output_bounds_3
	attr = attr_offset_new("output_bounds_3", _sym_char, attrflags,
		(method)0, (method)colortrack_set_output_bounds_3, calcoffset(t_max_jit_colortrack, output_bounds[TRACKER_3]));
	class_addattr(c, attr);
	
	// ATTRIBUTE: output_bounds_4
	attr = attr_offset_new("output_bounds_4", _sym_char, attrflags,
		(method)0, (method)colortrack_set_output_bounds_4, calcoffset(t_max_jit_colortrack, output_bounds[TRACKER_4]));
	class_addattr(c, attr);
	
		
	// ATTRIBUTE: output_center_1
	attr = attr_offset_new("output_center_1", _sym_char, attrflags,
		(method)0, (method)colortrack_set_output_center_1, calcoffset(t_max_jit_colortrack, output_center[TRACKER_1]));
	class_addattr(c, attr);
	
	// ATTRIBUTE: output_center_2
	attr = attr_offset_new("output_center_2", _sym_char, attrflags,
		(method)0, (method)colortrack_set_output_center_2, calcoffset(t_max_jit_colortrack, output_center[TRACKER_2]));
	class_addattr(c, attr);
	
	// ATTRIBUTE: output_center_3
	attr = attr_offset_new("output_center_3", _sym_char, attrflags,
		(method)0, (method)colortrack_set_output_center_3, calcoffset(t_max_jit_colortrack, output_center[TRACKER_3]));
	class_addattr(c, attr);
	
	// ATTRIBUTE: output_center_4
	attr = attr_offset_new("output_center_4", _sym_char, attrflags,
		(method)0, (method)colortrack_set_output_center_4, calcoffset(t_max_jit_colortrack, output_center[TRACKER_4]));
	class_addattr(c, attr);
	
		
	// ATTRIBUTE: output_size_1
	attr = attr_offset_new("output_size_1", _sym_char, attrflags,
		(method)0, (method)colortrack_set_output_size_1, calcoffset(t_max_jit_colortrack, output_size[TRACKER_1]));
	class_addattr(c, attr);
	
	// ATTRIBUTE: output_size_2
	attr = attr_offset_new("output_size_2", _sym_char, attrflags,
		(method)0, (method)colortrack_set_output_size_2, calcoffset(t_max_jit_colortrack, output_size[TRACKER_2]));
	class_addattr(c, attr);
	
	// ATTRIBUTE: output_size_3
	attr = attr_offset_new("output_size_3", _sym_char, attrflags,
		(method)0, (method)colortrack_set_output_size_3, calcoffset(t_max_jit_colortrack, output_size[TRACKER_3]));
	class_addattr(c, attr);
	
	// ATTRIBUTE: output_size_4
	attr = attr_offset_new("output_size_4", _sym_char, attrflags,
		(method)0, (method)colortrack_set_output_size_4, calcoffset(t_max_jit_colortrack, output_size[TRACKER_4]));
	class_addattr(c, attr);
	

	// ATTRIBUTE: hue_1
	attr = attr_offset_new("hue_1", _sym_long, attrflags,
		(method)0, (method)colortrack_set_hue_1, calcoffset(t_max_jit_colortrack, hue[TRACKER_1]));
	class_addattr(c, attr);

	// ATTRIBUTE: hue_variation_1
	attr = attr_offset_new("hue_variation_1", _sym_long, attrflags,
		(method)0, (method)colortrack_set_hue_variation_1, calcoffset(t_max_jit_colortrack, hue_variation[TRACKER_1]));
	class_addattr(c, attr);

	// ATTRIBUTE: hue_2
	attr = attr_offset_new("hue_2", _sym_long, attrflags,
		(method)0, (method)colortrack_set_hue_2, calcoffset(t_max_jit_colortrack, hue[TRACKER_2]));
	class_addattr(c, attr);

	// ATTRIBUTE: hue_variation_2
	attr = attr_offset_new("hue_variation_2", _sym_long, attrflags,
		(method)0, (method)colortrack_set_hue_variation_2, calcoffset(t_max_jit_colortrack, hue_variation[TRACKER_2]));
	class_addattr(c, attr);

	// ATTRIBUTE: hue_3
	attr = attr_offset_new("hue_3", _sym_long, attrflags,
		(method)0, (method)colortrack_set_hue_3, calcoffset(t_max_jit_colortrack, hue[TRACKER_3]));
	class_addattr(c, attr);

	// ATTRIBUTE: hue_variation_3
	attr = attr_offset_new("hue_variation_3", _sym_long, attrflags,
		(method)0, (method)colortrack_set_hue_variation_3, calcoffset(t_max_jit_colortrack, hue_variation[TRACKER_3]));
	class_addattr(c, attr);

	// ATTRIBUTE: hue_4
	attr = attr_offset_new("hue_4", _sym_long, attrflags,
		(method)0, (method)colortrack_set_hue_4, calcoffset(t_max_jit_colortrack, hue[TRACKER_4]));
	class_addattr(c, attr);

	// ATTRIBUTE: hue_variation_4
	attr = attr_offset_new("hue_variation_4", _sym_long, attrflags,
		(method)0, (method)colortrack_set_hue_variation_4, calcoffset(t_max_jit_colortrack, hue_variation[TRACKER_4]));
	class_addattr(c, attr);
	

	// ATTRIBUTE: saturation_1
	attr = attr_offset_new("saturation_1", _sym_long, attrflags,
		(method)0, (method)colortrack_set_saturation_1, calcoffset(t_max_jit_colortrack, saturation[TRACKER_1]));
	class_addattr(c, attr);

	// ATTRIBUTE: saturation_variation_1
	attr = attr_offset_new("saturation_variation_1", _sym_long, attrflags,
		(method)0, (method)colortrack_set_saturation_variation_1, calcoffset(t_max_jit_colortrack, saturation_variation[TRACKER_1]));
	class_addattr(c, attr);

	// ATTRIBUTE: saturation_2
	attr = attr_offset_new("saturation_2", _sym_long, attrflags,
		(method)0, (method)colortrack_set_saturation_2, calcoffset(t_max_jit_colortrack, saturation[TRACKER_2]));
	class_addattr(c, attr);

	// ATTRIBUTE: saturation_variation_2
	attr = attr_offset_new("saturation_variation_2", _sym_long, attrflags,
		(method)0, (method)colortrack_set_saturation_variation_2, calcoffset(t_max_jit_colortrack, saturation_variation[TRACKER_2]));
	class_addattr(c, attr);

	// ATTRIBUTE: saturation_3
	attr = attr_offset_new("saturation_3", _sym_long, attrflags,
		(method)0, (method)colortrack_set_saturation_3, calcoffset(t_max_jit_colortrack, saturation[TRACKER_3]));
	class_addattr(c, attr);

	// ATTRIBUTE: saturation_variation_3
	attr = attr_offset_new("saturation_variation_3", _sym_long, attrflags,
		(method)0, (method)colortrack_set_saturation_variation_3, calcoffset(t_max_jit_colortrack, saturation_variation[TRACKER_3]));
	class_addattr(c, attr);

	// ATTRIBUTE: saturation_4
	attr = attr_offset_new("saturation_4", _sym_long, attrflags,
		(method)0, (method)colortrack_set_saturation_4, calcoffset(t_max_jit_colortrack, saturation[TRACKER_4]));
	class_addattr(c, attr);

	// ATTRIBUTE: saturation_variation_4
	attr = attr_offset_new("saturation_variation_4", _sym_long, attrflags,
		(method)0, (method)colortrack_set_saturation_variation_4, calcoffset(t_max_jit_colortrack, saturation_variation[TRACKER_4]));
	class_addattr(c, attr);
	
	
	// ATTRIBUTE: brightness_1
	attr = attr_offset_new("brightness_1", _sym_long, attrflags,
		(method)0, (method)colortrack_set_brightness_1, calcoffset(t_max_jit_colortrack, brightness[TRACKER_1]));
	class_addattr(c, attr);

	// ATTRIBUTE: brightness_variation_1
	attr = attr_offset_new("brightness_variation_1", _sym_long, attrflags,
		(method)0, (method)colortrack_set_brightness_variation_1, calcoffset(t_max_jit_colortrack, brightness_variation[TRACKER_1]));
	class_addattr(c, attr);

	// ATTRIBUTE: brightness_2
	attr = attr_offset_new("brightness_2", _sym_long, attrflags,
		(method)0, (method)colortrack_set_brightness_2, calcoffset(t_max_jit_colortrack, brightness[TRACKER_2]));
	class_addattr(c, attr);

	// ATTRIBUTE: brightness_variation_2
	attr = attr_offset_new("brightness_variation_2", _sym_long, attrflags,
		(method)0, (method)colortrack_set_brightness_variation_2, calcoffset(t_max_jit_colortrack, brightness_variation[TRACKER_2]));
	class_addattr(c, attr);

	// ATTRIBUTE: brightness_3
	attr = attr_offset_new("brightness_3", _sym_long, attrflags,
		(method)0, (method)colortrack_set_brightness_3, calcoffset(t_max_jit_colortrack, brightness[TRACKER_3]));
	class_addattr(c, attr);

	// ATTRIBUTE: brightness_variation_3
	attr = attr_offset_new("brightness_variation_3", _sym_long, attrflags,
		(method)0, (method)colortrack_set_brightness_variation_3, calcoffset(t_max_jit_colortrack, brightness_variation[TRACKER_3]));
	class_addattr(c, attr);

	// ATTRIBUTE: brightness_4
	attr = attr_offset_new("brightness_4", _sym_long, attrflags,
		(method)0, (method)colortrack_set_brightness_4, calcoffset(t_max_jit_colortrack, brightness[TRACKER_4]));
	class_addattr(c, attr);

	// ATTRIBUTE: brightness_variation_4
	attr = attr_offset_new("brightness_variation_4", _sym_long, attrflags,
		(method)0, (method)colortrack_set_brightness_variation_4, calcoffset(t_max_jit_colortrack, brightness_variation[TRACKER_4]));
	class_addattr(c, attr);
	

	// this is very important!
class_register(_sym_box, c); 	max_jit_colortrack_class = c;
}


/**************************************************************************************/
// INSTANCE CONSTRUCTOR/DESTRUCTOR

void *max_jit_colortrack_new(t_symbol *s, long argc, t_atom *argv)
{
	t_max_jit_colortrack *x;

	x = (t_max_jit_colortrack *)object_alloc(max_jit_colortrack_class);;
	if(x){
		object_obex_store((void *)x, _sym_dumpout, (object *)outlet_new(x,NULL));
		x->valout = outlet_new(x,0L);
		
		//no normal args, no matrices
		attr_args_process(x,argc,argv); //handle attribute args	
	}
	return (x);
}


void max_jit_colortrack_free(t_max_jit_colortrack *x)
{
	;
}


/**************************************************************************************/
// METHODS

// ASSISTANCE STRINGS
void max_jit_colortrack_assist(t_max_jit_colortrack *x, void *b, long m, long a, char *s)
{
	if(m == 1){ 					//input
		strcpy(s,"(matrix) in");
	}
	else{ 							//output
		switch (a) {
		case 0:
			strcpy(s, "(int) colortrack of values");
			break; 			
		case 1:
			strcpy(s, "dumpout");
			break; 			
		}
	}
}


// MATRIX PROCESSOR (args are GIMME - really just the name of the matrix)
void max_jit_colortrack_jit_matrix(t_max_jit_colortrack *x, t_symbol *s, long argc, t_atom *argv)
{
	long in_savelock=0;
	t_jit_matrix_info in_minfo;
	char *in_bp;
	long dim[JIT_MATRIX_MAX_DIMCOUNT];
	void *in_matrix=NULL;

	if(argc && argv)
		in_matrix = jit_object_findregistered(atom_getsym(argv));		//find matrix
	else 
		goto out;
	
	if (in_matrix && jit_object_method(in_matrix, _sym_class_jit_matrix)){
		in_savelock = (long) jit_object_method(in_matrix,_sym_lock,1);
		jit_object_method(in_matrix,_sym_getinfo,&in_minfo);
		jit_object_method(in_matrix,_sym_getdata,&in_bp);
		
		if(!in_bp){ 
			object_error((t_object *)x, "Invalid input");
			goto out;
		}
		
		//get dimensions 
		dim[0] = in_minfo.dim[0];	// For speed: unroll the for loop and ASSUME a two dimensional matrix
		dim[1] = in_minfo.dim[1];	// !!! BUT IS THIS STUFF EVEN REALLY USED?
		
		//calculate
		jit_colortrack_calculate_ndim(x, in_minfo.dimcount, dim, &in_minfo, in_bp);
	}
	else
		goto out;
	
out:
	jit_object_method(in_matrix,gensym("lock"),in_savelock);
}


//recursive function to handle higher dimension matrices, by processing 2D sections at a time 
void jit_colortrack_calculate_ndim(t_max_jit_colortrack *x, long dimcount, long *dim, t_jit_matrix_info *in_minfo, char *bip)
{
	t_atom	my_val[6];				// for output
	float	normalized_bounds[4];
	long	i, j, n;
	long	min_x[4], min_y[4], max_x[4], max_y[4];
	char	inrange_flag_x[4], inrange_flag_y[4];
	int		tracker;
	uchar	*ip;
	char	red, green, blue;
	short	hue, saturation, lightness;
	bool	no_match;
		
	n = dim[0];

	min_x[TRACKER_1] = min_x[TRACKER_2] = min_x[TRACKER_3] = min_x[TRACKER_4] = min_y[TRACKER_1] = min_y[TRACKER_2] = min_y[TRACKER_3] = min_y[TRACKER_4] = 0x7FFFFFFF;	// a really high number, because we're looking to see if a pixel is less than the one stored
	max_x[TRACKER_1] = max_x[TRACKER_2] = max_x[TRACKER_3] = max_x[TRACKER_4] = max_y[TRACKER_1] = max_y[TRACKER_2] = max_y[TRACKER_3] = max_y[TRACKER_4] = -1;
	
//object_post((t_object *)x, "1 is looking for this: %i %i - %i %i - %i %i", x->min[TRACKER_1][HUE], x->max[TRACKER_1][HUE], x->min[TRACKER_1][SATURATION], x->max[TRACKER_1][SATURATION], x->min[TRACKER_1][BRIGHTNESS], x->max[TRACKER_1][BRIGHTNESS]);	
//object_post((t_object *)x, "2 is looking for this: %i %i - %i %i - %i %i", x->min[TRACKER_2][HUE], x->max[TRACKER_2][HUE], x->min[TRACKER_2][SATURATION], x->max[TRACKER_2][SATURATION], x->min[TRACKER_2][BRIGHTNESS], x->max[TRACKER_2][BRIGHTNESS]);	

	for(i=0;i<dim[1];i++){								// ITERATE THROUGH THE Y-AXIS
		ip = (uchar *)(bip + i * in_minfo->dimstride[1]);
//		ip = bip + i * in_minfo->dimstride[1];
		inrange_flag_y[TRACKER_1] = inrange_flag_y[TRACKER_2] = inrange_flag_y[TRACKER_3] = inrange_flag_y[TRACKER_4] = FALSE;
		
		for(j=0;j<n;j++){								// ITERATE THROUGH THE X-AXIS
			ip++;										// skip the alpha channel
			red = *ip++;
			green = *ip++;
			blue = *ip++;
			
			rgb2hsl(red, green, blue, &hue, &saturation, &lightness);
//object_post((t_object *)x, "The incoming HSL is: %i %i %i", hue, saturation, lightness);	

// ********* UNROLLING THIS FOR-LOOP FOR SPEED ********************
//			for(tracker = TRACKER_1; tracker < NUM_TRACKERS; tracker++){
//				if(x->use[tracker]){							// ** NOTE: MAY WANT TO REPLACE SOME BRANCHING WITH FUNCTION POINTERS
				if(x->use[TRACKER_1]){
					tracker = TRACKER_1;
					inrange_flag_x[tracker] = TRUE;
					
					if(x->hue_check[tracker]){			
						if(hue > x->min[tracker][HUE]) 				// HUE - range to match crosses 0 on the color wheel
							inrange_flag_x[tracker] = FALSE;
						else if(hue < x->max[tracker][HUE])	 
							inrange_flag_x[tracker] = FALSE;
					}
					else{
						if(hue < x->min[tracker][HUE]) 				// HUE - normal
							inrange_flag_x[tracker] = FALSE;
						else if(hue > x->max[tracker][HUE]) 
							inrange_flag_x[tracker] = FALSE;			
					}
					
					if(saturation < x->min[tracker][SATURATION]) 			// SATURATION
						inrange_flag_x[tracker] = FALSE;
					else if(saturation > x->max[tracker][SATURATION]) 
						inrange_flag_x[tracker] = FALSE;
					
					if(lightness < x->min[tracker][LIGHTNESS]) 				// LIGHTNESS
						inrange_flag_x[tracker] = FALSE;
					else if(lightness > x->max[tracker][LIGHTNESS]) 
						inrange_flag_x[tracker] = FALSE;
					
					if(inrange_flag_x[tracker]){			// ANY TIME A PIXEL IS TRUE
						inrange_flag_y[tracker] = TRUE;		// set the Y-axis flag to true
						if(j < min_x[tracker]) 			// min_x or max_x = is updated with the X-axis pixel location
							min_x[tracker] = j;
						else if(j > max_x[tracker])
							max_x[tracker] = j;
					}
				}
				if(x->use[TRACKER_2]){
					tracker = TRACKER_2;
					inrange_flag_x[tracker] = TRUE;
					
					if(x->hue_check[tracker]){			
						if(hue > x->min[tracker][HUE]) 				// HUE - range to match crosses 0 on the color wheel
							inrange_flag_x[tracker] = FALSE;
						else if(hue < x->max[tracker][HUE])	 
							inrange_flag_x[tracker] = FALSE;
					}
					else{
						if(hue < x->min[tracker][HUE]) 				// HUE - normal
							inrange_flag_x[tracker] = FALSE;
						else if(hue > x->max[tracker][HUE]) 
							inrange_flag_x[tracker] = FALSE;			
					}
					
					if(saturation < x->min[tracker][SATURATION]) 			// SATURATION
						inrange_flag_x[tracker] = FALSE;
					else if(saturation > x->max[tracker][SATURATION]) 
						inrange_flag_x[tracker] = FALSE;
					
					if(lightness < x->min[tracker][LIGHTNESS]) 				// LIGHTNESS
						inrange_flag_x[tracker] = FALSE;
					else if(lightness > x->max[tracker][LIGHTNESS]) 
						inrange_flag_x[tracker] = FALSE;
					
					if(inrange_flag_x[tracker]){			// ANY TIME A PIXEL IS TRUE
						inrange_flag_y[tracker] = TRUE;		// set the Y-axis flag to true
						if(j < min_x[tracker]) 			// min_x or max_x = is updated with the X-axis pixel location
							min_x[tracker] = j;
						else if(j > max_x[tracker])
							max_x[tracker] = j;
					}
				}
				if(x->use[TRACKER_3]){
					tracker = TRACKER_3;
					inrange_flag_x[tracker] = TRUE;
					
					if(x->hue_check[tracker]){			
						if(hue > x->min[tracker][HUE]) 				// HUE - range to match crosses 0 on the color wheel
							inrange_flag_x[tracker] = FALSE;
						else if(hue < x->max[tracker][HUE])	 
							inrange_flag_x[tracker] = FALSE;
					}
					else{
						if(hue < x->min[tracker][HUE]) 				// HUE - normal
							inrange_flag_x[tracker] = FALSE;
						else if(hue > x->max[tracker][HUE]) 
							inrange_flag_x[tracker] = FALSE;			
					}
					
					if(saturation < x->min[tracker][SATURATION]) 			// SATURATION
						inrange_flag_x[tracker] = FALSE;
					else if(saturation > x->max[tracker][SATURATION]) 
						inrange_flag_x[tracker] = FALSE;
					
					if(lightness < x->min[tracker][LIGHTNESS]) 				// LIGHTNESS
						inrange_flag_x[tracker] = FALSE;
					else if(lightness > x->max[tracker][LIGHTNESS]) 
						inrange_flag_x[tracker] = FALSE;
					
					if(inrange_flag_x[tracker]){			// ANY TIME A PIXEL IS TRUE
						inrange_flag_y[tracker] = TRUE;		// set the Y-axis flag to true
						if(j < min_x[tracker]) 			// min_x or max_x = is updated with the X-axis pixel location
							min_x[tracker] = j;
						else if(j > max_x[tracker])
							max_x[tracker] = j;
					}
				}
				if(x->use[TRACKER_4]){
					tracker = TRACKER_4;
					inrange_flag_x[tracker] = TRUE;
					
					if(x->hue_check[tracker]){			
						if(hue > x->min[tracker][HUE]) 				// HUE - range to match crosses 0 on the color wheel
							inrange_flag_x[tracker] = FALSE;
						else if(hue < x->max[tracker][HUE])	 
							inrange_flag_x[tracker] = FALSE;
					}
					else{
						if(hue < x->min[tracker][HUE]) 				// HUE - normal
							inrange_flag_x[tracker] = FALSE;
						else if(hue > x->max[tracker][HUE]) 
							inrange_flag_x[tracker] = FALSE;			
					}
					
					if(saturation < x->min[tracker][SATURATION]) 			// SATURATION
						inrange_flag_x[tracker] = FALSE;
					else if(saturation > x->max[tracker][SATURATION]) 
						inrange_flag_x[tracker] = FALSE;
					
					if(lightness < x->min[tracker][LIGHTNESS]) 				// LIGHTNESS
						inrange_flag_x[tracker] = FALSE;
					else if(lightness > x->max[tracker][LIGHTNESS]) 
						inrange_flag_x[tracker] = FALSE;
					
					if(inrange_flag_x[tracker]){			// ANY TIME A PIXEL IS TRUE
						inrange_flag_y[tracker] = TRUE;		// set the Y-axis flag to true
						if(j < min_x[tracker]) 			// min_x or max_x = is updated with the X-axis pixel location
							min_x[tracker] = j;
						else if(j > max_x[tracker])
							max_x[tracker] = j;
					}
				}
//			}
		}
		
/*		for(tracker = TRACKER_1; tracker < NUM_TRACKERS; tracker++){
			if(x->use[tracker] && inrange_flag_y[tracker]){		// ANY TIME A PIXEL IS TRUE (the y-axis flag was set in the x-axis loop)
				if(i < min_y[tracker]) 				// min_y or max_y = is updated with the Y-axis pixel location
					min_y[tracker] = i;
				else if(i > max_y[tracker]) 
					max_y[tracker] = i;
			}
		}
*/
// *********** UNROLLED VERSION OF THE ABOVE FOR-LOOP FOR SPEED ************
			if(x->use[TRACKER_1] && inrange_flag_y[TRACKER_1]){		// ANY TIME A PIXEL IS TRUE (the y-axis flag was set in the x-axis loop)
				if(i < min_y[TRACKER_1]) 				// min_y or max_y = is updated with the Y-axis pixel location
					min_y[TRACKER_1] = i;
				else if(i > max_y[TRACKER_1]) 
					max_y[TRACKER_1] = i;
			}
			if(x->use[TRACKER_2] && inrange_flag_y[TRACKER_2]){		// ANY TIME A PIXEL IS TRUE (the y-axis flag was set in the x-axis loop)
				if(i < min_y[TRACKER_2]) 				// min_y or max_y = is updated with the Y-axis pixel location
					min_y[TRACKER_2] = i;
				else if(i > max_y[TRACKER_2]) 
					max_y[TRACKER_2] = i;
			}
			if(x->use[TRACKER_3] && inrange_flag_y[TRACKER_3]){		// ANY TIME A PIXEL IS TRUE (the y-axis flag was set in the x-axis loop)
				if(i < min_y[TRACKER_3]) 				// min_y or max_y = is updated with the Y-axis pixel location
					min_y[TRACKER_3] = i;
				else if(i > max_y[TRACKER_3]) 
					max_y[TRACKER_3] = i;
			}
			if(x->use[TRACKER_4] && inrange_flag_y[TRACKER_4]){		// ANY TIME A PIXEL IS TRUE (the y-axis flag was set in the x-axis loop)
				if(i < min_y[TRACKER_4]) 				// min_y or max_y = is updated with the Y-axis pixel location
					min_y[TRACKER_4] = i;
				else if(i > max_y[TRACKER_4]) 
					max_y[TRACKER_4] = i;
			}






	}
	
	// ***************
	// POST PROCESSING
	for(tracker = TRACKER_1; tracker < NUM_TRACKERS; tracker++){
		if(x->use[tracker]){
			no_match = true;
			if(min_x[tracker] == 0x7FFFFFFF){
				min_x[tracker] = -1;
//				goto nomatch;		// This is bad because if tracker1 doesn't match, but tracker 2 does it still skips tracker2!
			}
			else no_match = false;
			
			if(min_y[tracker] == 0x7FFFFFFF){
				min_y[tracker] = -1;
//				goto nomatch;
			}
			else no_match = false;
			
			if(max_x[tracker] == -1){
				max_x[tracker] = min_x[tracker];
//				goto nomatch;		
			}
			else no_match = false;
			
			if(max_y[tracker] == -1){ 
				max_y[tracker] = min_y[tracker];
//				goto nomatch;
			}
			else no_match = false;

			if(no_match) goto nomatch;
			
			// Normalize	
			normalized_bounds[0] = (float)min_x[tracker] / dim[0];
			normalized_bounds[1] = (float)min_y[tracker] / dim[1];
			normalized_bounds[2] = (float)max_x[tracker] / dim[0];
			normalized_bounds[3] = (float)max_y[tracker] / dim[1];	
			
			if(x->output_bounds[tracker]){
				atom_setlong(&(my_val[0]), tracker+1);								
				atom_setsym(&(my_val[1]), gensym("bounds"));
				atom_setfloat(&(my_val[2]), normalized_bounds[0]);
				atom_setfloat(&(my_val[3]), normalized_bounds[1]);
				atom_setfloat(&(my_val[4]), normalized_bounds[2]);
				atom_setfloat(&(my_val[5]), normalized_bounds[3]);
//				outlet_list(x->valout, 0L, 6, &my_val);
				outlet_list(x->valout, 0L, 6, &my_val[0]);
			}
			if(x->output_center[tracker]){			
				atom_setlong(&(my_val[0]), tracker+1);								
				atom_setsym(&(my_val[1]),gensym("center"));					
				atom_setfloat(&(my_val[2]), (normalized_bounds[2] + normalized_bounds[0]) * 0.5);
				atom_setfloat(&(my_val[3]), (normalized_bounds[3] + normalized_bounds[1]) * 0.5);
//				outlet_list(x->valout, 0L, 4, &my_val);
				outlet_list(x->valout, 0L, 4, &my_val[0]);
			}
			if(x->output_size[tracker]){
				atom_setlong(&(my_val[0]), tracker+1);								
				atom_setsym(&(my_val[1]),gensym("size"));					
				atom_setfloat(&(my_val[2]), (normalized_bounds[2] - normalized_bounds[0]) * (normalized_bounds[3] - normalized_bounds[1]));
//				outlet_list(x->valout, 0L, 3, &my_val);
				outlet_list(x->valout, 0L, 3, &my_val[0]);
			}
nomatch:;
		}	
	}	
	return;
}


// RGB-2-HSL
void rgb2hsl(uchar red, uchar green, uchar blue, short *hue, short *saturation, short *lightness)
{
	short	max, min;
	short 	L, S = 0;
	float	H = 0;
	float	delta;
	
	min = red;
	if(min > green)
		min = green;
	if(min > blue) 
		min = blue;

	max = red;
	if(max < green) 
		max = green;
	if(max < blue) 
		max = blue;

	L = (max + min) / 2;	// the most L could be is 255

	delta = max - min;
	
	if(delta){
		if(L < 128)
			S = 255.0f * (delta / (max + min));
		else 
			S = 255.0f * (delta / (511 - max - min));
			
		if(red == max) 
			H = (green - blue) / delta;
		else if(green == max) 
			H = 2 + (blue - red) / delta;
		else if(blue == max) 
			H = 4 + (red - green) / delta;

		H *= 60;
		if (H < 0) 
			H += 360;   
	}

	*hue = H;			// range: 0 - 360
	*saturation = S;	// range: 0 - 255
	*lightness = L;		// range: 0 - 255
}



// ******************************************************
// ATTRIBUTE SET METHODS

void colortrack_sethue(t_max_jit_colortrack *x, short tracker_num, short value)
{
	x->hue[tracker_num] = TTClip(value, (short)0, (short)359);
	x->min[tracker_num][HUE] = tt_audio_base::onewrap(x->hue[tracker_num] - x->hue_variation[tracker_num], 0L, 359L);
	x->max[tracker_num][HUE] = tt_audio_base::onewrap(x->hue[tracker_num] + x->hue_variation[tracker_num], 0L, 359L);	
	if(x->min[tracker_num][HUE] > x->max[tracker_num][HUE])
		x->hue_check[tracker_num] = 1;
	else
		x->hue_check[tracker_num] = 0;
}

void colortrack_sethuevar(t_max_jit_colortrack *x, short tracker_num, short value)
{
	x->hue_variation[tracker_num] = TTClip(value, (short)0, (short)359);
	x->min[tracker_num][HUE] = tt_audio_base::onewrap(x->hue[tracker_num] - x->hue_variation[tracker_num], 0L, 359L);
	x->max[tracker_num][HUE] = tt_audio_base::onewrap(x->hue[tracker_num] + x->hue_variation[tracker_num], 0L, 359L);
	if(x->min[tracker_num][HUE] > x->max[tracker_num][HUE])
		x->hue_check[tracker_num] = 1;
	else
		x->hue_check[tracker_num] = 0;
}


void colortrack_setsaturation(t_max_jit_colortrack *x, short tracker_num, short value)
{
	x->saturation[tracker_num] = TTClip(value, (short)0, (short)255);
	x->min[tracker_num][SATURATION] = TTClip(x->saturation[tracker_num] - x->saturation_variation[tracker_num], 0L, 255L);
	x->max[tracker_num][SATURATION] = TTClip(x->saturation[tracker_num] + x->saturation_variation[tracker_num], 0L, 255L);
}

void colortrack_setsaturationvar(t_max_jit_colortrack *x, short tracker_num, short value)
{
	x->saturation_variation[tracker_num] = TTClip(value, (short)0, (short)255);
	x->min[tracker_num][SATURATION] = TTClip(x->saturation[tracker_num] - x->saturation_variation[tracker_num], 0L, 255L);
	x->max[tracker_num][SATURATION] = TTClip(x->saturation[tracker_num] + x->saturation_variation[tracker_num], 0L, 255L);
}


void colortrack_setbrightness(t_max_jit_colortrack *x, short tracker_num, short value)
{
	x->brightness[tracker_num] = TTClip(value, (short)0, (short)255);
	x->min[tracker_num][BRIGHTNESS] = TTClip(x->brightness[tracker_num] - x->brightness_variation[tracker_num], 0L, 255L);
	x->max[tracker_num][BRIGHTNESS] = TTClip(x->brightness[tracker_num] + x->brightness_variation[tracker_num], 0L, 255L);
}

void colortrack_setbrightnessvar(t_max_jit_colortrack *x, short tracker_num, short value)
{
	x->brightness_variation[tracker_num] = TTClip(value, (short)0, (short)255);
	x->min[tracker_num][BRIGHTNESS] = TTClip(x->brightness[tracker_num] - x->brightness_variation[tracker_num], 0L, 255L);
	x->max[tracker_num][BRIGHTNESS] = TTClip(x->brightness[tracker_num] + x->brightness_variation[tracker_num], 0L, 255L);
}



// ATTRIBUTE: HUE_1
t_jit_err colortrack_set_hue_1(t_max_jit_colortrack *x, void *attr, long argc, t_atom *argv)
{
	colortrack_sethue(x, TRACKER_1, atom_getlong(argv));
	return MAX_ERR_NONE;
}

t_jit_err colortrack_set_hue_variation_1(t_max_jit_colortrack *x, void *attr, long argc, t_atom *argv)
{
	colortrack_sethuevar(x, TRACKER_1, atom_getlong(argv));
	return MAX_ERR_NONE;	
}

// ATTRIBUTE: HUE_2
t_jit_err colortrack_set_hue_2(t_max_jit_colortrack *x, void *attr, long argc, t_atom *argv)
{
	colortrack_sethue(x, TRACKER_2, atom_getlong(argv));
	return MAX_ERR_NONE;
}

t_jit_err colortrack_set_hue_variation_2(t_max_jit_colortrack *x, void *attr, long argc, t_atom *argv)
{
	colortrack_sethuevar(x, TRACKER_2, atom_getlong(argv));
	return MAX_ERR_NONE;
}

// ATTRIBUTE: HUE_3
t_jit_err colortrack_set_hue_3(t_max_jit_colortrack *x, void *attr, long argc, t_atom *argv)
{
	colortrack_sethue(x, TRACKER_3, atom_getlong(argv));
	return MAX_ERR_NONE;
}

t_jit_err colortrack_set_hue_variation_3(t_max_jit_colortrack *x, void *attr, long argc, t_atom *argv)
{
	colortrack_sethuevar(x, TRACKER_3, atom_getlong(argv));
	return MAX_ERR_NONE;
}

// ATTRIBUTE: HUE_4
t_jit_err colortrack_set_hue_4(t_max_jit_colortrack *x, void *attr, long argc, t_atom *argv)
{
	colortrack_sethue(x, TRACKER_4, atom_getlong(argv));
	return MAX_ERR_NONE;
}

t_jit_err colortrack_set_hue_variation_4(t_max_jit_colortrack *x, void *attr, long argc, t_atom *argv)
{
	colortrack_sethuevar(x, TRACKER_4, atom_getlong(argv));
	return MAX_ERR_NONE;
}


// ATTRIBUTE: SATURATION_1
t_jit_err colortrack_set_saturation_1(t_max_jit_colortrack *x, void *attr, long argc, t_atom *argv)
{
	colortrack_setsaturation(x, TRACKER_1, atom_getlong(argv));
	return MAX_ERR_NONE;
}

t_jit_err colortrack_set_saturation_variation_1(t_max_jit_colortrack *x, void *attr, long argc, t_atom *argv)
{
	colortrack_setsaturationvar(x, TRACKER_1, atom_getlong(argv));
	return MAX_ERR_NONE;
}

// ATTRIBUTE: SATURATION_2
t_jit_err colortrack_set_saturation_2(t_max_jit_colortrack *x, void *attr, long argc, t_atom *argv)
{
	colortrack_setsaturation(x, TRACKER_2, atom_getlong(argv));
	return MAX_ERR_NONE;
}

t_jit_err colortrack_set_saturation_variation_2(t_max_jit_colortrack *x, void *attr, long argc, t_atom *argv)
{
	colortrack_setsaturationvar(x, TRACKER_2, atom_getlong(argv));
	return MAX_ERR_NONE;
}

// ATTRIBUTE: SATURATION_3
t_jit_err colortrack_set_saturation_3(t_max_jit_colortrack *x, void *attr, long argc, t_atom *argv)
{
	colortrack_setsaturation(x, TRACKER_3, atom_getlong(argv));
	return MAX_ERR_NONE;
}

t_jit_err colortrack_set_saturation_variation_3(t_max_jit_colortrack *x, void *attr, long argc, t_atom *argv)
{
	colortrack_setsaturationvar(x, TRACKER_3, atom_getlong(argv));
	return MAX_ERR_NONE;
}

// ATTRIBUTE: SATURATION_4
t_jit_err colortrack_set_saturation_4(t_max_jit_colortrack *x, void *attr, long argc, t_atom *argv)
{
	colortrack_setsaturation(x, TRACKER_4, atom_getlong(argv));
	return MAX_ERR_NONE;
}

t_jit_err colortrack_set_saturation_variation_4(t_max_jit_colortrack *x, void *attr, long argc, t_atom *argv)
{
	colortrack_setsaturationvar(x, TRACKER_4, atom_getlong(argv));
	return MAX_ERR_NONE;
}




// ATTRIBUTE: BRIGHTNESS_1
t_jit_err colortrack_set_brightness_1(t_max_jit_colortrack *x, void *attr, long argc, t_atom *argv)
{
	colortrack_setbrightness(x, TRACKER_1, atom_getlong(argv));
	return MAX_ERR_NONE;
}

t_jit_err colortrack_set_brightness_variation_1(t_max_jit_colortrack *x, void *attr, long argc, t_atom *argv)
{
	colortrack_setbrightnessvar(x, TRACKER_1, atom_getlong(argv));
	return MAX_ERR_NONE;
}

// ATTRIBUTE: BRIGHTNESS_2
t_jit_err colortrack_set_brightness_2(t_max_jit_colortrack *x, void *attr, long argc, t_atom *argv)
{
	colortrack_setbrightness(x, TRACKER_2, atom_getlong(argv));
	return MAX_ERR_NONE;
}

t_jit_err colortrack_set_brightness_variation_2(t_max_jit_colortrack *x, void *attr, long argc, t_atom *argv)
{
	colortrack_setbrightnessvar(x, TRACKER_2, atom_getlong(argv));
	return MAX_ERR_NONE;
}

// ATTRIBUTE: BRIGHTNESS_3
t_jit_err colortrack_set_brightness_3(t_max_jit_colortrack *x, void *attr, long argc, t_atom *argv)
{
	colortrack_setbrightness(x, TRACKER_3, atom_getlong(argv));
	return MAX_ERR_NONE;
}

t_jit_err colortrack_set_brightness_variation_3(t_max_jit_colortrack *x, void *attr, long argc, t_atom *argv)
{
	colortrack_setbrightnessvar(x, TRACKER_3, atom_getlong(argv));
	return MAX_ERR_NONE;
}

// ATTRIBUTE: BRIGHTNESS_4
t_jit_err colortrack_set_brightness_4(t_max_jit_colortrack *x, void *attr, long argc, t_atom *argv)
{
	colortrack_setbrightness(x, TRACKER_4, atom_getlong(argv));
	return MAX_ERR_NONE;
}

t_jit_err colortrack_set_brightness_variation_4(t_max_jit_colortrack *x, void *attr, long argc, t_atom *argv)
{
	colortrack_setbrightnessvar(x, TRACKER_4, atom_getlong(argv));
	return MAX_ERR_NONE;
}



void colortrack_setoutputbounds(t_max_jit_colortrack *x, short tracker_num, short value)
{
	x->output_bounds[tracker_num] = value;
	x->use[tracker_num] = ((x->output_bounds[tracker_num]) || (x->output_center[tracker_num]) || (x->output_size[tracker_num]));
}

void colortrack_setoutputcenter(t_max_jit_colortrack *x, short tracker_num, short value)
{
	x->output_center[tracker_num] = value;
	x->use[tracker_num] = ((x->output_bounds[tracker_num]) || (x->output_center[tracker_num]) || (x->output_size[tracker_num]));
}

void colortrack_setoutputsize(t_max_jit_colortrack *x, short tracker_num, short value)
{
	x->output_size[tracker_num] = value;
	x->use[tracker_num] = ((x->output_bounds[tracker_num]) || (x->output_center[tracker_num]) || (x->output_size[tracker_num]));
}


// ATTRIBUTE: OUTPUT_BOUNDS_1
t_jit_err colortrack_set_output_bounds_1(t_max_jit_colortrack *x, void *attr, long argc, t_atom *argv)
{
	colortrack_setoutputbounds(x, TRACKER_1, atom_getlong(argv));
	return MAX_ERR_NONE;
}		

// ATTRIBUTE: OUTPUT_BOUNDS_2
t_jit_err colortrack_set_output_bounds_2(t_max_jit_colortrack *x, void *attr, long argc, t_atom *argv)
{
	colortrack_setoutputbounds(x, TRACKER_2, atom_getlong(argv));
	return MAX_ERR_NONE;
}		

// ATTRIBUTE: OUTPUT_BOUNDS_3
t_jit_err colortrack_set_output_bounds_3(t_max_jit_colortrack *x, void *attr, long argc, t_atom *argv)
{
	colortrack_setoutputbounds(x, TRACKER_3, atom_getlong(argv));
	return MAX_ERR_NONE;
}		

// ATTRIBUTE: OUTPUT_BOUNDS_4
t_jit_err colortrack_set_output_bounds_4(t_max_jit_colortrack *x, void *attr, long argc, t_atom *argv)
{
	colortrack_setoutputbounds(x, TRACKER_4, atom_getlong(argv));
	return MAX_ERR_NONE;
}		


// ATTRIBUTE: OUTPUT_CENTER_1
t_jit_err colortrack_set_output_center_1(t_max_jit_colortrack *x, void *attr, long argc, t_atom *argv)
{
	colortrack_setoutputcenter(x, TRACKER_1, atom_getlong(argv));
	return MAX_ERR_NONE;
}		

// ATTRIBUTE: OUTPUT_CENTER_2
t_jit_err colortrack_set_output_center_2(t_max_jit_colortrack *x, void *attr, long argc, t_atom *argv)
{
	colortrack_setoutputcenter(x, TRACKER_2, atom_getlong(argv));
	return MAX_ERR_NONE;
}		

// ATTRIBUTE: OUTPUT_CENTER_3
t_jit_err colortrack_set_output_center_3(t_max_jit_colortrack *x, void *attr, long argc, t_atom *argv)
{
	colortrack_setoutputcenter(x, TRACKER_3, atom_getlong(argv));
	return MAX_ERR_NONE;
}		

// ATTRIBUTE: OUTPUT_CENTER_4
t_jit_err colortrack_set_output_center_4(t_max_jit_colortrack *x, void *attr, long argc, t_atom *argv)
{
	colortrack_setoutputcenter(x, TRACKER_4, atom_getlong(argv));
	return MAX_ERR_NONE;
}		


// ATTRIBUTE: OUTPUT_SIZE_1
t_jit_err colortrack_set_output_size_1(t_max_jit_colortrack *x, void *attr, long argc, t_atom *argv)
{
	colortrack_setoutputsize(x, TRACKER_1, atom_getlong(argv));
	return MAX_ERR_NONE;
}		

// ATTRIBUTE: OUTPUT_SIZE_2
t_jit_err colortrack_set_output_size_2(t_max_jit_colortrack *x, void *attr, long argc, t_atom *argv)
{
	colortrack_setoutputsize(x, TRACKER_2, atom_getlong(argv));
	return MAX_ERR_NONE;
}		

// ATTRIBUTE: OUTPUT_SIZE_3
t_jit_err colortrack_set_output_size_3(t_max_jit_colortrack *x, void *attr, long argc, t_atom *argv)
{
	colortrack_setoutputsize(x, TRACKER_3, atom_getlong(argv));
	return MAX_ERR_NONE;
}		

// ATTRIBUTE: OUTPUT_SIZE_4
t_jit_err colortrack_set_output_size_4(t_max_jit_colortrack *x, void *attr, long argc, t_atom *argv)
{
	colortrack_setoutputsize(x, TRACKER_4, atom_getlong(argv));
	return MAX_ERR_NONE;
}		
