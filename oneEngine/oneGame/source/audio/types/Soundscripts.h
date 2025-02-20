#ifndef AUDIO_TYPE_STRUCTS_H_
#define AUDIO_TYPE_STRUCTS_H_

#include "core/types/types.h"
#include "core/containers/arstring.h"
#include "audio/mixing/Channels.h"

namespace audio
{
	enum eSoundScriptChannel : uint8_t
	{
		kChannelDefault = (uint8_t)MixChannel::kDefault,
		kChannelPhysics = (uint8_t)MixChannel::kPhysics,
		kChannelHeavy = (uint8_t)MixChannel::kHeavy,
		kChannelSpeech = (uint8_t)MixChannel::kSpeech,
		kChannelBackground = (uint8_t)MixChannel::kBackground,
		kChannelMusic = (uint8_t)MixChannel::kMusic,
	};

	enum eSoundScriptAIAlert : uint8_t
	{
		kAIAlertIgnore = 0,
		kAIAlertSmall,
		kAIAlertSpeech,
		kAIAlertNotice,
	};

	enum eSoundScriptSet : uint8_t
	{
		kSetStandard	= 0,
		kSetEnvironment	= 1,
		kSetMusic		= 2,
		kSetVoice		= 3,
		kSetPerLevel	= 4,

		kSoundScriptSet_MAX	= 4
	};

	//	arSoundScript
	// Contains audio script definitions.
	struct arSoundScript
	{
		float				pitch;
		float				attenuation;
		float				gain;

		eSoundScriptChannel	channel;
		eSoundScriptAIAlert	ai_alert_amount;
		char				loop;

		char				sound_count;
		arstring256			sounds [4];
		char				sound_usage [4];
	};

	//	arSoundIndex
	// Stores the position of a sound script within a soundscript file.
	// Used to avoid searching files for a definition during runtime.
	struct arSoundIndex
	{
		uint32_t			pos;
		eSoundScriptSet		set;
	};
}

#endif//AUDIO_TYPE_STRUCTS_H_