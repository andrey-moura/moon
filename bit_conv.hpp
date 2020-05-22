namespace Moon
{
	namespace BitConverter
	{
		__forceinline unsigned char FromHexNibble(const char& value)
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

		__forceinline unsigned char FromHexString(const char* str)
		{		
			return FromHexNibble(str[0]) << 4 | FromHexNibble(str[1]);
		}		
	}
}