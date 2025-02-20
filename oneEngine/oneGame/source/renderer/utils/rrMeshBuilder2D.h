#ifndef RENDERER_UTILS_RR_MESH_BUILDER_2D_H_
#define RENDERER_UTILS_RR_MESH_BUILDER_2D_H_

#include "IrrMeshBuilder.h"

#include "core/math/Rect.h"
#include "core/math/Color.h"
#include "core/math/Cubic.h"
#include "core/math/vect2d_template.h"

class rrMeshBuilder2D : public IrrMeshBuilder
{
public:
	//	Constructor (new data)
	// Pulls a model from the the pool that has at least the estimated input size.
	// If the estimation is incorrect, the data will be resized.
	// The screen mapping for the meshes created defaults to 1:1 pixel-mode mapping.
	// To assist in estimation, there are 4 vertices used per quad, and 16 per outlined quad.
	RENDER_API explicit		rrMeshBuilder2D ( const Vector2i& screenSize, const uint16_t estimatedVertexCount );
	//	Constructor (existing data)
	// Sets up model, using the input data.
	// As above, will re-allocate if the data is small, but will do so extremely conservatively (slowly).
	// The screen mapping for the meshes created defaults to 1:1 pixel-mode mapping.
	RENDER_API explicit		rrMeshBuilder2D ( const Vector2i& screenSize, arModelData* preallocatedModelData, uint16_t initialVertexCount = 0, uint16_t initialIndexCount = 0 );
	//	Constructor (cubic, new data)
	// Pulls a model from the the pool that has at least the estimated input size.
	// If the estimation is incorrect, the data will be resized.
	// To assist in estimation, there are 4 vertices used per quad, and 16 per outlined quad.
	RENDER_API explicit		rrMeshBuilder2D ( const core::math::Cubic& screenMapping, const uint16_t estimatedVertexCount );
	//	Constructor (cubic, existing data)
	// Sets up model, using the input data.
	// As above, will re-allocate if the data is small, but will do so extremely conservatively (slowly).
	RENDER_API explicit		rrMeshBuilder2D ( const core::math::Cubic& screenMapping, arModelData* preallocatedModelData, uint16_t initialVertexCount = 0, uint16_t initialIndexCount = 0 );

	//	getPrimitiveMode () : returns the primitive mode this mesh builder would like to render in.
	// Must be implemented by child classes.
	RENDER_API renderer::rrPrimitiveMode
							getPrimitiveMode ( void ) const override
		{ return renderer::kPrimitiveModeTriangleList_Indexed; }

	//	setScreenMapping (cubic) : Sets the mapping of the Rect coordinates to the screen.
	RENDER_API void			setScreenMapping ( const core::math::Cubic& screenMapping );
	//	getScreenMapping(out multiplier, out offset) : Gets the internal screen mapping
	RENDER_API void			getScreenMapping ( Vector2f& out_multiplier, Vector2f& out_offset );

	//	addRect (rect, color, outline) : Adds a rectangle to draw.
	// "Wireframe" is done via four thin quads, inset by what is calculated to be one pixel.
	RENDER_API void			addRect ( const Rect& rect, const Color& color, bool outline, Real outlineWidth = 1.0F );
	//	addRectTex (rect, tex, color, outline) : Adds a rectangle to draw, but with the specified texcoord0.
	// "Wireframe" is done via four thin quads, inset by what is calculated to be one pixel.
	RENDER_API void			addRectTex ( const Rect& rect, const Rect& tex, const Color& color, bool outline, Real outlineWidth = 1.0F );

	//	addLine (point1, point2, color) : Adds a line to draw.
	// The "line" is actually a very thin quad, with a width of what is calculated to be one pixel.
	RENDER_API void			addLine ( const Vector2f& point1, const Vector2f& point2, const Color& color );

	//	addCircle (center, radius, color, divs, outline) : Adds a circle to draw.
	// "Wireframe" is done via as many thin quads as divs, inset by what is calculated to be one pixel.
	// The argument "divs" refers to how many unique angles will be used to build the circle.
	// A 20-div circle adds 20 triangles at 18 degree intervals (360 degrees divided by 20).
	RENDER_API void			addCircle ( const Vector2f& center, const Real radius, const Color& color, int divs, bool outline );

	//	addQuad (vertices [4]) : Adds a quad to draw.
	// "Wireframe" is done via four thin quads, inset by what is calculated to be one pixel. It may not be correct.
	// Vertex order is done in the "Z" mesh order - similar to trianglestrip.
	RENDER_API void			addQuad ( const arModelVertex* vertices, bool outline );

protected:
	Vector2f	m_multiplier;
	Vector2f	m_offset;
};

#endif//RENDERER_UTILS_RR_MESH_BUILDER_2D_H_