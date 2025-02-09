#include "TextureIO.h"

#include "core/utils/string.h"
#include "core/system/io/CBinaryFile.h"
#include "core/math/Math.h"
#include "core/debug/console.h"
#include "core/exceptions.h"
#include "core/system/io/FileStdio.h"

#include "core-ext/system/io/Resources.h"
#include "core-ext/system/io/assets/Conversion.h"
#include "core-ext/system/io/Files.h"

#include "zlib/zlib.h"
#include "core-ext/system/io/assets/texloader/qoi.hpp"

core::BpdLoader::BpdLoader()
	: m_loadOnlySuperlow(false), m_loadImageInfo(false), m_loadMipmapMask(0), m_loadPalette(false), m_loadAnimation(false),
	// Buffer Outputs:
	m_buffer_Superlow(NULL), m_buffer_Mipmaps(),
	// Outputs:
	format(IMG_FORMAT_INVALID), mipmapCount(0), info(), animation(), frames(), palette(), paletteWidth(0),
	// Internal state:
	m_liveFile(NULL), m_liveStream(NULL)
{
	;
}
core::BpdLoader::~BpdLoader()
{
	// Close any live file:
	if (m_liveFile != NULL)
	{
		fclose(m_liveFile);
		m_liveFile = NULL;

		// Close any locally-created stream helper.
		if (m_liveStream)
		{
			delete m_liveStream;
			m_liveStream = NULL;
		}
	}
}

bool core::BpdLoader::LoadBpd ( const char* n_resourcename )
{
	// Close any live file:
	if (m_liveFile != NULL)
	{
		fclose(m_liveFile);
		m_liveFile = NULL;

		// Close any locally-created stream helper.
		if (m_liveStream)
		{
			delete m_liveStream;
			m_liveStream = NULL;
		}
	}

	// Create the resource name
	arstring256 image_rezname  (n_resourcename);
	std::string image_filename = image_rezname;
	{
		arstring256 file_extension = core::utils::string::GetFileExtension(image_rezname);
		std::string raw_filename = image_rezname;

		bool bRawFound = false;
		bool bBpdFound = false;

		core::utils::string::ToLower(file_extension, file_extension.length());
		if (file_extension.length() > 0)
		{	// Remove the extension
			image_rezname = raw_filename.substr(0, raw_filename.length() - (file_extension.length() + 1)).c_str();
		}

		const char* const image_extensions[] = {
			".png", ".jpg", ".jpeg", ".gif", ".tga", ".bmp", ".gal"
		};
		const size_t image_extensions_len = sizeof(image_extensions) / sizeof(const char* const);

		// Loop through and try to find the matching filename:
		for (size_t i = 0; i < image_extensions_len; ++i)
		{
			raw_filename = image_rezname + image_extensions[i];
			// Find the file to source data from:
			if (core::Resources::MakePathTo(raw_filename.c_str(), raw_filename))
			{
				bRawFound = true;
				break;
			}
		}

		// Select the BPD filename:
		image_filename = image_rezname + ".bpd";

		// Check if we have a BPD now
		if (core::Resources::MakePathTo(image_filename.c_str(), image_filename))
		{
			bBpdFound = true;
		}

		// If we have the raw, convert against the 
		if (bRawFound)
		{
			bool bBpdOutOfDate = false;
			bool bBpdCorrupted = false;

			if (bBpdFound)
			{
				// Read in the header
				m_liveFile = fopen(image_filename.c_str(), "rb");
				ARCORE_ASSERT(m_liveFile != NULL);
				textureFmtHeader header;
				fread(&header, sizeof(header), 1, m_liveFile);
				fclose(m_liveFile);

				// Check for correct BPD format & version
				bBpdCorrupted = strcmp(header.head, kTextureFormat_Header) != 0;
				bBpdOutOfDate = header.version[0] < kTextureFormat_VersionMajor || (header.version[0] == kTextureFormat_VersionMajor && header.version[1] < kTextureFormat_VersionMinor);

				// Grab the BPD's age
				uint64 bpd_datetime = header.datetime;

				// Grab the Raw's age
				uint64 raw_datetime = io::file::GetLastWriteTime(raw_filename.c_str());

				// Also check if we're out of date here
				bBpdOutOfDate = bBpdOutOfDate || (raw_datetime > bpd_datetime);
			}

			// Convert file
			if ((!bBpdFound || bBpdOutOfDate || bBpdCorrupted)
				&& core::Converter::ConvertFile(raw_filename.c_str()) == false)
			{
				debug::Console->PrintError( "BpdLoader::LoadBpd : Error occurred in core::Converter::ConvertFile call\n" );
			}

			// Find the file to open post-convert...
			image_filename = image_rezname + ".bpd";
			if (!core::Resources::MakePathTo(image_filename.c_str(), image_filename))
			{
				debug::Console->PrintError( "BpdLoader::LoadBpd : Could not find image file in the resources.\n" );
				return false;
			}
			else
			{
				bBpdFound = true;
			}
		}

		// Check we have a BPD
		if (!bBpdFound)
		{
			debug::Console->PrintError( "BpdLoader::LoadBpd : Could not find image file in the resources.\n" );
			return false;
		}
	}

	// Open the new file:
	m_liveFile = fopen(image_filename.c_str(), "rb");
	if (m_liveFile != NULL)
	{
		m_liveStream = new core::FileStdioRead(m_liveFile);
		return loadBpdCommon();
	}

	return false;
}

