#pragma once
#include <string>
#include <string_view>
#include <vector>

namespace Moon
{
	namespace String
	{
		inline auto ViewLines(const std::string& str)
		{
			const char* endLine = "\r\n";

			size_t npos = std::string::npos;

			std::vector<std::string_view> output;

			for (size_t first = str.find_first_not_of(endLine, 0); first != npos; first = str.find_first_not_of(endLine, first))
			{
				size_t last = str.find_first_of(endLine, first);

				output.push_back(std::string_view(str.data() + first, last != npos ? last - first : str.size() - first));

				first = last;
			}

			return output;
		}
	}
}