#include "TrConverterBMP.h"

#include "core-ext/system/io/assets/TextureIO.h"
#include "core-ext/system/io/assets/texloader/bmp.hpp"
#include "core/debug/console.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"

#include <chrono>

bool TrConverterBMP::Convert(const char* inputFilename, const char* outputFilename)
{
	core::gfx::tex::arImageInfo imageInfo = {};
	
	// Open the PNG and read the data:
	core::gfx::arPixel* pixel = core::texture::loadBMP(inputFilename, imageInfo);
	
	// It loaded properly?
	if (pixel != NULL)
	{
		// Set the data we need to write:
		core::BpdWriter writer;
		writer.m_generateMipmaps = true;
		writer.rawImage = pixel;
		writer.info = imageInfo;

		// Get the time of the input file
		//struct stat f_buf;
		//stat(inputFilename, &f_buf);
		//writer.datetime = (uint64_t)f_buf.st_mtime;
		writer.datetime = (uint64_t)std::chrono::system_clock::now().time_since_epoch().count();

		// Write the new BPD to the disk.
		return writer.WriteBpd(outputFilename);
	}
	return false;
}