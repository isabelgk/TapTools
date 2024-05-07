/** @file
 *
 * @ingroup dspSpatLib
 *
 * @brief #TTSpatEntity describes the geometry of a point, and #TTSpatSource and #TTSpatSink specialise this further for the description of sources and sinks.
 *
 * @details
 *
 * @authors Trond Lossius, Timothy Place, Nils Peters, 
 *
 * @copyright Copyright © 2011 by Trond Lossius, Nils Peters, and Timothy Place @n
 * This code is licensed under the terms of the "New BSD License" @n
 * http://creativecommons.org/licenses/BSD/
 */

#ifndef __SPATSNAPENTITY_H__
#define __SPATSNAPENTITY_H__

#include "TTDSP.h"
#include <array>

/** The TTSpatEntity class describes the geometry of a point.
 It can be used to describe a source or a sink, and includes the minimum
 amount of descriptors as defined in the core section of the SpatDIF specs.
 */
class TTSpatEntity {
protected:

	std::array<TTFloat64, 3> mPosition;		///< Position as Cartesian coordinates
	std::array<TTFloat64, 4> mOrientation;	///< The orientation of the object using Euler coordinates
	std::array<TTFloat64, 4> mData;			///< Storage for up to 4 values that can be used by subclasses
		
public:
	
	/** Constructor.
	 */
	TTSpatEntity();
	
	
	/** Destructor.
	 */
	virtual ~TTSpatEntity();
	
	
	/** Get the current position of the point.
	 @param x				Cartesian x-coordinate of the point.
	 @param y				Cartesian y-coordinate of the point.
	 @param z				Cartesian z-coordinate of the point.
	 */
	void getPosition(TTFloat64& x, TTFloat64& y, TTFloat64& z);
	
	
	/** Set the position of the point.
	 @param x				Cartesian x-coordinate of the point.
	 @param y				Cartesian y-coordinate of the point.
	 @param z				Cartesian z-coordinate of the point.
	 */
	void setPosition(TTFloat64 x, TTFloat64 y, TTFloat64 z);
	
	
	/** Get the orientation of the point expressed as quaternions.
	 @param q1				First coordinate of the point.
	 @param q2				Second coordinate of the point.
	 @param q3				Third coordinate of the point.
	 @param q4				Fourth coordinate of the point.
	 */
	void getOrientation(TTFloat64& q1, TTFloat64& q2, TTFloat64& q3, TTFloat64& q4);
	
	
	/** Set the orientation of the point expressed as quaternions.
	 @param q1				First coordinate of the point.
	 @param q2				Second coordinate of the point.
	 @param q3				Third coordinate of the point.
	 @param q4				Fourth coordinate of the point.
	 */
	void setOrientation(TTFloat64 q1, TTFloat64 q2, TTFloat64 q3, TTFloat64 q4);



};


/** The #TTSpatSource class extends and specialise the #TTSpatEntity class to provide attributes and methods suitable for the description of a source.
 @details		Actually #TTSpatSource provides no specialization at this time
 */
class TTSpatSource : public TTSpatEntity {
	// no specialization at this time
};


/** The #TTSpatSink class extends and specialise the #TTSpatEntity class to provide attributes and methods suitable for the description of a sink.
 @details		Actually #TTSpatSink provides no specialization at this time
 */
class TTSpatSink : public TTSpatEntity {
	// no specialization at this time
};


/** Pointer to a vector of #TTSpatEntity points.
 @ingroup typedefs
 */
typedef std::vector<TTSpatEntity>	TTSpatEntityVector;


/** Pointer to a vector of #TTSpatSource points.
 @ingroup typedefs
 */
typedef std::vector<TTSpatSource>	TTSpatSourceVector;


/** Pointer to a vector of #TTSpatSink points.
 @ingroup typedefs
 */
typedef std::vector<TTSpatSink>		TTSpatSinkVector;



#endif // __SPATSNAPENTITY_H__
