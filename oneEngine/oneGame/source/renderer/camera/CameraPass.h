#ifndef RENDERER_CAMERA_PASS_H_
#define RENDERER_CAMERA_PASS_H_

#include "core/math/Frustum.h"
#include "core/math/Rect.h"
#include "core/math/matrix/CMatrix.h"
#include "renderer/types/viewport.h"

#include "gpuw/Buffers.h"

enum rrCameraRenderType
{
	// Will run the 5x5 forward+deferred hybrid renderer
	kCameraRenderWorld,

	// Will run the forward+ shadow color renderer
	kCameraRenderShadow,
};

class RrOutputInfo;
namespace gpu {
	class Buffer;
	class GraphicsContext;
}

// Input struct for querying passes
struct rrCameraPassInput
{
	// maximum number of passes to return
	uint32_t			m_maxPasses = 0;

	// Output info for the viewport information
	const RrOutputInfo*	m_outputInfo = nullptr;

	// Context to use
	gpu::GraphicsContext*
						m_graphicsContext = nullptr;
};

// Pass information.
struct rrCameraPass
{
	// How does this camera act for this given pass?
	rrCameraRenderType	m_passType;

	// target viewport rendering to
	rrViewport			m_viewport;

	// camera cbuffer to be used with this pass
	gpu::Buffer			m_cbuffer;

	// Context to use
	gpu::GraphicsContext*
						m_graphicsContext = nullptr;

	// rendering properties
	Matrix4x4			m_viewTransform;
	Matrix4x4			m_projTransform;
	Matrix4x4			m_viewprojTransform;
	core::math::Frustum	m_frustum;

public:
	//	free() : Free the allocated data in this camera pass.
	RENDER_API void		free ( void )
	{
		m_cbuffer.free(NULL);
	}
};

#endif//RENDERER_CAMERA_PASS_H_