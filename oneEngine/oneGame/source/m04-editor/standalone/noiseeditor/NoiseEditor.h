#ifndef M04_EDITORS_NOISE_EDITOR_MAIN_
#define M04_EDITORS_NOISE_EDITOR_MAIN_

#include <vector>
#include <map>

#include "core-ext/containers/osfstructure.h"

#include "engine/behavior/CGameBehavior.h"
#include "m04/eventide/UserInterface.h"
#include "engine-common/dusk/Dusk.h"

class arStringEnumDefinition;
class RrWindow;
class RrWorld;
class RrCamera;
class RrRenderObject;

class RrTexture;

namespace m04 {
namespace editor {

	namespace noise {
		class AxesGizmo;
		class EditPanel;
	}

	enum class NoiseType
	{
		kPerlin,
		kSimplex,
		kMidpoint,
		kWorleyCell,
		kWorley,
	};

	struct NoiseEditorState
	{
		NoiseType	type = NoiseType::kPerlin;
		bool		is3D = false;

		int			seed = 0;
		int			size = 128;

		bool		clamp_bottom = true;
		bool		clamp_top = false;
		float		total_bias = 0.5F;
		float		total_scale = 0.5F;
		bool		invert_output = false;
		// Does the noise wrap around on the edges?
		bool		wrap = false;

		int			octaves = 3;
		float		frequency = 4.0F;

		int			edit_channel = 0;
		bool		edit_split = false;
	};

	class NoiseEditor : public CGameBehavior
	{
	public:
		EDITOR_API explicit		NoiseEditor ( void );
		EDITOR_API				~NoiseEditor ( void );

		EDITOR_API void			Update ( void );

		NoiseEditorState&		GetState ( void )
			{ return edit_state; }
		void					UpdateNoise ( void );
		void					SaveNoise ( const char* filename );

	protected:
		EDITOR_API void			UpdateViewDrag ( void );

	protected:
		RrWindow*			window = NULL;
		RrWorld*			editor_world = NULL;
		RrCamera*			editor_camera = NULL;

		ui::eventide::UserInterface*
							user_interface = NULL;
		dusk::UserInterface*
							dusk_interface = NULL;

		m04::editor::noise::AxesGizmo*
							axes_gizmo = NULL;
		m04::editor::noise::EditPanel*
							edit_panel = NULL;

		// Editing mode
		bool				is_3d_mode = true;

		// Camera options
		float				camera_zoom = 128.0F;
		Vector3f			camera_panning = Vector3f(0, 0, 0);
		Vector3f			camera_rotation = Vector3f(0, 0, 0);

		// Camera state
		bool				moving_view = false;
		Vector2f			mouse_position_reference;

		NoiseEditorState	edit_state;
		Vector4f*			m_raw_noise = nullptr;
		int					m_raw_noise_size = 0;

		RrTexture*			noise_texture = nullptr;
		RrRenderObject*		preview_primitive_2d = nullptr;
		RrRenderObject*		preview_primitive_3d = nullptr;
	};
}}

#endif//M04_EDITORS_NOISE_EDITOR_MAIN_