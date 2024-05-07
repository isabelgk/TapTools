/* 
 *	External object for Max/MSP
 *	Copyright © 2000 by Timothy Place
 * 
 *	License: This code is licensed under the terms of the "New BSD License"
 *	http://creativecommons.org/licenses/BSD/
 */

// Things to think about adding: a switch for the automatic gain cut

#include "TTClassWrapperMax.h"


static t_class *elixir_class;				// Required. Global pointing to this class

// Data Structure for this object
typedef struct _elixir
{
	t_pxobject	x_obj;
	int 		elixir_toggle[10];		// The 10 switches for the 10 inlets (array)
	double 		elixir_time[10];		// The 10 slew times for the 10 inlets (array)
	double		elixir_mult[10];		// Amount to scale the output by
	int 		elixir_numInlets;		// Number of inlets created by the _new method
	double 		elixir_gtime;
	int 		elixir_sr;
	int 		line_toggle[10];
	double 		line_result[10];
	double 		line_destination[10];
	double 		line_step[10];
	long 		line_samplenum[10];
	long 		line_samplesDone[10]; 
	long 		line_slewTime[10];
} t_elixir;

// Prototypes for methods: need a method for each incoming message type
void *elixir_new(t_symbol *s, short argc, t_atom *argv);			// New Object Creation Method

void elixir_perform264(t_elixir *x, t_object *dsp64, double **ins, long numins, double **outs, long numouts, long sampleframes, long flags, void *userparam); 
void elixir_perform364(t_elixir *x, t_object *dsp64, double **ins, long numins, double **outs, long numouts, long sampleframes, long flags, void *userparam);
void elixir_perform464(t_elixir *x, t_object *dsp64, double **ins, long numins, double **outs, long numouts, long sampleframes, long flags, void *userparam);
void elixir_perform564(t_elixir *x, t_object *dsp64, double **ins, long numins, double **outs, long numouts, long sampleframes, long flags, void *userparam);
void elixir_perform664(t_elixir *x, t_object *dsp64, double **ins, long numins, double **outs, long numouts, long sampleframes, long flags, void *userparam);
void elixir_perform764(t_elixir *x, t_object *dsp64, double **ins, long numins, double **outs, long numouts, long sampleframes, long flags, void *userparam);
void elixir_perform864(t_elixir *x, t_object *dsp64, double **ins, long numins, double **outs, long numouts, long sampleframes, long flags, void *userparam);
void elixir_perform964(t_elixir *x, t_object *dsp64, double **ins, long numins, double **outs, long numouts, long sampleframes, long flags, void *userparam);
void elixir_perform1064(t_elixir *x, t_object *dsp64, double **ins, long numins, double **outs, long numouts, long sampleframes, long flags, void *userparam);

void elixir_dsp64(t_elixir *x, t_object *dsp64, short *count, double samplerate, long maxvectorsize, long flags);			// ../../../../Jamoma/Core/DSP/library/build/JamomaDSP.dylib Method
void elixir_assist(t_elixir *x, void *b, long m, long a, char *s);	// Assistance Method
void elixir_int(t_elixir *x, long toggle);							// Int Method
//void elixir_free(t_elixir *x);
void elixir_doit(t_elixir *x, long val, short inlet, short typeofmess);
void elixir_gtime(t_elixir *x, double slewtime);
void elixir_tellmeeverything(t_elixir *x);
void elixir_list(t_elixir *x, t_symbol *s, short argc, t_atom *argv);
void interp_calc(t_elixir *x, short inletNumber, double oldmult);
double perform_interpolation(t_elixir *x, short inletNumber);


/************************************************************************************/
// Main() Function

