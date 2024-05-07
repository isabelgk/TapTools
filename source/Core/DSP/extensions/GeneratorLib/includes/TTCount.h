/** @file
 *
 * @ingroup dspGeneratorLib
 *
 * @brief GeneratorLib: Count the number of processed samples, starting from zero.
 *
 * @details
 *
 * @authors Trond Lossius
 *
 * @copyright Copyright © 2012 by Trond Lossius @n
 * This code is licensed under the terms of the "New BSD License" @n
 * http://creativecommons.org/licenses/BSD/
 */


#ifndef __TT_COUNT_H__
#define __TT_COUNT_H__

#include "TTDSP.h"


/**	TTCount counts the number of samples processed so far.
 */ 
class TTCount : TTAudioObjectBase {
	TTCLASS_SETUP(TTCount)

protected:

	TTFloat64 			count;			///< Sample count

	
	/**	Process method
	 @param inputs			Signal input vectors to process
	 @param outputs			Processed audio
	 @return				#TTErr error code if the method fails to execute, else #kTTErrNone.
	 */
	TTErr processAudio(TTAudioSignalArrayPtr inputs, TTAudioSignalArrayPtr outputs);
	
	
	/**	Start counting from zero again.
	 @return				#TTErr error code if the method fails to execute, else #kTTErrNone.
	 */
	TTErr reset();
	
	
	/**	Unit Tests
	 @return				#TTErr error code if the method fails to execute, else #kTTErrNone.
	 */
	//virtual TTErr test(TTValue& returnedTestInfo);
	
};


#endif // __TT_COUNT_H__
