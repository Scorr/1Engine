
#ifndef _PHYS_RIGIDBODY_H_
#define _PHYS_RIGIDBODY_H_

#include "core/types/types.h"
#include "physical/physics/CPhysics.h"
#include "physical/physics/shapes/physShape.h"

#define _FORCE_INLINE_

class physRigidBody
{
protected:
	_FORCE_INLINE_ PHYS_API explicit		physRigidBody ( void ) {;}
public:
	_FORCE_INLINE_ PHYS_API explicit		physRigidBody ( physRigidBodyInfo* info, bool isDynamic=true );
	_FORCE_INLINE_ PHYS_API virtual		~physRigidBody ( void );

	_FORCE_INLINE_ PHYS_API physCollidable*	getCollidableRw ( void );

	_FORCE_INLINE_ PHYS_API void			setUserData ( const uint64_t );
	_FORCE_INLINE_ PHYS_API uint64_t		getUserData ( void ) const;
	_FORCE_INLINE_ PHYS_API void			setCollisionFilterInfo ( const b2Filter& );
	_FORCE_INLINE_ PHYS_API void			setMotionType ( const hkpMotion::MotionType );
	_FORCE_INLINE_ PHYS_API hkpMotion::MotionType getMotionType ( void ) const;
	_FORCE_INLINE_ PHYS_API void			setQualityType ( const hkpCollidableQualityType );
	_FORCE_INLINE_ PHYS_API void			setAllowedPenetrationDepth ( const Real32 );
	_FORCE_INLINE_ PHYS_API void			setShape ( const b2Shape* shape );

	_FORCE_INLINE_ PHYS_API void			setMass ( const Real32 );
	_FORCE_INLINE_ PHYS_API Real32		getMass ( void ) const;
	_FORCE_INLINE_ PHYS_API void			setGravityFactor ( const Real32 );
	_FORCE_INLINE_ PHYS_API Real32		getGravityFactor ( void ) const;
	_FORCE_INLINE_ PHYS_API void			setRestitution ( const Real32 );
	_FORCE_INLINE_ PHYS_API Real32		getRestitution ( void ) const;
	_FORCE_INLINE_ PHYS_API void			setFriction ( const Real32 );
	_FORCE_INLINE_ PHYS_API Real32		getFriction ( void ) const;
	_FORCE_INLINE_ PHYS_API void			setLinearDamping ( const Real32 );
	_FORCE_INLINE_ PHYS_API void			setAngularDamping ( const Real32 );
	_FORCE_INLINE_ PHYS_API void			setMaxAngularVelocity ( Real32 );
	_FORCE_INLINE_ PHYS_API Real32		getMaxAngularVelocity ( void ) const;

	_FORCE_INLINE_ PHYS_API void			activate ( void );
	_FORCE_INLINE_ PHYS_API void			applyForce ( const Real32 delta, const Vector3f& vect );
	_FORCE_INLINE_ PHYS_API void			setIdentityRotation ( void );

	_FORCE_INLINE_ PHYS_API void			setPosition ( const Vector3f& );
	_FORCE_INLINE_ PHYS_API const Vector3f getPosition ( void ) const;
	_FORCE_INLINE_ PHYS_API void			setRotation ( const Quaternion& );
	_FORCE_INLINE_ PHYS_API const Quaternion getRotation ( void ) const;
	_FORCE_INLINE_ PHYS_API void			setPositionAndRotation ( const Vector3f&, const Quaternion& );
	_FORCE_INLINE_ PHYS_API void			setLinearVelocity ( const Vector3f& );
	_FORCE_INLINE_ PHYS_API const Vector3f getLinearVelocity ( void ) const;
	_FORCE_INLINE_ PHYS_API void			setAngularVelocity ( const Vector4f& );
	_FORCE_INLINE_ PHYS_API const Vector4f getAngularVelocity ( void ) const;

	_FORCE_INLINE_ PHYS_API void			addContactListener ( physContactListener* );
	_FORCE_INLINE_ PHYS_API void			removeContactListener ( physContactListener* );

	_FORCE_INLINE_ PHYS_API void			SetTransform ( core::Transform* );
	_FORCE_INLINE_ PHYS_API void			GetTransform ( core::Transform* );
	_FORCE_INLINE_ PHYS_API void			GetTranslation ( core::Transform* );
	_FORCE_INLINE_ PHYS_API void			ForcePropertyUpdate ( void );
protected:
	b2Body* body;
	//b2Fixture*	body;
	b2Fixture* fixture;
};


#endif//_PHYS_RIGIDBODY_H_