extern "C" int C74_EXPORT main(void)
{
	t_class *c;

	c = class_new("tap.elixir~",(method)elixir_new, (method)dsp_free, sizeof(t_elixir), 
		(method)0L, A_GIMME, 0L);

		common_symbols_init();												// Initialize TapTools
	class_addmethod(c, (method)elixir_int,				"int", A_LONG, 0L);
	class_addmethod(c, (method)elixir_list,				"list", A_GIMME, 0L);
	class_addmethod(c, (method)elixir_gtime,			"gtime", A_FLOAT, 0L);
	class_addmethod(c, (method)elixir_tellmeeverything,	"tellmeeverything", 0L);
	class_addmethod(c, (method)elixir_dsp64,			"dsp64", A_CANT, 0);
	class_addmethod(c, (method)elixir_assist, 			"assist", A_CANT, 0L);
	// no inletinfo method, all inlets are hot (which is the default)

	class_dspinit(c);									// Setup object's class to work with MSP
class_register(_sym_box, c); 	elixir_class = c;
}


/************************************************************************************/
// Object Creation Method
void *elixir_new(t_symbol *s, short argc, t_atom *argv)
{
	int			i, numInlets=0;
	double		slewtime=0;
	t_elixir	*x;
	
	if(argc > 0)
		numInlets = atom_getlong(argv);
	if(argc > 1)
		slewtime = atom_getfloat(argv+1);
	
	x = (t_elixir *)object_alloc(elixir_class);;
	if(x){
    	object_obex_store((void *)x, _sym_dumpout, (object *)outlet_new(x,NULL));	// dumpout	
		if (numInlets < 2) numInlets = 2;
		else if (numInlets > 10) numInlets = 10;
	 	x->elixir_numInlets = numInlets;
	        dsp_setup((t_pxobject *)x, numInlets);		// Create Object and Inlets
		outlet_new((t_pxobject *)x, "signal");		// Create a signal Outlet   

    	for (i=0; i<10; i++){
	    	if (slewtime > 0)
    			x->elixir_time[i] = slewtime;
    		else
    			x->elixir_time[i] = 0;
	  		x->elixir_toggle[i] = 0; 	
	  		x->elixir_mult[i] = 0;		// added 6-25-02	
	  		x->line_toggle[i] = 0;
			x->line_result[i] = 0;
			x->line_destination[i] = 0;
			x->line_step[i] = 0;
			x->line_samplenum[i] = 0;
			x->line_samplesDone[i] = 0; 
    	}
    	x->elixir_gtime = slewtime;
 		// attr_args_process(x,argc,argv);					//handle attribute args			   	
    }
	return (x);							// Return the pointer
}


/************************************************************************************/
// Methods bound to input/inlets

// Method for Assistance Messages
void elixir_assist(t_elixir *x, void *b, long msg, long arg, char *dst)
{
	if(msg==1) 	// Inlets
		snprintf(dst, 255, "(signal) Input %ld, (toggle) gate control, etc.", arg + 1);
	else if(msg==2) // Outlets
		strcpy(dst, "(signal) Mixed output");
}


// Tell Me Everything
void elixir_tellmeeverything(t_elixir *x)
{
	object_post((t_object *)x, "Tell Me Everything...");
	object_post((t_object *)x, "     tap.elixir version 1.2, Copyright © 2001 by Timothy Place");
	object_post((t_object *)x, "     Number of Inlets: %i", x->elixir_numInlets);
	object_post((t_object *)x, "     Toggle Position of Inlets: %i %i %i %i %i %i %i %i %i %i", x->elixir_toggle[0], x->elixir_toggle[1], x->elixir_toggle[2], x->elixir_toggle[3], x->elixir_toggle[4], x->elixir_toggle[5], x->elixir_toggle[6], x->elixir_toggle[7], x->elixir_toggle[8], x->elixir_toggle[9]);
	object_post((t_object *)x, "     Interpolation Time of Inlets: %f %f %f %f %f %f %f %f %f %f", x->elixir_time[0], x->elixir_time[1], x->elixir_time[2], x->elixir_time[3], x->elixir_time[4], x->elixir_time[5], x->elixir_time[6], x->elixir_time[7], x->elixir_time[8], x->elixir_time[9]);
	object_post((t_object *)x, "     Current Scaling Factors: %f %f %f %f %f %f %f %f %f %f", x->elixir_mult[0], x->elixir_mult[1], x->elixir_mult[2], x->elixir_mult[3], x->elixir_mult[4], x->elixir_mult[5], x->elixir_mult[6], x->elixir_mult[7], x->elixir_mult[8], x->elixir_mult[9]);
	object_post((t_object *)x, "     Inlets using interpolation: %i %i %i %i %i %i %i %i %i %i", x->line_toggle[0], x->line_toggle[1], x->line_toggle[2], x->line_toggle[3], x->line_toggle[4], x->line_toggle[5], x->line_toggle[6], x->line_toggle[7], x->line_toggle[8], x->line_toggle[9]);
}


