#ifndef _MATRIX_3_3_H_
#define _MATRIX_3_3_H_

class Matrix3x3
{//todo: alignment
public:
	FORCE_INLINE Matrix3x3 ( void );
	FORCE_INLINE Matrix3x3 ( const Matrix3x3& );
	FORCE_INLINE Matrix3x3 ( const Real* );
	FORCE_INLINE Matrix3x3 ( const Vector3f& row_x, const Vector3f& row_y, const Vector3f& row_z );

	// Setters - Scaling
	FORCE_INLINE void setScale ( const Real = 0, const Real = 0, const Real = 0 );
	FORCE_INLINE void setScale ( const Vector3f& );

	// Setters - Rotation
	FORCE_INLINE void setRotation ( const Real = 0, const Real = 0, const Real = 0 );
	FORCE_INLINE void setRotation ( const Vector3f& );
	FORCE_INLINE void setRotation ( const Quaternion& );
	FORCE_INLINE void setRotation ( const Matrix3x3& );

	FORCE_INLINE void setRotationZYX ( const Real = 0, const Real = 0, const Real = 0 );

	// Getters - Rotation
	FORCE_INLINE Vector3f	getEulerAngles ( void ) const;
	FORCE_INLINE Quaternion	getQuaternion ( void ) const;

	// Getters - Scale
	FORCE_INLINE Vector3f	getScaling ( void ) const;

	// Operations
	FORCE_INLINE Matrix3x3 transpose ( void ) const;
	FORCE_INLINE Matrix3x3 inverse ( void ) const;
	FORCE_INLINE Real determinant ( void ) const;
	
	FORCE_INLINE Matrix2x2 submatrix ( int, int ) const;

	FORCE_INLINE Matrix3x3 LerpTo( Matrix3x3 const&, Real const ) const;
	FORCE_INLINE void Lerp ( Matrix3x3 const&, Real const ); 

	// Multiplication
	FORCE_INLINE Matrix3x3 operator* ( Matrix3x3 const& ) const;
	FORCE_INLINE Matrix3x3 operator*=( Matrix3x3 const& );
	FORCE_INLINE Matrix3x3 operator* ( Real const ) const;

	FORCE_INLINE Matrix3x3 operator+ ( Matrix3x3 const& ) const;
	FORCE_INLINE Matrix3x3 operator+=( Matrix3x3 const& );

	FORCE_INLINE Vector3f operator* ( Vector3f const& ) const;

	// Transpose
	FORCE_INLINE Matrix3x3 operator! ( void ) const;

	// Accessors
	FORCE_INLINE const Real* operator[] ( int ) const;
	FORCE_INLINE Real* operator[] ( int );

	// Equal comparison overload
	FORCE_INLINE bool operator== (Matrix3x3 const& right) const;

	// Not equal comparison overload
	FORCE_INLINE bool operator!= (Matrix3x3 const& right) const;

	// Check for valid values
	FORCE_INLINE bool isOk ( void );

public:
	Real pData [9];
};

#endif//_MATRIX_3_3_H_