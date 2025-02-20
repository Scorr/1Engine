//===============================================================================================//
//
//		CInput - Input backend
//
// Input backend. Forwards input that is captured from the OS's message loop.
// On Windows, mouse input is hardware based. Unless configured, it does not correspond to the
// system cursor.
//
//===============================================================================================//
#ifndef CORE_INPUT_H_
#define CORE_INPUT_H_

#include "core/types.h"
#include "core/containers/arsingleton.h"
#include "core/math/Vector2.h"
#include "core/math/vect2d_template.h"
#include "core/debug.h"

#include <vector>
#include <array>

namespace core
{
	enum VirtualKey : int
	{
		kVkCount = 256,

		kVkBack			= 0x08,
		kVkBackspace	= 0x08,
		kVkTab			= 0x09,
		kVkReturn		= 0x0D,
		kVkEscape		= 0x1B,

		kVkSpace		= 0x20,
		kVkPageUp		= 0x21,
		kVkPageDown		= 0x22,
		kVkEnd			= 0x23,
		kVkHome			= 0x24,
		kVkPrintScreen	= 0x2C,
		kVkInsert		= 0x2D,
		kVkDelete		= 0x2E,

		kVkCapital		= 0x14,
		kVkCapslock		= 0x14,

		kVkShift		= 0x10,
		kVkControl		= 0x11,
		kVkAlt			= 0x12,
		kVkPause		= 0x13,

		kVkLeft			= 0x25,
		kVkUp			= 0x26,
		kVkRight		= 0x27,
		kVkDown			= 0x28,

		kVkNumpad0		= 0x60,
		kVkNumpad1		= 0x61,
		kVkNumpad2		= 0x62,
		kVkNumpad3		= 0x63,
		kVkNumpad4		= 0x64,
		kVkNumpad5		= 0x65,
		kVkNumpad6		= 0x66,
		kVkNumpad7		= 0x67,
		kVkNumpad8		= 0x68,
		kVkNumpad9		= 0x69,
		kVkNumpadMultiply	= 0x6A,
		kVkNumpadAdd	= 0x6B,
		kVkNumpadSubtract	= 0x6D,
		kVkNumpadDivide	= 0x6F,
		kVkNumpadDecimal= 0x6E,

		kVkTilde		= 0xC0,

		kVkF1			= 0x70,
		kVkF2			= 0x71,
		kVkF3			= 0x72,
		kVkF4			= 0x73,
		kVkF5			= 0x74,
		kVkF6			= 0x75,
		kVkF7			= 0x76,
		kVkF8			= 0x77,
		kVkF9			= 0x78,
		kVkF10			= 0x79,
		kVkF11			= 0x7A,
		kVkF12			= 0x7B,
	};

	enum VirtualMouseButton : int
	{
		kMBLeft			= 0,
		kMBRight		= 1,
		kMBMiddle		= 2,
		kMBBackward		= 3,
		kMBForward		= 4,

		kMBCount = 16,
	};

	CORE_API extern unsigned char VkToAsciiTable [256];
	extern void InitializeVkToAsciiTable ( void );

	namespace internal_input
	{
		struct /*alignas(1)*/ KeyState // TODO: Fix alignment issue
		{
			uint	key : 1;
			uint	prev : 1;
			uint	down : 1;
			uint	up : 1;

			KeyState()
				: key(false)
				, prev(false)
				, down(false)
				, up(false)
			{
			}

			void PreUpdate ( void )
			{
				prev = key;
				down = false;
				up = false;
			}

			void Make ( void )
			{
				down = true;
				key = true;
			}

			void Break ( void )
			{
				up = true;
				key = false;
			}
		};

		struct KeypressEntry
		{
			uint	code : 8;
			uint	isMake : 1;
		};

		struct InputState
		{
			// Keyboard state
			std::vector<KeypressEntry>
								m_keypressesLastFrame;
			std::string			m_stringLastFrame;
			std::array<KeyState, kVkCount>
								m_keys;

			// Mouse state
			Vector2f			m_mouse;
			Vector2i			m_sysMouse;
			Vector2f			m_deltaMouse;
			Vector2i			m_rawDeltaMouse;
			Vector2i			m_prevRawDeltaMouse;
			bool				m_deltaMouseScrollChange = false;
			int					m_deltaMouseScroll = 0;
			bool				m_deltaMouseZoomChange = false;
			int					m_deltaMouseZoom = 0;
			bool				m_deltaMouseHScrollChange = false;
			int					m_deltaMouseHScroll = 0;
			bool				m_deltaMouseHZoomChange = false;
			int					m_deltaMouseHZoom = 0;

			bool				m_syncRawAndSystemMouse = false;

			std::array<KeyState, kMBCount>
								m_mouseButtons;
		};
	}

	class Input
	{
		ARSINGLETON_H_STORAGE(Input,CORE_API)
		ARSINGLETON_H_ACCESS(Input)

	private:
		explicit				Input ( void ) {}
								~Input ( void ) {}

	public:
		int					m_currentActiveWindow = 0;
		Vector2f			m_mouseSensitivity = Vector2f( 1.0F, 1.0F );

	public:
		//	Initialize() : Initializes input system, allocating memory.
		CORE_API static void	Initialize ( void );
		//	Free() : Frees input system.
		CORE_API static void	Free ( void );