// Set a global slew time
void elixir_gtime(t_elixir *x, double slewtime)
{
	short i;
	x->elixir_gtime = slewtime;
    	if (slewtime > 0){
    		for (i=0; i<10; i++)
    			x->elixir_time[i] = slewtime;
    	}
    	else{
    		for (i=0; i<10; i++)
    			x->elixir_time[i] = 0;
    	}		
}


// List Method
void elixir_list(t_elixir *x, t_symbol *s, short argc, t_atom *argv)
{	
	short inlet = x->x_obj.z_in;

	x->elixir_time[inlet] = atom_getfloat(argv+1);
	elixir_doit(x, atom_getfloat(argv), inlet, 1);
}


// Int (toggle) Method
void elixir_int(t_elixir *x, long val)
{
	elixir_doit(x, val, x->x_obj.z_in ,0);
}


// Do it
void elixir_doit(t_elixir *x, long val, short inlet, short typeofmess)
{
	int i;
	double n, oldmult = x->elixir_mult[inlet];

	x->elixir_toggle[inlet] = val;
	
	if (typeofmess == 0)							// If input is a single int, supply the gtime		
		x->elixir_time[inlet] = x->elixir_gtime;			

	val = 0;									// Count the number of channels that are on
	for (i = 0; i < 10; i = i + 1)
	{
		if (x->elixir_toggle[i] == 1)
			val = val + 1;			
	}
	n = val;
	for (i=0; i<10; i++)							// Set the multiply amount for this channel
	{		
		if (n != 0)
		{
			if (x->elixir_toggle[i] != 0)
				x->elixir_mult[i] = 1.0 / n;
			else
				x->elixir_mult[i] = 0;	
		}
		else 
			x->elixir_mult[i] = 0.;		
	}
	for(i=0; i<10; i++)							// Recalculate and trigger the mixer channels
		interp_calc(x, i, oldmult);
}


void interp_calc(t_elixir *x, short inletNumber, double oldmult)
{
	double ff, diff, slew, step;
	
	if ( x->elixir_time[inletNumber] != 0 )			// If interpolation is desired...
	{	
		slew = x->line_slewTime[inletNumber] = x->elixir_time[inletNumber] * 0.001 * x->elixir_sr;	// ms -> sec * sr = slewtime_in_samples

		x->line_toggle[inletNumber] = 1;							
		ff = x->line_result[inletNumber]; 							// most recent output value
		diff = x->elixir_mult[inletNumber] - ff;						// difference between it, and new value
		step = diff / slew;										// increment per sample to interpolate linearly
		x->line_destination[inletNumber] = x->elixir_mult[inletNumber];		// store final destination
		x->line_step[inletNumber] = step;							// store step size of the interpolation
		x->line_samplenum[inletNumber] = x->line_slewTime[inletNumber];	// needed for triggering the interpolation
		x->line_samplesDone[inletNumber] = 0;						// Reset the position in the interpolation process
	}
	else 
		x->line_toggle[inletNumber] = 0;							
}


