#include "renderer/state/Options.h"

#include "renderer/state/RrRenderer.h"
//#include "renderer/resource/CResourceManager.h"
//#include "renderer/material/RrPassDeferred.h"

#include "core-ext/resources/ResourceManager.h"
#include "core/debug/Console.h"

void renderer::Options::TextureStreaming ( const bool enabled )
{
	//RrRenderer::Active->mResourceManager->settings.streamTextures = enabled;
	core::ArResourceManager::Active()->m_settings.streamTextures = enabled;
}

void renderer::Options::DeferredShaders ( const rrDeferredShaderSettings& shaderInfo )
{
	//renderer::gDefferedShaderSettings = shaderInfo;
	//ARCORE_ERROR("Not yet implemented");
	debug::Console->PrintError("renderer::Options::DeferredShaders() : Not yet implemented\n");
}