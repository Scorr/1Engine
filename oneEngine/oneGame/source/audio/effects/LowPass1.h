#ifndef AUDIO_EFFECTS_LOW_PASS_1_H_
#define AUDIO_EFFECTS_LOW_PASS_1_H_

#include "audio/Effect.h"

#include <array>

namespace audio {
namespace effect {

	struct LowPass1State : public EffectState
	{
		// Pitch to begin the cutoff at
		Real					m_cutoffPitch = 440.0F;
		// Cutoff fade in Hz, scaled against a 440.0 pitch. At 220 Hz, this will be a 2x narrower fade (50% width).
		Real					m_cutoffFade = 100.0F;
		// Mix of the "wet" and "dry" sounds
		Real					m_strength = 1.0F;
	};

	class LowPass1 : public audio::Effect
	{
	public:
		AUDIO_API explicit		LowPass1 ( MixChannel targetChannel );

		//	AllocateState() : Allocates delete-safe state for the mixer thread.
		AUDIO_API virtual EffectState*
								AllocateState ( void );
		
		//	UpdateStateForMixerThread( state ) : Grabs current game-side state and updates the given state in the mixer thread.
		AUDIO_API virtual void	UpdateStateForMixerThread ( EffectState* state );
		
		//	Evaluate( input, output, state ) : Performs the actual work in the mixer thread.
		AUDIO_API virtual void	Evaluate ( WorkbufferStereo<audio::kWorkbufferSize>& input, WorkbufferStereo<audio::kWorkbufferSize>& output, EffectState* effectState );

	public:
		LowPass1State			m_state;

	private:
		WorkbufferStereo<audio::kWorkbufferSize * 4>
								m_waveform;
		WorkbufferStereo<audio::kWorkbufferSize * 4>
								m_fft;
		WorkbufferStereo<audio::kWorkbufferSize * 4>
								m_fft_imag;
		WorkbufferStereo<audio::kWorkbufferSize * 4>
								m_waveformOutput;
	};

}}

#endif//AUDIO_EFFECTS_LOW_PASS_1_H_