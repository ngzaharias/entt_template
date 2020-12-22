#pragma once

namespace core
{
	template <typename... Types>
	struct TypeList 
	{ 
		template <typename... Added>
		using Append = TypeList<Types..., Added...>;
	};
}