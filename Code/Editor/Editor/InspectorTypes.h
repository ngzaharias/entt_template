#pragma once

#include <Editor/Address.h>
#include <Editor/Transaction.h>

#include <Engine/AttributeTypes.h>

namespace editor
{
	struct Attributes
	{
		std::optional<attr::Name> m_Name = { };
		std::optional<attr::Range> m_Range = { };
	};

	struct InspectorInfo
	{
		using Transactions = std::vector<Transaction>;

		Address		 m_Address = { };
		Attributes	 m_Attributes = { };
		Transactions m_Transactions = { };
	};

	template <typename Descriptor>
	editor::Attributes GetAttributes(Descriptor descriptor)
	{
		using namespace refl::descriptor;

		editor::Attributes attributes;
		if constexpr (has_attribute<attr::Name>(descriptor))
			attributes.m_Name = get_attribute<attr::Name>(descriptor);
		if constexpr (has_attribute<attr::Range>(descriptor))
			attributes.m_Range = get_attribute<attr::Range>(descriptor);
		return attributes;
	}
}