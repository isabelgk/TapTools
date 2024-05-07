/*
 *******************************************************
 *		CROSSFADER (2 INPUTS / 1 OUTPUT)
 *******************************************************
 *		TapTools Blue Object
 *		copyright � 2003 by Timothy A. Place
 *
 */

// Check against redundant including
#ifndef TT_CROSSFADE_H
#define TT_CROSSFADE_H

// Include appropriate headers
#include "tt_audio_base.h"


/********************************************************
	CLASS INTERFACE/IMPLEMENTATION

	The entire class is implemented inline for speed.
 ********************************************************/

class tt_crossfade:public tt_audio_base{

	private:
		// Function pointer for the ../../../../Jamoma/Core/DSP/library/build/JamomaDSP.dylib Loop (use this instead of branching for speed)
		typedef void (tt_crossfade::*FuncPtr)(tt_audio_signal *, tt_audio_signal *, tt_audio_signal *);
		FuncPtr dsp_executor;

		// Attribute Values & Variables
		tt_attribute_value		position;
		tt_attribute_value		shape;
		tt_attribute_value		mode;
			
		
	public:
		enum selectors{										// Attribute Selectors
			k_shape,
			k_mode,
			k_position,
			k_shape_equalpower,
			k_shape_linear,
			k_mode_lookup,
			k_mode_calculate
		};
		
		// OBJECT LIFE					
		tt_crossfade(void)									// Constructor		
		{
			position = 0.5;
			shape = k_shape_equalpower;
			set_attr(k_mode, k_mode_lookup);
		}

		~tt_crossfade(void)								// Destructor
		{
			;
		}


		// ATTRIBUTES
		void set_attr(tt_selector sel, tt_attribute_value val)	// Set Attributes
		{
			switch (sel){
				case k_position:
					position = clip(val, 0.0, 1.0);
					break;
				case k_shape:
					shape = val;
					break;
				case k_mode:
					mode = val;
					break;
			}
			// set the function pointer for the correct dsp loop to run
			if(shape == k_shape_linear)
				dsp_executor = &tt_crossfade::dsp_vector_calc_linear;
			else if(mode == k_mode_calculate)
				dsp_executor = &tt_crossfade::dsp_vector_calc_equalpower_calc;
			else if(mode == k_mode_lookup)
				dsp_executor = &tt_crossfade::dsp_vector_calc_equalpower_lookup;
			
		}

		tt_attribute_value get_attr(tt_selector sel)				// Get Attributes
		{
			switch (sel){
				case k_position:
					return position;
				case k_shape:
					return shape;
				case k_mode:
					return mode;
				default:
					return 0.0;
			}
		}
		
		
		/*****************************************************
		 * ../../../../Jamoma/Core/DSP/library/build/JamomaDSP.dylib LOOPS
		 *****************************************************/
		
		// Publically exposed interface for this object's dsp routine
		void dsp_vector_calc(tt_audio_signal *in1, tt_audio_signal *in2, tt_audio_signal *out)
		{
			(*this.*dsp_executor)(in1, in2, out);	// Run the function pointed to by our function pointer
		}
	
	private:
		// ../../../../Jamoma/Core/DSP/library/build/JamomaDSP.dylib LOOP: LINEAR SHAPE
		void dsp_vector_calc_linear(tt_audio_signal *in1, tt_audio_signal *in2, tt_audio_signal *out)
		{
			temp_vs = in1->vectorsize;
			while(temp_vs--)
				*out->vector++ = (*in2->vector++ * position) + (*in1->vector++ * (1.0 - position));
			in1->reset(); in2->reset(); out->reset();
		}

		// ../../../../Jamoma/Core/DSP/library/build/JamomaDSP.dylib LOOP: EQUAL POWER LOOKUP MODE
		void dsp_vector_calc_equalpower_lookup(tt_audio_signal *in1, tt_audio_signal *in2, tt_audio_signal *out)
		{
			int index;
			temp_vs = in1->vectorsize;
			while(temp_vs--){
				index = (int)(position * 511.0);
				*out->vector++ = (*in2->vector++ * lookup_equalpower[511 - index]) + (*in1->vector++ * lookup_equalpower[index]);
			}
			in1->reset(); in2->reset(); out->reset();
		}

		// ../../../../Jamoma/Core/DSP/library/build/JamomaDSP.dylib LOOP: EQUAL POWER CALCULATED
		void dsp_vector_calc_equalpower_calc(tt_audio_signal *in1, tt_audio_signal *in2, tt_audio_signal *out)
		{
			temp_vs = in1->vectorsize;
			while(temp_vs--)
				*out->vector++ = (*in2->vector++ * (sin(position * 1.5707963))) + (*in1->vector++ * (sin((1 - position) * 1.5707963)));
			in1->reset(); in2->reset(); out->reset();
		}
		
};

#endif // TT_CROSSFADE_H


