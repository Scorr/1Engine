#ifndef CORE_GFX_TEXTURE_FORMATS_H_
#define CORE_GFX_TEXTURE_FORMATS_H_

#include "core/types/types.h"
#include "core/debug.h" // todo: remove?

namespace core {
namespace gfx {
namespace tex {

	// Enumerations for the texture type
	enum arTextureType : uint8_t
	{
		kTextureTypeNone = 0,

		kTextureType1D		= 0x01,
		kTextureType2D		= 0x02,
		kTextureType3D		= 0x03,
		kTextureTypeCube	= 0x04,

		// Odd, not quite supported formats:
		kTextureType1DArray		= 0x05,
		kTextureType2DArray		= 0x06,
		kTextureTypeCubeArray	= 0x07,
	};

	// Enumerations for the internal format
	enum arColorFormat : uint8_t
	{
		//
		// RGBA Color formats:

		kColorFormatNone = 0,

		__COLOR_FORMAT_RGB_MIN,
		kColorFormatRGB8,
		kColorFormatRGB16,
		kColorFormatRGB16F,
		kColorFormatRGB32,
		kColorFormatRGB32F,
		__COLOR_FORMAT_RGB_MAX,

		__COLOR_FORMAT_RGBA_MIN,
		kColorFormatRGBA8,
		kColorFormatRGBA8UI,
		kColorFormatRGBA16,
		kColorFormatRGBA16UI,
		kColorFormatRGBA16F,
		kColorFormatRGBA32,
		kColorFormatRGBA32F,
		__COLOR_FORMAT_RGBA_MAX,

		__COLOR_FORMAT_RG_MIN,
		kColorFormatRG8,
		kColorFormatRG16,
		kColorFormatRG16F,
		kColorFormatRG32,
		kColorFormatRG32UI,
		kColorFormatRG32F,
		__COLOR_FORMAT_RG_MAX,

		__COLOR_FORMAT_R_MIN,
		kColorFormatR8,
		kColorFormatR16,
		kColorFormatR16F,
		kColorFormatR32,
		kColorFormatR32F,
		__COLOR_FORMAT_R_MAX,

		__COLOR_FORMAT_MAX,

		//
		// Depth formats:

		kDepthFormatNone = __COLOR_FORMAT_MAX,

		kDepthFormat16,
		kDepthFormat24,
		kDepthFormat32,
		// What is even the difference between this and 32?
		kDepthFormat32F,

		__DEPTH_FORMAT_MAX,

		//
		// Stencil formats:

		kStencilFormatNone = __DEPTH_FORMAT_MAX,

		KStencilFormatIndex1,
		KStencilFormatIndex4,
		KStencilFormatIndex8, // This is the only supported value on textures
		KStencilFormatIndex16,

		kDepthFormat32FStencil8,
		kDepthFormat24Stencil8,

		__STENCIL_FORMAT_MAX,
	};

	// Enumerations for the texture wrap
	enum arWrappingType : uint8_t
	{
		kWrappingRepeat,
		kWrappingClamp,
		kWrappingBorder,
		kWrappingMirroredRepeat,
	};
	// Enumerations for import options
	enum arOrientation : uint8_t
	{
		kOrientationNoFlip,
		kOrientationFlip,
	};
	// Enumerations for mipmap generation options
	enum arMipmapGenerationStyle : uint8_t
	{
		kMipmapGenerationNormal = 0,
		kMipmapGenerationNone,
		kMipmapGenerationNearest
	};
	// Enumeration for the texture class
	enum arTextureClass : uint8_t
	{
		kTextureClassBase,
		kTextureClass3D,
		kTextureClassFont,
		kTextureClassCube,
		kTextureClassRenderTarget,
		kTextureClassRenderTarget_Cube,
		kTextureClassRenderTarget_MRT,
	};
	// Enumeration for texture sampling
	enum arSamplingFilter : uint8_t
	{
		kSamplingPoint = 0,
		kSamplingLinear,
		//kSamplingAnisotropic,
	};

	//	getTextureFormatByteSize(type) : Given a format, returns the byte width of a single texel.
	static size_t getColorFormatByteSize (arColorFormat format) // todo: move to CPP?
	{
		switch (format)
		{
		case kColorFormatRGB8:		return 3;
		case kColorFormatRGB16:		return 6;
		case kColorFormatRGB16F:	return 6;
		case kColorFormatRGB32:		return 12;
		case kColorFormatRGB32F:	return 12;

		case kColorFormatRGBA8:		return 4;
		case kColorFormatRGBA8UI:	return 4;
		case kColorFormatRGBA16:	return 8;
		case kColorFormatRGBA16UI:	return 8;
		case kColorFormatRGBA16F:	return 8;
		case kColorFormatRGBA32:	return 16;
		case kColorFormatRGBA32F:	return 16;

		case kColorFormatRG8:		return 2;
		case kColorFormatRG16:		return 4;
		case kColorFormatRG16F:		return 4;
		case kColorFormatRG32:		return 8;
		case kColorFormatRG32UI:	return 8;
		case kColorFormatRG32F:		return 8;

		case kColorFormatR8:		return 1;
		case kColorFormatR16:		return 2;
		case kColorFormatR16F:		return 2;
		case kColorFormatR32:		return 4;
		case kColorFormatR32F:		return 4;
		}
		ARCORE_ERROR("Invalid or unsupported type passed.");
		return 0;
	}

}}}

#endif//CORE_GFX_TEXTURE_FORMATS_H_