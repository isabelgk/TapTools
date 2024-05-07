/*
 *******************************************************
 *		VECTOR SUBTRACTION
 *******************************************************
 *		TapTools Blue Object
 *		copyright � 2003 by Timothy A. Place
 *
 */

// Check against redundant including
#ifndef TT_SUBTRACT_H
#define TT_SUBTRACT_H

// Include appropriate headers
#include "tt_audio_base.h"


/********************************************************
	CLASS INTERFACE/IMPLEMENTATION

	The entire class is implemented inline for speed.
 ********************************************************/

class tt_subtract:public tt_audio_base{

	private:
		
	public:
		tt_subtract(void)	// Constructor		
		{
			;
		}

		~tt_subtract(void)		// Destructor
		{
			;
		}
		
		// ../../../../Jamoma/Core/DSP/library/build/JamomaDSP.dylib LOOP
		void dsp_vector_calc(tt_audio_signal *in1, tt_audio_signal *in2, tt_audio_signal *out)
		{
			temp_vs = in1->vectorsize;
			while(temp_vs--)
				*out->vector++ = (*in1->vector++) - (*in2->vector++);
			in1->reset(); in2->reset(); out->reset();
		}
};

#endif // TT_SUBTRACT_H


