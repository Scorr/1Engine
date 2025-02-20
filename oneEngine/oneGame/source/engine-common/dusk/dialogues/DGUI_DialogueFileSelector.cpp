#include "core/input/CInput.h"

#include <algorithm>
#include "DGUI_DialogueFileSelector.h"
//#include "boost/filesystem.hpp"
#include "../CDuskGUI.h"

#include <filesystem>


Dusk::Handle CDuskGUI::DialogueOpenFilename ( System::sFileDialogueEntry* nFiletypes, int nFiletypeCount, const char* nInitialDir, const char* nDialogueTitle )
{
	if ( hCurrentDialogue == -1 )
	{
		vElements.push_back( new Dusk::DialogueFileSelector() );

		Handle handle ( vElements.size()-1 );
		Dusk::DialogueFileSelector* selector = (Dusk::DialogueFileSelector*)*handle;

		selector->io_mode = 0;
		selector->inDialogueMode = true;
		selector->label = nDialogueTitle;
		selector->rect = Rect( 0.1f,0.1f,0.4f,0.8f );
		for ( int i = 0; i < nFiletypeCount; ++i ) {
			selector->m_fileTypes.push_back( nFiletypes[i] );
		}
		selector->m_currentPath = nInitialDir;

		// Set the current dialogue
		hCurrentDialogue = handle;
		forceUpdateRects.push_back( Rect( 0,0, (Real)Screen::Info.width, (Real)Screen::Info.height) );
		// Save the handle
		return handle;
	}

	return -1;
}
Dusk::Handle CDuskGUI::DialogueSaveFilename ( System::sFileDialogueEntry* nFiletypes, int nFiletypeCount, const char* nInitialDir, const char* nDialogueTitle )
{
	if ( hCurrentDialogue == -1 )
	{
		vElements.push_back( new Dusk::DialogueFileSelector() );

		Handle handle ( vElements.size()-1 );
		Dusk::DialogueFileSelector* selector = (Dusk::DialogueFileSelector*)*handle;

		selector->io_mode = 1;
		selector->inDialogueMode = true;
		selector->label = nDialogueTitle;
		selector->rect = Rect( 0.1f,0.1f,0.4f,0.8f );
		for ( int i = 0; i < nFiletypeCount; ++i ) {
			selector->m_fileTypes.push_back( nFiletypes[i] );
		}
		selector->m_currentPath = nInitialDir;

		// Set the current dialogue
		hCurrentDialogue = handle;
		forceUpdateRects.push_back( Rect( 0,0, (Real)Screen::Info.width, (Real)Screen::Info.height) );
		// Save the handle
		return handle;
	}
	return -1;
}

bool CDuskGUI::GetOpenFilename ( char* nOutFilename, const Handle& handleOverride )
{
	forceUpdateRects.push_back( Rect( 0,0, (Real)Screen::Info.width, (Real)Screen::Info.height) );

	if ( hCurrentDialogue >= 0 && hCurrentDialogue->m_type == 40 )
	{
		if ( ((int)handleOverride != -1) && hCurrentDialogue != handleOverride ) {
			return false;
		}
		Dusk::DialogueFileSelector* selector = (Dusk::DialogueFileSelector*)*hCurrentDialogue;
		if ( selector->io_mode != 0 ) {
			return false;
		}
		if ( selector->hasSelection ) {
			strcpy( nOutFilename, (selector->m_currentPath + "/" + selector->m_namebox_value).c_str() );
			DeleteElement( hCurrentDialogue );
			hCurrentDialogue = -1;
			return true;
		}
	}
	return false;
}
bool CDuskGUI::GetSaveFilename ( char* nOutFilename, const Handle& handleOverride )
{
	forceUpdateRects.push_back( Rect( 0,0, (Real)Screen::Info.width, (Real)Screen::Info.height) );

	if ( hCurrentDialogue >= 0 && hCurrentDialogue->m_type == 40 )
	{
		if ( ((int)handleOverride != -1) && hCurrentDialogue != handleOverride ) {
			return false;
		}
		Dusk::DialogueFileSelector* selector = (Dusk::DialogueFileSelector*)*hCurrentDialogue;
		if ( selector->io_mode != 1 ) {
			return false;
		}
		if ( selector->hasSelection ) {
			strcpy( nOutFilename, (selector->m_currentPath + "/" + selector->m_namebox_value).c_str() );
			DeleteElement( hCurrentDialogue );
			hCurrentDialogue = -1;
			return true;
		}
	}
	return false;
}

