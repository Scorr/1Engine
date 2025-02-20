#include "RrPipelineStandard.h"

#include "renderer/camera/RrCamera.h"
#include "renderer/state/RrRenderer.h"
#include "renderer/types/shaders/cbuffers.h"
#include "renderer/types/shaders/sbuffers.h"

#include "gpuw/Pipeline.h"
#include "gpuw/ShaderPipeline.h"

void RrPipelineStandardRenderer::DrawWithPipelineAndGBuffers (
	rrRenderContext* context,
	const rrPipelineCompositeInput& compositeInput,
	gpu::Pipeline* pipeline,
	gpu::Buffer* cbuffer,
	gpu::Buffer* sbuffer,
	std::function<void(RrRenderer*, rrRenderContext*)> renderCall)
{
	auto renderer = RrRenderer::Active; // TODO: make argument or class variable
	auto gfx = context->context_graphics;

	gfx->setPipeline(pipeline);
	gfx->setShaderTextureAuto(gpu::kShaderStagePs, 0, compositeInput.deferred_albedo);
	gfx->setShaderTextureAuto(gpu::kShaderStagePs, 1, compositeInput.deferred_normals);
	gfx->setShaderTextureAuto(gpu::kShaderStagePs, 2, compositeInput.deferred_surface);
	gfx->setShaderTextureAuto(gpu::kShaderStagePs, 3, compositeInput.deferred_emissive);
	gfx->setShaderTextureAuto(gpu::kShaderStagePs, 4, compositeInput.combined_depth);
	if (compositeInput.old_forward_color != nullptr)
	{
		gfx->setShaderTextureAuto(gpu::kShaderStagePs, 5, compositeInput.old_forward_color);
	}
	gfx->setShaderCBuffer(gpu::kShaderStageVs, renderer::CBUFFER_PER_CAMERA_INFORMATION, &compositeInput.cameraPass->m_cbuffer);
	gfx->setShaderCBuffer(gpu::kShaderStagePs, renderer::CBUFFER_PER_CAMERA_INFORMATION, &compositeInput.cameraPass->m_cbuffer);
	gfx->setShaderCBuffer(gpu::kShaderStageVs, renderer::CBUFFER_USER0, cbuffer);
	gfx->setShaderCBuffer(gpu::kShaderStagePs, renderer::CBUFFER_USER0, cbuffer);
	gfx->setShaderSBuffer(gpu::kShaderStageVs, renderer::SBUFFER_USER0, sbuffer);
	gfx->setShaderSBuffer(gpu::kShaderStagePs, renderer::SBUFFER_USER0, sbuffer);
	renderCall(renderer, context);
};

void RrPipelineStandardRenderer::CopyRenderTexture (
	gpu::Texture* output_copy,
	gpu::Texture* input_texture,
	gpu::GraphicsContext* gfx,
	RrOutputState* state )
{
	auto renderer = RrRenderer::Active; // TODO: make argument or class variable

	// TODO: probably need something on gfx, ie
	// gfx->copyTexture( output_copy, input_texture );
}