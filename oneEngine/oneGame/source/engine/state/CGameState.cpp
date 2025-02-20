#include "core/time.h"
#include "core-ext/transform/Transform.h"
#include "core-ext/animation/AnimationControl.h"
#include "engine/state/CGameState.h"

//#include "physical/physics/CPhysics.h"
//#include "engine/physics/CPhysicsEngine.h"
#include "core-ext/profiler/CTimeProfiler.h"
//#include "CResourceManager.h"
#include "core-ext/resources/ResourceManager.h"

#include <iostream>

using std::vector;

//===Class static member data===

CGameState*	CGameState::mActive = NULL;
guid32_t	CGameState::nextGUID = 0;

//===Class functions===

// Return current instance
// Accessor
CGameState*	CGameState::Active ( void )
{
	return mActive;
}

// Class constructor
//  saves current instance into pActive
//  initializes list of renderers
CGameState::CGameState ( void )
	: messenger( this ), mResourceManager(NULL)
{
	mActive = this;

	// Create the object listing
	iListSize = 16;
	pBehaviors = new CGameBehavior* [iListSize];

	iCurrentIndex = 0;

	pScene = NULL;
	pNextScene = NULL;
	bSceneActive = true;

	bEndingGame = false;
}

// Class destructor
//  sets pActive pointer to null
//  frees list of objects
CGameState::~CGameState ( void )
{
	// Reset active gamestate
	mActive = NULL;
	// Delete the list of instances
	if ( pBehaviors != NULL )
	{
		delete[] pBehaviors;
	}
	// Reset active instance list
	pBehaviors = NULL;
	iCurrentIndex = 0;
	iListSize = 0;
}

void CGameState::CleanWorld ( void )
{
	isCleaningWorld = true;

	// Loop through all instances. However, at each stage, we need to empty out the deletion list in order to ensure the creation list is respected.
	for ( unsigned int i = 0; i < iCurrentIndex; i += 1 )
	{
		//  Delete if valid (not null and not persistent)
		if ( pBehaviors[i] != NULL && !pBehaviors[i]->persistent )
		{
			ARCORE_ASSERT_MSG( !pBehaviors[i]->HasReference(), "Attempted to delete object with existing references!" );

			if ( i != pBehaviors[i]->GetId() )
			{
				debug::Console->PrintWarning( "Warning: an improper id (%d) was found. Ignoring object.\n  Note that this is a fatal warning, and should be reported.", i );
				debug::Console->PrintError( "Mismatched object ID detected! PLEASE NOTE, THIS WARNING IS FATAL IN SINGLEPLAYER GAMES AND SHOULD BE REPORTED." );
				ARCORE_ERROR( "Behavior ID did not match index. Stale/invalid pointers?" );
				pBehaviors[i] = NULL;
			}
			else
			{
				delete (pBehaviors[i]);
				pBehaviors[i] = NULL;
			}

			// Destroy any objects that were queued up.
			EmptyDestructionList();
		}
	}

	isCleaningWorld = false;
}

void CGameState::EmptyDestructionList ( void )
{
	// Loop through the delete list
	if ( !vDeletionList.empty() )
	{
		// Work on a copy of the list
		vector<sObjectFCounter> t_deletionList; 
		{
			// Lock the list
			std::lock_guard<std::mutex> lock( mtListLock );
			// Grab the list copy
			t_deletionList = vDeletionList;
		}

		// Setup deletion list
		for ( vector<sObjectFCounter>::iterator it = t_deletionList.begin(); it != t_deletionList.end(); )
		{
			// If timer is negative, delete
			if ( it->fCounter <= 0.0f )
			{
				if ( it->id < iCurrentIndex && pBehaviors[it->id] == it->pBehavior )
				{
					CGameBehavior* t_behavior = it->pBehavior; // Save the behavior

					// Call OnDestroy
					t_behavior->OnDestroy();
					// Delete the behavior now
					delete t_behavior; // Destructor should nullify index automatically
				}
				// Remove it from the list and get next iterator
				it = t_deletionList.erase( it );
			}
			else
			{
				// else decrement counter and continue on
				it->fCounter -= Time::deltaTime;
				it++;
			}
		}

		// Copy the list over again
		{
			// Lock the list
			std::lock_guard<std::mutex> lock( mtListLock );
			// Copy over the resulting list
			vDeletionList = t_deletionList;
		}
	}
}

void CGameState::EndGame ( void )
{
	// End game
	//RrWindow::pActive->sendEndMessage();
	bEndingGame = true;
}

