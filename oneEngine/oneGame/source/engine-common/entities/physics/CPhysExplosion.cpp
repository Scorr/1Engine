
#include "CPhysExplosion.h"
#include "core/math/Math.h"
#include "engine-common/physics/wind/physExplosion.h"

CPhysExplosion::CPhysExplosion( Vector3f &vPos, Real fMagnitude, Real fPhysicsMultiplier )
	: CGameObject()
{
	transform.world.position = vPos;
	magnitude = fMagnitude;
	physics_multiplier = fPhysicsMultiplier;

	exploded = false;
}

CPhysExplosion::~CPhysExplosion ( void )
{
	// Not sure yet
}

void CPhysExplosion::Update ( void )
{
	if ( exploded )
		DeleteObject( this );
}

void CPhysExplosion::Explode ( void )
{
	if ( !exploded )
	{
		physExplosion* physExplo = new physExplosion ( transform.world.position, sqrt(magnitude)*physics_multiplier, magnitude, magnitude*2.5f );
	}
	exploded = true;

	// Delete this object
	DeleteObject( this );
}

//fezzes are cool