
#include "CParticleSystem.h"

#include "core/system/io/FileUtils.h"
#include "core/system/io/CBinaryFile.h"
#include "core/system/io/CSegmentedFile.h"
#include "core-ext/system/io/serializer/ISerialBinary.h"
#include "core-ext/system/io/Resources.h"

#include "engine-common/types/ParticleEnums.h"

#include "renderer/material/RrMaterial.h"
/*
#include "renderer/logic/particle/CParticleEmitter.h"
#include "renderer/logic/particle/CParticleUpdater.h"
#include "renderer/object/particle/CParticleRenderer.h"

#include "renderer/logic/particle/CParticleEmitterSkeleton.h"

#include "renderer/logic/particle/modifiers/CParticleMod_Windmotion.h"
#include "renderer/object/particle/CParticleRenderer_Animated.h"
*/
#include "renderer/object/RrRenderObject.h"
#include "renderer/particle/RrParticleSystem.h"

#include <sstream>

using namespace std;

CParticleSystem::CParticleSystem ( const string& s_ps, const bool b_hasMeshOverride )
	: CGameObject(), bAutoDestroy( false )
{
	this->name = "Particle System";
	enabled = true;
	Init( s_ps, b_hasMeshOverride );
}
CParticleSystem::CParticleSystem ( const string& s_ps, const string& s_mat )
	: CGameObject(), bAutoDestroy( false )
{
	this->name = "Particle System";
	enabled = true;
	Init( s_ps, false );

	if ( s_mat.size() > 0 )
	{
		RrMaterial* mat = new RrMaterial();
		mat->loadFromFile( s_mat.c_str() );
		mat->removeReference();
		RrRenderObject* targetRenderable;
		targetRenderable = GetRenderable();
		if ( targetRenderable )
		{
			targetRenderable->SetMaterial( mat );
		}
	}
}

CParticleSystem::~CParticleSystem ( void )
{
	// Emitters are also added to the logic object list, so no need to delete them here.
	// Instead, they are deleted by CExtendableGameObject's destructor.
	vpEmitters.clear();
}