//	LoadBpd ( stream )
// Attempts to load BPD with given stream.
// If the file could not be loaded, will return false.
bool core::BpdLoader::LoadBpd ( core::IFileRead* m_stream )
{
	// Close any live file:
	if (m_liveFile != NULL)
	{
		fclose(m_liveFile);
		m_liveFile = NULL;

		// Close any locally-created stream helper.
		if (m_liveStream)
		{
			delete m_liveStream;
			m_liveStream = NULL;
		}
	}

	m_liveStream = m_stream;
	if (m_liveStream != NULL)
	{
		return loadBpdCommon();
	}

	return false;
}

//	LoadBpd() : Continues loading.
// Continues to load the live BPD file initially opened with LoadBpd.
bool core::BpdLoader::LoadBpd ( void )
{
	if (m_liveStream != NULL)
	{
		return loadBpdCommon();
	}
	return false;
}

//	LoadBpdCommon() : loads BPD file
bool core::BpdLoader::loadBpdCommon ( void )
{
	m_liveStream->Seek(0);

	bool read_header = false;

	//===============================
	// Read in the header

	textureFmtHeader header;
	if (!m_loadOnlySuperlow)
	{
		m_liveStream->Read(&header, sizeof(header), 1);
		read_header = true; 

		if (strcmp(header.head, kTextureFormat_Header) != 0)
		{
			throw core::CorruptedDataException();
			return false;
		}

		// Save new image info
		if (m_loadImageInfo)
		{
			info.width	= header.width;
			info.height	= header.height;
			info.depth	= header.depth;
			info.levels	= (uint8_t)std::min<uint16_t>(255, header.levels);
			info.type	= (core::gfx::tex::arTextureType)((header.flags & 0xFF000000) >> 24);
			
			// Backwards compatibility patch:
			if (info.type == core::gfx::tex::kTextureTypeNone)
			{
				info.type = core::gfx::tex::kTextureType2D;
			}

			format		= (ETextureFormatTypes)(header.flags & 0x000000FF);
			compression	= (ETextureCompressionType)((header.flags & 0x0000F000) >> 12);
		}

		// Check format
		if (format == IMG_FORMAT_DXT3 || format == IMG_FORMAT_DXT5 || format == IMG_FORMAT_ASTC)
		{
			debug::Console->PrintError("BpdLoader::loadBpdCommon : unsupported pixel format. only rgba8 supported at this time\n");
			throw core::YouSuckException();
			return false;
		}
	}

	// Based on that data, read in the other segments

	//===============================
	// Read in the superlow (doesnt need the header)

	if (m_buffer_Superlow)
	{
		m_liveStream->Seek(sizeof(textureFmtHeader));
		uint32_t formatFlags;
		m_liveStream->Read(&formatFlags, sizeof(uint32_t), 1);
		format = (ETextureFormatTypes)(formatFlags & 0xFF);
		m_liveStream->Read(m_buffer_Superlow, kTextureFormat_SuperlowByteSize, 1);

		switch (formatFlags & 0xFF)
		{
		case IMG_FORMAT_RGBA16:
		case IMG_FORMAT_RGBA16F:
			ARCORE_ERROR("Unsupported expansion at this time");
			break;
		}
	}

	//===============================
	// Read in texture levels

	if (read_header)
	{
		for (int i = 0; i < header.levels; ++i)
		{
			if (m_buffer_Mipmaps[i] && (m_loadMipmapMask & (1 << i)))
			{
				textureFmtLevel levelInfo;

				// Read in the level info first
				m_liveStream->Seek(header.levelsOffset + sizeof(textureFmtLevel) * i);
				m_liveStream->Read(&levelInfo, sizeof(levelInfo), 1);
			
				// Ensure data is correct
				if (strcmp(levelInfo.head, kTextureFormat_HeadLevel) != 0)
				{
					throw core::CorruptedDataException();
					return false;
				}
				if (levelInfo.level != i)
				{
					debug::Console->PrintError( "BpdLoader::loadBpdCommon : Mismatch in mipmap ids, wanted %d got %d.\n", i, levelInfo.level );
					return false;
				}

				// Seek to the actual level data
				m_liveStream->Seek(levelInfo.offset);

				// Read in the data to a temp buffer
				uchar* t_sideBuffer = new uchar [levelInfo.size];
				m_liveStream->Read(t_sideBuffer, levelInfo.size, 1);
			
				// Decompress the data directly into target pointer:
				const size_t	pixelByteSize = getTextureFormatByteSize(format);
				const unsigned long t_effectiveWidth	= std::max<unsigned long>(1, header.width / math::exp2(i));
				const unsigned long t_effectiveHeight	= std::max<unsigned long>(1, header.height / math::exp2(i));
				const unsigned long t_effectiveDepth	= std::max<unsigned long>(1, header.depth / math::exp2(i));
				const unsigned long t_mipmapByteCount	= (uint32_t)pixelByteSize * t_effectiveWidth * t_effectiveHeight * t_effectiveDepth;
				unsigned long t_mipmapByteCountDecompressed = t_mipmapByteCount;
				
				if (compression == kTextureCompressionTypeZlib)
				{
					int z_result = uncompress( (uchar*)m_buffer_Mipmaps[i], &t_mipmapByteCountDecompressed, (uchar*)t_sideBuffer, levelInfo.size );

					// Check decompress result
					switch( z_result )
					{
					case Z_OK:
						break;
					case Z_MEM_ERROR:
						debug::Console->PrintError("BpdLoader::loadBpdCommon : zlib : out of memory\n");
						break;
					case Z_BUF_ERROR:
						debug::Console->PrintError("BpdLoader::loadBpdCommon : zlib : output buffer wasn't large enough\n");
						break;
					case Z_DATA_ERROR:
						debug::Console->PrintError("BpdLoader::loadBpdCommon : zlib : corrupted data\n");
						break;
					}
				}
				else if (compression == kTextureCompressionTypeQOI)
				{
					qoi_desc desc;
					desc.width = t_effectiveWidth;
					desc.height = t_effectiveHeight * t_effectiveDepth;
					desc.channels = (uchar)pixelByteSize;
					desc.colorspace = QOI_LINEAR;
					
					void* result = core::texture::qoi::qoi_decode2( (uchar*)t_sideBuffer, t_mipmapByteCountDecompressed, &desc, (int)pixelByteSize, m_buffer_Mipmaps[i]);
					
					// Check decompress result
					if (result == NULL)
					{
						debug::Console->PrintError("BpdLoader::loadBpdCommon : zlib : corrupted data\n");
					}
				}

				// Ensure data decompressed actually fills the mip.
				ARCORE_ASSERT(t_mipmapByteCountDecompressed == t_mipmapByteCount);

				// Delete the side buffer
				delete [] t_sideBuffer;
			}
		}
	}

	//===============================
	// Read in animation info

	if (m_loadAnimation && read_header && header.animationOffset != 0)
	{
		m_liveStream->Seek(header.animationOffset);
		
		// Read in animation header
		textureFmtAnimation animation;
		m_liveStream->Read(&animation, sizeof(animation), 1);

		// Ensure data is correct
		if (strcmp(animation.head, kTextureFormat_HeadAnimation) != 0)
		{
			throw core::CorruptedDataException();
			return false;
		}

		// Set data
		this->animation.framecount	= animation.frames;
		this->animation.xdivs		= animation.xdivs;
		this->animation.ydivs		= animation.ydivs;
		this->animation.framerate	= (animation.framerate == 0) ? 60 : animation.framerate;

		// Read in all the frames too
		frames.resize(animation.frames);
		m_liveStream->Read(frames.data(), sizeof(textureFmtFrame) * animation.frames, 1);
	}

	//===============================
	// Read in palette info

	if (m_loadPalette && read_header && header.paletteOffset != 0)
	{
		m_liveStream->Seek(header.paletteOffset);

		// Read in palette header
		textureFmtPalette palette_info;
		m_liveStream->Read(&palette_info, sizeof(palette_info), 1);

		// Ensure data is correct
		if (strcmp(palette_info.head, kTextureFormat_HeadPalette) != 0)
		{
			throw core::CorruptedDataException();
			return false;
		}

		// Set data
		paletteWidth = palette_info.depth;

		// Read in entire palette
		palette.resize(palette_info.rows * palette_info.depth);
		m_liveStream->Read(palette.data(), sizeof(gfx::arPixel) * palette_info.rows * palette_info.depth, 1);
	}

	return true;
}