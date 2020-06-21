namespace Moon
{
	namespace BitConverter
	{
		static char* s_Nibbles = new char[16]{ '0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F' };

		__forceinline unsigned char FromHexNibble(const unsigned char& value)
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

		__forceinline std::string ToHexString(const uint32_t& value)
		{
			std::string text(8, '0');			

			for (size_t i = 0; i < 8; ++i)
			{
				text[i] = s_Nibbles[(value & (0xF0000000 >> (4*i))) >> 28-(4*i)];				
			}

			return text;
		}

		__forceinline std::string ToHexChar(const unsigned char& c)
		{
			std::string hex(2, '0');
			hex[0] = s_Nibbles[(c & 0xF0) >> 4];
			hex[1] = s_Nibbles[c & 0x0F];
			return hex;
		}

		__forceinline unsigned char FromHexChar(const unsigned char* str)
		{		
			return FromHexNibble(str[0]) << 4 | FromHexNibble(str[1]);
		}		
	}
}