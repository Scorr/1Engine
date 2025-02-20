
#ifndef _TERRAIN_INSTANCE_TYPES_H_
#define _TERRAIN_INSTANCE_TYPES_H_

#include "core/types/types.h"

namespace Terrain
{
	// Instance type
	enum EInstanceType : int16_t
	{
		NST_NULL = -1,

		// Terrain + procedurally generated instances
		NST_RAVINE = 0,
		NST_CANYON,
		NST_RUIN_SPIKE,
		NST_RUIN_PILLAR,

		NST_CRYSTAL_SPIKE,
		NST_XPLO_CRYSTAL_SPIKE,
		NST_GEODE,

		NST_PATH_GRAVEL,

		NST_DESERT_ROCK_FORMATION_0,
		NST_DESERT_OASIS,
		NST_DESERT_MIRAGE,
		NST_DESERT_ROCK_WATER_SPRING,
		NST_DESERT_SINKHOLE,
		NST_DESERT_WELL,

		NST_VEIN_CRYSTAL,
		NST_VEIN_CLAY,

		NST_ORE_VEIN,
		NST_ORE_SPLOTCH,
		NST_ORE_SPLATTER,

		NST_BOULDER_SMALL,

		NST_CLEARING,

		NST_SYSTEM_END_PROCEDURAL_LIST,
		// VXG defined instance
		NST_VXG_WATCHTOWER0,

		NST_SYSTEM_END_VXG_LIST,
		// Area Instances
		NST_DEADLANDS_CRATER,

		NST_SYSTEM_END_AREA_LIST
	};
}

#endif//_TERRAIN_INSTANCE_TYPES_H_