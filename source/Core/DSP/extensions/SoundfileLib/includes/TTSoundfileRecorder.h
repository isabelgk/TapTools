/** @file
 *
 * @ingroup dspSoundFileLib
 *
 * @brief Jamoma DSP Soundfile Recorder
 *
 * @details
 *
 * @authors Timothy Place
 *
 * @copyright Copyright © 2010 by Timothy Place @n
 * This code is licensed under the terms of the "New BSD License" @n
 * http://creativecommons.org/licenses/BSD/
 */

#ifndef __TT_SOUNDFILERECORDER_H__
#define __TT_SOUNDFILERECORDER_H__

#include "TTDSP.h"

#ifdef uint
#undef uint
#endif
#include "../libsndfile/sndfile.h"


/**	Enables user to record a soundfile to disk */
class TTSoundfileRecorder : public TTAudioObjectBase {
	TTCLASS_SETUP(TTSoundfileRecorder)

protected:

	TTSymbol			mFilePath;			///< full POSIX path to the file, including file name
	TTSymbol			mFormat;			///< format of the file, e.g. "WAV", "AIFF", "FLAC", "FLAC-16bit", etc.
	SNDFILE*			mSoundFile;			///< libsndfile handle for the actual file we open
	SF_INFO				mSoundFileInfo;		///< libsndfile metadata for the file we open
	TTBoolean			mRecord;			///< is actively recording the file?
	TTUInt16			mNumChannels;		///< read-only: number of channels in the open file
	TTUInt16			mNumBufferFrames;	///< number of frames in the buffer to be read from the file at a time
	TTFloat64			mLength;			///< length of the file in ms
	TTInt32				mLengthInSamples;	///< length of the file in samples
	TTUInt16			mCycles;
	TTSampleVector		mBuffer;			///< buffer of mNumBufferFrames * mNumChannels;
	sf_count_t			numSamplesWritten;
	/**	Setter */
	TTErr setRecord(const TTValue& value);
	TTErr setLength(const TTValue& value);
	TTErr setFilePath(const TTValue& value);
	
	// internal use: map human symbols to libsndfile's bitmask
	int translateFormatFromName(TTSymbol& name);
	
	TTErr updateSampleRate(const TTValue& oldSampleRate);

	// internal use: opens the file for recording
	TTErr openFile();
	
	// Block-based Audio Processing Methods
	TTErr processAudioRecording(TTAudioSignalArrayPtr inputs, TTAudioSignalArrayPtr outputs);
	TTErr processTimedAudioRecording(TTAudioSignalArrayPtr inputs, TTAudioSignalArrayPtr outputs);
	TTErr processAudioBypass(TTAudioSignalArrayPtr inputs, TTAudioSignalArrayPtr outputs);

};


#endif // __TT_SOUNDFILERECORDER_H__