void elixir_perform264(t_elixir *x, t_object *dsp64, double **ins, long numins, double **outs, long numouts, long sampleframes, long flags, void *userparam)
{
   	
    double *in1 = ins[0];				// Input1
    double *in2 = ins[1];				// Input2
    double *out = outs[0];
	int n = sampleframes;
		
	double input[2], val;
	short i;

	while(n--) {
		input[0] = *in1++; 
		input[1] = *in2++; 

		for(i=0; i<2; i++){
			if(x->line_toggle[i] == 0)
				input[i] = input[i] * x->elixir_mult[i];
			else
				input[i] = input[i] * perform_interpolation(x, i);
		}

		val = 0;
		for(i=0; i<2; i++)
			val = val + input[i];
			
		*out++ = val * 0.95;
	}	
    return;
}


void elixir_perform364(t_elixir *x, t_object *dsp64, double **ins, long numins, double **outs, long numouts, long sampleframes, long flags, void *userparam)
{
   	
	double *in1 = ins[0];	// Input1
	double *in2 = ins[1];	// Input2
	double *in3 = ins[2];	// Input3

	double *out = outs[0];
	int n = sampleframes;
		
	double input[3], val;
	short i;
						
	while(n--){
		input[0] = *in1++; 
		input[1] = *in2++; 
		input[2] = *in3++;

		for(i=0; i<3; i++){
			if(x->line_toggle[i] == 0)
				input[i] = input[i] * x->elixir_mult[i];
			else
				input[i] = input[i] * perform_interpolation(x, i);
		}

		val = 0;
		for(i=0; i<3; i++)
			val = val + input[i];
			
		*out++ = val * 0.95;
	}	
    return;
}


void elixir_perform464(t_elixir *x, t_object *dsp64, double **ins, long numins, double **outs, long numouts, long sampleframes, long flags, void *userparam)
{
    double *in1 = ins[0];	// Input1
    double *in2 = ins[1];	// Input2
	double *in3 = ins[2];	// Input3
	double *in4 = ins[3];

    double *out = outs[0];
   	
	int n = sampleframes;
		
	double input[4], val;
	short i;
						
	while (n--) 
	{
		input[0] = *in1++; 
		input[1] = *in2++; 
		input[2] = *in3++;
		input[3] = *in4++;

		for(i=0; i<4; i++){
			if(x->line_toggle[i] == 0)
				input[i] = input[i] * x->elixir_mult[i];
			else
				input[i] = input[i] * perform_interpolation(x, i);
		}

		val = 0;
		for(i=0; i<4; i++)
			val = val + input[i];
			
		*++out = val * 0.95;
	}	
    return;
}


void elixir_perform564(t_elixir *x, t_object *dsp64, double **ins, long numins, double **outs, long numouts, long sampleframes, long flags, void *userparam)
{
    double *in1 = ins[0];	// Input1
    double *in2 = ins[1];	// Input2
	double *in3 = ins[2];	// Input3
	double *in4 = ins[3];
	double *in5 = ins[4];	

    double *out = outs[0];
   	
	int n = sampleframes;
		
	double input[5], val;
	short i;
						
	while (n--) 
	{
		input[0] = *in1++; 
		input[1] = *in2++; 
		input[2] = *in3++;
		input[3] = *in4++;
		input[4] = *in5++;

		for(i=0; i<5; i++){
			if(x->line_toggle[i] == 0)
				input[i] = input[i] * x->elixir_mult[i];
			else
				input[i] = input[i] * perform_interpolation(x, i);
		}

		val = 0;
		for(i=0; i<5; i++)
			val = val + input[i];
			
		*++out = val * 0.95;
	}	
    return;
}


