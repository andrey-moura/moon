#pragma once
#include <vector>
#include <string>
#include <wx/file.h>
#include <wx/filename.h>
#include "string.hpp"

namespace Moon
{
	namespace File
	{
		template <class T>
		static std::vector<T> ReadAllBytes(const std::string& path)
		{
			wxFile file;
			std::vector<T> ret;

			if (file.Exists(path))
			{
				if (file.Open(path))
				{					
					ret.resize(file.Length()*sizeof(T));
					file.Read(ret.data(), file.Length());
				}
			}

			return ret;
		}

		static std::string ReadAllText(const std::string& path)
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

		static auto ReadAllLines(const std::string& path, const bool& emptyLines = false)
		{
			return String::GetLines(ReadAllText(path), emptyLines);
		}

		static void WriteAllBytes(const std::string& path, const void* bytes, size_t size)
		{
			wxFile file;

			file.Create(path, true);
			file.Open(path, wxFile::OpenMode::write);
			file.Write(bytes, size);
		}

		template <class T>
		static void WriteAllBytes(const std::string& path, const std::vector<T>& bytes)
		{
			WriteAllBytes(path, (void*)bytes.data(), bytes.size() * sizeof(T));
		}

		static void WriteAllText(const std::string& path, const std::string& text)
		{
			WriteAllBytes(path, text.data(), text.size());
		}

		static std::string AppenPath(const std::string& path, const std::string& other)
		{
			std::string ret = path;
			size_t oldSize = ret.size();
			ret.reserve(oldSize + other.size() + 2);

			if (ret[oldSize - 1] != '\\')
				ret.push_back('\\');

			ret.append(other);
			ret.push_back('\\');

			return ret;
		}

		static bool MakeDir(const std::string& path)
		{			
			wxFileName fn;
			fn.SetPath(path);

			if (fn.DirExists())
			{
				return true;
			}			

			return fn.Mkdir(511, wxPATH_MKDIR_FULL);
		}

		static std::string SetName(const std::string& path, const std::string& name)
		{
			wxFileName fn;			
			fn.SetFullName(name);
			fn.SetPath(path);
			return fn.GetFullPath().ToStdString();
		}
		static bool Exists(const std::string& path)
		{
			return wxFile().Exists(path);
		}

		static bool Exists(const wxString& path)
		{
			return wxFile().Exists(path);
		}

		static bool Create(const std::string& path, const bool& replace = false)
		{			
			return wxFile().Create(path, replace);
		}

		static bool Create(const wxString& path, const bool& replace = false)
		{
			return wxFile().Create(path, replace);
		}
	}
}