bool CDuskGUI::OpenDialogueHasSelection ( const Handle& handleOverride )
{
	forceUpdateRects.push_back( Rect( 0,0, (Real)Screen::Info.width, (Real)Screen::Info.height) );

	if ( hCurrentDialogue >= 0 && hCurrentDialogue->m_type == 40 )
	{
		if ( hCurrentDialogue != handleOverride ) {
			return false;
		}
		Dusk::DialogueFileSelector* selector = (Dusk::DialogueFileSelector*)*hCurrentDialogue;
		if ( selector->io_mode != 0 ) {
			return false;
		}
		if ( selector->endMe ) {
			DeleteElement( hCurrentDialogue );
			hCurrentDialogue = -1;
			return false;
		}
		return selector->hasSelection;
	}
	else if ( hCurrentDialogue == -1 ) {
		Handle handle ( handleOverride );
		Dusk::DialogueFileSelector* selector = (Dusk::DialogueFileSelector*)*handle;
		if ( selector && selector->m_type == 40 && selector->endMe ) {
			DeleteElement( handle );
		}
	}
	return false;
}
bool CDuskGUI::SaveDialogueHasSelection ( const Handle& handleOverride )
{
	forceUpdateRects.push_back( Rect( 0,0, (Real)Screen::Info.width, (Real)Screen::Info.height) );

	if ( hCurrentDialogue >= 0 && hCurrentDialogue->m_type == 40 )
	{
		if ( hCurrentDialogue != handleOverride ) {
			return false;
		}
		Dusk::DialogueFileSelector* selector = (Dusk::DialogueFileSelector*)*hCurrentDialogue;
		if ( selector->io_mode != 1 ) {
			return false;
		}
		if ( selector->endMe ) {
			DeleteElement( hCurrentDialogue );
			hCurrentDialogue = -1;
			return false;
		}
		return selector->hasSelection;
	}
	else if ( hCurrentDialogue == -1 ) {
		Handle handle ( handleOverride );
		Dusk::DialogueFileSelector* selector = (Dusk::DialogueFileSelector*)*handle;
		if ( selector && selector->m_type == 40 && selector->endMe ) {
			DeleteElement( handle );
		}
	}
	return false;
}

/// == Dialogue Code ==
Dusk::DialogueFileSelector::DialogueFileSelector ( const int moverride )
	: CDuskGUIDialogue(moverride), hasSelection(false), endMe(false),
	m_mouseover(-1), m_selected(-1), m_namebox_focus(false),
	m_context_showsave(true), m_context_exists(false), m_context_nameconflict(false),
	m_button_mouseover(-1), m_button_mousedown(false)
{
	// Create the child elements here
}