//-Game Step-
// Called during the game updating routine before window rendering
void CGameState::Update ( void )
{
	CGameBehavior * pBehavior;
	unsigned int i;

	// Update resource manager
	if ( mResourceManager ) {
		mResourceManager->FileUpdate();
	}

	// Call OnCreate on all new instances
	while ( !vCreationList.empty() )
	{
		size_t list_size = vCreationList.size();
		for (size_t i = 0; i < list_size; ++i)
		{
			vCreationList[i]->OnCreate();
		}
		vCreationList.erase( vCreationList.begin(), vCreationList.begin() + list_size );
	}

	// Loop through all instances
	//  Render if active
#if defined(_ENGINE_UNOPTIMIZED) || defined(_ENGINE_DEBUG)
	TimeProfiler.BeginTimeProfile( "gs_update" );
#endif
	for ( i = 0; i < iCurrentIndex; i += 1 )
	{
		pBehavior = pBehaviors[i];
		if (( pBehavior != NULL )&&( pBehavior->active ))
		{
			pBehavior->Update();
		}
	}
#if defined(_ENGINE_UNOPTIMIZED) || defined(_ENGINE_DEBUG)
	TimeProfiler.EndTimeProfile( "gs_update" );
#endif
}
// Called after updating but before window pre-rendering
void CGameState::LateUpdate ( void )
{
	CGameBehavior * pBehavior;
	unsigned int i;

	// Loop through all instances
	//  Update if active
#	if defined(_ENGINE_UNOPTIMIZED) || defined(_ENGINE_DEBUG)
	TimeProfiler.BeginTimeProfile( "gs_lateupdate" );
#	endif
	for ( i = 0; i < iCurrentIndex; i += 1 )
	{
		pBehavior = pBehaviors[i];
		if (( pBehavior != NULL )&&( pBehavior->active ))
		{
			EXCEPTION_CATCH_BEGIN
			pBehavior->LateUpdate();
			EXCEPTION_CATCH_END
		}
	}

	EmptyDestructionList();

	// Add time to the delta time
	/*Time::fixedTime += Time::deltaTime;
	if ( Time::fixedTime >= Time::targetFixedTime )
	{
		Real temp = Time::deltaTime;
		Time::deltaTime = Time::fixedTime;

		Physics::Update( Time::deltaTime );

		// Loop through all instances
		//  Update if active
		for ( i = 0; i < min( iCurrentIndex, iListSize ); i += 1 )
		{
			pBehavior = pBehaviors[i];
			if (( pBehavior != NULL )&&( pBehavior->active ))
			{
				pBehavior->FixedUpdate();
			}
		}
		// Reset the fixed time counter
		//Time::fixedTime -= Time::targetFixedTime;
		Time::fixedTime = 0;
		// Set delta time to the proper value
		Time::deltaTime = temp;
	}*/
	//Physics::UpdateThreaded( Time::deltaTime, Time::targetFixedTime, this, &CGameState::FixedUpdate, &CGameState::PhysicsUpdate );

	// Add time to the delta time
#if defined(_ENGINE_UNOPTIMIZED) || defined(_ENGINE_DEBUG)
	TimeProfiler.BeginTimeProfile( "gs_physics" );
#endif
	//Time::fixedTime += Time::smoothDeltaTime;
	Time::fixedTime += Time::deltaTime;
	if ( Time::fixedTime >= Time::targetFixedTime )
	{
		Real temp = Time::deltaTime;
		Time::deltaTime = Time::fixedTime;

#ifdef PHYSICS_MULTITHREADED
		IPrGameUpdate::PhysicsUpdateThreaded( Time::deltaTime, Time::fixedTime );
#else
		Time::limitedDeltaTime = std::min<Real>( Time::limitedDeltaTime*2.0f, std::max<Real>( Time::limitedDeltaTime/2.0f, (Time::deltaTime*2+Time::targetFixedTime)/3.0f ) );
		IPrGameUpdate::PhysicsUpdate( Time::fixedTime );
#endif

		Time::fixedTime = 0;
		// Set delta time to the proper value
		Time::deltaTime = temp;
	}
#if defined(_ENGINE_UNOPTIMIZED) || defined(_ENGINE_DEBUG)
	TimeProfiler.EndTimeProfile( "gs_physics" );
#endif

	// Loop through all instances
	//  Update if active
#if defined(_ENGINE_UNOPTIMIZED) || defined(_ENGINE_DEBUG)
	TimeProfiler.BeginTimeProfile( "gs_postupdate" );
#endif
	for ( i = 0; i < iCurrentIndex; i += 1 )
	{
		pBehavior = pBehaviors[i];
		if (( pBehavior != NULL )&&( pBehavior->active ))
		{
			pBehavior->PostFixedUpdate();
		}
	}
	for ( i = 0; i < iCurrentIndex; i += 1 )
	{
		pBehavior = pBehaviors[i];
		if (( pBehavior != NULL )&&( pBehavior->active ))
		{
			pBehavior->PostUpdate();
		}
	}
#if defined(_ENGINE_UNOPTIMIZED) || defined(_ENGINE_DEBUG)
	TimeProfiler.EndTimeProfile( "gs_postupdate" );
#endif


	// Now, AFTER everything is updated, then do a new scene
	if ( pNextScene != NULL )
	{
		debug::Console->PrintMessage( "Loading scene \"%s\"\n", pNextScene->GetSceneName() );

		if ( pScene != NULL )
		{
			if ( !pNextScene->bIsAdditive )
			{
				// Clean up everything in the world that we can
				for ( i = 0; i < 3; ++i )
					CleanWorld();
				// Now shift all gameobjects around
				unsigned int newIndex = 0;
				for ( i = 0; i < iCurrentIndex; i += 1 )
				{
					pBehavior = pBehaviors[i];
					if ( pBehavior != NULL )
					{
						pBehaviors[i] = NULL;
						pBehaviors[newIndex] = pBehavior;
						pBehavior->id = newIndex;
						newIndex += 1;
					}
				}
				iCurrentIndex = newIndex;
			}
			else
			{
				debug::Console->PrintWarning( "Scene is additive. Additive scenes currently leak, be aware of your available resources.\n" );
			}
			// Remove old scene
			delete pScene;
		}
		pScene = pNextScene;
		pNextScene = NULL;
		bSceneActive = true;
		pScene->Load();
	}

}

