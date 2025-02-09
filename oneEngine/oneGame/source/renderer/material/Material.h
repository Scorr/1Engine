#ifndef RENDERER_MATERIAL_USABILITY_LAYER_H_
#define RENDERER_MATERIAL_USABILITY_LAYER_H_

#include "RrPass.h"
#include "renderer/texture/RrTexture.h"
#include "renderer/object/RrRenderObject.h"
#include "gpuw/GraphicsContext.h"

namespace gpu
{
	class GraphicsContext;
	class Pipeline;
}

namespace renderer
{
	//	Material usability layer.
	// Utility class that consolidates many common shader & pass based functionality.
	class Material
	{
	public:

		RENDER_API explicit Material ( RrRenderObject* thisObject, rrRenderContext* ctx, const RrRenderObject::rrRenderParams* passParams, gpu::Pipeline* pipeline )
		{
			m_object = thisObject;
			m_ctx = ctx;
			m_pipeline = pipeline;
			m_pass = &thisObject->m_passes[passParams->pass];

			m_renderPassType = passParams->pass_type;
		}

		RENDER_API explicit Material ( RrRenderObject* thisObject, rrRenderContext* ctx, int32 pass, rrPassType passType, gpu::Pipeline* pipeline )
		{
			m_object = thisObject;
			m_ctx = ctx;
			m_pipeline = pipeline;
			m_pass = &thisObject->m_passes[pass];

			m_renderPassType = passType;
		}

		RENDER_API Material& setAll ( RrRenderObject::rrRenderParams* thisParams )
		{
			return setStart().setDepthStencilState().setRasterizerState().setBlendState().setTextures().setCommonCBuffers(thisParams);
		}

		RENDER_API Material& setStart ( void )
		{
			m_ctx->context_graphics->setPipeline(m_pipeline);
			return *this;
		}

		RENDER_API Material& setDepthStencilState ( void )
		{
			if (m_pass->m_overrideDepth)
			{
				gpu::DepthStencilState ds;
				ds.depthWriteEnabled = m_pass->m_overrideDepthWrite;
				ds.depthTestEnabled = true;
				ds.depthFunc = m_pass->m_overrideDepthTest;
				ds.stencilTestEnabled = false;

				m_ctx->context_graphics->setDepthStencilState(ds);
			}
			// If translucent, set to the default less-equal
			else if (m_pass->isTranslucent())
			{
				gpu::DepthStencilState ds;
				ds.depthWriteEnabled = false;
				ds.depthTestEnabled = true;
				ds.depthFunc = gpu::kCompareOpLessEqual;
				ds.stencilTestEnabled = false;

				m_ctx->context_graphics->setDepthStencilState(ds);
			}
			// If opaque and doing depth pass, render depth normally.
			else if (m_renderPassType == kPassTypeSystemDepth)
			{
				gpu::DepthStencilState ds;
				ds.depthWriteEnabled = true;
				ds.depthTestEnabled = true;
				ds.depthFunc = gpu::kCompareOpLessEqual;
				ds.stencilTestEnabled = false;

				m_ctx->context_graphics->setDepthStencilState(ds);
			}
			// If opaque, set to default depth-equal
			else
			{
				gpu::DepthStencilState ds;
				ds.depthWriteEnabled = false;
				ds.depthTestEnabled = true;
				ds.depthFunc = gpu::kCompareOpEqual;
				ds.stencilTestEnabled = false;

				m_ctx->context_graphics->setDepthStencilState(ds);
			}
			return *this;
		}

		RENDER_API Material& setRasterizerState ( void )
		{
			gpu::RasterizerState rs;
			rs.cullmode = m_pass->m_cullMode;
			/*rs.fillmode = gpu::kFillModeSolid;
			rs.frontface = gpu::kFrontFaceCounterClockwise;
			rs.scissorEnabled = true;*/

			m_ctx->context_graphics->setRasterizerState(rs);
			return *this;
		}

