#ifndef RENDERER_RENDERABLE_SHAPE_ISOSPHERE_H_
#define RENDERER_RENDERABLE_SHAPE_ISOSPHERE_H_

#include "renderer/object/CRenderable3D.h"

class RrShapeIsosphere : public RrRenderObject
{
public:
	// Constructor and Destructor
	RENDER_API explicit		RrShapeIsosphere ( const int divisions = 1 );
	RENDER_API				~RrShapeIsosphere ( void );

	//		Render()
	// Renders the shape.
	RENDER_API bool			Render ( const rrRenderParams* params ) override;

protected:
	void					BuildMeshBuffer ( const int divisions );

	rrMeshBuffer		m_meshBuffer;
	uint				m_indexCount;
};

#endif//RENDERER_RENDERABLE_SHAPE_ISOSPHERE_H_