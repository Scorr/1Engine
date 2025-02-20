#ifndef GPU_WRAPPER_DEVICE_H_
#define GPU_WRAPPER_DEVICE_H_

#include "core/types/types.h"
#include "renderer/types/types.h"
#include "gpuw/Public/Transfer.h"
#include "./GraphicsContext.h"
#include "./ComputeContext.h"

namespace gpu
{
	class Fence;
	class ComputeContext;
	class OutputSurface;

	enum DeviceLayer
	{
		kDeviceLayerDebug,
	};

	class Device
	{
	public:
		// Non-exposed API for creating device.
		GPUW_EXLUSIVE_API explicit
								Device ( intptr_t module_handle, intptr_t module_window );
		// Non-exposed API for destroying device.
		GPUW_EXLUSIVE_API		~Device ( void );

		// Non-exposed API for initializing the device. (before OutputSurface ready)
		GPUW_EXLUSIVE_API int	create ( DeviceLayer* layers, uint32_t layerCount );
		// Non-exposed API for starting up the device. (after OutputSurface ready)
		GPUW_EXLUSIVE_API int	initialize ( OutputSurface* surface );
		// Non-exposed API for refreshing the device
		GPUW_EXLUSIVE_API int	refresh ( intptr_t module_handle, intptr_t module_window );

		// Grabs the graphics context for the device.
		GPUW_API GraphicsContext*
								getContext ( void );
		// Grabs the first compute-enabled context for the device.
		GPUW_API ComputeContext*
								getComputeContext ( void );

	private:
		friend OutputSurface;
		intptr_t			mw_module;
		intptr_t			mw_window;
		intptr_t			mw_deviceContext;
		intptr_t			mw_renderContext;

		GraphicsContext*	m_graphicsContext;
		ComputeContext*		m_computeContext;

		DeviceLayer*		m_layers;
		uint32_t			m_layerCount;
	};

	GPUW_API Device* getDevice ( void );

}

#endif//GPU_WRAPPER_BUFFERS_H_