void elixir_perform664(t_elixir *x, t_object *dsp64, double **ins, long numins, double **outs, long numouts, long sampleframes, long flags, void *userparam)
{
    double *in1 = ins[0];	// Input1
    double *in2 = ins[1];	// Input2
	double *in3 = ins[2];	// Input3
	double *in4 = ins[3];
	double *in5 = ins[4];	
	double *in6 = ins[5];	
	
    double *out = outs[0];
   	
	int n = sampleframes;
		
	double input[6], val;
	short i;
						
	while (n--) 
	{
		input[0] = *in1++; 
		input[1] = *in2++; 
		input[2] = *in3++;
		input[3] = *in4++;
		input[4] = *in5++;
		input[5] = *in6++;
		
		for(i=0; i<6; i++){
			if(x->line_toggle[i] == 0)
				input[i] = input[i] * x->elixir_mult[i];
			else
				input[i] = input[i] * perform_interpolation(x, i);
		}

		val = 0;
		for(i=0; i<6; i++)
			val = val + input[i];
			
		*++out = val * 0.95;
	}	
    return;
}


void elixir_perform764(t_elixir *x, t_object *dsp64, double **ins, long numins, double **outs, long numouts, long sampleframes, long flags, void *userparam)
{
    double *in1 = ins[0];	// Input1
    double *in2 = ins[1];	// Input2
	double *in3 = ins[2];	// Input3
	double *in4 = ins[3];
	double *in5 = ins[4];	
	double *in6 = ins[5];	
	double *in7 = ins[6];	
		
    double *out = outs[0];
   	
	int n = sampleframes;				// Vector Size
		
	double input[7], val;
	short i;
						
	while (n--)
	{
		input[0] = *in1++; 
		input[1] = *in2++; 
		input[2] = *in3++;
		input[3] = *in4++;
		input[4] = *in5++;
		input[5] = *in6++;
		input[6] = *in7++;
		
		for(i=0; i<7; i++){
			if(x->line_toggle[i] == 0)
				input[i] = input[i] * x->elixir_mult[i];
			else
				input[i] = input[i] * perform_interpolation(x, i);
		}

		val = 0;
		for(i=0; i<7; i++)
			val = val + input[i];
			
		*++out = val * 0.95;
	}		
    return;
}


void elixir_perform864(t_elixir *x, t_object *dsp64, double **ins, long numins, double **outs, long numouts, long sampleframes, long flags, void *userparam)
{
    double *in1 = ins[0];	// Input1
    double *in2 = ins[1];	// Input2
	double *in3 = ins[2];	// Input3
	double *in4 = ins[3];
	double *in5 = ins[4];	
	double *in6 = ins[5];	
	double *in7 = ins[6];	
	double *in8 = ins[7];	
			
    double *out = outs[0];
   	
	int n = sampleframes;				// Vector Size
		
	double input[8], val;
	short i;
						
	while (n--) 
	{
		input[0] = *in1++; 
		input[1] = *in2++; 
		input[2] = *in3++;
		input[3] = *in4++;
		input[4] = *in5++;
		input[5] = *in6++;
		input[6] = *in7++;
		input[7] = *in8++;

		for(i=0; i<8; i++){
			if(x->line_toggle[i] == 0)
				input[i] = input[i] * x->elixir_mult[i];
			else
				input[i] = input[i] * perform_interpolation(x, i);
		}

		val = 0;
		for(i=0; i<8; i++)
			val = val + input[i];
			
		*++out = val * 0.95;
	}	
    return;
}


void elixir_perform964(t_elixir *x, t_object *dsp64, double **ins, long numins, double **outs, long numouts, long sampleframes, long flags, void *userparam)
{
    double *in1 = ins[0];	// Input1
    double *in2 = ins[1];	// Input2
	double *in3 = ins[2];	// Input3
	double *in4 = ins[3];
	double *in5 = ins[4];	
	double *in6 = ins[5];	
	double *in7 = ins[6];	
	double *in8 = ins[7];	
	double *in9 = ins[8];	
				
    double *out = outs[0];	// Output
   	
	int n = sampleframes;				// Vector Size
		
	double input[9], val;
	short i;
						
	while (n--) 
	{
		input[0] = *in1++; 
		input[1] = *in2++; 
		input[2] = *in3++;
		input[3] = *in4++;
		input[4] = *in5++;
		input[5] = *in6++;
		input[6] = *in7++;
		input[7] = *in8++;
		input[8] = *in9++;
		
		for(i=0; i<9; i++){
			if(x->line_toggle[i] == 0)
				input[i] = input[i] * x->elixir_mult[i];
			else
				input[i] = input[i] * perform_interpolation(x, i);
		}

		val = 0;
		for(i=0; i<9; i++)
			val = val + input[i];
			
		*++out = val * 0.95;
	}	
    return;
}


