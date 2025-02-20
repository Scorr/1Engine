//===============================================================================================//
//
//	ModelLoader.h
//
// Definitions for the SegmentedFile model format. 
//
//===============================================================================================//
#ifndef CORE_IO_ASSETS_MODEL_LOADER_
#define CORE_IO_ASSETS_MODEL_LOADER_

#include "core/types.h"
#include "core/containers/arstring.h"
#include "core/math/Math3d.h"
#include "core/math/XTransform.h"
#include "core/types/ModelData.h"
#include "core-ext/system/io/Resources.h"

namespace core
{
	static const char* MODELFMT_HEADER			= "$_INFO";
	static const char* MODELFMT_MATERIAL_PREFIX	= "$_MATERIAL";
	static const char* MODELFMT_ACTIONS			= "$_ACTIONS";
	static const char* MODELFMT_BONE_PREFIX		= "$_BONE";
	static const char* MODELFMT_MESH_PREFIX		= "$_MESH";
	static const char* MODELFMT_PHYSMESH_PREFIX	= "$_COLLISIONMESH";
	static const char* MODELFMT_MORPH_PREFIX	= "$_MORPH";
	static const char* MODELFMT_IK				= "$_IK";
	static const char* MODELFMT_HITBOXES		= "$_HITBOXES";
	static const char* MODELFMT_FX_PREFIX		= "$_FX";
	static const char* MODELFMT_MISC_PREFIX		= "$_MISC";

	struct modelFmtHeader
	{
		// Date of creation/conversion
		uint64_t datetime;
		// Model file version.
		// Different versions may be missing certain chunks or have different reading formats.
		uint32_t m_version;

		// Number of mesh
		uint32_t meshnum;
		uint32_t uniquemeshnum;
		// Animated?
		uint32_t animated;
		// Number of frames in the animation
		uint32_t framenumber;
		Real32	framerate;
		// Number of materials in the file
		uint32_t materialnum;
		// Number of bones in the file
		uint32_t bonenum;
		uint32_t splinenum;
		uint32_t cameranum;
		uint32_t miscnum;
		// Number of morphs in the model
		uint32_t morphnum;
		// Which mesh the morphs are targeting by default
		uint32_t morphmesh;
		// Number of FX in the model
		uint32_t fxnum;
	};
	struct modelFmtMaterialEntry
	{
		arstring128	name;
		uint8_t		referenced;
		arstring128	filename;
	};
	struct modelFmtActionEntry
	{
		uint32_t	start;
		uint32_t	end;
		arstring128	name;
		uint8_t		flags;
	};
	struct modelFmtBoneEntry
	{
		arstring128	name;
		// Parent node. -1 for root.
		int32_t		parent;
		XTransform	world_transform;
		XTransform	firstframe_transform;
	};
	struct modelFmtMeshEntry
	{
		arstring128	name;
		// Animated per vertex. It should always be false.
		uint8_t		apv;
		uint32_t	material_index;
		Matrix4x4	transform;
		arModelData	model;
		uint32_t	collision_index;
		// Parent node. "_is_root_" for no parent.
		arstring128	parent;
	};
	struct modelFmtCollisionEntry
	{
		arstring128	name;
		Matrix4x4	transform;
		arModelPhysicsData	model;
	};
	struct modelFmtMorphEntry
	{
		arstring128		name;
		arModelVertex*	vertices;
	};
	struct modelFmtIkEntry
	{
		uint32_t		type;
		arstring128		name;
		uint32_t		datacount;
		arstring128		data [5];
	};
	struct modelFmtHitboxEntry
	{
		uint8_t			bone;
		uint8_t			parentbone;
		Vector3f		center;
		Vector3f		extents;
		arstring128		name;
	};
	struct modelFmtFxEntry
	{
		//arstring128		name;
		char __padding [127];
		uint8_t			type;
		arstring128		data;
		float			parameters [32];
	};
	typedef std::vector<XTransform> modelFmtAnimationList_t;

