#pragma once
#include <string>

#include "file.hpp"
#include "string.hpp"
#include "bit_conv.hpp"

namespace Moon
{
	namespace Hacking
	{
		class Table
		{
			public:
				Table(const std::string& path)
				{					
					Open(path);
				}
				Table() = default;
				void Open(const std::string& path)
				{
					NewTable();
					std::string s = File::ReadAllText(path);

					if (s.empty())
						return;

					auto lines = String::ViewLines(s);

					for (const auto& line : lines)
					{
						unsigned char left = BitConverter::FromHexChar((const unsigned char*)line.data());

						if (left != line[3])
						{
							m_Change = true;
							m_Table[left] = line[3];
						}
					}
				}
				inline void Input(std::string& str) const
				{
					Input((unsigned char*)str.data(), str.size());
				}
				inline void Input(unsigned char* str, size_t lenght) const
				{
					if (!m_Change)
						return;

					for (size_t i = 0; i < lenght; ++i)
					{
						str[i] = m_Table[str[i]];
					}
				}
				inline void Output(unsigned char* str, size_t lenght) const
				{
					if (!m_Change)
						return;

					for (size_t i = 0; i < lenght; ++i)
					{
						size_t pos = m_Table.find(str[i]);

						if (pos != std::string::npos)
						{
							str[i] = pos;
						}
					}
				}
				inline void Output(std::string& str) const
				{
					Output((unsigned char*)str.data(), str.size());
				}
				inline char& operator[](const unsigned char& pos)
				{
					return m_Table[pos];
				}

				void NewTable()
				{
					m_Table.clear();
					m_Table.reserve(256);

					for (unsigned char i = 0; i < 0xff; ++i)
					{
						m_Table.push_back(i);
					}
				}
				inline unsigned char Find(const char& c, const size_t& start = 0) const
				{
					return m_Table.find(c, start);
				}
				inline void ReplaceAll(const char& c, const char& other)
				{
					if (c != other)
					{
						m_Change = true;

						unsigned char pos = Find(c);

						while (pos != 0xff)
						{
							m_Table[pos] = other;
							pos = Find(c, pos + 1);
						}
					}
				}

		private:
			std::string m_Table;
			bool m_Change = false;
		};
	};
};