void CParticleSystem::Init ( const string& sSystemFile, const bool bHasMeshOverride )
{
	string sActualSystemFile = core::Resources::PathTo( sSystemFile );

#ifdef _ENGINE_DEBUG
	if (!IO::FileExists(sActualSystemFile))
	{
		throw core::MissingFileException();
	}
#endif

#if 0
	uint32_t iVersion = 0;
	{
		// Input file!
		CSegmentedFile inFile ( sActualSystemFile );

		// Read in the data
		if ( inFile.ReadData() == false )
		{
			throw core::CorruptedDataException();
		}

		// Check header for version number
		{
			// Get data
			string str;
			inFile.GetSectionData( string("header"), str );
			stringstream sin ( str, ios_base::binary|ios_base::in|ios_base::out );

			// Read version
			sin.read( (char*)(&iVersion), sizeof(uint32_t) );
		}
	}

	switch ( iVersion )
	{
	case 1:
		// Create a version 1 particle system attached to this object
		{
			CParticleEmitter* emitter;
			if ( !bHasMeshOverride ) { 
				emitter = new CParticleEmitter();
			}
			else {
				emitter = new CParticleEmitterSkeleton();
			}
			emitter->LoadFromFile( sActualSystemFile );
			CParticleUpdater* updater = new CParticleUpdater( emitter );
			CParticleRenderer* renderer = new CParticleRenderer( emitter );

			AddComponent( emitter );
			AddComponent( updater );
			AddComponent( renderer );

			vpEmitters.push_back( emitter );
		}
		break;
	case 2:
	case 3:
		// Create a version 2 particle system attached to this object
		{
			CBinaryFile inFile;
			inFile.Open( sActualSystemFile.c_str(), CBinaryFile::IO_READ );
			// First read in past the ascii header
			{
				string gaurdSearch = "";
				bool endGaurd = false;
				while ( !endGaurd ) {
					gaurdSearch += inFile.ReadChar();
					if ( string("-end>>").find( gaurdSearch ) == string::npos ) {
						gaurdSearch = "";
					}
					else if ( gaurdSearch == "-end>>" ) {
						endGaurd = true;
					}
				}
			}
			// Now that the header is read in, read in component type, and run serializer for each class
			{
				ISerialBinary deserializer ( &inFile, iVersion );

				void* newComponent = NULL;
				CParticleEmitter* lastEmitter = NULL;
				CParticleUpdater* lastUpdater = NULL;

				uint32_t currentObjType = inFile.ReadUInt32();
				while ( !feof( inFile.GetFILE() ) )
				{
					switch ( currentObjType )
					{
					case engine::PARTICLESYS_EMITTER:
						if ( !bHasMeshOverride )
							newComponent = new CParticleEmitter();
						else
							newComponent = new CParticleEmitterSkeleton();
						deserializer >> ((CParticleEmitter*)(newComponent));
						AddComponent( (CParticleEmitter*)(newComponent) );
						lastEmitter = (CParticleEmitter*)(newComponent);
						vpEmitters.push_back( lastEmitter );
						break;

					case engine::PARTICLESYS_UPDATER:
						newComponent = new CParticleUpdater(lastEmitter);
						deserializer >> ((CParticleUpdater*)(newComponent));
						AddComponent( (CParticleUpdater*)(newComponent) );
						break;

					case engine::PARTICLESYS_RENDERER:
						newComponent = new CParticleRenderer(lastEmitter);
						((CParticleRenderer*)(newComponent))->SetMaterial( new RrMaterial );
						((CParticleRenderer*)(newComponent))->GetMaterial()->removeReference();
						deserializer >> ((CParticleRenderer*)(newComponent));
						AddComponent( (CParticleRenderer*)(newComponent) );
						break;

					case engine::PARTICLESYS_EMITTER_CLOUDS: // Skip this
						break;

					case engine::PARTICLESYS_RENDERER_ANIMATED: // Renderer - Animation; not yet implemented
						newComponent = new CParticleRenderer_Animated(lastEmitter);
						((CParticleRenderer_Animated*)(newComponent))->SetMaterial( new RrMaterial );
						((CParticleRenderer_Animated*)(newComponent))->GetMaterial()->removeReference();
						deserializer >> ((CParticleRenderer_Animated*)(newComponent));
						AddComponent( (CParticleRenderer_Animated*)(newComponent) );
						break;

					case engine::PARTICLESYS_MODIFIER_WIND: // Modifier - Wind
						newComponent = new CParticleMod_Windmotion();
						deserializer >> ((CParticleMod_Windmotion*)(newComponent));
						lastUpdater->AddModifier( ((CParticleMod_Windmotion*)(newComponent)) );
						break;

					default:
						throw core::CorruptedDataException();
						debug::Console->PrintError( "particle system: unrecognized component type!" );
						break;
					}
					currentObjType = inFile.ReadUInt32();
				}
			}

		}
		break;
	default:
		debug::Console->PrintError( "could not read " );
		debug::Console->PrintError( sSystemFile.c_str() );
		debug::Console->PrintError( ": unsupported file version or invalid\n" );
		break;
	}
#endif
}

void CParticleSystem::PostUpdate ( void )
{
#if 0
	// Update transform & activity of particle emitters...
	for ( vector<CParticleEmitter*>::iterator it = vpEmitters.begin(); it != vpEmitters.end(); it++ )
	{
		(*it)->transform.Get( this->transform );
		(*it)->SetActive( enabled );
	}
	// Update the transform of particle renderers...
	for ( vector<RrRenderObject*>::iterator it = vpRComponents.begin(); it != vpRComponents.end(); it++ )
	{
		(*it)->transform.Get( this->transform );
	}

	// Check for automatic system destruction:
	if ( bAutoDestroy )
	{
		bool can_destroy = true;

		// Loop through emitters
		for ( vector<CParticleEmitter*>::iterator it = vpEmitters.begin(); it != vpEmitters.end(); it++ )
		{
			// If the emitter is not one-shot or has particles out, then destroy self automatically
			CParticleEmitter* emitter = *it;
			
			if ( emitter->bOneShot )
			{	// One shots may not have emitted yet, so we wait until they have.
				if ( emitter->GetParticleCount() > 0 || !emitter->HasEmitted() )
				{
					can_destroy = false;
					continue;
				}
			}
			else
			{	// Constant emitters will be set to autodestroy after their owner has died. We just check their particle count.
				if ( emitter->GetParticleCount() > 0 )
				{
					can_destroy = false;
				}
			}
		}

		// Send delete message if should be deleting
		if ( can_destroy )
		{
			DeleteObject( this );
		}
	}
#endif
}