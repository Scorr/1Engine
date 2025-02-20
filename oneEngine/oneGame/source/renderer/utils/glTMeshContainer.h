//===============================================================================================//
// todo
//
//===============================================================================================//

#ifndef _GL_TERRAIN_MESH_CONTAINER_H_
#define _GL_TERRAIN_MESH_CONTAINER_H_

#include "core/types/types.h"
#include "core/types/ModelData.h"
#include "renderer/types/types.h"
#include "renderer/object/RrRenderObject.h"

namespace renderer
{
	class glTMeshContainer
	{
	public:
		RENDER_API explicit glTMeshContainer ( void );
		RENDER_API			~glTMeshContainer ( void );

		RENDER_API void UploadVerts ( const arTerrainVertex* verts, const size_t vert_count );
		RENDER_API void UploadFaces ( const arModelTriangle* faces, const size_t face_count );

		RENDER_API FORCE_INLINE const glHandle GetVBOverts ( void ) const
		{
			return vbo_verts;
		}
		RENDER_API FORCE_INLINE const glHandle GetVBOfaces ( void ) const
		{
			return vbo_faces;
		}
		RENDER_API FORCE_INLINE const uint GetElementCount ( void ) const 
		{
			return element_count;
		}
	private:
		glHandle	vbo_verts;
		glHandle	vbo_faces;
		uint		element_count;
	};
};

#endif//_GL_TERRAIN_MESH_CONTAINER_H_