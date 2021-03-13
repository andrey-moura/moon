#pragma once

namespace Moon
{
	namespace BitConverter
	{
		static char s_Nibbles[16]{ '0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F' };
		static std::string s_False = "False";
		static std::string s_True = "True";

		static unsigned char FromHexNibble(const uint8_t& value)
		{
			if (value >= 0x30 && value <= 0x46)
			{
				if (value < 0x40)
				{
					return value - 0x30;
				}
				else if (value > 0x40)
				{
					return value - 0x37;
				}
			}

			return 0x00;
		}		
						
		static std::string ToHexString(const uint8_t* values, size_t count)
		{
			std::string text;
			text.reserve(count*2);
			values += count-1;

			for(size_t i = 0; i < count; ++i)
			{
				text.push_back(s_Nibbles[(values[0] & 0xF0) >> 4]);
				text.push_back(s_Nibbles[values[0] & 0x0F]);

				--values;
			}

			return text;
		}
		
		template<typename T>
		static std::string ToHexString(const T& value)
		{
			return ToHexString((uint8_t*)&value, sizeof(T));
		}

		static std::string ToHexChar(const uint8_t& c)
		{
			std::string hex(2, '0');
			hex[0] = s_Nibbles[(c & 0xF0) >> 4];
			hex[1] = s_Nibbles[c & 0x0F];
			return hex;
		}
		static unsigned char FromHexChar(const uint8_t* str)
		{		
			return FromHexNibble(str[0]) << 4 | FromHexNibble(str[1]);
		}		
		static unsigned char ReverseBits(const uint8_t& c)
		{
			uint8_t output = c;

			output = (output & 0xF0) >> 4 | (output & 0x0F) << 4;
			output = (output & 0xCC) >> 2 | (output & 0x33) << 2;
			output = (output & 0xAA) >> 1 | (output & 0x55) << 1;

			return output;
		}
		static unsigned char ReverseNibbles(const uint8_t& c)
		{
			return c << 4 | c >> 4;
		}
		static const std::string& ToBooleanString(const bool& value)
		{
			return value ? s_True : s_False;
		}
		static unsigned int SwapBytes(uint32_t num)
		{
			return	((num>>24)&0xff) |
					((num<<8)&0xff0000) |
					((num>>8)&0xff00) |
					((num<<24)&0xff000000);
		}
		
		static bool IsHexString(const std::string& str)
		{
			for(const char& c : str)
			{
				if(isxdigit(c) == 0)
				{
					return false;
				}				
			}

			return true;
		}

		static uint8_t ReplaceNibble(const uint8_t& b, const uint8_t& n, bool i)
		{
			if(i == 0)
			{
				return (b & 0xF0) | n;
			} else 
			{
				return (b & 0x0F) | (n<<4);
			}
		}
	}
}
