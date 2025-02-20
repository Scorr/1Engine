
#include "physical/liscensing.cxx" // Include liscense info

#include "engine/utils/CDeveloperConsole.h"
#include "engine-common/lua/LuaSys.h"
#include "engine-common/lua/Lua_Console.h"
#include "engine-common/engine-common.h"
#include "engine-common/engine-common-scenes.h"
#include "after-common.h"

#include "after/scenes/gmsceneCharacterViewer.h"
#include "after-editor/scenes/gmsceneParticleEditor.h"
#include "after-editor/scenes/gmsceneLipsyncEditor.h"

#include "after/physics/water/Water.h"
#include "after/physics/wind/WindMotion.h"

//===============================================================================================//
//	GameInitialize
//
// After the main engine components have been started, this function is called.
// This calls EngineCommonInitialize first, then performs its own actions.
// It adds game specific bindings to the console, Lua system, and other systems.
//===============================================================================================//
int GameInitialize ( void )
{
	// Engine
	EngineCommonInitialize();

	// Scene registration
	EngineCommon::RegisterScene<gmsceneCharacterViewer>( "test" );
	EngineCommon::RegisterScene<gmsceneParticleEditor>( "pce" );
	EngineCommon::RegisterScene<gmsceneLipsyncEditor>( "lse" );

	//===============================================================================================//
	// PLACE YOUR CUSTOM ENGINE MODULES BELOW HERE
	//===============================================================================================//

	// create testers
	new CAfterWaterTester();
	new CWindMotion();

	return 0;
}