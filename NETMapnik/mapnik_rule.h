#pragma once

#include <memory>

#include <mapnik\rule.hpp>

namespace NETMapnik
{
	typedef std::shared_ptr<mapnik::rule> rule_ptr;
	public ref class Rule
	{
	public:
		Rule();
		~Rule();

	internal:
		Rule(mapnik::rule const & rule);
		rule_ptr NativeObject();

	private:
		rule_ptr * _rule;
	};
}

