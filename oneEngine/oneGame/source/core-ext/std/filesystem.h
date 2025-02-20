#ifndef CORE_EXT_SYSTEM_IO_STD_FILESYSTEM_H_
#define CORE_EXT_SYSTEM_IO_STD_FILESYSTEM_H_

#include <filesystem>

#ifdef _MSC_VER
#	if _MSC_VER<=1900 // VS2015
namespace fs = std::tr2::sys;
#	elif _MSC_VER<=1916 // VS2017
#		if __cplusplus < 201703L
namespace fs = std::experimental::filesystem;
#		else
namespace fs = std::filesystem;
#		endif
#	else // VS2019+
#		if __cplusplus < 201703L
namespace fs = std::experimental::filesystem;
#		else
namespace fs = std::filesystem;
#		endif
#	endif
#else
namespace fs = std::filesystem;
#endif

#endif//CORE_EXT_SYSTEM_IO_STD_FILESYSTEM_H_