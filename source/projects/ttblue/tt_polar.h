/*
 *******************************************************
 *		CONVERSION BETWEEN 
 *		POLAR AND CARTESIAN COORDINATES
 *******************************************************
 *		TapTools Blue Object
 *		copyright � 2003 by Timothy A. Place
 *
 */

// Check against redundant including
#ifndef TT_POLAR_H
#define TT_POLAR_H

// Include appropriate headers
#include "tt_audio_base.h"
#include <math.h>

/********************************************************
	CLASS INTERFACE/IMPLEMENTATION

	The entire class is implemented inline for speed.
 ********************************************************/

class tt_polar:public tt_audio_base{

	private:
		// Function pointer for the ../../../../Jamoma/Core/DSP/library/build/JamomaDSP.dylib Loop (use this instead of branching for speed)
		typedef void (tt_polar::*function_ptr_2in_2out)(tt_audio_signal *, tt_audio_signal *, tt_audio_signal *, tt_audio_signal *);			
		function_ptr_2in_2out dsp_executor;

		// Attributes Values & Variables
		tt_attribute_value_discrete	mode;
		
	
	public:
		enum selectors{									// Attribute Selectors
			k_mode,				
			k_mode_cartopol,
			k_mode_poltocar,
		};
		

		// OBJECT LIFE					
		tt_polar()										// Constructor		
		{
			set_attr(k_mode, k_mode_cartopol);
		}
		
		tt_polar(selectors init_mode)
		{
			set_attr(k_mode, init_mode);
		}

		~tt_polar()									// Destructor
		{
			;
		}


		// ATTRIBUTES
		void set_attr(tt_selector sel, tt_attribute_value val)	// Set Attributes
		{
			switch (sel){
				case k_mode:
					mode = (tt_attribute_value_discrete)val;
					if(mode == k_mode_cartopol)
						dsp_executor = &tt_polar::dsp_vector_calc_cartopol;
					else if(mode == k_mode_poltocar)
						dsp_executor = &tt_polar::dsp_vector_calc_poltocar;
					break;
			}
		}

		tt_attribute_value get_attr(tt_selector sel)				// Get Attributes
		{
			switch (sel){
				case k_mode:
					return mode;
				default:
					return 0.0;
			}
		}
		
		
		/*****************************************************
		 * ../../../../Jamoma/Core/DSP/library/build/JamomaDSP.dylib LOOPS
		 *****************************************************/
		
		// Publically exposed interface for the dsp routine
		void dsp_vector_calc(tt_audio_signal *in1, tt_audio_signal *in2, tt_audio_signal *out1, tt_audio_signal *out2)
		{
			(*this.*dsp_executor)(in1, in2, out1, out2);	// Run the function pointed to by our function pointer
		}
	
	private:
		// ../../../../Jamoma/Core/DSP/library/build/JamomaDSP.dylib LOOP: CARTOPOL
		void dsp_vector_calc_cartopol(tt_audio_signal *in1, tt_audio_signal *in2, tt_audio_signal *out1, tt_audio_signal *out2)
		{
			tt_sample_value	real, imaginary, magnitude, phase;
			temp_vs = in1->vectorsize;
									
		    while (temp_vs--){
				real = *in1->vector++;
				imaginary = *in2->vector++;

				magnitude = sqrt((real * real) + (imaginary * imaginary));
				
				if (real == 0)
					real = 0.000001;				// prevent divide by zero
				phase = atan(imaginary / real);	
				if ((real < 0) && (imaginary < 0))		// arctangent corrections
					phase = phase - 3.14159265;
				else if ((real < 0) && (imaginary >= 0))
					phase = phase + 3.14159265;

				*out1->vector++ = magnitude;
				*out2->vector++ = phase;	
		    }
		    in1->reset(); in2->reset(); out1->reset(); out2->reset();
		}

		// ../../../../Jamoma/Core/DSP/library/build/JamomaDSP.dylib LOOP: POLTOCAR
		void dsp_vector_calc_poltocar(tt_audio_signal *in1, tt_audio_signal *in2, tt_audio_signal *out1, tt_audio_signal *out2)
		{
			tt_sample_value	real, imaginary, magnitude, phase;
			temp_vs = in1->vectorsize;
									
		    while (temp_vs--){
				magnitude = *in1->vector++;
				phase = *in2->vector++;

				real = magnitude * cos(phase);
				imaginary = magnitude * sin(phase);

				*out1->vector++ = real;
				*out2->vector++ = imaginary;	
		    }
		    in1->reset(); in2->reset(); out1->reset(); out2->reset();
		}		
};

#endif		// TT_POLAR_H