// Overridable update
void Dusk::DialogueFileSelector::Update ( void )
{
	// Set proper cursor position for the situation
	//Vector2f old_cursor_pos = cursor_pos;
	//cursor_pos = Vector2f( core::Input::MouseX(), core::Input::MouseY() );

	// Set the constant Rect size (40% of the screen)
	const Real margin = 10.0F;
	rect = Rect( margin, margin, 512 - margin, 720 - margin * 2 );

	// Set up the subparts

	// Load up the file list
	if ( m_filelist.size() == 0 )
	{
		// Query all files in the folder
		std::tr2::sys::path t_currentPath ( m_currentPath );
		std::tr2::sys::directory_iterator	end_iter;
		// Set the proper path
		m_currentPath = t_currentPath.string();
		if ( m_currentPath[m_currentPath.length()-1] == '\\' || m_currentPath[m_currentPath.length()-1] == '/' ) {
			m_currentPath.resize( m_currentPath.size()-1 );
		}
		
		if ( !std::tr2::sys::exists( t_currentPath ) )
		{
			m_currentPath = "C:\\";
		}
		else
		{
			for( std::tr2::sys::directory_iterator dir_iter( t_currentPath ) ; dir_iter != end_iter ; ++dir_iter)
			{
				filelist_entry_t file;
				file.filename = dir_iter->path().filename().string();
				if ( file.filename == ".." || file.filename == "." ) continue; // Skip on Unix
				file.isFolder = std::tr2::sys::is_directory( dir_iter->status() ); // Set if directory
				m_filelist.push_back( file );
			}
			// Add in the back directory
			if ( t_currentPath.has_parent_path() )
			{
				filelist_entry_t file;
				file.filename = "..";
				file.isFolder = true;
				m_filelist.push_back( file );
			}
			// Now sort the results
			struct
			{
				bool operator() ( const filelist_entry_t& left, const filelist_entry_t& right )
				{
					if ( left.isFolder != right.isFolder ) {
						return left.isFolder;
					}
					else {
						return left.filename.compare( right.filename ) < 0;
					}
				}
			} fileListSorter;
			std::sort( m_filelist.begin(), m_filelist.end(), fileListSorter );
		}
	}

	// Check the mouseover for the file list
	m_mouseover = -1;
	for ( uint f = 0; f < m_filelist.size(); ++f )
	{
		if ( Rect( rect.pos.x + 60, rect.pos.y + 40 + 20*f, 400, 18 ).Contains( cursor_pos ) )
		{
			m_mouseover = (int)f;
		}
	}
	// Check the mouse over for the buttons
	m_button_mouseover = -1;
	if ( m_mouseover == -1 )
	{
		if ( Rect( rect.pos.x + 40, rect.pos.y + 615, 115, 25 ).Contains( cursor_pos ) )
		{
			m_button_mouseover = 1;
		}
		else if ( m_context_showsave && Rect( rect.pos.x + 160, rect.pos.y + 615, 115, 25 ).Contains( cursor_pos ) )
		{
			m_button_mouseover = 2;
		}
	}
	if ( m_button_mouseover == -1 )
	{
		m_button_mousedown = false;
	}

	bool t_checknewfile = false;
	// Check the mouse click action
	if ( core::Input::MouseDown(core::kMBLeft) )
	{
		// Clicked on a file
		if ( m_mouseover != -1 )
		{
			m_selected = m_mouseover;
			// Change variables based on the selected type (if it's a file, use that file's name)
			if ( !m_filelist[m_selected].isFolder ) {
				m_namebox_value = m_filelist[m_selected].filename;
				t_checknewfile = true;
			}
			// Clicked on a folder, follow the link
			else {
				// Go to the folder
				if ( m_filelist[m_selected].filename == ".." ) {
					std::tr2::sys::path t_currentPath ( m_currentPath );
					t_currentPath = t_currentPath.parent_path();
					m_currentPath = t_currentPath.string();
				}
				else {
					std::tr2::sys::path t_currentPath ( m_currentPath );
					t_currentPath /= m_filelist[m_selected].filename;
					m_currentPath = t_currentPath.string();
				}
				// Clear out the list
				m_filelist.clear();
				m_mouseover = -1;
				m_selected = -1;
			}
		}
		// Check if clicked in name box. If in name box, need to give namebox focus.
		if ( Rect( rect.pos.x + 90, rect.pos.y + 515, 250, 25 ).Contains( cursor_pos ) )
		{
			m_namebox_focus = true;
		}
		// Otherwise, remove namebox focus.
		else
		{
			m_namebox_focus = false;
		}
		// Clicked on a button
		if ( m_button_mouseover != -1 ) 
		{
			m_button_mousedown = true;
		}
	}
	if ( core::Input::MouseUp(core::kMBLeft) )
	{
		// Clicked on a button
		if ( m_button_mouseover != -1 && m_button_mousedown ) 
		{
			m_button_mousedown = false;
			if ( m_button_mouseover == 1 ) // Cancel
			{
				hasSelection	= false;
				endMe			= true;
			}
			if ( m_button_mouseover == 2 && m_context_showsave ) // Save
			{
				hasSelection	= true;
				endMe			= false;
			}
		}
	}

	// Check for focus in namebox and type.
	if ( m_namebox_focus )
	{
		if ( m_namebox_value.size() < 96 )
		{
			unsigned char i = Input::GetTypeChar(); // Get typed character
			if (( i != core::kVkReturn )&&( i != 0 ))
			{
				m_namebox_value += i;
				t_checknewfile = true;
			}
		}
		if ( Input::Keydown( Keys.Backspace ) ) // Delete char if backspace
		{
			if ( m_namebox_value.size() > 0 )
			{
				m_namebox_value.resize( m_namebox_value.size()-1 );
				t_checknewfile = true;
			}
		}
	}

	// Check that the file already exists
	if ( t_checknewfile )
	{
		std::tr2::sys::path t_currentPath ( m_currentPath + "/" + m_namebox_value );

		if ( std::tr2::sys::is_directory( t_currentPath ) )
		{
			m_context_nameconflict = true;
			m_context_exists = true;
			m_context_showsave = false;
		}
		else if ( std::tr2::sys::exists( t_currentPath ) )
		{
			m_context_nameconflict = false;
			m_context_exists = true;
			m_context_showsave = true;
		}
		else
		{
			m_context_nameconflict = false;
			m_context_exists = false;
			m_context_showsave = (io_mode==1);
		}
	}

	// Restore old stuff
	//cursor_pos = old_cursor_pos;
}
void Dusk::DialogueFileSelector::Render ( void )
{
	//bool pixelMode = activeGUI->bInPixelMode;
	//Screen::_screen_info_t oldinfo = Screen::Info;
	//Screen::Info.width = 1.0F;
	//Screen::Info.height = 1.0F;
	//activeGUI->bInPixelMode = false;

	// Render the background
	setDrawDefault();
	setSubdrawDefault();
	setSubdrawOverrideColor( Color(0,0,0,0.3F) );
	drawRect( Rect(0, 0, (Real)Screen::Info.width, (Real)Screen::Info.height) );

	// Render the main panel
	setDrawDefault();
	setSubdrawDefault();
	drawRect( rect );
	drawRectWire( rect );

	// Draw the prompt text
	drawText( rect.pos.x + 10, rect.pos.y + 20, label.c_str() );

	// Create the file list background
	setDrawDefault();
	setSubdrawDefault();
	//setDrawDown();
	//drawRect( Rect( rect.pos.x + 0.03f, rect.pos.y + 0.05f, 0.33f, 0.7f ) );
	drawRectWire( Rect( rect.pos.x + 40, rect.pos.y + 37, 420, 470 ) );

	// Draw the file list
	for ( uint f = 0; f < m_filelist.size(); ++f )
	{
		setDrawDefault();
		setSubdrawDefault();
		if ( m_selected == f ) {
			setDrawDown();
		}
		drawRect( Rect( rect.pos.x + 60, rect.pos.y + 40 + 20*f, 398, 18 ) );
		if ( m_mouseover == f ) {
			setDrawHover();
			drawRectWire( Rect( rect.pos.x + 60, rect.pos.y + 40 + 20*f, 398, 18 ) );
		}
		drawText( rect.pos.x + 65, rect.pos.y + 55 + 20*f, m_filelist[f].filename.c_str() );
		if ( m_filelist[f].isFolder ) {
			drawText( rect.pos.x + 400, rect.pos.y + 55 + 20*f, "DIR" );
			setDrawDefault();
			setSubdrawDefault();
			setDrawHover();
			drawLine( rect.pos.x + 45, rect.pos.y + ((f==0)?37:30) + 20*f, rect.pos.x + 45, rect.pos.y + 50 + 20*f );
			drawLine( rect.pos.x + 45, rect.pos.y + 50 + 20*f, rect.pos.x + 60, rect.pos.y + 50 + 20*f );
		}
	}

	// Draw the namebox
	setDrawDefault();
	setSubdrawDefault();
	drawRectWire( Rect( rect.pos.x + 90, rect.pos.y + 515, 250, 25 ) );
	if ( m_namebox_focus ) {
		setDrawDown();
	}
	drawRect( Rect( rect.pos.x + 90, rect.pos.y + 515, 250, 25 ) );
	drawText( rect.pos.x + 40, rect.pos.y + 535, "Filename:" );
	drawText( rect.pos.x + 95, rect.pos.y + 535, m_namebox_value.c_str() );

	// Draw the conflict information
	if ( (io_mode == 0 && !m_context_exists) || (io_mode == 1 && m_context_exists) ) {
		setDrawDefault();
		setSubdrawDefault();
		setSubdrawOverrideColor( Color( 1,1,0,0.3f ) );
		drawRectWire( Rect( rect.pos.x + 90, rect.pos.y + 545, 115, 25 ) );
		setSubdrawPulse();
		drawRect( Rect( rect.pos.x + 90, rect.pos.y + 545, 115, 25 ) );
		drawText( rect.pos.x + 95, rect.pos.y + 563, (io_mode==1)?"File exists":"File not found" );
	}
	if ( m_context_nameconflict ) {
		setDrawDefault();
		setSubdrawDefault();
		setSubdrawOverrideColor( Color( 1,0,0,0.3f ) );
		drawRectWire( Rect( rect.pos.x + 210, rect.pos.y + 545, 115, 25 ) );
		setSubdrawPulse();
		drawRect( Rect( rect.pos.x + 210, rect.pos.y + 545, 115, 25 ) );
		drawText( rect.pos.x + 215, rect.pos.y + 563, "Conflict" );
	}

	// Draw the buttons
	setDrawDefault();
	setSubdrawDefault();
	if ( m_button_mouseover == 1 ) {
		if ( m_button_mousedown ) setDrawDown(); else setDrawHover();
	}
	drawRect( Rect( rect.pos.x + 40, rect.pos.y + 615, 115, 25 ) );
	drawRectWire( Rect( rect.pos.x + 40, rect.pos.y + 615, 115, 25 ) );
	drawText( rect.pos.x + 45, rect.pos.y + 633, "Cancel" );

	setDrawDefault();
	setSubdrawDefault();
	if ( m_context_showsave && m_button_mouseover == 2 ) {
		if ( m_button_mousedown ) setDrawDown(); else setDrawHover();
	}
	if (!m_context_showsave)
		setSubdrawOverrideColor( Color( 0.6F,0.1F,0.1F,0.5F ) );
	drawRect( Rect( rect.pos.x + 160, rect.pos.y + 615, 115, 25 ) );
	drawRectWire( Rect( rect.pos.x + 160, rect.pos.y + 615, 115, 25 ) );
	setSubdrawDefault();
	drawText( rect.pos.x + 165, rect.pos.y + 633, (io_mode==1)?"Save":"Open" );

	//Screen::Info = oldinfo;
	//activeGUI->bInPixelMode = pixelMode;
}