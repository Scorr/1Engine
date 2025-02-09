#ifndef RENDERER_RENDER_TEXTURE_CUBE_H_
#define RENDERER_RENDER_TEXTURE_CUBE_H_

class RrRenderTextureCube : public RrRenderTexture
{
};

#if 0
// ==Includes==
#include "core/math/Vector3.h"
// RrRenderTexture class
#include "RrRenderTexture.h"
// RrTextureCube class
//#include "RrTextureCube.h"

// ===CLASS===
// Class definition
class RrRenderTextureCube : public RrRenderTexture
{
	TextureType( TextureClassRenderTarget_Cube );
protected:
	explicit RrRenderTextureCube ( const string& sInFilename )
		: RrRenderTexture( sInFilename )
	{
		;
	}
public:
	explicit RrRenderTextureCube ( 
		eTextureType	textureType		= TextureCube,
		eColorFormat	format			= RGBA8,
		unsigned int	maxTextureWidth	= 1024,
		unsigned int	maxTextureHeight= 1024,
		eDepthFormat	depthType		= DepthNone,
		bool			depthFetch		= false,
		bool			ignoreRGB		= false,
		eStencilFormat	stencilType		= StencilNone,
		bool			stencilFetch	= false
		);
	~RrRenderTextureCube ( void );

	// BindBufferFace ( face ) : Bind face to render to texture
	//  Because of the way cubemaps work, only one cubemap binding should be on the stack at the same time.
	//  Any other usage will likely not work.
	virtual void BindBufferFace ( const glHandle );

	// UnbindBufferFace ( face ) : Unbind face to render to texture
	virtual void UnbindBufferFace ( const glHandle );

	// Unbind All
	//static void UnbindBuffer ( char );

	// Public Offtexture binding
	//void BindDepth ( void );
	//void BindStencil ( void );

	// Public Offtexture grabbing
	/*GLuint GetDepthSampler ( void ) const {
		return rtInfo.depthtex;
	}
	GLuint GetStencilSampler ( void ) const {
		return rtInfo.stenciltex;
	}*/

	// Public copy generation
	RrRenderTexture* GenerateCopy ( void ) override;

	void Reload ( void ) override {
		;
	}

	// Public property queries
	/*eColorFormat GetInternalFormat ( void ) const {
		return info.internalFormat;
	};
	// State queue
	const tRenderTargetInfo& GetRTInfo ( void ) const {
		return rtInfo;
	}*/
public:
	// Is set to the position of rendering when used as an RT.
	Vector3f m_renderPosition;

protected:
	// Private routines
	void GenerateFramebuffer ( void ) override;

protected:
	//tRenderTargetInfo	rtInfo;
	//string rtUniqueSId;

protected:
	//static std::stack<unsigned int> buffer_stack;
};
#endif
#endif//RENDERER_RENDER_TEXTURE_CUBE_H_
