#include "CMatrix.h"
#include "string.h"
#include <algorithm>

FORCE_INLINE Matrix2x2::Matrix2x2 ( void )
{
	pData[0] = 1.0F;
	pData[1] = 0.0F;
	pData[2] = 0.0F;
	pData[3] = 1.0F;
}
FORCE_INLINE Matrix2x2::Matrix2x2 ( const Matrix2x2& nSrc )
{
	std::copy(nSrc.pData, nSrc.pData + 4, pData);
}
FORCE_INLINE Matrix2x2::Matrix2x2 ( const Real* nData )
{
	std::copy(nData, nData + 4, pData);
}
FORCE_INLINE Matrix2x2::Matrix2x2 ( const Vector2f& row_x, const Vector2f& row_y )
{
	pData[0] = row_x[0];
	pData[1] = row_x[1];

	pData[2] = row_y[0];
	pData[3] = row_y[1];
}

// Settin the translation
FORCE_INLINE bool Matrix2x2::setTranslation ( const Real x, const Real y )
{
	pData[1] = x;
	pData[3] = y;
	return true;
}
// Settin the translation via Vector2D
FORCE_INLINE bool Matrix2x2::setTranslation ( const Vector2f& v )
{
	pData[1] = v.x;
	pData[3] = v.y;
	return true;
}
// Settin the scaling
FORCE_INLINE bool Matrix2x2::setScale ( const Real x, const Real y )
{
	pData[0] = x;
	pData[3] = y;
	return true;
}
// Settin the scaling via Vector3D
FORCE_INLINE bool Matrix2x2::setScale ( const Vector2f& vect )
{
	pData[0] = vect.x;
	pData[3] = vect.y;
	return true;
}

// Settin the rotation of a 2D matrix
FORCE_INLINE bool Matrix2x2::setRotation ( const Real angle )
{
	/*Matrix2x2 rotationResult;
	Real trueAngle = angle * 3.14159265f / 180.0f;
	rotationResult.pData[0] = cos( trueAngle );
	rotationResult.pData[3] = rotationResult.pData[0];
	rotationResult.pData[2] = sin( trueAngle );
	rotationResult.pData[2] = -rotationResult.pData[1];

	(*this) *= rotationResult;*/
	Real trueAngle = angle * 3.14159265f / 180.0f;
	pData[0] = cos( trueAngle );
	pData[3] = pData[0];
	pData[1] = sin( trueAngle );
	pData[2] = -pData[1];

	return true;
}

// Getters - Translation
FORCE_INLINE Vector2f Matrix2x2::getTranslation ( void ) const
{
	Vector2f position;

	position.x = pData[0];
	position.y = pData[3];

	return position;
}

// Modders - Translation
FORCE_INLINE void Matrix2x2::translate ( Vector2f const& v )
{
	pData[0] += v.x;
	pData[3] += v.y;
}

//---Function definition
// Matrix transposition
FORCE_INLINE Matrix2x2 Matrix2x2::transpose ( void ) const
{
	return !(*this);
}


// Matrix inverse 2x2
FORCE_INLINE Matrix2x2 Matrix2x2::inverse ( void ) const
{
	Real detVal = determinant();
	if ( fabs( detVal ) <= 0.0005f )
	{
		return Matrix2x2();
	}
	else
	{
		Matrix2x2 mresult;
		mresult.pData[0] = pData[3] / detVal;
		mresult.pData[3] = pData[0] / detVal;
		mresult.pData[1] = -pData[2] / detVal;
		mresult.pData[2] = -pData[1] / detVal;

		return mresult;
	}
}

// Matrix determinant
FORCE_INLINE Real Matrix2x2::determinant ( void ) const
{
	return ( (pData[0]*pData[3]) - (pData[1]*pData[2]) );
}

// Linear Interpolation
FORCE_INLINE void Matrix2x2::Lerp ( const Matrix2x2& right, const Real t )
{
	int x;
	for ( x = 0; x < 4; x += 1 )
	{
		pData[x] += ( right.pData[x] - pData[x] )*t;
	}
}
FORCE_INLINE Matrix2x2 Matrix2x2::LerpTo ( const Matrix2x2& right, const Real t ) const
{
	Matrix2x2 result;
	int x;
	for ( x = 0; x < 4; x += 1 )
	{
		result.pData[x] = pData[x] + ( right.pData[x] - pData[x] )*t;
	}
	return result;
}


//---Function Operators
// Multiplication
FORCE_INLINE Matrix2x2 Matrix2x2::operator* ( const Matrix2x2& right ) const
{
	Matrix2x2 result;
	int x, y, c;
	for ( x = 0; x < 2; x += 1 )
	{
		for ( y = 0; y < 2; y += 1 )
		{
			result[y][x] = 0.0;
			for ( c = 0; c < 2; c += 1 )
			{
				result[y][x] += ((*this)[y][c]) * (right[c][x]);
			}
		}
	}
	for ( x = 0; x < 2; x += 1 )
	{
		for ( y = 0; y < 2; y += 1 )
		{
			/*if ( result[y][x] != result[y][x] )
				cout << "NAN!" << endl;
			else if (result[y][x] <= FLT_MAX && result[y][x] >= -FLT_MAX)
				cout << "INF!" << endl;*/
			assert( result[y][x] == result[y][x] );
			assert( result[y][x] <= FLT_MAX && result[y][x] >= -FLT_MAX );
		}
	}

	return result;
}
FORCE_INLINE Matrix2x2 Matrix2x2::operator*=( const Matrix2x2& right )
{
	(*this) = (*this) * right;
	return (*this);
}

FORCE_INLINE Vector2f Matrix2x2::operator* ( const Vector2f& vect ) const
{
	return Vector2f(
		pData[0]*vect.x+pData[1]*vect.y,
		pData[2]*vect.x+pData[3]*vect.y
		);
}


FORCE_INLINE Matrix2x2 Matrix2x2::operator* ( const Real right ) const
{
	Matrix2x2 result;
	for ( int x = 0; x < 4; x++ )
		result.pData[x] = pData[x] * right;
	return result;
}


// Add matrices
FORCE_INLINE Matrix2x2 Matrix2x2::operator+ ( const Matrix2x2& right ) const
{
	Matrix2x2 result;
	for ( int x = 0; x < 4; x++ )
		result.pData[x] = pData[x] + right.pData[x];
	return result;
}
FORCE_INLINE Matrix2x2 Matrix2x2::operator+= ( const Matrix2x2& right )
{
	(*this) = (*this) + right;
	return (*this);
}

// Transpose Matrix
FORCE_INLINE Matrix2x2 Matrix2x2::operator! ( void ) const
{
	Matrix2x2 result;
	result.pData[0] = this->pData[0];
	result.pData[1] = this->pData[2];
	result.pData[2] = this->pData[1];
	result.pData[3] = this->pData[3];
	return result;
}

// Public Accessor
FORCE_INLINE const Real* Matrix2x2::operator[] ( int index ) const
{
	return &( pData[index*2] );
}

// Private Accessor and Editor
FORCE_INLINE Real* Matrix2x2::operator[] ( int index )
{
	return &( pData[index*2] );
}

//Equal comparison overload
FORCE_INLINE bool Matrix2x2::operator== (Matrix2x2 const& right) const
{
	for (short i = 0; i < 4; i++)
		if (fabs(right.pData[i] - pData[i]) > FTYPE_PRECISION)
			return false;

	return true;
}

//Not equal comparison overload
FORCE_INLINE bool Matrix2x2::operator!= (Matrix2x2 const& right) const
{
	return !((*this) == right);
}