// Called after LateUpdate, during the physics updating
void CGameState::FixedUpdate ( void )
{
	CGameBehavior * pBehavior;
	unsigned int i;

	// Loop through all instances
	//  Update if active
	for ( i = 0; i < iCurrentIndex; i += 1 )
	{
		pBehavior = pBehaviors[i];
		if (( pBehavior != NULL )&&( pBehavior->active ))
		{
			//EXCEPTION_CATCH_BEGIN
			pBehavior->FixedUpdate();
			//EXCEPTION_CATCH_END
		}
	}

	// Loop through all animation instances
	//	Update them.
	for ( i = 0; i < AnimationControl::Instances().size(); ++i )
	{
		if ( AnimationControl::Instances()[i] != NULL )
		{
			AnimationControl::Instances()[i]->Update( Time::deltaTime );
		}
	}
}
// Called after FixedUpdate, during the physics updating
void CGameState::RigidbodyUpdate ( Real interpolation )
{
	CGameBehavior * pBehavior;
	unsigned int i;

	// Loop through all instances
	//  Update if active
	for ( i = 0; i < iCurrentIndex; ++i )
	{
		pBehavior = pBehaviors[i];
		if (( pBehavior != NULL )&&( pBehavior->active ))
		{
			pBehavior->RigidbodyUpdate(interpolation);
		}
	}
}


//-Adding and Removing Renderable Objects-
// AddRO ( pointer to new RO )
//  adds an RO to be drawn and returns the index
//  used only by the RO constructor
gameid_t CGameState::AddBehavior ( CGameBehavior * pBehavior )
{
	std::lock_guard<std::mutex> lock( mtListLock );

	// First check if behavior is already added
	for ( unsigned int i = 0; i < iListSize; i += 1 )
	{
		if ( pBehaviors[i] == pBehavior ) {
			debug::Console->PrintWarning( "Double instance in list detected! Preventing.\n" );
			return i;
		}
	}

	vCreationList.push_back( pBehavior );

	// Check if current index is valid
	// If the index is not valid, resize the array
	if ( iCurrentIndex >= iListSize )
	{
		unsigned int i = 0;
		unsigned int iOldListSize = iListSize;
		CGameBehavior** pOldList = pBehaviors;

		// Double list size
		iListSize *= 2;
		// Copy old data
		pBehaviors = new CGameBehavior* [iListSize];
		for ( i = 0; i < iOldListSize; i += 1 )
		{
			pBehaviors[i] = pOldList[i];
		}
		// Set new data to NULL
		for ( i = iOldListSize; i < iListSize; i += 1 )
		{
			pBehaviors[i] = NULL;
		}
		
		// Delete the old list
		delete [] pOldList;
	}
	// Set current object to pRO
	pBehaviors[iCurrentIndex] = pBehavior;
	// Increment index for next time
	iCurrentIndex += 1;
	return iCurrentIndex-1;
}

// RemoveRO ( RO index given by AddRO )
//  sets the specified index to NULL (which the cleaner will see later)
//  only to be used by RO destructor
void CGameState::RemoveBehavior ( const gameid_t id )
{
	// Lock the list
	std::lock_guard<std::mutex> lock( mtListLock );

	// Remove from creation list
	vector<CGameBehavior*>::iterator it = find( vCreationList.begin(), vCreationList.end(), pBehaviors[id] );
	if ( it != vCreationList.end() ) {
		vCreationList.erase(it);
	}

	// Remove from deletion list
	for ( auto it = vDeletionList.begin(); it != vDeletionList.end(); )
	{
		if ( it->pBehavior == pBehaviors[id] )
		{
			it = vDeletionList.erase( it );
		}
		else
		{
			++it;
		}
	}

	// Remove behavior from the list completely
	pBehaviors[id] = NULL;
}

