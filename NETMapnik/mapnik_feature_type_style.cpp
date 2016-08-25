#include "stdafx.h"
#include "mapnik_feature_type_style.h"
#include "mapnik_rule.h"
#include "utils.h"

namespace NETMapnik
{
	RulesList::RulesList(mapnik::feature_type_style const & style)
	{
		_style = new style_ptr(std::make_shared<mapnik::feature_type_style>(style));
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
			(*_style)->get_rules().size();
		}
		catch (const std::exception& ex)
		{
			throw gcnew System::Exception(GetManagedString(ex));
		}
		return -1;
	}

	bool RulesList::IsReadOnly::get()
	{
		return false;
	}

	Rule^ RulesList::default::get(int index)
	{
		if (index > (*_style)->get_rules().size() || index < 0)
		{
			throw gcnew System::ArgumentOutOfRangeException("invalid style index");
		}
		std::vector<mapnik::rule> const& rules = (*_style)->get_rules();
		return gcnew Rule(rules[index]);
	}

	void RulesList::default::set(int index, Rule^ value)
	{
		if (index > (*_style)->get_rules().size() || index < 0)
		{
			throw gcnew System::ArgumentOutOfRangeException("invalid style index");
		}
		if (value == nullptr)
		{
			throw gcnew System::ArgumentNullException("value is null");
		}
		mapnik::rules rules = (*_style)->get_rules_nonconst();
		rules[index] = *value->NativeObject();
	}

	bool RulesList::CanDisposeItems::get()
	{
		return true;
	}

	void RulesList::Add(Rule^ rule)
	{
		(*_style)->add_rule(static_cast<mapnik::rule &&>(*rule->NativeObject()));
	}

	bool RulesList::Remove(Rule^ item)
	{
		auto index = IndexOf(item);
		if (index < 0)
		{
			return false;
		}
		RemoveAt(index);
		return true;
	}

	void RulesList::RemoveAt(int index)
	{
		mapnik::rules & rules = (*_style)->get_rules_nonconst();
		if (index > rules.size())
		{
			throw gcnew System::ArgumentOutOfRangeException("index out of range");
		}
		rules.erase(rules.begin() + index);
	}

	void RulesList::Clear()
	{
		mapnik::rules & rules = (*_style)->get_rules_nonconst();
		rules.clear();
	}

	void RulesList::Insert(int index, Rule^ item)
	{
		mapnik::rules & rules = (*_style)->get_rules_nonconst();
		if (index > rules.size())
		{
			throw gcnew System::ArgumentOutOfRangeException("index out of range");
		}
		rules.insert(rules.begin(), *item->NativeObject());
	}

	FeatureTypeStyle::FeatureTypeStyle(mapnik::feature_type_style const & style)
	{
		_style = new style_ptr(std::make_shared<mapnik::feature_type_style>(style));
	}

	FeatureTypeStyle::FeatureTypeStyle()
	{
		_style = new style_ptr(std::make_shared<mapnik::feature_type_style>());
	}

	FeatureTypeStyle::~FeatureTypeStyle()
	{
		if (_style != NULL)
		{
			delete _style;
			_style = NULL;
		}
	}

	RulesList^ FeatureTypeStyle::Rules::get()
	{
		if (_rules == nullptr)
		{
			_rules = gcnew RulesList(*NativeObject());
		}
		return _rules;
	}

	FilterMode FeatureTypeStyle::Mode::get()
	{
		const mapnik::filter_mode_enum mode = (*_style)->get_filter_mode();
		return (FilterMode)mode;
	}

	void FeatureTypeStyle::Mode::set(FilterMode value)
	{
		(*_style)->set_filter_mode((mapnik::filter_mode_enum)value);
	}

	style_ptr FeatureTypeStyle::NativeObject()
	{
		return *_style;
	}
}