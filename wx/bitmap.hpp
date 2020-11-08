#include <wx/bitmap.h>
#include <wx/rawbmp.h>

namespace Moon
{
	namespace wxWidgets
	{
		namespace Bitmap
		{
			template<typename function>
			static void for_each_pixel_32(wxBitmap& bitmap, function f)
			{
				wxAlphaPixelData data(bitmap);
				wxAlphaPixelData::Iterator it = data.GetPixels();

				for(size_t y = 0; y < data.GetHeight(); ++y)
				{
					wxAlphaPixelData::Iterator row_start = it;

					for(size_t x = 0; x < data.GetWidth(); ++x, it++)
					{
						f(it.Red(), it.Green(), it.Blue(), it.Alpha());
					}

					it = row_start;
					it.OffsetY(data, 1);
				}
			}

			template<typename function>
			static void for_each_pixel_24(wxBitmap& bitmap, function f)
			{
				wxNativePixelData data(bitmap);
				wxNativePixelData::Iterator it = data.GetPixels();	

				for(size_t y = 0; y < data.GetHeight(); ++y)
				{
					wxNativePixelData::Iterator row_start = it;

					for(size_t x = 0; x < data.GetWidth(); ++x, it++)
					{
						f(it.Red(), it.Green(), it.Blue(), 255);
					}

					it = row_start;
					it.OffsetY(data, 1);
				}
			}

			template<typename function>
			static void for_each_pixel(wxBitmap& bitmap, function f)
			{
				if(bitmap.GetDepth() == 32)
				{
					for_each_pixel_32(bitmap, f);
				}
				else if(bitmap.GetDepth() == 24)
				{
					for_each_pixel_24(bitmap, f);
				}

				wxASSERT_MSG(bitmap.GetDepth() == 32 || bitmap.GetDepth() == 24, L"Only 24 or 32 bits bitmaps are supported.");
			}
		}
	}
}