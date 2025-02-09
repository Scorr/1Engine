
#ifndef _MATRIX_4_4_H_
#define _MATRIX_4_4_H_

class ALIGNAS(64) Matrix4x4
{
public:
	FORCE_INLINE Matrix4x4 ( void );
	FORCE_INLINE Matrix4x4 ( const Matrix4x4& );
	FORCE_INLINE Matrix4x4 ( const Real* );
	FORCE_INLINE Matrix4x4 ( const Vector4f& row_x, const Vector4f& row_y, const Vector4f& row_z, const Vector4f& row_w );
	// Construction from Matrix3x3
	FORCE_INLINE Matrix4x4 ( const Matrix3x3& );

	// Setters - Translation
	FORCE_INLINE bool setTranslation ( const Real = 0, const Real = 0, const Real = 0 );
	FORCE_INLINE bool setTranslation ( const Vector3f& );

	// Setters - Scaling
	FORCE_INLINE bool setScale ( const Real = 0, const Real = 0, const Real = 0 );
	FORCE_INLINE bool setScale ( const Vector3f& );

	// Setters - Rotation
	FORCE_INLINE bool setRotation ( const Real = 0, const Real = 0, const Real = 0 );
	FORCE_INLINE bool setRotation ( const Vector3f& );
	FORCE_INLINE bool setRotation ( const Quaternion& );
	FORCE_INLINE bool setRotation ( const Matrix3x3& );

	FORCE_INLINE bool setRotationZYX ( const Real = 0, const Real = 0, const Real = 0 );

	// Modders
	FORCE_INLINE void translate ( const Vector3f& );

	// Getters - Translation
	FORCE_INLINE Vector3f	getTranslation ( void ) const;

	// Getters - Rotation
	FORCE_INLINE Vector3f	getEulerAngles ( void ) const;
	FORCE_INLINE Quaternion	getQuaternion ( void ) const;
	FORCE_INLINE Matrix3x3	getRotator ( void ) const;

	// Getters - Scale
	FORCE_INLINE Vector3f	getScaling ( void ) const;

	// Operations
	FORCE_INLINE Matrix4x4 transpose ( void ) const;
	FORCE_INLINE Matrix4x4 inverse ( void ) const;
	FORCE_INLINE Real determinant ( void ) const;
	
	FORCE_INLINE Matrix3x3 submatrix ( int, int ) const;

	FORCE_INLINE Matrix4x4 LerpTo( Matrix4x4 const&, Real const ) const;
	FORCE_INLINE void Lerp ( Matrix4x4 const&, Real const ); 

	// Multiplication
	FORCE_INLINE Matrix4x4 operator* ( Matrix4x4 const& ) const;
	FORCE_INLINE Matrix4x4 operator*=( Matrix4x4 const& );
	FORCE_INLINE Matrix4x4 operator* ( Real const ) const;

	FORCE_INLINE Matrix4x4 operator+ ( Matrix4x4 const& ) const;
	FORCE_INLINE Matrix4x4 operator+=( Matrix4x4 const& );

	FORCE_INLINE Vector3f operator* ( Vector3f const& ) const;
	FORCE_INLINE Vector4f operator* ( Vector4f const& ) const;

	// Transpose
	FORCE_INLINE Matrix4x4 operator! ( void ) const;

	// Accessor
	FORCE_INLINE const Real* operator[] ( int ) const;
	// Editor
	FORCE_INLINE Real* operator[] ( int );

	//Equal comparison overload
	FORCE_INLINE bool operator== (Matrix4x4 const& right) const;

	//Not equal comparison overload
	FORCE_INLINE bool operator!= (Matrix4x4 const& right) const;

	// Check for valid values
	FORCE_INLINE bool isOk ( void );

public:
	float pData[16];
};

// Ensure it's aligned
static_assert(sizeof(Matrix4x4)==64,"Invalid alignment for Matrix4x4");
//static_assert(std::alignment_of<Matrix4x4>::value == 64, "Alignment of Matrix4x4 must be 64");

#endif//_MATRIX_4_4_H_