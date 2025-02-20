#ifndef CORE_GFX_TEXTURE_STRUCTURES_H_
#define CORE_GFX_TEXTURE_STRUCTURES_H_

#include "core/types/types.h"
#include "core/gfx/textureFormats.h"

namespace core {
namespace gfx {
namespace tex {

	//===============================================================================================//
	// Texture info structures
	//===============================================================================================//

	// Texture info struct
	struct arTextureInfo
	{	
		arTextureType	type;
		arColorFormat	internalFormat;
		arMipmapGenerationStyle	mipmapStyle;
		arSamplingFilter	filter; // filter for the texture's default sampler (can be overridden by other filter objects)

		uint32_t	width;
		uint32_t	height;
		uint32_t	depth;
		uint32_t	levels;	// mipmap levels, often mirrors in arTextureState

		arWrappingType	repeatX;
		arWrappingType	repeatY;
		arWrappingType	repeatZ;

		uint32_t	userdata;
		void*		userpdata;

		arTextureInfo ( void )
			:
			type(kTextureTypeNone), internalFormat(kColorFormatNone), mipmapStyle(kMipmapGenerationNone), filter(kSamplingLinear),
			width(0), height(0), depth(0), levels(0),
			repeatX(kWrappingRepeat), repeatY(kWrappingRepeat), repeatZ(kWrappingRepeat),
			userdata(0), userpdata(NULL)
			{ ; }
	};

	// Texture state struct
	struct arTextureState
	{
		// current starting mipmap level
		uint32_t	level_base;
		// current max mipmap level
		uint32_t	level_max;
	};

	// IMAGE INFO
	struct arImageInfo
	{
		// Image size format information:

		uint16_t	width;
		uint16_t	height;
		uint16_t	depth; // usually 1
		uint8_t		levels;
		arTextureType
					type = kTextureType2D;
	};

	// Image animation info
	struct arAnimationInfo
	{
		uint16_t	framecount;
		uint8_t		xdivs;	
		uint8_t		ydivs;

		uint16_t	framerate;	// Base framerate. If 0, assumes 60.
	};

	// Sprite info
	struct arSpriteInfo
	{
		arImageInfo	info;
		arAnimationInfo
					animationInfo;
		uint16_t*	frame_times;
	};

}}}

#endif//CORE_GFX_TEXTURE_STRUCTURES_H_