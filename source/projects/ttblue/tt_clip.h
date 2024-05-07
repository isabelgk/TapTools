/*
 *******************************************************
 *		HARD CLIP AN AUDIO SIGNAL
 *******************************************************
 *		TapTools Blue Object
 *		copyright � 2003 by Timothy A. Place
 *
 */

// Check against redundant including
#ifndef TT_CLIP_H
#define TT_CLIP_H

// Include appropriate headers
#include "tt_audio_base.h"


/********************************************************
	CLASS INTERFACE/IMPLEMENTATION

	The entire class is implemented inline for speed.
 ********************************************************/

class tt_clip:public tt_audio_base{

	private:
		tt_attribute_value		clip_high_value;
		tt_attribute_value		clip_low_value;	
		
	public:
		enum selectors{										// Attribute Selectors
			k_clip,
			k_clip_high,
			k_clip_low
		};
		
		// OBJECT LIFE					
		tt_clip(void)										// Constructor		
		{
			set_attr(k_clip_high, 1.0);
			set_attr(k_clip_low, -1.0);
		}

		~tt_clip(void)										// Destructor
		{
			;
		}


		// ATTRIBUTES
		void set_attr(tt_selector sel, tt_attribute_value val)	// Set Attributes
		{
			switch (sel){
				case k_clip:
					clip_high_value = decibels_to_amplitude(val);
					clip_low_value = clip_high_value * -1.0;
					break;
				case k_clip_high:
					clip_high_value = val;
					break;
				case k_clip_low:
					clip_low_value = val;
					break;
			}
		}

		tt_attribute_value get_attr(tt_selector sel)				// Get Attributes
		{
			switch (sel){
				case k_clip:		// fall through
				case k_clip_high:
					return clip_high_value;
					break;
				case k_clip_low:
					return clip_low_value;
			}
			return 0.0;
		}
				
		
		// ../../../../Jamoma/Core/DSP/library/build/JamomaDSP.dylib LOOP
		void dsp_vector_calc(tt_audio_signal *in, tt_audio_signal *out)
		{
			temp_vs = in->vectorsize;		
			while(temp_vs--)
				*out->vector++ = clip(*in->vector++, clip_low_value, clip_high_value);
			in->reset(); out->reset();
		}
		
		// ../../../../Jamoma/Core/DSP/library/build/JamomaDSP.dylib LOOP
		void dsp_vector_calc(tt_audio_signal *in1, tt_audio_signal *in2,
			tt_audio_signal *out1, tt_audio_signal *out2)
		{
			temp_vs = in1->vectorsize;		
			while(temp_vs--){
				*out1->vector++ = clip(*in1->vector++, clip_low_value, clip_high_value);
				*out2->vector++ = clip(*in2->vector++, clip_low_value, clip_high_value);
			}
			in1->reset(); in2->reset(); out1->reset(); out2->reset();
		}
		
};

#endif // TT_CLIP_H