// Generate a unique GUID for the caller
guid32_t CGameState::GetGUID ( void )
{
	return nextGUID++;
}

// Finder functions
// TODO: BAH BAH BAH
bool CGameState::ObjectExists ( CGameBehavior* pTargetBehavior )
{
	std::lock_guard<std::mutex> lock( mtListLock );

	for ( unsigned int i = 0; i < iCurrentIndex; i += 1 )
	{
		if ( pTargetBehavior == pBehaviors[i] )
			return true;
	}
	return false;
}
vector<CGameBehavior*>* CGameState::FindObjectsWithLayer ( physical::prLayer targetLayer )
{
	vector<CGameBehavior*>* pFoundObjects = new vector<CGameBehavior*> ();
	for ( unsigned int i = 0; i < iCurrentIndex; i += 1 )
	{
		if ( pBehaviors[i] != NULL )
			if ( pBehaviors[i]->layer == targetLayer )
				pFoundObjects->push_back( pBehaviors[i] );
	}
	return pFoundObjects;
}
CGameBehavior*	CGameState::FindFirstObjectWithName ( const string & targetName )
{
	for ( unsigned int i = 0; i < iCurrentIndex; i += 1 )
	{
		if ( pBehaviors[i] != NULL )
			if ( pBehaviors[i]->name == targetName )
				return pBehaviors[i];
	}
	return NULL;
}
CGameBehavior*	CGameState::FindFirstObjectWithTypename ( const string & targetName )
{
	for ( unsigned int i = 0; i < iCurrentIndex; i += 1 )
	{
		if ( pBehaviors[i] != NULL )
			if ( pBehaviors[i]->GetTypeName() == targetName )
				return pBehaviors[i];
	}
	return NULL;
}
void CGameState::FindObjectsWithTypename ( const string & targetName, vector<CGameBehavior*> & foundObjects )
{
	foundObjects.clear();
	for ( unsigned int i = 0; i < iCurrentIndex; i += 1 )
	{
		if ( pBehaviors[i] != NULL )
			if ( pBehaviors[i]->GetTypeName() == targetName )
				foundObjects.push_back( pBehaviors[i] );
	}
}

// Delete objectsss
void CGameState::DeleteObject ( CGameBehavior* pObjectToDelete )
{
	ARCORE_ASSERT( pObjectToDelete != nullptr );
	
	if ( isCleaningWorld )
	{
		// Immediately delete the object.
		if ( pObjectToDelete->id < iCurrentIndex && pBehaviors[pObjectToDelete->id] == pObjectToDelete )
		{
			// Call OnDestroy
			pObjectToDelete->OnDestroy();
			// Delete the behavior now
			delete pObjectToDelete; // Destructor should nullify index automatically
		}
	}
	else
	{
		// First check requests for if the current behavior is already queued
		for ( vector<sObjectFCounter>::iterator it = vDeletionList.begin(); it != vDeletionList.end(); ++it )
		{
			if ( it->pBehavior == pObjectToDelete )
			{
				return;
			}
		}

		// Make sure the object exists in the system
		bool validObject = false;
		for ( unsigned int i = 0; i < iCurrentIndex; i += 1 )
		{
			if ( pBehaviors[i] == pObjectToDelete ) {
				validObject = true;
			}
		}

		if ( validObject )
		{
			sObjectFCounter newRequest;
			newRequest.pBehavior = pObjectToDelete;
			newRequest.id = pObjectToDelete->GetId();
			newRequest.fCounter = -1.0f;
			pObjectToDelete->active = false;
			vDeletionList.push_back( newRequest );
		}
	}
}
void CGameState::DeleteObjectDelayed ( CGameBehavior* pObjectToDelete, float fDeleteTime )
{
	// First check requests for if the current behavior is already queued
	for ( vector<sObjectFCounter>::iterator it = vDeletionList.begin(); it != vDeletionList.end(); ++it )
	{
		if ( it->pBehavior == pObjectToDelete )
		{
			return;
		}
	}

	sObjectFCounter newRequest;
	newRequest.pBehavior = pObjectToDelete;
	newRequest.id = pObjectToDelete->GetId();
	newRequest.fCounter = fDeleteTime;
	vDeletionList.push_back( newRequest );
}

// Returns a pointer to the spot in the array where the object is held.
// This is very unsafe and can cause memory leaks if not used properly!
// Avoid use of this function if possible!
CGameBehavior**	CGameState::GetBehaviorArrayPointer ( CGameBehavior* pBehavior )
{
	if ( pBehavior ) {
		return &(pBehaviors[pBehavior->GetId()]);
	}
	return NULL;
}