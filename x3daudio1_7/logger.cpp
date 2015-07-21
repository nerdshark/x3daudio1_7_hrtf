#include "stdafx.h"
#include "logger.h"
#include <fstream>
#include <memory>

#define ENABLE_LOG _DEBUG

std::unique_ptr<std::wostream> stream;

void logger::log(const std::wstring & message)
{
#if ENABLE_LOG
	if (!stream)
	{
		stream = std::unique_ptr<std::wostream>(new std::wofstream("log.txt"));
	}

	*stream << message << std::endl << std::flush;
	stream->flush();
#endif
}
