
#ifndef _C_PARTICLE_EMITTER_H_
#define _C_PARTICLE_EMITTER_H_

// Include gameobject
//#include "CGameObject.h"
#include "renderer/logic/CLogicObject.h"
// Include particle
#include "CParticle.h"	// Which includes all attributes

// Include Vector Arrays
#include <vector>

// Include all attributes 
/*#include "standard_types.h"
#include "floattype.h"
#include "Vector2d.h"
#include "Vector3d.h"
#include "Vector4d.h"
#include "Color.h"
#include "RangeValue.h"*/

// Include saving and loading
class CBaseSerializer;
typedef CBaseSerializer Serializer;

// Updater and Renderer Prototype for friending
class CParticleUpdater;
class CParticleRenderer;
class CParticleRenderer_Animated;
class CParticleEditor;

// Class Definition
class CParticleEmitter : public CLogicObject
{
	RegisterRenderBaseClass( "CParticleEmitter" );
	RegisterRenderClassName( "CParticleEmitter" );
private:
	friend CParticleUpdater;
	friend CParticleRenderer;
	friend CParticleRenderer_Animated;

	friend CParticleEditor;

public:
	RENDER_API explicit		CParticleEmitter ( void );
	RENDER_API 				~CParticleEmitter ( void );
	
	RENDER_API void			PreStepSynchronus ( void ) override;

	// Emit particles
	RENDER_API virtual void	Emit ( void );

	RENDER_API unsigned int	GetParticleCount ( void );
	RENDER_API bool			HasEmitted ( void );

	// Start up system
	RENDER_API void			Initialize ( void );
	// Load system from file
	RENDER_API void			LoadFromFile ( const string & );
	// Serialization
	RENDER_API virtual void	serialize ( Serializer &, const uint );

protected:
	
	RENDER_API void CreateParticle ( const Vector3d & );

	// Emitter states
	bool		bHasEmitted;
	Real		fEmitCount;

	Vector3d	m_lastPosition;
	Vector3d	m_worldVelocity;

public:
	// Emitter Shape Data
	Vector3d	vEmitterSize;

protected:
	// Common Particle Emitter Data
	//std::vector<CParticle>	vParticles;
	CParticle*	m_particles;
	uint16_t	m_max_particle_index;
	uint16_t	m_next_particle_index;
	uint16_t	m_particle_array_size;
	uint16_t	m_particle_count;

	bool		m_strictly_ordered_particles;

public:
	bool				bOneShot;
	bool				bSimulateInWorldspace;
	bool				bSmoothEmitter;

	Real				fVelocityScale;

	RangeValue<Real>	rfParticlesSpawned;
	Real				fMaxParticles;
	
	RangeValue<Real>	rfLifetime;

	RangeValue<Real>	rfStartSize;
	RangeValue<Real>	rfEndSize;

	RangeValue<Real>	rfStartAngle;
	RangeValue<Real>	rfAngularVelocity;
	RangeValue<Real>	rfAngularAcceleration;
	RangeValue<Real>	rfAngularDamping;

	RangeValue<Vector3d>	rvVelocity;
	RangeValue<Vector3d>	rvAcceleration;
	RangeValue<Vector3d>	rvLinearDamping;

	std::vector<Color>		vcColors;
};

#endif