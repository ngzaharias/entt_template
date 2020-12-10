#pragma once

#include <variant>

namespace core
{
	template<typename T> struct IsVariant : std::false_type {};

	template<typename ...Types>
	struct IsVariant<std::variant<Types...>> : std::true_type {};

	template<class... Types> struct VariantOverload : Types... { using Types::operator()...; };
	template<class... Types> VariantOverload(Types...)->VariantOverload<Types...>;

	template<class T, class U>
	struct VariantDefaults;

	template<class...Types, std::size_t... Indexes>
	struct VariantDefaults<std::variant<Types...>, std::index_sequence<Indexes...>> {
		using Variant = std::variant<Types...>;
		constexpr static Variant variants[sizeof...(Types)]{ Variant{std::in_place_index<Indexes>}... };
		// #todo: build display names as well
	};

	template<class Variant>
	struct VariantBuilder;

	template<class...Types>
	struct VariantBuilder<std::variant<Types...>>
	{
		using indices = std::make_index_sequence<sizeof...(Types)>;
		using type = VariantDefaults<std::variant<Types...>, indices>;
	};
}