void elixir_perform1064(t_elixir *x, t_object *dsp64, double **ins, long numins, double **outs, long numouts, long sampleframes, long flags, void *userparam)
{
    double *in1 = ins[0];	// Input1
    double *in2 = ins[1];	// Input2
	double *in3 = ins[2];	// Input3
	double *in4 = ins[3];
	double *in5 = ins[4];	
	double *in6 = ins[5];	
	double *in7 = ins[6];	
	double *in8 = ins[7];	
	double *in9 = ins[8];	
	double *in10 = ins[9];	
					
    double *out = outs[0];	// Output
   	
	int n = sampleframes;				// Vector Size
		
	double input[10], val;
	short i;
						
	while (n--) 
	{
		input[0] = *in1++; 
		input[1] = *in2++; 
		input[2] = *in3++;
		input[3] = *in4++;
		input[4] = *in5++;
		input[5] = *in6++;
		input[6] = *in7++;
		input[7] = *in8++;
		input[8] = *in9++;
		input[9] = *in10++;
		
		for(i=0; i<10; i++){
			if(x->line_toggle[i] == 0)
				input[i] = input[i] * x->elixir_mult[i];
			else
				input[i] = input[i] * perform_interpolation(x, i);
		}

		val = 0;
		for(i=0; i<10; i++)
			val = val + input[i];
			
		*++out = val * 0.95;
	}	
    return;
}


// Interpolator...
double perform_interpolation(t_elixir *x, short inletNumber)
{
	double xx;
	if (x->line_samplesDone[inletNumber] != x->line_samplenum[inletNumber])
	{
		xx = x->line_result[inletNumber] + x->line_step[inletNumber];
		x->line_samplesDone[inletNumber]= x->line_samplesDone[inletNumber] + 1;
		x->line_result[inletNumber] = xx;				// store the result							
		return(xx);
	}
	return(x->line_result[inletNumber]);
}


void elixir_dsp64(t_elixir *x, t_object *dsp64, short *count, double samplerate, long maxvectorsize, long flags)
{
	short i;
	x->elixir_sr = samplerate;
	
	for (i=0; i<10; i++) {
		x->line_samplenum[i] = 0;
		x->line_samplesDone[i] = 0; 
	}
	
	switch (x->elixir_numInlets) {
		case 2:
			object_method(dsp64, gensym("dsp_add64"), (t_object*)x, elixir_perform264, 0, NULL);
			break;
		case 3:
			object_method(dsp64, gensym("dsp_add64"), (t_object*)x, elixir_perform364, 0, NULL);
			break;
		case 4:
			object_method(dsp64, gensym("dsp_add64"), (t_object*)x, elixir_perform464, 0, NULL);
			break;
		case 5:
			object_method(dsp64, gensym("dsp_add64"), (t_object*)x, elixir_perform564, 0, NULL);
			break;
		case 6:
			object_method(dsp64, gensym("dsp_add64"), (t_object*)x, elixir_perform664, 0, NULL);
			break;
		case 7:
			object_method(dsp64, gensym("dsp_add64"), (t_object*)x, elixir_perform764, 0, NULL);
			break;
		case 8:
			object_method(dsp64, gensym("dsp_add64"), (t_object*)x, elixir_perform864, 0, NULL);
			break;
		case 9:
			object_method(dsp64, gensym("dsp_add64"), (t_object*)x, elixir_perform964, 0, NULL);
			break;
		case 10:
			object_method(dsp64, gensym("dsp_add64"), (t_object*)x, elixir_perform1064, 0, NULL);
			break;
	}
}
