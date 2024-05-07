/*
 *******************************************************
 *		SWITCH (1 INPUT / N OUTPUTS)
 *******************************************************
 *		TapTools Blue Object
 *		copyright � 2003 by Timothy A. Place
 *
 */

// Check against redundant including
#ifndef TT_SWITCH_H
#define TT_SWITCH_H

// Include appropriate headers
#include "tt_audio_base.h"


/********************************************************
	CLASS INTERFACE/IMPLEMENTATION

	The entire class is implemented inline for speed.
 ********************************************************/

class tt_switch:public tt_audio_base{

	private:
		tt_attribute_value_discrete	position;			
		
	public:
		enum selectors{										// Attribute Selectors
			k_position,
		};
		
		// OBJECT LIFE					
		tt_switch(void)										// Constructor		
		{
			position = 1;
		}

		~tt_switch(void)										// Destructor
		{
			;
		}


		// ATTRIBUTES
		void set_attr(tt_selector sel, tt_attribute_value val)	// Set Attributes
		{
			switch (sel){
				case k_position:
					position = (tt_attribute_value_discrete)val;
					break;
			}
		}

		tt_attribute_value get_attr(tt_selector sel)				// Get Attributes
		{
			switch (sel){
				case k_position:
					return tt_attribute_value(position);
				default:
					return 0.0;
			}
		}
		
		
		// ../../../../Jamoma/Core/DSP/library/build/JamomaDSP.dylib LOOP: 1 OUTPUT
		void dsp_vector_calc(tt_audio_signal *in, tt_audio_signal *out1)
		{
			temp_vs = in->vectorsize;
			while(temp_vs--){
				switch(position){
					case 0:
						*out1->vector++ = 0.0;
						break;
					case 1:
						*out1->vector++ = *in->vector++;
						break;
				}
			}
			in->reset(); out1->reset();
		}

		// ../../../../Jamoma/Core/DSP/library/build/JamomaDSP.dylib LOOP: 2 OUTPUTS
		void dsp_vector_calc(tt_audio_signal *in, tt_audio_signal *out1, tt_audio_signal *out2)
		{
			temp_vs = in->vectorsize;
			while(temp_vs--){
				switch(position){
					case 0:
						*out1->vector++ = 0.0;
						*out2->vector++ = 0.0;
						break;
					case 1:
						*out1->vector++ = *in->vector++;
						*out2->vector++ = 0.0;
						break;
					case 2:
						*out1->vector++ = 0.0;
						*out2->vector++ = *in->vector++;
						break;
				}
			}
			in->reset(); out1->reset(); out2->reset();
		}
		
		// ../../../../Jamoma/Core/DSP/library/build/JamomaDSP.dylib LOOP: 3 OUTPUTS
		void dsp_vector_calc(tt_audio_signal *in, tt_audio_signal *out1, tt_audio_signal *out2, tt_audio_signal *out3)
		{
			temp_vs = in->vectorsize;
			while(temp_vs--){
				switch(position){
					case 0:
						*out1->vector++ = 0.0;
						*out2->vector++ = 0.0;
						*out3->vector++ = 0.0;
						break;
					case 1:
						*out1->vector++ = *in->vector++;
						*out2->vector++ = 0.0;
						*out3->vector++ = 0.0;
						break;
					case 2:
						*out1->vector++ = 0.0;
						*out2->vector++ = *in->vector++;
						*out3->vector++ = 0.0;
						break;
					case 3:
						*out1->vector++ = 0.0;
						*out2->vector++ = 0.0;
						*out3->vector++ = *in->vector++;
				}
			}
			in->reset(); out1->reset(); out2->reset(); out3->reset();
		}

		// ../../../../Jamoma/Core/DSP/library/build/JamomaDSP.dylib LOOP: 4 OUTPUTS
		void dsp_vector_calc(tt_audio_signal *in, tt_audio_signal *out1, tt_audio_signal *out2, tt_audio_signal *out3, tt_audio_signal *out4)
		{
			temp_vs = in->vectorsize;
			while(temp_vs--){
				switch(position){
					case 0:
						*out1->vector++ = 0.0;
						*out2->vector++ = 0.0;
						*out3->vector++ = 0.0;
						*out4->vector++ = 0.0;
						break;
					case 1:
						*out1->vector++ = *in->vector++;
						*out2->vector++ = 0.0;
						*out3->vector++ = 0.0;
						*out4->vector++ = 0.0;
						break;
					case 2:
						*out1->vector++ = 0.0;
						*out2->vector++ = *in->vector++;
						*out3->vector++ = 0.0;
						*out4->vector++ = 0.0;
						break;
					case 3:
						*out1->vector++ = 0.0;
						*out2->vector++ = 0.0;
						*out3->vector++ = *in->vector++;
						*out4->vector++ = 0.0;
					case 4:
						*out1->vector++ = 0.0;
						*out2->vector++ = 0.0;
						*out3->vector++ = 0.0;
						*out4->vector++ = *in->vector++;
				}
			}
			in->reset(); out1->reset(); out2->reset(); out3->reset(); out4->reset();
		}

		// ../../../../Jamoma/Core/DSP/library/build/JamomaDSP.dylib LOOP: 5 OUTPUTS
		void dsp_vector_calc(tt_audio_signal *in, tt_audio_signal *out1, tt_audio_signal *out2, tt_audio_signal *out3, tt_audio_signal *out4, tt_audio_signal *out5)
		{
			temp_vs = in->vectorsize;
			while(temp_vs--){
				switch(position){
					case 0:
						*out1->vector++ = 0.0;
						*out2->vector++ = 0.0;
						*out3->vector++ = 0.0;
						*out4->vector++ = 0.0;
						*out5->vector++ = 0.0;
						break;
					case 1:
						*out1->vector++ = *in->vector++;
						*out2->vector++ = 0.0;
						*out3->vector++ = 0.0;
						*out4->vector++ = 0.0;
						*out5->vector++ = 0.0;
						break;
					case 2:
						*out1->vector++ = 0.0;
						*out2->vector++ = *in->vector++;
						*out3->vector++ = 0.0;
						*out4->vector++ = 0.0;
						*out5->vector++ = 0.0;
						break;
					case 3:
						*out1->vector++ = 0.0;
						*out2->vector++ = 0.0;
						*out3->vector++ = *in->vector++;
						*out4->vector++ = 0.0;
						*out5->vector++ = 0.0;
					case 4:
						*out1->vector++ = 0.0;
						*out2->vector++ = 0.0;
						*out3->vector++ = 0.0;
						*out4->vector++ = *in->vector++;
						*out5->vector++ = 0.0;
					case 5:
						*out1->vector++ = 0.0;
						*out2->vector++ = 0.0;
						*out3->vector++ = 0.0;
						*out4->vector++ = 0.0;
						*out5->vector++ = *in->vector++;
				}
			}
			in->reset(); out1->reset(); out2->reset(); out3->reset(); out4->reset(); out5->reset();
		}
		
};

#endif // tt_SWITCH_H


