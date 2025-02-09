#ifndef CORE_RESOURCES_RESOURCE_INTERFACE_H_
#define CORE_RESOURCES_RESOURCE_INTERFACE_H_

#include "core/types.h"
#include "core-ext/containers/arresource.h"

namespace core
{
	enum arResourceType : uint8_t
	{
		kResourceTypeUserData = 0,

		kResourceTypeRawAudio,

		kResourceTypeSkeleton,
		kResourceTypePrMesh,
		kResourceTypeHitbox,
		kResourceTypeAnimation,

		// Material type.
		// Has extensions of interfaces in other modules that use the data.
		kResourceTypeArMaterial,

		kResourceTypeRawImage,
		kResourceTypeRrTexture,
		kResourceTypeRrShader,
		// stores a collection of RrMesh, skeleton, and morpher
		kResourceTypeRrMeshGroup,

		kResourceType_MAX,
	};

	typedef uint32_t arResourceHash;

	// Forward declare subsystem: it's passed into resource's OnStreamStep.
	class IArResourceSubsystem;
}

// Base resource type.
class IArResource
{
protected:
	core::arResourceHash	m_resourceHashValue;

public:
	//	virtual Destructor - often Destroyed through IArResource pointer.
	CORE_API virtual		~IArResource ( void ) {};

	//	virtual ResourceType() : What type of resource is this?
	// Identifies the type of resource this is.
	CORE_API virtual core::arResourceType
							ResourceType ( void ) = 0;
	//	virtual ResourceName() : Returns the resource name.
	// This is used to search for the resource. The smaller, the better.
	CORE_API virtual const char* const
							ResourceName ( void ) = 0;
	//	virtual ResourceHash() : Returns a resource hash.
	// This is used to search for the resource. The return value should be cached in m_resourceHashValue.
	// Some String hashes are available in core/math/hash.h
	CORE_API virtual core::arResourceHash
							ResourceHash ( void ) 
		{ return m_resourceHashValue; }

	//	virtual InitResourceHash() : Recalculates the resource hash.
	// Is called by ResourceManager on add.
	CORE_API virtual void	InitResourceHash ( void );

	//	virtual IsStreamedLoad() : Is this a streamed resource?
	// Returns:
	//	bool:	True when the resource should be added to the streaming list.
	//			False when the resource should be loaded immediately.
	CORE_API virtual bool	IsStreamedLoad ( void )
		{ return false; }

	//	virtual OnStreamStep() : Callback for streaming load.
	// Arguments:
	//	sync_client: True when performing a synchronized stream step.
	//               When synchronized, some remote commands (GPU) will need to finish before continuing.
	//	subsystem: Subsystem instance provided for this resource type. Can be null.
	//             Any casts of this argument must be unsafe - there are no provisions for type strictness.
	// Returns:
	//	bool:	False when still loading, True when done.
	//          The resource will stay in the "loading" list until it is done.
	CORE_API virtual bool	OnStreamStep ( bool sync_client, core::IArResourceSubsystem* subsystem ) 
		{ return true; }
};


#endif//CORE_RESOURCES_RESOURCE_INTERFACE_H_