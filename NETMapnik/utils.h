#pragma once

#include <mapnik\box2d.hpp>
#include <mapnik\value_types.hpp>
#include <mapnik\value.hpp>
#include <mapnik\util\variant.hpp>

#include <msclr\marshal_cppstd.h>

#include <string>

namespace NETMapnik
{
	std::string UnicodeToUtf8(const std::wstring& str)
	{
		std::string utf8;
		utf8.reserve(str.length() * 3 / 2);

		const std::wstring::const_iterator end = str.end();
		for (std::wstring::const_iterator ci = str.begin(); ci != end; )
		{
			unsigned long wc = static_cast<unsigned long>(*ci);
			++ci;

			// handle surrogates for UTF-16
			if (sizeof(wchar_t) == 2 && wc >= 0xD800 && wc < 0xE000)
			{
				if (wc >= 0xDC00)
				{
					//invalid first surrogate
					wc = '?';
				}
				else if (ci != end)
				{
					const unsigned long lo = static_cast<unsigned long>(*ci);
					if (lo >= 0xDC00 && lo < 0xE000)
					{
						++ci;
						wc = 0x10000 + ((wc & 0x3FF) << 10 | lo & 0x3FF);
					}
					else
					{
						//invalid second surrogate
						wc = '?';
					}
				}
			}

			if (wc < (1 << 7))
			{
				utf8.push_back(static_cast<char>(wc));
			}
			else if (wc < (1 << 11))
			{
				utf8.push_back(static_cast<char>((wc >> 6) | 0xC0));
				utf8.push_back(static_cast<char>((wc & 0x3F) | 0x80));
			}
			else if (wc < (1 << 16))
			{
				utf8.push_back(static_cast<char>((wc >> 12) | 0xE0));
				utf8.push_back(static_cast<char>(((wc >> 6) & 0x3F) | 0x80));
				utf8.push_back(static_cast<char>((wc & 0x3F) | 0x80));
			}
			else if (wc < (1 << 21))
			{
				utf8.push_back(static_cast<char>((wc >> 18) | 0xF0));
				utf8.push_back(static_cast<char>(((wc >> 12) & 0x3F) | 0x80));
				utf8.push_back(static_cast<char>(((wc >> 6) & 0x3F) | 0x80));
				utf8.push_back(static_cast<char>((wc & 0x3F) | 0x80));
			}
			/*else if (wc < (1 << 26))
			{
			utf8.push_back(static_cast<char>((wc >> 24) | 0xF8));
			utf8.push_back(static_cast<char>(((wc >> 18) & 0x3F) | 0x80));
			utf8.push_back(static_cast<char>(((wc >> 12) & 0x3F) | 0x80));
			utf8.push_back(static_cast<char>(((wc >>  6) & 0x3F) | 0x80));
			utf8.push_back(static_cast<char>((wc & 0x3F)         | 0x80));
			}
			else if (wc < (1 << 31))
			{
			utf8.push_back(static_cast<char>((wc >> 30) | 0xFC));
			utf8.push_back(static_cast<char>(((wc >> 24) & 0x3F) | 0x80));
			utf8.push_back(static_cast<char>(((wc >> 18) & 0x3F) | 0x80));
			utf8.push_back(static_cast<char>(((wc >> 12) & 0x3F) | 0x80));
			utf8.push_back(static_cast<char>(((wc >>  6) & 0x3F) | 0x80));
			utf8.push_back(static_cast<char>((wc & 0x3F)         | 0x80));
			}*/
			else
			{
				utf8.push_back('?');
			}
		}

		return utf8;
	}

