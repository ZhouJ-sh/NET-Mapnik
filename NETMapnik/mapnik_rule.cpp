#include "stdafx.h"
#include "mapnik_rule.h"

namespace NETMapnik
{
	Rule::Rule(mapnik::rule const & rule)
	{
		_rule = new rule_ptr(std::make_shared<mapnik::rule>(rule));
	}

	Rule::Rule()
	{
		_rule = new rule_ptr(std::make_shared<mapnik::rule>());
	}

	Rule::~Rule()
	{
		if (_rule != NULL)
		{
			delete _rule;
		}
	}
}