// MSP External: fftlist~.c
// Converts spectral data from signal stream to a list

#include "TTClassWrapperMax.h"

#define MAXSIZE 2048L	

static t_class *fftlist_class;				// Required. Global pointing to this class

// Data Structure for this object
typedef struct _fftlist	
{
	t_pxobject	x_obj;				// Header;  Must always be the first field; used by MSP
	int 		s_bins;		    			// Value from the typed in argument
   	void 		*fftlist_out;				// Pointer to outlet. need one for each outlet 
   	t_atom 		fft_compiled[MAXSIZE];		// Compiled list of spectral data - based on MSP's FFT limit of 4096
   	float 		fftlist_mult;				// scaling factor
   	long 		fftlist_nyquist;			// truncate at nyquist?
   	long 		fftlist_autopoll;			// Automatically spit the data?
   	void		*fftlist_clock;				// Pointer to the clock object
} t_fftlist;

// Prototypes for methods: need a method for each incoming message type
void *fftlist_new(t_symbol *msg, short argc, t_atom *argv);			// New Object Creation Method
t_int *fftlist_perform(t_int *w);									// An MSP Perform (signal) Method
void fftlist_dsp(t_fftlist *x, t_signal **sp, short *count);		// ../../../../Jamoma/Core/DSP/library/build/JamomaDSP.dylib Method
void fftlist_assist(t_fftlist *x, void *b, long m, long a, char *s);// Assistance Method
void fftlist_bang(t_fftlist *x);									// 
void fftlist_free(t_fftlist *x);									// 


/************************************************************************************/
// Main() Function

extern "C" int C74_EXPORT main(void)
{
	t_class *c;

	c = class_new("tap.fft.list~",(method)fftlist_new, (method)fftlist_free, sizeof(t_fftlist), 
		(method)0L, A_GIMME, 0);

		common_symbols_init();									// Initialize TapTools
	class_addmethod(c, (method)fftlist_bang,	"bang", 0L);
 	class_addmethod(c, (method)fftlist_dsp, 	"dsp", A_CANT, 0L);		
	class_addmethod(c, (method)fftlist_assist, 	"assist", A_CANT, 0L); 
	class_addmethod(c, (method)stdinletinfo,	"inletinfo",	A_CANT, 0);

	CLASS_ATTR_FLOAT(c,		"mult",	0,	t_fftlist, fftlist_mult);
	
	CLASS_ATTR_LONG(c,		"nyquist",	0,	t_fftlist, fftlist_nyquist);
	CLASS_ATTR_STYLE(c,		"nyquist",	0,	"onoff");
	
	CLASS_ATTR_LONG(c,		"autopoll",	0,	t_fftlist, fftlist_autopoll);
	CLASS_ATTR_STYLE(c,		"autopoll",	0,	"onoff");
	
	class_dspinit(c);									// Setup object's class to work with MSP
class_register(_sym_box, c); 	fftlist_class = c;
}


/************************************************************************************/
// Object Creation Method

void *fftlist_new(t_symbol *msg, short argc, t_atom *argv)
{
	t_atom_long value = 512;
	t_fftlist 	*x;
	long		attrstart;

	attrstart = attr_args_offset(argc, argv);
	if(attrstart && argv)
		atom_arg_getlong(&value, 0, attrstart, argv);	// support a normal float argument
	
	x = (t_fftlist *)object_alloc(fftlist_class);;
	if(x){
	   	object_obex_store((void *)x, _sym_dumpout, (object *)outlet_new(x,NULL));	// dumpout	
		dsp_setup((t_pxobject *)x, 2);							// Create Object and 2 Inlets
	 	x->fftlist_out = listout(x);							// Create a list Outlet
	 	x->fftlist_clock = clock_new(x, (method)fftlist_bang);	// Create the queue clock

		x->s_bins = TTClip<TTInt32>(value, 2, MAXSIZE);		// Number of bins in analysis
		x->fftlist_mult = 1.;									// 
		x->fftlist_nyquist = 1;									// 
		x->fftlist_autopoll = 1;								// 
		
		attr_args_process(x,argc,argv);							//handle attribute args					
	}
	return x;													// Return pointer
}


/************************************************************************************/
// Methods bound to input/inlets

// Method for Assistance Messages
void fftlist_assist(t_fftlist *x, void *b, long msg, long arg, char *dst)
{
	if(msg==1){ 	// Inlets
		switch(arg){
			case 0: strcpy(dst, "(signal) Input from FFT"); break;
			case 1: strcpy(dst, "(signal) Index from FFT"); break;
		}
	}
	else if(msg==2) // Outlets
		strcpy(dst, "(list) the data frame-by-frame");
}


// Memory Deallocation
void fftlist_free(t_fftlist *x)
{
	dsp_free((t_pxobject *)x);	// THIS MUST BE FIRST
	clock_unset(x->fftlist_clock);	// remove the clock routine from the scheduler
	clock_free((object *)x->fftlist_clock);	// free the clock memory
}


// Listout Method
void fftlist_bang(t_fftlist *x)
{
	if (x->fftlist_nyquist == 0) 
		outlet_list(x->fftlist_out, 0L, x->s_bins, x->fft_compiled);
	else
	 	outlet_list(x->fftlist_out, 0L, x->s_bins * 0.5, x->fft_compiled);
}


// Perform (signal) Method
t_int *fftlist_perform(t_int *w)
{
	t_float *in = (t_float *)(w[1]);		// Spectral data Input  
	t_float *in2 = (t_float *)(w[2]);	// Index Input		
   	t_fftlist *x = (t_fftlist *)(w[3]);	//  Object Pointer
	int n = (int)(w[4]);				// "Vector" Size
	float value;
 	int index, bins = x->s_bins - 1, changeFlag = 0;
 	double index2;
 
 	if (x->x_obj.z_disabled) goto byebye;
	while (--n)
	{
		value = *++in;
		index2 = *++in2;
		index = index2 + 0.49;	
		value = value * x->fftlist_mult;				// Amplify values as desired
		atom_setfloat(x->fft_compiled + index, value);
		if (index == bins) changeFlag = 1;			// and we are at the last index
	}
	if((changeFlag == 1)&&(x->fftlist_autopoll != 0)) 
		clock_delay(x->fftlist_clock, 0);				// Set clock to go off ASAP
byebye:	
	return (w + 5);								// return pointer to NEXT object in dsp chain
}


// ../../../../Jamoma/Core/DSP/library/build/JamomaDSP.dylib Method
void fftlist_dsp(t_fftlist *x, t_signal **sp, short *count)
{
	dsp_add(fftlist_perform, 4, sp[0]->s_vec-1, sp[1]->s_vec-1, x, sp[0]->s_n+1);	// Add Perform (signal) Method to the ../../../../Jamoma/Core/DSP/library/build/JamomaDSP.dylib Call Chain
}