
#ifndef _RANGE_VALUE_H_
#define _RANGE_VALUE_H_

#include <algorithm>
#include "core/math/Vector3.h"

#ifdef max
	#undef max
#endif
#ifdef min
	#undef min
#endif

template <typename valtype>
class RangeValue
{
public:
	// == Constructors ==
	RangeValue ( void )
	{
		SetRange( valtype(), valtype() );
		SetValue( valtype() );
	}
	RangeValue ( const valtype& lowVal, const valtype& highVal, const valtype& curVal )
	{
		SetRange( lowVal, highVal );
		SetValue( curVal );
	};
	RangeValue ( const valtype& lowVal, const valtype& highVal )
	{
		SetRange( lowVal, highVal );
		SetValue( lowVal );
	};

	// == Public Setters ==
	void SetRange ( const valtype& lowVal, const valtype& highVal )
	{
		if ( lowVal <= highVal )
		{
			mMinVal = lowVal;
			mMaxVal = highVal;
		}
		else
		{
			mMinVal = highVal;
			mMaxVal = lowVal;
		}
	};
	void SetValue ( const valtype& curVal )
	{
		mCurVal = std::max( mMinVal, curVal );
		mCurVal = std::min( mMaxVal, mCurVal );
	};

	// == Public Getters ==
	valtype GetRandom ( void )
	{
		return ( mMinVal + (mMaxVal-mMinVal)*(rand()/((Real)RAND_MAX)) );
	};
	valtype GetNext ( void )
	{
		SetValue( mCurVal + ((mMinVal-mMaxVal)*0.5f + (mMaxVal-mMinVal)*(rand()/((Real)RAND_MAX)) )*0.2f );
		return mCurVal;
	}

	// == Type Data ==
	valtype	mMinVal;
	valtype	mMaxVal;
	valtype	mCurVal;
};



inline void RangeValue<Vector3f>::SetRange ( const Vector3f& lowVal, const Vector3f& highVal )
{
	/*if ( lowVal.x <= highVal.x )
	{
		mMinVal.x = lowVal.x;
		mMaxVal.x = highVal.x;
	}
	else
	{
		mMinVal.x = highVal.x;
		mMaxVal.x = lowVal.x;
	}
	if ( lowVal.y <= highVal.y )
	{
		mMinVal.y = lowVal.y;
		mMaxVal.y = highVal.y;
	}
	else
	{
		mMinVal.y = highVal.y;
		mMaxVal.y = lowVal.y;
	}
	if ( lowVal.z <= highVal.z )
	{
		mMinVal.z = lowVal.z;
		mMaxVal.z = highVal.z;
	}
	else
	{
		mMinVal.z = highVal.z;
		mMaxVal.z = lowVal.z;
	}*/
	mMinVal = lowVal;
	mMaxVal = highVal;
};
inline void RangeValue<Vector3f>::SetValue ( const Vector3f& curVal )
{
	mCurVal.x = std::max( mMinVal.x, curVal.x );
	mCurVal.x = std::min( mMaxVal.x, mCurVal.x );
	mCurVal.y = std::max( mMinVal.y, curVal.y );
	mCurVal.y = std::min( mMaxVal.y, mCurVal.y );
	mCurVal.z = std::max( mMinVal.z, curVal.z );
	mCurVal.z = std::min( mMaxVal.z, mCurVal.z );
};
inline Vector3f RangeValue<Vector3f>::GetRandom ( void )
{
	Vector3f result;
	result.x = ( mMinVal.x + (mMaxVal.x-mMinVal.x)*(rand()/((Real)RAND_MAX)) );
	result.y = ( mMinVal.y + (mMaxVal.y-mMinVal.y)*(rand()/((Real)RAND_MAX)) );
	result.z = ( mMinVal.z + (mMaxVal.z-mMinVal.z)*(rand()/((Real)RAND_MAX)) );
	return result;
};
inline Vector3f RangeValue<Vector3f>::GetNext ( void )
{
	SetValue( Vector3f(
		mCurVal.x + ((mMinVal.x-mMaxVal.x)*0.5f + (mMaxVal.x-mMinVal.x)*(rand()/((Real)RAND_MAX)) )*0.2f,
		mCurVal.y + ((mMinVal.y-mMaxVal.y)*0.5f + (mMaxVal.y-mMinVal.y)*(rand()/((Real)RAND_MAX)) )*0.2f,
		mCurVal.z + ((mMinVal.z-mMaxVal.z)*0.5f + (mMaxVal.z-mMinVal.z)*(rand()/((Real)RAND_MAX)) )*0.2f )
		);
	return mCurVal;
}

#endif