/*
 *******************************************************
 *		RUNNING AVERAGER
 *******************************************************
 *		TapTools Blue Object
 *		copyright � 2003 by Timothy A. Place
 *
 */

// Check against redundant including
#ifndef TT_AVERAGE_H
#define TT_AVERAGE_H

// Include appropriate headers
#include "tt_audio_base.h"


/********************************************************
	CLASS INTERFACE/IMPLEMENTATION

	The entire class is implemented inline for speed.
 ********************************************************/

class tt_average:public tt_audio_base{

	private:
		// Function pointer for the ../../../../Jamoma/Core/DSP/library/build/JamomaDSP.dylib Loop (use this instead of branching for speed)
		typedef void (tt_average::*FuncPtr)(tt_audio_signal *, tt_audio_signal *);
		FuncPtr dsp_executor;
		
		// Attribute Values & Variables
		tt_attribute_value_discrete		interval;
		tt_attribute_value_discrete		mode;
		double 							accumulator, intervalReciprocal;
		tt_sample_vector				bins, bins_in, bins_out, bins_end;
	
		// Constants
		enum constants{
			MAX_AVERAGE_INTERVAL = 256
		};
	
	
	public:
		enum selectors{								
			k_mode,							// Attribute Selectors
			k_mode_bipolar,
			k_mode_absolute,
			k_mode_rms,
			k_interval,		
		};
		

		// OBJECT LIFE					
		tt_average()								// Constructor		
		{
			bins = bins_in = bins_out = bins_end = 0;
			bins = (tt_sample_value *)mem_alloc((1 + MAX_AVERAGE_INTERVAL) * sizeof(tt_sample_value));
			set_attr(k_interval, 100);
			set_attr(k_mode, k_mode_absolute);
			clear();
		}

		~tt_average()								// Destructor
		{
			mem_free(bins);
		}


		// ATTRIBUTES
		void set_attr(tt_selector sel, tt_attribute_value val)	// Set Attributes
		{
			switch (sel){
				case k_interval:
					val = clip(int(val), 1, int(MAX_AVERAGE_INTERVAL));
					interval = (tt_attribute_value_discrete)val;
					clear();
					break;
				case k_mode:
					mode = (tt_attribute_value_discrete)val;
					if(mode == k_mode_bipolar)
						dsp_executor = &tt_average::dsp_vector_calc_bipolar;
					else if(mode == k_mode_absolute)
						dsp_executor = &tt_average::dsp_vector_calc_absolute;
					else if(mode == k_mode_rms)
						dsp_executor = &tt_average::dsp_vector_calc_rms;
					break;
			}
		}

		tt_attribute_value get_attr(tt_selector sel)				// Get Attributes
		{
			switch (sel){
				case k_interval:
					return interval;
				case k_mode:
					return mode;
				default:
					return 0.0;
			}
		}
		
		
		// clear
		void clear()
		{
			int i;
			
			bins_in = bins;
			for (i = 0; i < MAX_AVERAGE_INTERVAL; i++)
				*(bins_in+i) = 0.0;
			
			accumulator = 0.0;
			intervalReciprocal = 1.0 / interval;
			bins_end = bins + interval;
			bins_out = bins_in - interval;
			if (bins_out < bins)
				bins_out = bins_end + (bins_out - bins) + 1;
		}


		/*****************************************************
		 * ../../../../Jamoma/Core/DSP/library/build/JamomaDSP.dylib LOOPS
		 *****************************************************/
		
		// Publically exposed interface for this object's dsp routine
		void dsp_vector_calc(tt_audio_signal *in, tt_audio_signal *out)
		{
			(*this.*dsp_executor)(in, out);	// Run the function pointed to by our function pointer
		}
	
	private:
		// ../../../../Jamoma/Core/DSP/library/build/JamomaDSP.dylib LOOP: BIPOLAR
		void dsp_vector_calc_bipolar(tt_audio_signal *in, tt_audio_signal *out)
		{
			temp_vs = in->vectorsize;
			while(temp_vs--){
				if (bins_out > bins_end)
					bins_out = bins;
				if (bins_in > bins_end)
					bins_in = bins;
				accumulator -= *bins_out++;
				
				accumulator += *bins_in++ = *in->vector++;
				*out->vector++ = accumulator * intervalReciprocal;
			}
			in->reset(); out->reset();
		}

		// ../../../../Jamoma/Core/DSP/library/build/JamomaDSP.dylib LOOP: ABSOLUTE
		void dsp_vector_calc_absolute(tt_audio_signal *in, tt_audio_signal *out)
		{
			temp_vs = in->vectorsize;
			while(temp_vs--){
				if (bins_out > bins_end)
					bins_out = bins;
				if (bins_in > bins_end)
					bins_in = bins;
				accumulator -= *bins_out++;
				
				accumulator += *bins_in++ = fabs(*in->vector++);
				*out->vector++ = accumulator * intervalReciprocal;
			}
			in->reset(); out->reset();
		}

		// ../../../../Jamoma/Core/DSP/library/build/JamomaDSP.dylib LOOP: RMS
		void dsp_vector_calc_rms(tt_audio_signal *in, tt_audio_signal *out)
		{
			tt_sample_value value;
			temp_vs = in->vectorsize;
			
			while(temp_vs--){
				if (bins_out > bins_end)
					bins_out = bins;
				if (bins_in > bins_end)
					bins_in = bins;
				accumulator -= *bins_out++;
				
				value = *in->vector++;
				accumulator += *bins_in++ = value * value;
				*out->vector++ = sqrt(accumulator * intervalReciprocal);
			}
			in->reset(); out->reset();
		}
};

#endif	// TT_AVERAGE_H
