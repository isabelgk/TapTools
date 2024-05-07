/*
 *******************************************************
 *		2ND ORDER BUTTERWORTH LOWPASS FILTER
 *		Butterworth filters have maximum flat frequency responce in the pass band.
 *
 *		Based on an algorithm from Dodge & Jerse (1997): Computer Music -
 * 		Synthesis, Composition, and Performance. 2nd edition. Schirmer.
 *
 *		TapTools Blue Object
 *		copyright � 2007 by Trond Lossius
 *
 *******************************************************
 */

// Check against redundant including
#ifndef TT_LOWPASS_BUTTERWORTH_H
#define TT_LOWPASS_BUTTERWORTH_H

// Include appropriate headers
#include "tt_audio_base.h"

static const double kSquareRoot2 = sqrt(2);


/********************************************************
	CLASS INTERFACE/IMPLEMENTATION

	The entire class is implemented inline for speed.
 ********************************************************/

class tt_lowpass_butterworth:public tt_audio_base{

	private:
		tt_attribute_value		frequency;						// filter cutoff frequency
		double					c, a0, a1, a2, b1, b2;			// filter coefficients
		double					xm1, xm2, ym1, ym2;				// previous input and output samples
		double 					f, fb;
	
	
	public:
		enum selectors{															
			k_frequency											// Attribute Selectors
		};
		

		// OBJECT LIFE					
		tt_lowpass_butterworth()								// Constructor		
		{
			set_attr(k_frequency, 4000.0);	
			clear();
		}

		~tt_lowpass_butterworth()								// Destructor
		{
			;
		}


		// ATTRIBUTES
		void set_attr(tt_selector sel, tt_attribute_value val)	// Set Attributes
		{
			switch (sel){			
				case k_frequency:
					frequency = val;
					// calculations
					c = 1 / ( tan( PI*(frequency/sr) ) );
					a0 = 1 / (1 + kSquareRoot2*c + c*c);
					a1 = 2*a0;
					a2 = a0;
					b1 = 2*a0*( 1 - c*c );
					b2 = a0 * (1 - kSquareRoot2*c + c*c);					
					break;
			}
		}

		tt_attribute_value get_attr(tt_selector sel)				// Get Attributes
		{
			switch (sel){
				case k_frequency:
					return frequency;
				default:
					return 0.0;
			}
		}
		
		
		// ../../../../Jamoma/Core/DSP/library/build/JamomaDSP.dylib LOOP
		void dsp_vector_calc(tt_audio_signal *input, tt_audio_signal *output)
		{
			tt_sample_value tempx;
			tt_sample_value tempy;

			temp_vs = input->vectorsize;
			
			while(temp_vs--){
				tempx = *input->vector++;
				tempy = anti_denormal(a0*tempx + a1*xm1 + a2*xm2 - b1*ym1 - b2*ym2);
				xm2 = xm1;
				xm1 = tempx;
				ym2 = ym1;
				ym1 = tempy;
				*output->vector++ = tempy;
			}
			input->reset(); output->reset();
		}


		// clear
		void clear()
		{		
			xm1 = 0;
			xm2 = 0;
			ym1 = 0;
			ym2 = 0;
		}
		
};


#endif	// TT_LOWPASS_BUTTERWORTH_H

