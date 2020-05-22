#pragma once
#include <vector>
#include <string>
#include <wx/file.h>

namespace Moon
{
	namespace File
	{
		__forceinline std::string ReadAllText(const std::string& path)
		{
			std::string ret;

			if (wxFile::Exists(path))
			{
				wxFile file(path);

				size_t size = file.Length();

				ret.resize(size);
				file.Read(ret.data(), size);
			}

			return ret;
		}		
	}
}