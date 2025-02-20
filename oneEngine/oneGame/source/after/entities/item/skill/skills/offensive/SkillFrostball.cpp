
#include "SkillFrostBall.h"

#include "core/time/time.h"
#include "engine/physics/raycast/Raycaster.h"
#include "core/math/random/Random.h"

#include "after/entities/projectile/magical/ProjectileFrostball.h"

#include "engine-common/entities/CParticleSystem.h"
#include "renderer/logic/particle/CParticleEmitter.h"

#include "after/entities/character/CCharacter.h"

SkillFrostBall::SkillFrostBall ( void ) : CSkill( ItemData() )
{
	//mana_cost	= 5;
	//cooldown	= 0.3f;
	passive		= false;

	ps_firetoss_active = new CParticleSystem ( "particlesystems/flame03.pcf", "particle_fluuxflame" );
	//ps_firetoss_active = new CParticleSystem( ".res/particlesystems/spells/magicbolt_projectile.pcf" );
	ps_firetoss_active->enabled = false;

	/*mMagicModel = new CModel( "models/effects/magic_ball.fbx" );
	glMaterial* newMat = new glMaterial;
	newMat->loadFromFile( "effects/spell_fireball" );
	mMagicModel->SetMaterial( newMat );
	newMat->removeReference();
	mMagicModel->visible = false;
	mMagicModel->transform.Get( transform );
	mMagicModel->transform.SetParent( &transform );*/
		
	holdType = Item::SpellHanded;
	iAnimationSubset = 2;
}

SkillFrostBall::~SkillFrostBall ( void )
{
	delete_safe(ps_firetoss_active);
}

bool SkillFrostBall::Use ( int use )
{
	if ( use == Item::UPrimary )
	{
		if ( CanUse(0) && CanCast() )
		{
			// Decrement mana
			CastMana();
			// Put on a short cooldown
			SetCooldown( 0,cooldown );

			if ( pOwner ) {
				((CCharacter*)pOwner)->OnRangedAttack();
			}

			if ( pOwner ) {
				((CCharacter*)pOwner)->PlayItemAnimation( NPC::ItemAnim::Toss, iAnimationSubset, mHand, 3.0f );
			}
		}
	}
	return false;
}

// Attack function
void SkillFrostBall::Attack ( XTransform& )
{
	// Create projectile here
	if ( isCasting )
	{
		ProjectileFrostball*	tempArrow;
		Ray		aimDir;
		ftype	fireSpeed;
		Damage	arrowDamage;
	
		aimDir = pOwner->GetAimRay( transform.position );
		fireSpeed = 30.0f;

		arrowDamage.amount	= 0.0f;// + skill_lvl * 1.0f;
		arrowDamage.type	= DamageType::Magical | DamageType::Ice;
						
		tempArrow = new ProjectileFrostball( aimDir, fireSpeed );
		tempArrow->SetDamping( 0.1f );
		tempArrow->SetOwner( pOwner );
		tempArrow->SetDamage( arrowDamage );

		// Has casted, so turn that shit off
		isCasting = false;
	}
}

// CastCombine. Combines two casting skills for an effect. Is called when both skills are casted and not channelling.
// Return true if something happens. Generally, objects look through all the spells, and stop the skills by setting isCasting to false.
// Returning true will normally stop non-instant skills.
bool SkillFrostBall::CastCombine ( CSkill* combineSkill )
{
	// Same skill here
	if ( combineSkill->GetID() == this->GetID() )
	{
		/*ProjectileMagicBolt*	tempArrow;
		Ray		aimDir;
		ftype	fireSpeed;
		Damage	arrowDamage;
	
		aimDir = pOwner->GetAimRay( transform.position );
		fireSpeed = 40.0f;

		arrowDamage.amount	= 10.0f + skill_lvl * 5.0f;
		arrowDamage.type	= DamageType::Fire | DamageType::Burn | DamageType::Magical;
						
		tempArrow = new ProjectileMagicBolt( aimDir, fireSpeed );
		tempArrow->SetDamping( 0.0f );
		tempArrow->SetOwner( pOwner );
		tempArrow->SetDamage( arrowDamage );

		return true;*/
	}
	// Nothing matches
	return false;
}

void SkillFrostBall::EndUse ( int use )
{
	//return false;

}

void SkillFrostBall::Update ( void )
{
	CSkill::Update();

	if ( holdState == Item::Holding && pOwner && CanUse(0) ) {
		ps_firetoss_active->transform.position = transform.position;
		ps_firetoss_active->enabled = true;
		/*
		//mMagicModel->transform.position = transform.position;
		mMagicModel->transform.localScale = Vector3d(1,1,1) * 1.1f;
		mMagicModel->visible = true;*/
	}
	else {
		ps_firetoss_active->enabled = false;
		/*if ( holdState == Holding ) {
			//mMagicModel->transform.position = transform.position;
			mMagicModel->transform.localScale = Vector3d(1,1,1) * sqrt(fabs(std::max<ftype>( 0,  (0.6f - GetCooldown(0)/cooldown)/0.6f ))) * 1.1f;
			mMagicModel->visible = true;
		}
		else {
			mMagicModel->visible = false;
		}*/
	}

	cooldown	= 4.5f - skill_lvl * 0.5f;
	mana_cost	= 5.0f + skill_lvl * 2.0f;
}