		//	ResizeInputState( count ) : Resizes input state count.
		CORE_API static void	ResizeInputState ( int input_state_count );

		//	PreUpdate() : Resets the input system just before message loop updates.
		CORE_API static void	PreUpdate ( void );
		//	Update() : Updates the input logic after the message loop updates.
		CORE_API static void	Update ( void );
		//	Reset() : Zeroes out all the current input
		CORE_API static void	Reset ( void );

	public:

		//	Key(virtual_keycode) : Checks if the key is currently down.
		CORE_API static bool	Key ( const unsigned char vkeycode_ascii, const int windowIndex = -1 );
		//	Keydown(virtual_keycode) : Checks if the key was pressed in the previous frame.
		CORE_API static bool	Keydown ( const unsigned char vkeycode_ascii, const int windowIndex = -1 );
		//	Keydown(virtual_keycode) : Checks if the key was released in the previous frame.
		CORE_API static bool	Keyup ( const unsigned char vkeycode_ascii, const int windowIndex = -1 );

		//	KeypressAny() : Checks if any key is currently down.
		CORE_API static bool	KeypressAny ( const int windowIndex = -1 );
		//	KeydownAny() : Checks if any key was pressed in the previous frame.
		CORE_API static bool	KeydownAny ( const int windowIndex = -1 );
		//	KeyupAny() : Checks if any key was released in the previous frame.
		CORE_API static bool	KeyupAny ( const int windowIndex = -1 );

		//	FrameInputString() : Returns the sum of all the inputs over the past frame.
		CORE_API static const std::string&
								FrameInputString ( const int windowIndex = -1 );

	public:

		CORE_API static void	WSetKeyMake ( const unsigned char vkeycode_ascii, const int windowIndex );
		CORE_API static void	WSetKeyBreak ( const unsigned char vkeycode_ascii, const int windowIndex );

	public:

		//	MouseX() : Current virtual mouse cursor X position
		CORE_API static float	MouseX ( const int windowIndex = -1 );
		//	MouseY() : Current virtual mouse cursor Y position
		CORE_API static float	MouseY ( const int windowIndex = -1 );

		//	SysMouseX() : Current local system mouse cursor X position
		CORE_API static int		SysMouseX ( const int windowIndex = -1 );
		//	SysMouseY() : Current local system mouse cursor X position
		CORE_API static int		SysMouseY ( const int windowIndex = -1 );

		//	DeltaMouseX() : Current virtual mouse cursor X delta
		CORE_API static float	DeltaMouseX ( const int windowIndex = -1 );
		//	DeltaMouseY() : Current virtual mouse cursor Y delta
		CORE_API static float	DeltaMouseY ( const int windowIndex = -1 );
		//	DeltaMouseScroll() : Current virtual mouse wheel delta.
		CORE_API static int		DeltaMouseScroll ( const int windowIndex = -1 );
		//	DeltaMouseHScroll() : Current virtual mouse wheel delta.
		CORE_API static int		DeltaMouseHScroll ( const int windowIndex = -1 );
		//	DeltaMouseZoom() : Current virtual mouse wheel zoom.;
		CORE_API static int		DeltaMouseZoom ( const int windowIndex = -1 );

		//	Mouse(mousebutton) : Checks if the mouse button is currently down.
		CORE_API static bool	Mouse ( const int mousebutton_id, const int windowIndex = -1 );
		//	MouseDown(mousebutton) : Checks if the mouse button was pressed in the previous frame.
		CORE_API static bool	MouseDown ( const int mousebutton_id, const int windowIndex = -1 );
		//	MouseUp(mousebutton) : Checks if the mouse button was released in the previous frame.
		CORE_API static bool	MouseUp ( const int mousebutton_id, const int windowIndex = -1 );

		//	SetMouseSensitivity(sensitivity) : Sets virtual mouse sensitivity.
		CORE_API static void	SetMouseSensitivity ( const Real sensitivity )
		{
			m_Active->m_mouseSensitivity = {sensitivity, sensitivity};
		}
		//	GetMouseSensitivity() : Returns current mouse sensitivity
		CORE_API static Real	GetMouseSensitivity ( void )
		{
			return m_Active->m_mouseSensitivity.x;
		}

	public:

		CORE_API static void	WSetSysMouse ( const Vector2i mouse, const int windowIndex );
		CORE_API static void	WAddRawMouse ( const Vector2i deltamouse, const int windowIndex );
		CORE_API static void	WSetCurrMouseScroll ( const int mousew, const int windowIndex );
		CORE_API static void	WSetCurrMouseZoom ( const int mousew, const int windowIndex );
		CORE_API static void	WSetCurrMouseHScroll ( const int mousew, const int windowIndex );
		CORE_API static void	WSetCurrMouseHZoom ( const int mousew, const int windowIndex );

		CORE_API static void	WSetMouseMake ( const int mousebutton_id, const int windowIndex );
		CORE_API static void	WSetMouseBreak ( const int mousebutton_id, const int windowIndex );

		CORE_API static void	WSetSyncRawAndSystemMouse ( const bool sync, const int windowIndex );

	};
}

#endif//CORE_INPUT_H_