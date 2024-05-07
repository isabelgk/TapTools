/** @file
 *
 * @ingroup dspTrajectoryLib
 *
 * @brief Lissajous Knot Function Unit in 3D for Jamoma DSP
 *
 * @details see http://en.wikipedia.org/wiki/Lissajous_knot for details @n
 * 
 * @authors Nils Peters
 *
 * @copyright Copyright © 2011 by Nils Peters @n
 * This code is licensed under the terms of the "New BSD License" @n
 * http://creativecommons.org/licenses/BSD/
 */

#ifndef __LISSAJOUS_3D_H__
#define __LISSAJOUS_3D_H__

#include "TTDSP.h"


class Lissajous3D : TTAudioObjectBase {
	TTCLASS_SETUP(Lissajous3D)

protected:

	TTFloat64			mA, mB, mC, mDeltaX, mDeltaY;
		
	/**	A standard audio processing method as used by Jamoma DSP objects.
	 @param inputs				The input vector that is to be processed.
	 @param outputs				The resulting windowed vector.
	 @return					#TTErr error code if the method fails to execute, else #kTTErrNone.
	 */			
	TTErr processAudio(TTAudioSignalArrayPtr inputs, TTAudioSignalArrayPtr outputs);
};


#endif // __LISSAJOUS_3D_H__
