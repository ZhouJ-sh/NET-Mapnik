#pragma once

#include <memory>

#include <mapnik\color.hpp>

namespace NETMapnik
{
	typedef std::shared_ptr<mapnik::color> color_ptr;

	public ref class Color
	{
	public:
		Color(System::String^ color);
		Color(System::Int32 r, System::Int32 g, System::Int32 b);
		Color(System::Int32 r, System::Int32 g, System::Int32 b, System::Int32 a);
		~Color();

		property System::Int32 R
		{
			System::Int32 get();
			void set(System::Int32);
		}

		property System::Int32 G
		{
			System::Int32 get();
			void set(System::Int32);
		}

		property System::Int32 B
		{
			System::Int32 get();
			void set(System::Int32);
		}

		property System::Int32 A
		{
			System::Int32 get();
			void set(System::Int32);
		}

		 System::String^ ToString() override;
		 System::String^ Hex();

	internal:
		Color(mapnik::color const& color);
		color_ptr NativeObject();

	private:
		color_ptr* _color;
	};
}
