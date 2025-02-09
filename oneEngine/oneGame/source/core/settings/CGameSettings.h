//===============================================================================================//
//
//		CGameSettings - Settings class for global external info.
//
// CGameSettings performs IO on simple command line and CFG files.
// Values are stored under public members. Variables are manually parsed for simplicity.
//
//===============================================================================================//
#ifndef C_GAME_SETTINGS_
#define C_GAME_SETTINGS_

#include "core/types/types.h"
#include <vector>
#include <string>
#include <map>
using std::string;

// Class define
class CGameSettings
{
	// Struct definitions
public:
	// Application data stuff
	struct resolution_t {
		int w, h;
		bool operator== ( const resolution_t & a ) {
			return (a.w==w)&&(a.h==h);
		}
	};

	//=========================================//
	// Static Members
	//=========================================//
public:
	// Static Grabber
	CORE_API static CGameSettings* Active ( void );
	CORE_API static void SetActive ( CGameSettings* );
private:
	// Current active game settings
	static CGameSettings* pActive;


	void Initialize ( void );
public:
	// == Constructor ==
	// Creates default directories and loads options.
	CORE_API explicit CGameSettings ( string& command_line, bool read_only_mode );
	CORE_API explicit CGameSettings ( char** command_line, bool read_only_mode );

	//=========================================//
	// Settings system
	//=========================================//

	// Called when settings object is created.
	CORE_API void LoadSettings ( void );

	//		SaveSettings
	// Saves all settings into the default gameoptions.cfg file, normally in the working game directory.
	// Normally called when the program exits, but may be called manually to save options - especially in case of a crash.
	CORE_API void SaveSettings ( void );

	//		EditSetting
	// Either adds a new setting or changes an existing setting in the corresponding list.
	CORE_API void EditSetting ( const char * s_id, const int value ); 
	CORE_API void EditSetting ( const char * s_id, const Real value ); 
	CORE_API void EditSetting ( const char * s_id, const char* value ); 

	//		EditSaveSetting
	//
	//template <typename Type>
	//CORE_API void EditSaveSetting ( const char * s_id, const Type value );

	//		GetSettingExists
	// Returns if the setting with the given name exists
	CORE_API bool GetSettingExists ( const char * s_id );

	//		GetSettingAs*
	// Grabs a setting from the key-value list.
	CORE_API const int GetSettingAsInt ( const char * s_id );
	CORE_API const Real GetSettingAsReal ( const char * s_id );
	CORE_API const string GetSettingAsString ( const char * s_id );

private:
	//		LinkSettingVariable
	// Adds a variable to the list to be updated from and to the current gameoptions.cfg file
	void LinkSettingVariable ( int& variable, const char * s_id );
	void LinkSettingVariable ( Real& variable, const char * s_id );

public:
	//=========================================//
	// Filesystem
	//=========================================//

	// Make a directory
	CORE_API void MakeDirectory ( const string& );
	CORE_API void MakeDirectory ( const char * );

	//=========================================//
	// Setters
	//=========================================//

	CORE_API void SetWorldSaveTarget ( const char* );
	CORE_API void SetRealmSaveTarget ( const char* );
	CORE_API void SetPlayerSaveTarget ( const char* );

	//=========================================//
	// Getters
	//=========================================//

	// These can return references because they're references to class data
	CORE_API const string& GetWorldTargetName ( void );
	CORE_API const string& GetRealmTargetName ( void );
	CORE_API const string& GetPlayerTargetName ( void );

	// These return the actual directories to saving
	CORE_API string MakeWorldSaveDirectory ( void );
	CORE_API string MakeRealmSaveDirectory ( void );
	CORE_API string MakeRealmSaveDirectory ( const string& realmName );
	CORE_API string MakePlayerSaveDirectory ( void );
	CORE_API string MakePlayerSaveDirectory ( const string& playerName );

	// Populates a vector of strings with all the found player files
	CORE_API void GetPlayerFileList ( std::vector<string>& );
	// Populates a vector of strings with all the found realm files
	CORE_API void GetRealmFileList ( std::vector<string>& );

	// Populates a vector of resolutions with all the found (and wanted) resolutions and returns it
	//const vector<resolution_t> & GetResolutionList ( void ); // TODO: does the renderer even work this way still?

public: // Members that are edited by outside objects often are put here, as a get/set would be redundant like a second asshole
	//=========================================//
	// Data members
	//=========================================//

	// General systems
	int		b_dbg_logToFile;

	// Rendering options
	int		b_ro_EnableShaders;
	int		i_ro_TargetResX;
	int		i_ro_TargetResY;
	int		i_ro_ShadowMapResolution;
	int		b_ro_EnableShadows;
	int		i_ro_GrassSubdivisions;

	int		b_ro_UseBufferModel;
	int		b_ro_UseHighRange;
	int		b_ro_Enable30Blit;
	int		b_ro_Enable30Steroscopic;

	int		i_ro_RendererMode;
	
	float	f_ro_DefaultCameraRange;

	// Game options and settings
	int		b_cl_FastStart;
	int		i_cl_DefaultSeed;
	int		b_cl_MinimizeTerrainThreads;
	int		i_cl_KeyboardStyle;
	float	f_cl_MouseSensitivity;

	float	f_cl_ReticleScale;

	// Terrain options
	int		i_cl_ter_Range;
	int		b_cl_ter_ShadowOfTheCollussusRenderStyle;
	int		i_cl_ter_FarLODRangeCount;

	// Debugging Rendering System
	int		b_dbg_ro_EnableSkinning;
	int		b_dbg_ro_GPUSkinning;
	int		b_dbg_ro_ShowMissingLinks;

	//=========================================//
	// OS command system
	//=========================================//

	string	s_cmd;	// command line options
	std::vector<string> m_cmd; // parsed

	//=========================================//
	// System Properties
	//=========================================//

	string	sysprop_developerstring;
	string	sysprop_default_cmd;
	string	sysprop_editor;

private:
	//=========================================//
	// Private data
	//=========================================//

	// Current save targets (for game)
	string	m_target_file_player;
	string	m_target_file_world;
	string	m_target_file_realm;

	// Current saved data
	std::map<string,int*>	m_settings_pint;
	std::map<string,Real*>	m_settings_preal;
	std::map<string,int>	m_settings_int;
	std::map<string,float>	m_settings_real;
	std::map<string,string>	m_settings_string;

	// Current state
	bool		m_readonly = false;
};

#endif//C_GAME_SETTINGS_