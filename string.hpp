#pragma once
#include <string>
#include <string_view>
#include <vector>

namespace Moon
{
	namespace String
	{
		//Splits string into lines
		static auto ViewLines(const std::string& str, const bool& keepEmpty = true)
		{
			//output
			std::vector<std::string_view> lines;

			if (str.empty())
				return lines;

			//current char
			const char* c = str.c_str();
			//the last endline found. Is start of string by default
			const char* lastPos = c;
			//the end of the string
			const char* cend = c + str.size();

			while (c < cend)
			{
				if (*c == '\r')
				{
					//Add the current string, if the lenght is 0 and keepEmpty is false, not add
					size_t lenght = c - lastPos;
					if (keepEmpty || lenght > 0)
					{
						lines.push_back(std::string_view(lastPos, lenght));
					}
					//If the next char is \n, we are at a \r\n ending. Ignore next
					if (c[1] == '\n')
					{
						c++;
					}
					//Ignore current endline in the next string
					lastPos = c + 1;
				}
				else if (*c == '\n')
				{
					//Add the current string, if the lenght is 0 and keepEmpty is false, not add
					size_t lenght = c - lastPos;
					if (keepEmpty || lenght > 0)
					{
						lines.push_back(std::string_view(lastPos, lenght));
					}
					//Ignore current endline in the next string
					lastPos = c + 1;
				}
				//Next char
				c++;
			}
			//Store last line
			size_t lenght = c - lastPos;
			if (keepEmpty || lenght > 0)
			{
				lines.push_back(std::string_view(lastPos, c - lastPos));
			}
			return lines;
		}

		//Splits string into lines (Makes copy)
		static auto GetLines(const std::string& str, bool keepempty)
		{
			//View lines of the string
			auto lines = ViewLines(str, keepempty);
			//Buffer to copy
			std::vector<std::string> str_lines;
			str_lines.reserve(lines.size());
			//Copy the lines
			for (const std::string_view& line : lines)
			{
				str_lines.push_back(std::string(line));
			}

			return str_lines;
		}

		template<typename T>
		static std::string Join(const std::vector<T>& strings, const std::string& separator, bool ignoreEmpty)
		{
			size_t totalSize = 0;
			size_t count = strings.size();

			for (const std::string_view& str : strings)
			{
				totalSize += str.size();
			}

			totalSize += separator.size() * strings.size();

			std::string output;
			output.reserve(totalSize);

			if (ignoreEmpty)
			{
				if (strings[0].size() != 0)
					output.append(strings[0]);
			}
			else
				output.append(strings[0]);

			for (size_t i = 1; i < count; ++i)
			{
				output.append(separator);
				output.append(strings[i]);
			}

			return output;
		}

		static void Remove(std::string& str, const std::string& find)
		{
			size_t pos = str.find(find);

			while (pos != std::string::npos)
			{
				str.erase(pos, find.size());
				pos = str.find(find, pos+find.size());
			}
		}		
		
		static void ReplaceDiferentSize(std::string& str, const std::string& find, const std::string replace)
		{
			int diference = replace.size() - find.size();			
			size_t index = str.find(find);

			if (index == std::string::npos) return;

			std::vector<size_t> indexes;

			size_t count = 0;

			while (index != std::string::npos)
			{
				indexes.push_back(index + (diference * count));
				index = str.find(find, index + find.size());
				++count;
			}

			str.reserve(str.size() + (diference * count));

			for (const size_t& pos : indexes)
			{				
				str.erase(pos, find.size());
				str.insert(pos, replace);
			}
		}
		
		static void ReplaceSameSize(std::string& str, const std::string& find, const std::string& replace)
		{
			if (find == replace) return;

			size_t pos = str.find(find);

			while (pos != std::string::npos)
			{
				str.replace(pos, replace.size(), replace);
				pos = str.find(find, pos + find.size());				
			}
		}
		
		static void Replace(std::string& str, const std::string& find, const std::string& replace)
		{
			if (str.empty()) return;
			if (find.empty()) return;

			if (replace.empty())
			{
				Remove(str, find);
			}

			if (find.size() == replace.size())
			{
				ReplaceSameSize(str, find, replace);
			}
			else
			{
				ReplaceDiferentSize(str, find, replace);
			}			
		}
		
		template <typename T>
		static void ConvertLineEnds(std::basic_string<T>& str, const uint32_t& eol)
		{
			for (size_t pos = 0; pos < str.size(); pos++) {
				if (str[pos] == '\r')
				{
					if (str[pos+1] == '\n')
					{
						// CRLF
						if (eol == 0x0d)
						{
							str.erase(pos + 1, 1); // Delete the LF
						}
						else if (eol == 0x0a)
						{
							str.erase(pos, 1); // Delete the CR
						}
						else
						{
							pos++;
						}
					}
					else {
						// CR
						if (eol == 0x0d0a)
						{
							str.insert(pos + 1, 1, '\n'); // Insert LF
							pos++;
						}
						else if (eol == 0x0a)
						{
							str[pos] = '\n';							
						}
					}
				}
				else if (str[pos] == '\n')
				{
					// LF
					if (eol == 0x0d0a)
					{
						str.insert(pos, 1, '\r'); // Insert CR
						pos++;
					}
					else if (eol == 0x0d)
					{
						str[pos] = '\r';						
					}
				}
			}
		}

		template <typename T>
		static uint32_t EOLToInt(const std::basic_string<T>& eol)
		{
			if (eol.size() == 0)
			{
				return 0;
			}
			else if (eol.size() == 1)
			{
				return eol[0];
			}
			else
			{
				return 0x0d0a;
			}
		}

		static const char* IntToEol(const uint32_t& eol)
		{
			switch(eol)
			{
				case 0x0a:
					return "\n"; 
				break;
				case 0x0d:
					return "\r";
				break;
				default:
					return "\r\n";
				break;
			}
		}

		static bool CheckEndLineCRLF(const std::string_view& str)
		{
			if (str.empty())
				return true;

			size_t crPos = str.find("\r");
			size_t lfPos = str.find("\n");

			//npos
			if (crPos == lfPos)
				return true;

			while (crPos != std::string::npos || lfPos != std::string::npos)
			{
				if (lfPos != (crPos + 1))
					return false;

				crPos = str.find("\r", crPos + 1);
				lfPos = str.find("\n", lfPos + 1);
			}

			return true;
		}

		static bool CheckEndLineLF(const std::string_view& str)
		{
			return std::string::npos != str.find("\r");
		}
	}
}