	static_assert(sizeof(modelFmtHeader)		== sizeof(uint32_t)*16,	"Invalid structure size");
	static_assert(sizeof(modelFmtMaterialEntry)	== 257,					"Invalid structure size");
	static_assert(sizeof(modelFmtActionEntry)	== 140,					"Invalid structure size");
	static_assert(sizeof(modelFmtFxEntry)		== 384,					"Invalid structure size");

	enum EModelFxCodes : uint8_t
	{
		kModelFxDefault					= 0,

		kModelFxSoftbody				= 101,

		kModelFxJigglebone_Test			= 11,
		kModelFxJigglebone_Breast		= 12,
		kModelFxJigglebone_TailWaggle	= 25,
		kModelFxJigglebone_General		= 35,
	};
	enum EModelIkCodes : uint8_t
	{
		kModelIkDefault		= 255,

		kModelIkLookAt		= 0,
		kModelIkFootstep	= 1,
		kModelIkAiming		= 2,
		kModelIkProps		= 3,
	};

	static const char* AD_KEY_ACTIONS		= "$actions";
	static const char* AD_KEY_IK			= "$ikdefinitions";
	static const char* AD_KEY_HITBOXES		= "$hitboxes";
	static const char* AD_KEY_PROPERTIES	= "$properties";
	static const char* AD_KEY_LOD			= "$lod";
	static const char* AD_KEY_EVENTS_BEGIN	= "$begin_events";
	static const char* AD_KEY_EVENTS_END	= "$end_events";
	static const char* AD_KEY_ANIMATION_EXTRAPOLATE			= "$extrapolate";
	static const char* AD_KEY_ANIMATION_EXTRAPOLATE_ROOT	= "$extrapolate_root";
	static const char* AD_KEY_JIGGLE		= "$jiggle";
	static const char* AD_KEY_SOFTBODY		= "$softbody";

	static const char* AD_KEY_IK_LOOKAT		= "kModelIkLookAt";
	static const char* AD_KEY_IK_FOOTSTEP	= "kModelIkFootstep";
	static const char* AD_KEY_IK_AIMING		= "kModelIkAiming";
	static const char* AD_KEY_IK_PROPS		= "kModelIkProps";

	//	class ModelLoader
	// Use to read model data from the disk. Set flags, hit LoadModel, and let it rip.
	class ModelLoader
	{
	public:
		CORE_API explicit ModelLoader ( void );
		CORE_API		 ~ModelLoader ( void );

		//	LoadModel ( resource name )
		// Attempts to load a model with the given resource name.
		// If the file could not be loaded, will return false, and default model data will be loaded instead.
		CORE_API bool LoadModel ( const char* n_resourcename );

	public:
		//	Load options:

		// Load mesh and materials?
		bool		m_loadMesh;
		// Load morphs?
		bool		m_loadMorphs;
		// Load actions?
		bool		m_loadActions;
		// Load animation, FX? [SLOW] This will leak memory if loaded data is not handled properly!
		bool		m_loadAnimation;
		// Load skeleton, IK, hitboxes, FX?
		bool		m_loadSkeleton;

		//	Loaded data:

		modelFmtHeader							header;
		std::vector<modelFmtMaterialEntry>		materials;
		std::vector<modelFmtActionEntry>		actions;
		std::vector<modelFmtBoneEntry>			skeleton;
		std::vector<modelFmtMeshEntry>			meshes;
		std::vector<modelFmtCollisionEntry>		collisions;
		std::vector<modelFmtMorphEntry>			morphs;
		std::vector<modelFmtIkEntry>			ik;
		std::vector<modelFmtHitboxEntry>		hitboxes;
		std::vector<modelFmtFxEntry>			effects;

		std::vector<modelFmtAnimationList_t>	animation;

	protected:
		//	ConvertFile ( filename )
		// Attempts to convert file. Returns true on no convert or successful convert.
		// Returns false on issue.
		bool ConvertFile ( const char* n_filename );

	};
}

#endif//CORE_IO_ASSETS_MODEL_LOADER_