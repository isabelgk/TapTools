/** @file
 *
 * @ingroup dspSpatLib
 *
 * @brief The #TTSpatDBAPSource class provides an extended description of a source as compared to the #TTSpatSource class that it inherits from.
 *
 * @details It is extended to describe source width.
 *
 * @authors Trond Lossius, Timothy Place, Nils Peters,
 *
 * @copyright Copyright © 2011 by Trond Lossius, Nils Peters, and Timothy Place @n
 * This code is licensed under the terms of the "New BSD License" @n
 * http://creativecommons.org/licenses/BSD/
 */


#include "TTSpatDBAPSource.h"

TTSpatDBAPSource::TTSpatDBAPSource()
{
	;
}


TTSpatDBAPSource::~TTSpatDBAPSource()
{
	
}


void TTSpatDBAPSource::getWidth(TTFloat64& aWidth)
{
	aWidth = mData[0];
}


void TTSpatDBAPSource::setWidth(TTFloat64 aWidth)
{
	mData[0] = aWidth;
}

