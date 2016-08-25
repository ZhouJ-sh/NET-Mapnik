#pragma once

#include <memory>

#include <mapnik/feature_type_style.hpp>
#include <mapnik/rule.hpp>

#include "Collections.h"

using namespace NETMapnik::Collections;

namespace NETMapnik
{
	typedef std::shared_ptr<mapnik::feature_type_style> style_ptr;

	ref class Rule;

	enum class FilterMode
	{
		FILTER_ALL,
		FILTER_FIRST,
		filter_mode_enum_MAX
	};

	public ref class RulesList : public ListAdapter<Rule^>
	{
	public:
		~RulesList();
		!RulesList();
		
		virtual property int Count
		{
			virtual int get() override;
		}

		virtual property bool IsReadOnly
		{
			virtual bool get() override;
		}

		virtual property Rule^ default[int]
		{
			virtual Rule^ get(int index) override;
			virtual void set(int index, Rule^ value) override;
		}
			virtual property bool CanDisposeItems
		{
			virtual bool get() override;
		}

		virtual void Clear() override;
		virtual void Insert(int index, Rule^ item) override;
		virtual void RemoveAt(int index) override;
		virtual void Add(Rule^ item) override;
		virtual bool Remove(Rule^ item) override;
	internal:
		RulesList(mapnik::feature_type_style const & style);

	private:
		style_ptr * _style;
	};

	public ref class FeatureTypeStyle
	{
	public:
		FeatureTypeStyle();
		~FeatureTypeStyle();

		property RulesList^ Rules
		{
			RulesList^ get();
		}

		property FilterMode Mode
		{
			FilterMode get();
			void set(FilterMode value);
		}

	internal:
		FeatureTypeStyle(mapnik::feature_type_style const & style);
		style_ptr NativeObject();

	private:
		RulesList ^ _rules;
		style_ptr * _style;
	};
}