		RENDER_API Material& setBlendState ( void )
		{
			gpu::BlendState bs;
			if (m_renderPassType == kPassTypeSystemDepth)
			{
				bs.channelMask = 0x00;
				bs.enable = false;
			}
			else
			{
				bs.channelMask = 0xFF;
				if (m_pass->m_blendMode == renderer::kHLBlendModeNone)
				{
					bs.enable = (m_pass->m_alphaMode == renderer::kAlphaModeTranslucent) ? true : false;
					if (m_pass->m_alphaMode == renderer::kAlphaModeTranslucent)
					{
						bs.dst = gpu::kBlendModeInvSrcAlpha;
						bs.src = gpu::kBlendModeSrcAlpha;
						bs.op = gpu::kBlendOpAdd;

						bs.dstAlpha = gpu::kBlendModeOne;
						bs.srcAlpha = gpu::kBlendModeOne;
						bs.opAlpha = gpu::kBlendOpAdd;
					}
				}
				else
				{
					bs.enable = true;
					switch (m_pass->m_blendMode)
					{
					case renderer::kHLBlendModeNormal:
						bs.dst = gpu::kBlendModeInvSrcAlpha;
						bs.src = gpu::kBlendModeSrcAlpha;
						bs.op = gpu::kBlendOpAdd;

						bs.dstAlpha = gpu::kBlendModeOne;
						bs.srcAlpha = gpu::kBlendModeOne;
						bs.opAlpha = gpu::kBlendOpAdd;
						break;

					case renderer::kHLBlendModeAdd:
						bs.dst = gpu::kBlendModeOne;
						bs.src = gpu::kBlendModeOne;
						bs.op = gpu::kBlendOpAdd;

						bs.dstAlpha = gpu::kBlendModeOne;
						bs.srcAlpha = gpu::kBlendModeOne;
						bs.opAlpha = gpu::kBlendOpAdd;
						break;

					case renderer::kHLBlendModeMultiplyX2:
						bs.dst = gpu::kBlendModeSrcColor;
						bs.src = gpu::kBlendModeDstColor;
						bs.op = gpu::kBlendOpAdd;

						bs.dstAlpha = gpu::kBlendModeOne;
						bs.srcAlpha = gpu::kBlendModeOne;
						bs.opAlpha = gpu::kBlendOpAdd;
						break;

					default: // Not yet implemented
						ARCORE_ERROR("Not yet implemented");
					}
				}
			}

			m_ctx->context_graphics->setBlendState(bs);
			return *this;
		}

		RENDER_API Material& setTextures ( void )
		{
			for (int i = 0; i < kPass_MaxTextureSlots; ++i)
			{
				if (m_pass->m_texturesRaw[i] != NULL)
				{
					if (m_pass->m_samplers[i] != NULL)
					{
						m_ctx->context_graphics->setShaderSampler(gpu::kShaderStagePs, i, m_pass->m_samplers[i]);
						m_ctx->context_graphics->setShaderTexture(gpu::kShaderStagePs, i, m_pass->m_texturesRaw[i]);
					}
					else
						m_ctx->context_graphics->setShaderTextureAuto(gpu::kShaderStagePs, i, m_pass->m_texturesRaw[i]);
				}
				else if (m_pass->m_textures[i] != NULL)
				{
					if (m_pass->m_samplers[i] != NULL)
					{
						m_ctx->context_graphics->setShaderSampler(gpu::kShaderStagePs, i, m_pass->m_samplers[i]);
						m_ctx->context_graphics->setShaderTexture(gpu::kShaderStagePs, i, &m_pass->m_textures[i]->GetTexture());
					}
					else
						m_ctx->context_graphics->setShaderTextureAuto(gpu::kShaderStagePs, i, &m_pass->m_textures[i]->GetTexture());
				}
			}
			return *this;
		}

		RENDER_API Material& setCommonCBuffers ( RrRenderObject::rrRenderParams* thisParams )
		{
			m_ctx->context_graphics->setShaderCBuffer(gpu::kShaderStageVs, renderer::CBUFFER_PER_OBJECT_MATRICES, &m_object->m_cbufPerObjectMatrices);
			m_ctx->context_graphics->setShaderCBuffer(gpu::kShaderStageVs, renderer::CBUFFER_PER_OBJECT_EXTENDED, &m_object->m_cbufPerObjectSurfaces[thisParams->pass]);
			m_ctx->context_graphics->setShaderCBuffer(gpu::kShaderStageVs, renderer::CBUFFER_PER_CAMERA_INFORMATION, thisParams->cbuf_perCamera);
			m_ctx->context_graphics->setShaderCBuffer(gpu::kShaderStageVs, renderer::CBUFFER_PER_PASS_INFORMATION, thisParams->cbuf_perPass);
			m_ctx->context_graphics->setShaderCBuffer(gpu::kShaderStageVs, renderer::CBUFFER_PER_FRAME_INFORMATION, thisParams->cbuf_perFrame);
			return *this;
		}

		RENDER_API Material& execute ( rrMaterialRenderFunction function )
		{
			if (function != NULL)
			{
				function(this);
			}
			return *this;
		}

		RENDER_API Material& executePassCallback ( void )
		{
			if (m_pass->m_renderCallback != NULL)
			{
				m_pass->m_renderCallback(m_ctx);
			}
			return *this;
		}

	public:
		RrRenderObject*
							m_object;
		rrRenderContext*	m_ctx;
		gpu::Pipeline*		m_pipeline;
		RrPass*				m_pass;
		rrPassType			m_renderPassType;
	};
};


#endif//RENDERER_MATERIAL_USABILITY_LAYER_H_