	std::wstring Utf8ToUnicode(const std::string& str)
	{
		std::wstring unicode;
		unicode.reserve(str.length() * 2 / 3);

		const std::string::const_iterator end = str.end();
		for (std::string::const_iterator ci = str.begin(); ci != end; )
		{
			const unsigned char b = static_cast<unsigned char> (*ci);
			++ci;
			if ((b & 0x80) == 0)
			{
				// 1-byte sequence: 000000000xxxxxxx = 0xxxxxxx
				unicode.push_back(static_cast<wchar_t>(b));
			}
			else if ((b & 0xE0) == 0xC0)
			{
				// 2-byte sequence: 00000yyyyyxxxxxx = 110yyyyy 10xxxxxx
				wchar_t wc = static_cast<wchar_t>((b & 0x1F) << 6);
				if (ci == end)
					break;
				wc |= static_cast<wchar_t>(static_cast<unsigned char> (*ci) & 0x3F);
				++ci;
				unicode.push_back(wc);
			}
			else if ((b & 0xF0) == 0xE0)
			{
				// 3-byte sequence: zzzzyyyyyyxxxxxx = 1110zzzz 10yyyyyy 10xxxxxx
				wchar_t wc = static_cast<wchar_t>((b & 0x0F) << 12);
				if (ci == end)
					break;
				wc |= static_cast<wchar_t>((static_cast<unsigned char>(*ci) & 0x3F) << 6);
				++ci;
				if (ci == end)
					break;
				wc |= static_cast<wchar_t>(static_cast<unsigned char>(*ci) & 0x3F);
				++ci;
				unicode.push_back(wc);
			}
			else if ((b & 0xF8) == 0xF0)
			{
				// 4-byte sequence: 110110ww:wwzzzzyy + 110111yy:yyxxxxxx
				//     = 11110uuu 10uuzzzz 10yyyyyy 10xxxxxx
				// where (uuuuu = wwww + 1)
				const unsigned char b1 = b;
				if (ci == end)
					break;
				const unsigned char b2 = static_cast<unsigned char>(*ci);
				++ci;
				wchar_t wc1 = static_cast<wchar_t>(0xD800 | ((((b1 & 0x07) << 2) + ((b2 & 0x30) >> 4) - 1) << 6) | ((b2 & 0x0F) << 2));
				if (ci == end)
					break;
				const unsigned char b3 = static_cast<unsigned char>(*ci);
				++ci;
				wc1 |= static_cast<wchar_t>((b3 & 0x30) >> 4);
				unicode.push_back(wc1);

				wchar_t wc2 = static_cast<wchar_t>(0xDC00 | ((b3 & 0x0F) << 6));
				if (ci == end)
					break;
				const unsigned char b4 = static_cast<unsigned char>(*ci);
				++ci;
				wc2 |= static_cast<wchar_t>(b4 & 0x3F);
				unicode.push_back(wc2);

				// TODO: Check surrogate.
			}
			else
			{
				// Unrecognized sequence.
				unicode.push_back(L'?');
			}
		}

		return unicode;
	}

	inline System::String^ GetManagedString(std::wstring& str)
	{
		//auto length = str.length();
		//auto buffer = gcnew System::Text::StringBuilder(length + 1);
		//buffer[0] = '\0';
		//const size_t msgLen = std::min((size_t)buffer->Capacity - 1, length);
		//::wcsncpy((wchar_t*)buffer, str.c_str(), msgLen);
		//buffer[msgLen] = '\0';
		//return buffer->ToString();
		return msclr::interop::marshal_as<System::String^>(str);
	}

	inline System::String^ GetManagedString(const std::exception& ex)
	{
		return GetManagedString(Utf8ToUnicode(ex.what()));
	}

	inline System::String^ GetManagedString(const std::string& str)
	{
		return GetManagedString(Utf8ToUnicode(str));
	}

	inline mapnik::box2d<double> ArrayToBox2D(array<System::Double>^ boxArray)
	{
		if (boxArray->Length != 4)
			throw gcnew System::Exception("Bbox array must contain 4 values: minx,miny,maxx,maxy");

		return mapnik::box2d<double>(boxArray[0], boxArray[1], boxArray[2], boxArray[3]);
	}

	inline array<System::Double>^ Box2DToArray(mapnik::box2d<double> bbox)
	{
		array<System::Double>^ boxArray = gcnew array<System::Double>(4);
		boxArray[0] = bbox.minx();
		boxArray[1] = bbox.miny();
		boxArray[2] = bbox.maxx();
		boxArray[3] = bbox.maxy();

		return boxArray;
	}

	struct params_to_object : public mapnik::util::static_visitor<>
	{
		params_to_object(System::Collections::Generic::IDictionary<System::String^, System::Object^>^ ds, System::String^ key) :
			_ds(ds),
			_key(key) {}

		void operator() (mapnik::value_integer val) const
		{
			_ds->Add(_key, gcnew long long(val));
		}

		void operator() (double val) const
		{
			_ds->Add(_key, gcnew double(val));
		}

		void operator() (bool val) const
		{
			_ds->Add(_key, gcnew bool(val));
		}

		void operator() (std::string const& s) const
		{
			_ds->Add(_key, msclr::interop::marshal_as<System::String^>(s));
		}

		void operator() (mapnik::value_unicode_string const& s) const
		{
			std::string buffer;
			mapnik::to_utf8(s, buffer);
			_ds->Add(_key, msclr::interop::marshal_as<System::String^>(buffer));
		}

		void operator() (mapnik::value_null const& /*s*/) const
		{
			_ds->Add(_key, nullptr);
		}

	private:
		gcroot<System::Collections::Generic::IDictionary<System::String^, System::Object^>^> _ds;
		gcroot<System::String^> _key;
	};
}