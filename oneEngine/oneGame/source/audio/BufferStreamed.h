#ifndef AUDIO_BUFFER_STREAMED_H_
#define AUDIO_BUFFER_STREAMED_H_

#include "audio/Buffer.h"
#include "audio/types/Formats.h"

#include "ogg/ogg.h"
#include "vorbis/codec.h"
#include "vorbis/vorbisenc.h"
#include "vorbis/vorbisfile.h"

#include <mutex>

namespace audio
{
	class BufferStreamed : public Buffer
	{
	private:
		explicit				BufferStreamed ( const char* filename );
								~BufferStreamed ( void );

		friend	BufferManager;

	public:
		virtual void			Sample ( uint32_t& inout_sample_position, uint32_t sample_count, float* sample_output ) override;

		//	GetSampleLength() : returns length of the audio buffer, in samples
		AUDIO_API virtual uint32_t
								GetSampleLength ( void ) const override;
		//	GetSampleRate() : returns sample rate of the audio buffer
		AUDIO_API virtual uint32_t
								GetSampleRate ( void ) const override;
		//	GetLength() : return length of the audio buffer, in seconds
		AUDIO_API virtual double
								GetLength ( void ) const override;

	protected:
		// Streaming state
		FILE*				m_file = NULL;
		OggVorbis_File		m_oggStream;
		std::mutex			m_oggStreamLock;
		vorbis_info*		m_vorbisInfo = NULL;
		vorbis_comment*		m_vorbisComment = NULL;
		double				m_vorbisLength = 0.0;

		void					InitStream ( const char* filename );
		void					FreeStream ( void );
	};
}

#endif//AUDIO_BUFFER_STREAMED_H_