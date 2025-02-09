#include "gpuw/module_config.h"
#ifdef GPU_API_OPENGL
#	include "gpuw/ogl/RenderTarget.h"
#endif

#ifdef GPU_API_VULKAN
#	include "gpuw/vk/RenderTarget.vk.h"
#endif

#ifdef GPU_API_DIRECTX11
#	include "gpuw/dx11/RenderTarget.dx11.h"
#endif