#ifndef M04_EDITORS_SEQUENCE_EDITOR_PROPERTY_RENDERER_FLOAT_H_
#define M04_EDITORS_SEQUENCE_EDITOR_PROPERTY_RENDERER_FLOAT_H_

#include "./IPropertyRenderer.h"

namespace m04 {
namespace editor {
namespace sequence {

	class FloatPropertyRenderer : public IPropertyRenderer
	{
	public:
		explicit				FloatPropertyRenderer ( const CreationParameters& params )
			: IPropertyRenderer(params)
			{}

	public:

		virtual void			BuildMesh ( void ) override;
		virtual void			OnClicked ( const ui::eventide::Element::EventMouse& mouse_event ) override;
		virtual void			OnGameFrameUpdate ( const ui::eventide::Element::GameFrameUpdateInput& input_frame ) override;

		virtual void			UpdateLayout ( const Vector3f& upper_left_corner, const Real left_column_width, const core::math::BoundingBox& node_bbox ) override;

	};

}}}

#endif//M04_EDITORS_SEQUENCE_EDITOR_PROPERTY_RENDERER_FLOAT_H_
