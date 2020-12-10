#pragma once

namespace core
{
	template<typename Type>
	void TypeName(std::vector<const char*>& names)
	{
		using Descriptor = refl::type_descriptor<Type>;
		const char* displayName = get_display_name(refl::reflect<Type>());
		names.push_back(displayName);
	}

	template <typename ...Types>
	std::vector<const char*> TypeNames()
	{
		std::vector<const char*> names;
		(TypeName<Types>(names), ...);
		return names;
	}
}