#include "RrShapePlane.h"
#include "gpuw/Device.h"
#include "gpuw/GraphicsContext.h"
#include "renderer/material/Material.h"

rrMeshBuffer RrShapePlane::m_MeshBuffer;
void RrShapePlane::BuildMeshBuffer ( void )
{
	if (m_MeshBuffer.m_mesh_uploaded == false)
	{
		const Real hxsize = 0.5F;
		const Real hysize = 0.5F;

		arModelData model;

		model.position = new Vector3f [4];
		model.normal = new Vector3f [4];
		model.tangent = new Vector3f [4];
		model.color = new Vector4f [4];
		model.texcoord0 = new Vector3f [4];
		model.vertexNum = 4;

		model.indices = new uint16_t [6];
		model.indexNum = 6;

		// set commons
		for ( uint i = 0; i < 4; i += 1 )
		{
			model.normal[i] = Vector3f(0, 0, 1.0F);
			model.color[i] = Vector4f(1.0F, 1.0F, 1.0F, 1.0F);
			model.tangent[i] = Vector3f(1.0F, 0, 0);
		}

		// Set UVs:
		model.texcoord0[0] = Vector2f(0, 0);
		model.texcoord0[1] = Vector2f(1, 0);
		model.texcoord0[2] = Vector2f(0, 1);
		model.texcoord0[3] = Vector2f(1, 1);

		// Set triangles:
		model.indices[0] = 0;
		model.indices[1] = 2;
		model.indices[2] = 1;

		model.indices[3] = 2;
		model.indices[4] = 3;
		model.indices[5] = 1;

		// Set new positions
		model.position[0] = Vector2f(-hxsize, -hysize);
		model.position[1] = Vector2f( hxsize, -hysize);
		model.position[2] = Vector2f(-hxsize,  hysize);
		model.position[3] = Vector2f( hxsize,  hysize);

		// Model is created, we upload:
		m_MeshBuffer.InitMeshBuffers(&model);
		m_MeshBuffer.m_modeldata = NULL;

		// Free the CPU model data:
		delete_safe_array(model.position);
		delete_safe_array(model.normal);
		delete_safe_array(model.tangent);
		delete_safe_array(model.color);
		delete_safe_array(model.texcoord0);

		delete_safe_array(model.indices);
	}
}

RrShapePlane::RrShapePlane ( void )
	: RrRenderObject()
{
	BuildMeshBuffer();
}

bool RrShapePlane::CreateConstants ( rrCameraPass* cameraPass )
{
	// Set up transformation for the mesh
	PushCbufferPerObject(this->transform.world, cameraPass);
	return true;
}

// Render the mesh
bool RrShapePlane::Render ( const rrRenderParams* params )
{
	gpu::GraphicsContext* gfx = params->context->context_graphics;

	gpu::Pipeline* pipeline = GetPipeline( params->pass );
	gfx->setPipeline(pipeline);
	// Set up the material helper...
	renderer::Material(this, params->context, params, pipeline)
		// set the depth & blend state registers
		.setDepthStencilState()
		.setRasterizerState()
		// bind the samplers & textures
		.setBlendState()
		.setTextures()
		// execute callback
		.executePassCallback();

	// bind the vertex buffers
	auto passAccess = PassAccess(params->pass);
	for (int i = 0; i < passAccess.getVertexSpecificationCount(); ++i)
	{
		int buffer_index = (int)passAccess.getVertexSpecification()[i].location;
		int buffer_binding = (int)passAccess.getVertexSpecification()[i].binding;
		if (m_MeshBuffer.m_bufferEnabled[buffer_index])
			gfx->setVertexBuffer(buffer_binding, &m_MeshBuffer.m_buffer[buffer_index], 0);
	}

	// bind the index buffer
	gfx->setIndexBuffer(&m_MeshBuffer.m_indexBuffer, gpu::kIndexFormatUnsigned16);
	// bind the cbuffers: TODO
	gfx->setShaderCBuffer(gpu::kShaderStageVs, renderer::CBUFFER_PER_OBJECT_MATRICES, &m_cbufPerObjectMatrices);
	gfx->setShaderCBuffer(gpu::kShaderStagePs, renderer::CBUFFER_PER_OBJECT_MATRICES, &m_cbufPerObjectMatrices);
	gfx->setShaderCBuffer(gpu::kShaderStageVs, renderer::CBUFFER_PER_OBJECT_EXTENDED, &m_cbufPerObjectSurfaces[params->pass]);
	gfx->setShaderCBuffer(gpu::kShaderStagePs, renderer::CBUFFER_PER_OBJECT_EXTENDED, &m_cbufPerObjectSurfaces[params->pass]);
	gfx->setShaderCBuffer(gpu::kShaderStageVs, renderer::CBUFFER_PER_CAMERA_INFORMATION, params->cbuf_perCamera);
	gfx->setShaderCBuffer(gpu::kShaderStagePs, renderer::CBUFFER_PER_CAMERA_INFORMATION, params->cbuf_perCamera);
	gfx->setShaderCBuffer(gpu::kShaderStageVs, renderer::CBUFFER_PER_PASS_INFORMATION, params->cbuf_perPass);
	gfx->setShaderCBuffer(gpu::kShaderStagePs, renderer::CBUFFER_PER_PASS_INFORMATION, params->cbuf_perPass);
	gfx->setShaderCBuffer(gpu::kShaderStageVs, renderer::CBUFFER_PER_FRAME_INFORMATION, params->cbuf_perFrame);
	gfx->setShaderCBuffer(gpu::kShaderStagePs, renderer::CBUFFER_PER_FRAME_INFORMATION, params->cbuf_perFrame);
	// draw now
	//gfx->drawIndexed(m_MeshBuffer.m_modeldata->indexNum, 0, 0);
	gfx->drawIndexed(6, 0, 0);

	return true;
}