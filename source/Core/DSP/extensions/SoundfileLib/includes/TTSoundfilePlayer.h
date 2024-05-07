/** @file
 *
 * @ingroup dspSoundFileLib
 *
 * @brief Jamoma DSP Soundfile Player
 *
 * @details
 *
 * @authors Timothy Place
 *
 * @copyright Copyright © 2010 by Timothy Place @n
 * This code is licensed under the terms of the "New BSD License" @n
 * http://creativecommons.org/licenses/BSD/
 */

#ifndef __TT_SOUNDFILEPLAYER_H__
#define __TT_SOUNDFILEPLAYER_H__

#include "TTDSP.h"

#ifdef uint
#undef uint
#endif
#include "../libsndfile/sndfile.h"


/**	Enables user to play a soundfile from disk */
class TTSoundfilePlayer : public TTAudioObjectBase {
	TTCLASS_SETUP(TTSoundfilePlayer)

protected:

	TTSymbol			mFilePath;			///< full POSIX path to the file, including file name
	TTSymbol			mTitle, mAnnotation, mArtist, mDate;
	SNDFILE*			mSoundFile;			///< libsndfile handle for the actual file we open
	SF_INFO				mSoundFileInfo;		///< libsndfile metadata for the file we open
	sf_count_t			mSeekInFrames;
	TTBoolean			mPlay;				///< is actively playing back the file?
	TTBoolean			mLoop;				///< Loop flag
	TTFloat64			mSeek;			    ///< Cue time start
	TTFloat64			mDuration;			///< Length of the loaded soundfile - readonly
	TTBoolean			mContinue;			///< Pause/Resume flag
	TTUInt16			mNumChannels;		///< read-only: number of channels in the open file
	TTUInt16			mNumBufferFrames;	///< number of frames in the buffer to be read from the file at a time
	TTSampleVector		mBuffer;			///< buffer of mNumBufferFrames * mNumChannels;
	
	

	/**	Setter for the mode attribute.
	 	This also sets the audio processing method.  */
	TTErr setFilePath(const TTValue& value);
	TTErr setPlay(const TTValue& value);
	TTErr setSeek(const TTValue& value);
	TTErr pause();
	TTErr resume();
	
	// Block-based Audio Processing Methods
	TTErr processAudio(TTAudioSignalArrayPtr inputs, TTAudioSignalArrayPtr outputs);
};


#endif // __TT_SOUNDFILEPLAYER_H__
