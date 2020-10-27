#pragma once

namespace Moon
{
	namespace BitConverter
	{
		static char s_Nibbles[16]{ '0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F' };
		static std::string s_False = "False";
		static std::string s_True = "True";

		static unsigned char FromHexNibble(const unsigned char& value)
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
		static std::string ToHexString(const unsigned int& value)
		{
			std::string text(8, '0');			

			for (size_t i = 0; i < 8; ++i)
			{
				text[i] = s_Nibbles[(value & (0xF0000000 >> (4*i))) >> (28-(4*i))];
			}

			return text;
		}
		static std::string ToHexChar(const unsigned char& c)
		{
			std::string hex(2, '0');
			hex[0] = s_Nibbles[(c & 0xF0) >> 4];
			hex[1] = s_Nibbles[c & 0x0F];
			return hex;
		}
		static unsigned char FromHexChar(const unsigned char* str)
		{		
			return FromHexNibble(str[0]) << 4 | FromHexNibble(str[1]);
		}		
		static unsigned char ReverseBits(const unsigned char& c)
		{
			uint8_t output = c;

			output = (output & 0xF0) >> 4 | (output & 0x0F) << 4;
			output = (output & 0xCC) >> 2 | (output & 0x33) << 2;
			output = (output & 0xAA) >> 1 | (output & 0x55) << 1;

			return output;
		}
		static unsigned char ReverseNibbles(const unsigned char& c)
		{
			return c << 4 | c >> 4;
		}
		static const std::string& ToBooleanString(const bool& value)
		{
			return value ? s_True : s_False;
		}
		static unsigned int SwapBytes(unsigned int num)
		{
			((num>>24)&0xff) |
            ((num<<8)&0xff0000) |
            ((num>>8)&0xff00) |
            ((num<<24)&0xff000000);

			return num;
		}
	}
}
