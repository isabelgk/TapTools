/** @file
 *
 * @ingroup dspFilterLib
 *
 * @brief #TTLowpassOnePole is a 1-Pole Lowpass Filter Object.
 *
 * @details
 *
 * @authors Timothy Place, Trond Lossius
 *
 * @copyright Copyright © 2008, Timothy Place @n
 * This code is licensed under the terms of the "New BSD License" @n
 * http://creativecommons.org/licenses/BSD/
 */


#ifndef __TT_LOWPASS_ONEPOLE_H__
#define __TT_LOWPASS_ONEPOLE_H__

#include "TTOnePole.h"


/**	The simplest of lowpass filters: a single-pole, no-zero algorithm. */
class TTLowpassOnePole : public TTOnePole {
	TTCLASS_SETUP(TTLowpassOnePole)

protected:
	
	/**	Unit Tests
	 @param	returnedTestInfo		Used to return test information
	 @return						#TTErr error code if the method fails to execute, else #kTTErrNone.
	 */
	virtual TTErr test(TTValue& returnedTestInfo);
	
};


#endif // __TT_LOWPASS_ONEPOLE_H__
