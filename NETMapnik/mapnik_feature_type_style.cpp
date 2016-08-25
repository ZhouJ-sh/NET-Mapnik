#include "stdafx.h"
#include "mapnik_feature_type_style.h"
#include "utils.h"

namespace NETMapnik
{
	RulesList::RulesList(mapnik::feature_type_style* style)
	{
		if (style != NULL)
		{
			_style = style;
		}
		else
		{
			throw gcnew System::ArgumentNullException("style");
		}
	}

	RulesList::~RulesList()
	{
		this->!RulesList();
	}

	RulesList::!RulesList()
	{
		if (_style != NULL)
		{
			delete _style;
			_style = NULL;
		}
	}

	int RulesList::Count::get()
	{
		try
		{
			//return _style->get_rules().size();
			return 0;
		}
		catch (const std::exception& ex)
		{
			throw gcnew System::Exception(GetManagedString(ex));
		}
	}

	bool RulesList::IsReadOnly::get()
	{
		return false;
	}

	Rule^ RulesList::default::get(int index)
	{
		if (index > _style->get_rules().size())
		{
			throw gcnew System::ArgumentOutOfRangeException("index");
		}
		return gcnew Rule(_style->get_rules()[index]);
	}

	bool RulesList::CanDisposeItems::get()
	{
		return true;
	}

	void RulesList::Add(Rule^ rule)
	{

	}


}